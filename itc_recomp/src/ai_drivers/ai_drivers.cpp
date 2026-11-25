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

uint32_t Load(uint32_t address) {
  return guest::LoadU32(address);
}
int32_t LoadS16(uint32_t address) {
  return int16_t(guest::LoadU16(address));
}
// An lfs then stfs of a constant (its bits: CopyWord, as an lfs then stfs
// of one word keeps them in the translated code).
void StoreConstant(uint32_t address, uint32_t constant) {
  CopyWord(constant, address);
}
double Fsqrts(double a) {
  return double(float(std::sqrt(a)));
}

}  // namespace

// --- Set-up and clears ---

void InitPath(uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_8213AD48 AiDriver::InitPath");
  AiDriver(driver).set_route_section(0);
  const double one = Lfs(kOne);
  AiDriver(driver).set_route_slice(0);
  Stfs(AiDriver(driver).plan_a_w_address(), one);
  Stfs(AiDriver(driver).plan_b_w_address(), one);
  Stfs(AiDriver(driver).point_w_address(), one);
  for (uint32_t i = 0; i < 80; i++) {
    const uint32_t entry = driver + 528 + 64 * i;
    Stfs(AiPathEntry(entry).w_address(), one);
    AiPathEntry(entry).set_section(0);
    AiPathEntry(entry).set_slice(0);
  }
  Stfs(AiDriver(driver).target_w_address(), one);
}

uint64_t OfIndex(uint32_t index) {
  ITC_NATIVE_RUNNING("sub_82139728 AiDriver::Of");
  const uint32_t table = Load(G::drivers_address());
  if (table == 0) {
    return 0;
  }
  return uint64_t(index) * AiDriver::kSize + table;
}

uint64_t Of(uint32_t index) {
  return OfIndex(Load(index));
}

void ClearCarInput(uint32_t input) {
  ITC_NATIVE_RUNNING("sub_820C96E8 CarInput::Clear");
  CarInput(input).set_throttle(0);
  CarInput(input).set_brake(0);
  CarInput(input).set_handbrake(0);
  CarInput(input).set_steering(0);
  CarInput(input).set_flags(0);
  CarInput(input).set_kind(0);
  CarInput(input).set_steering_rate(0);
  CarInput(input).set_brake_rate(0);
  CarInput(input).set_throttle_rate(0);
  CarInput(input).set_camera(0);
}

void ClearFollow(uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82146508 AiDriver::ClearFollow");
  StoreConstant(AiDriver(driver).cruise_speed_address(), G::five_hundred_address());
  const double zero = Lfs(kZero);
  const double one = Lfs(kOne);
  AiDriver(driver).set_lane(0xFFFF);
  AiDriver(driver).set_flags(0);
  AiDriver(driver).set_route_position(0xFFFFFFFFu);
  AiDriver(driver).set_state(0);
  AiDriver(driver).set_previous_state(0);
  AiDriver(driver).set_racer_mask(0);
  AiDriver(driver).set_plan_word(0xFFFFFFFFu);
  Stfs(AiDriver(driver).plan_a_x_address(), zero);
  Stfs(AiDriver(driver).plan_a_y_address(), zero);
  Stfs(AiDriver(driver).plan_a_z_address(), zero);
  Stfs(AiDriver(driver).plan_a_w_address(), one);
  Stfs(AiDriver(driver).plan_b_x_address(), zero);
  Stfs(AiDriver(driver).plan_b_y_address(), zero);
  Stfs(AiDriver(driver).plan_b_z_address(), zero);
  Stfs(AiDriver(driver).plan_b_w_address(), one);
}

void ClearOtherIndexes(uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82144ED8 AiDriver::ClearOtherIndexes");
  for (uint32_t i = 0; i < 7; i++) {
    guest::StoreU16(driver + 202 + 40 * i, 0);
  }
}

void ClearCorners(uint32_t path) {
  ITC_NATIVE_RUNNING("sub_8213C720 AiPath::ClearCorners");
  AiPath(path).set_corner_count(0);
  const double zero = Lfs(kZero);
  const double one = Lfs(kOne);
  for (uint32_t i = 0; i < 8; i++) {
    const uint32_t record = path + 5164 + 44 * i;
    Stfs(AiCorner(record).direction_address(), one);
    AiCorner(record).set_data(0);
    Stfs(AiCorner(record).speed_address(), zero);
    AiCorner(record).set_table(0);
    Stfs(AiCorner(record).slope_before_address(), zero);
    AiCorner(record).set_number(0);
    Stfs(AiCorner(record).slope_after_address(), zero);
    AiCorner(record).set_entry_offset(0);
    AiCorner(record).set_end(0);
    AiCorner(record).set_position(0);
    AiCorner(record).set_unknown_24(0);
    AiCorner(record).set_joined_before(0);
    AiCorner(record).set_joined_after(0);
    AiCorner(record).set_kind(0);
  }
}

void ClearCornerKind(uint32_t corner) {
  ITC_NATIVE_RUNNING("sub_8213C710 AiPath::ClearCornerKind");
  AiCorner(corner).set_kind(0);
}

void ResetSkill(uint32_t skill) {
  ITC_NATIVE_RUNNING("sub_8213F698 AiSkill::Reset");
  for (uint32_t i = 0; i < 3; i++) {
    guest::StoreU16(skill + 24 + 4 * i, 0);
    guest::StoreU8(skill + 26 + 4 * i, 0);
  }
  for (uint32_t i = 0; i < 7; i++) {
    guest::StoreU16(skill + 4 + 2 * i, 0);
  }
  AiSkill(skill).set_speed_scale(0);
  StoreConstant(AiSkill(skill).percentage_address(), G::hundred_address());
  AiSkill(skill).set_speed(500);
  AiSkill(skill).set_throttle_cap(80);
  AiSkill(skill).set_corner(uint16_t(-100));
}

// --- Small reads and steps ---

void WrapAngle(uint32_t angle) {
  ITC_NATIVE_RUNNING("sub_82145408 Angle::Wrap");
  const double a = Lfs(angle);
  if (a > Lfs(G::pi_address())) {
    Stfs(angle, Fsubs(a, Lfs(G::two_pi_address())));
  } else if (a < Lfs(G::minus_pi_address())) {
    Stfs(angle, Fadds(a, Lfs(G::two_pi_address())));
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

uint32_t SetFlag2(uint32_t driver, uint32_t word) {
  ITC_NATIVE_RUNNING("sub_82138D20 AiDriver::SetFlag2");
  const bool set = Load(word) != 0;
  const uint32_t flags = AiDriver(driver).flags();
  AiDriver(driver).set_flags(set ? flags | 2 : flags & ~2u);
  const uint32_t vehicle = AiDriver(driver).vehicle().address;
  if (vehicle != 0) {
    vehicle_accessors::VehicleSetObjectHalf114(vehicle, Load(word));
  }
  return 1;
}

uint32_t IsFree(uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_821464B8 AiDriver::IsFree");
  const uint32_t vehicle = AiDriver(driver).vehicle().address;
  if (vehicle == 0) {
    return 0;
  }
  if ((Vehicle(vehicle).car_flags() & 129) != 0 &&
      (AiDriver(driver).flags() & 2) == 0) {
    return 0;
  }
  const uint32_t state = AiDriver(driver).state();
  return state == 0 || state == 6 ? 1 : 0;
}

double SpeedOverDistance(uint32_t driver, uint32_t distance) {
  ITC_NATIVE_RUNNING("sub_821450E0 AiDriver::SpeedOverDistance");
  double speed = Lfs(AiDriver(driver).speed_kmh_address());
  const double factor = Lfs(AiDriver(driver).distance_factor_address());
  const double hundred = Lfs(G::hundred_address());
  if (speed > hundred) {
    speed = Fdivs(hundred, speed);
  } else {
    speed = Lfs(kOne);
  }
  return Fmuls(Fmuls(Lfs(distance), speed), factor);
}

double CornerFactor(uint32_t driver, uint32_t curvature, uint32_t width) {
  ITC_NATIVE_RUNNING("sub_821453A0 AiDriver::CornerFactor");
  double c = Lfs(curvature);
  const double least = Lfs(G::thirty_address());
  double f1 = least;
  const double ten = Lfs(G::ten_address());
  if (c > ten) {
    c = Fsubs(c, ten);
    const double f11 = Lfs(AiDriver(driver).steer_factor_address());
    const double f10 = Lfs(AiDriver(driver).corner_grip_address());
    c = Fsqrts(c);
    c = Fmuls(c, f11);
    c = Fmuls(c, f10);
    f1 = Fmadds(c, ten, least);
  }
  double w = Lfs(width);
  const double room = Lfs(AiDriver(driver).car_radius_address());
  if (w < room) {
    w = Fsubs(room, w);
    f1 = Fnmsubs(w, ten, f1);
  }
  if (f1 < least) {
    f1 = least;
  }
  return f1;
}

uint32_t ReadCar(uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82144F50 AiDriver::ReadCar");
  const uint32_t vehicle = AiDriver(driver).vehicle().address;
  if (vehicle == 0) {
    return 0;
  }
  double width = Fneg(Lfs(vehicle + 5144));
  double f0 = Lfs(vehicle + 5160);
  if (width < f0) {
    width = f0;
  }
  Stfs(AiDriver(driver).car_half_width_address(), width);
  const double back = Fneg(Lfs(vehicle + 5136));
  double length = Lfs(vehicle + 5152);
  if (!(back < length)) {
    length = back;
  }
  const double f9 = Fmuls(width, width);
  Stfs(AiDriver(driver).half_width_address(), width);
  Stfs(AiDriver(driver).car_half_length_address(), length);
  const double f13 = Lfs(0x82002A10);
  Stfs(AiDriver(driver).corner_grip_address(), f13);
  const double f11 = Lfs(0x82002CB8);
  double diagonal = Fmadds(length, length, f9);
  Stfs(AiDriver(driver).distance_factor_address(), f11);
  const double f10 = Lfs(0x82002A14);
  diagonal = Fsqrts(diagonal);
  Stfs(AiDriver(driver).car_radius_address(), diagonal);
  const double half = Lfs(G::half_address());
  f0 = Lfs(vehicle + 3900);
  f0 = Fmuls(f0, half);
  f0 = Fmadds(f0, f10, f13);
  Stfs(AiDriver(driver).corner_grip_address(), f0);
  f0 = Lfs(0x82001FD4);
  double f13b = Lfs(vehicle + 3444);
  f13b = Fdivs(f0, f13b);
  f0 = Lfs(0x82002A2C);
  f0 = Fmadds(f13b, half, f0);
  const double one = Lfs(kOne);
  Stfs(AiDriver(driver).steer_factor_address(), f0);
  if (f0 > one) {
    Stfs(AiDriver(driver).steer_factor_address(), one);
  }
  double sum = Lfs(vehicle + 3512);
  const double f12 = Lfs(vehicle + 3508);
  sum = Fadds(sum, f12);
  f0 = Lfs(0x82002204);
  Stfs(AiDriver(driver).distance_factor_address(), Fmadds(sum, f0, f11));
  return 1;
}

void UpdateHalfWidth(uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82145230 AiDriver::UpdateHalfWidth");
  const double cos = c_runtime::Cos(Lfs(AiDriver(driver).road_angle_address()));
  const double sin = c_runtime::Sin(Lfs(AiDriver(driver).road_angle_address()));
  const double c = Fabs(double(float(cos)));
  const double across = Lfs(AiDriver(driver).car_half_width_address());
  const double s = Fabs(double(float(sin)));
  const double along = Lfs(AiDriver(driver).car_half_length_address());
  Stfs(AiDriver(driver).half_width_address(), Fmadds(along, s, Fmuls(across, c)));
}

uint32_t TargetPoint(uint32_t path, double ahead, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8213B4D8 AiPath::TargetPoint");
  const int32_t count = LoadS16(AiPath(path).entry_count_address());
  const uint32_t file = RG::route_file().address;
  const int32_t k = LoadS16(AiPath(path).look_ahead_address());
  if (!(k + 1 < count)) {
    return 0;
  }
  const double length = Lfs(TrackRouteFile(file).slice_length_address());
  const int32_t k2 = LoadS16(AiPath(path).look_ahead_address());
  const int32_t at = LoadS16(AiPath(path).first_address()) + k2;
  const int32_t next = at + 1;
  const uint32_t a = path + uint32_t(at - (at / 80) * 80) * 64;
  const uint32_t b = path + uint32_t(next - (next / 80) * 80) * 64;
  double t = double(float(double(int64_t(k))));
  t = Fnmsubs(t, length, ahead);
  double f0 = Fdivs(t, length);
  double f13 = Lfs(kZero);
  if (f0 < f13) {
    f0 = f13;
    f13 = Lfs(kOne);
  } else {
    const double one = Lfs(kOne);
    if (f0 < one) {
      f13 = Fsubs(one, f0);
    } else {
      f0 = one;
    }
  }
  for (uint32_t i = 0; i < 3; i++) {
    const double from = Fmuls(Lfs(a + 16 + 4 * i), f13);
    Stfs(out + 4 * i, Fmadds(Lfs(b + 16 + 4 * i), f0, from));
  }
  return 1;
}

// --- The skill ---

void ClearSkill(uint32_t skill) {
  ITC_NATIVE_RUNNING("sub_8213F700 AiSkill::Clear");
  ResetSkill(skill);
  AiSkill(skill).set_source(0);
  AiSkill(skill).set_runs_triggers(0);
}

namespace {
void ClearTriggers(uint32_t skill) {
  for (uint32_t i = 0; i < 3; i++) {
    guest::StoreU16(skill + 24 + 4 * i, 0);
    guest::StoreU8(skill + 26 + 4 * i, 0);
  }
}
}  // namespace

void SetSkillSource(uint32_t skill, uint32_t source) {
  ITC_NATIVE_RUNNING("sub_8213F900 AiSkill::SetSource");
  ClearTriggers(skill);
  ResetSkill(skill);
  AiSkill(skill).set_runs_triggers(0);
  AiSkill(skill).set_source(source);
  if (source != 0) {
    ResetSkill(skill);
  }
}

void LoadSkill(uint32_t skill) {
  ITC_NATIVE_RUNNING("sub_8213F730 AiSkill::Load");
  if (AiSkill(skill).source().address != 0) {
    for (uint32_t i = 0; i < 14; i += 2) {
      const uint32_t source = AiSkill(skill).source().address;
      guest::StoreU16(skill + 4 + i, guest::LoadU16(source + i));
    }
    ClearTriggers(skill);
  } else {
    ResetSkill(skill);
    AiSkill(skill).set_throttle_cap(100);
    if (G::race_data().kind() == 0) {
      AiSkill(skill).set_speed_scale(uint16_t(-5));
    }
  }
  StoreConstant(AiSkill(skill).percentage_address(), G::hundred_address());
}

// --- Speeds and steering ---

double LimitSpeed(uint32_t driver, uint32_t speed, uint32_t curvature,
                  uint32_t width, uint32_t slices, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82139248 AiDriver::LimitSpeed");
  double f1 = CornerFactor(driver, curvature, width);
  const int32_t n = int32_t(Load(slices));
  double limit = Fmuls(Lfs(speed), f1);
  if (n > 0) {
    const double length = Lfs(RG::route_file().slice_length_address());
    const double count = double(float(double(int64_t(n))));
    Stfs(frame + 80, Fmuls(length, count));
    f1 = SpeedOverDistance(driver, frame + 80);
    limit = Fadds(f1, limit);
  }
  if (Lfs(AiDriver(driver).target_speed_address()) > limit) {
    Stfs(AiDriver(driver).target_speed_address(), limit);
  }
  return f1;
}

void LimitByCurve(uint32_t driver, uint32_t table, uint32_t fraction,
                  uint32_t index, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_821390D0 AiDriver::LimitByCurve");
  const int32_t i = int32_t(Load(index));
  if (i < 0) {
    return;
  }
  if (!(i < LoadS16(Load(table + 0)))) {
    return;
  }
  if (record == 0) {
    return;
  }
  const uint32_t entries = Load(table + 4);
  const double at = Lfs(AiDriver(driver).road_offset_address());
  const double entry = Lfs(entries + uint32_t(i) * 8 + 4);
  const double past = Fsubs(at, entry);
  const double zero = Lfs(kZero);
  const double range = past < zero ? Lfs(AiPathEntry(record).range_low_address())
                                   : Lfs(AiPathEntry(record).range_high_address());
  const double span = Fsubs(range, entry);
  if (!(Fmuls(span, past) > zero)) {
    return;
  }
  const double part = Fdivs(past, span);
  const double one = Lfs(kOne);
  if (part < one) {
    if (part > Lfs(fraction)) {
      Stfs(fraction, part);
    }
  } else {
    Stfs(fraction, one);
  }
}

void EaseThrottleForSteer(uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82139B98 AiDriver::EaseThrottleForSteer");
  double steer = Lfs(AiDriver(driver).steer_shift_address());
  const double zero = Lfs(kZero);
  if (steer < zero) {
    steer = Fneg(steer);
  }
  steer = Fmuls(steer, Lfs(G::tenth_address()));
  if (!(steer > zero)) {
    return;
  }
  const double throttle = Lfs(AiDriver(driver).throttle_request_address());
  if (throttle < steer) {
    Stfs(AiDriver(driver).throttle_request_address(), zero);
  } else {
    Stfs(AiDriver(driver).throttle_request_address(), Fsubs(throttle, steer));
  }
}

// --- The path's corners and points ---

namespace {
// The first corner record before the first with a positive offset whose
// kind is in [low, high]: 1, else 0.
uint32_t KindAhead(uint32_t path, int32_t low, int32_t high) {
  const int32_t count = LoadS16(AiPath(path).corner_count_address());
  for (int32_t i = 0; i < count; i++) {
    const uint32_t record = path + 5164 + 44 * uint32_t(i);
    if (LoadS16(AiCorner(record).entry_offset_address()) > 0) {
      return 0;
    }
    const int32_t kind = int32_t(AiCorner(record).kind());
    if (kind >= low && kind <= high) {
      return 1;
    }
  }
  return 0;
}
}  // namespace

uint32_t HasKind2Ahead(uint32_t path) {
  ITC_NATIVE_RUNNING("sub_8213C148 AiPath::HasKind2Ahead");
  return KindAhead(path, 2, 2);
}

uint32_t IsSlow(uint32_t path) {
  ITC_NATIVE_RUNNING("sub_8213C0E8 AiPath::IsSlow");
  return KindAhead(path, 2, 3);
}

uint32_t PreferredSections(uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_8213C358 AiPath::PreferredSections");
  const int32_t first = int8_t(guest::LoadU8(a));
  if (uint32_t(first - 4) > 5) {
    return 0;
  }
  const int32_t second = int8_t(guest::LoadU8(b));
  const uint32_t which = uint32_t(first - 4) * 2 + uint32_t(second);
  if (which > 11) {
    return 0;
  }
  return G::section_lists_address() + which * 64;
}

uint32_t RaceAllowsSides() {
  ITC_NATIVE_RUNNING("sub_8213D318 AiPath::RaceAllowsSides");
  const int32_t mode = int32_t(G::race_data().race_mode());
  if (mode == 4 || mode == 7 || mode == 8) {
    return 1;
  }
  if (mode == 5) {
    const int32_t state = int32_t(G::race_state().stage_type());
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

void RefreshPoints(uint32_t path) {
  ITC_NATIVE_RUNNING("sub_8213B5D8 AiPath::RefreshPoints");
  for (int32_t i = 1; i < LoadS16(AiPath(path).plan_count_address()); i++) {
    const uint32_t entry = RingEntry(path, LoadS16(AiPath(path).first_address()) + i);
    const uint32_t record = Load(entry + 68);
    const double offset = Lfs(entry + 40);
    const double cx = Lfs(RouteSlice(record).center_x_address());
    Stfs(entry + 16,
         Fmadds(Lfs(RouteSlice(record).side_x_address()), offset, cx));
    CopyWord(RouteSlice(record).center_y_address(), entry + 20);
    const double dz = Lfs(RouteSlice(record).side_z_address());
    Stfs(entry + 24,
         Fmadds(dz, offset, Lfs(RouteSlice(record).center_z_address())));
  }
}

uint32_t ResetPath(uint32_t path) {
  ITC_NATIVE_RUNNING("sub_8213D438 AiPath::Reset");
  AiPath(path).set_look_ahead(1);
  AiPath(path).set_entry_count(0);
  const double one = Lfs(kOne);
  AiPath(path).set_first(0);
  const double zero = Lfs(kZero);
  AiPath(path).set_plan_count(0);
  for (uint32_t i = 0; i < 80; i++) {
    const uint32_t entry = path + 16 + 64 * i;
    Stfs(AiPathEntry(entry).x_address(), zero);
    Stfs(AiPathEntry(entry).y_address(), zero);
    AiPathEntry(entry).set_lane(0xFFFFFFFFu);
    Stfs(AiPathEntry(entry).z_address(), zero);
    AiPathEntry(entry).set_slice_record(0);
    Stfs(AiPathEntry(entry).w_address(), one);
    Stfs(AiPathEntry(entry).offset_address(), zero);
    Stfs(AiPathEntry(entry).target_offset_address(), zero);
    AiPathEntry(entry).set_route_position(0xFFFFFFFFu);  // the route position (-1, -1)
    Stfs(AiPathEntry(entry).step_address(), zero);
    Stfs(AiPathEntry(entry).range_low_address(), zero);
    Stfs(AiPathEntry(entry).range_high_address(), zero);
    Stfs(AiPathEntry(entry).room_low_address(), zero);
    Stfs(AiPathEntry(entry).room_high_address(), zero);
  }
  ClearCorners(path);
  AiPath(path).set_section_count(0);
  for (uint32_t i = 0; i < 8; i++) {
    guest::StoreU16(path + 5146 + 2 * i, 0xFFFF);
  }
  const uint32_t driver = AiPath(path).driver().address;
  if (driver != 0) {
    ClearOtherIndexes(driver);
  }
  return driver;
}

void SetLaneOffset(uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_8213B390 AiPathEntry::SetLaneOffset");
  const uint32_t record = AiPathEntry(entry).slice_record().address;
  const int32_t lane = int32_t(AiPathEntry(entry).lane());
  const int32_t lanes = RouteSlice(record).lanes() & 0xF;
  if (lane < 0) {
    AiPathEntry(entry).set_lane(0);
  } else if (!(lane < lanes)) {
    AiPathEntry(entry).set_lane(lanes > 1 ? uint32_t(lanes - 1) : 0);
  }
  const int32_t kept = int32_t(AiPathEntry(entry).lane());
  const double width = Lfs(RouteSlice(record).width_address());
  const double half = Lfs(G::half_address());
  const double n = double(float(double(int64_t(RouteSlice(record).lanes() & 0xF))));
  const double at = double(float(double(int64_t(kept))));
  const double from = Fnmsubs(Fsubs(n, Lfs(kOne)), half, at);
  const double offset = Fmuls(Fdivs(from, n), width);
  Stfs(AiPathEntry(entry).offset_address(), offset);
  Stfs(AiPathEntry(entry).target_offset_address(), offset);
}

// --- The other racers ---

void UpdateOtherFlags(uint32_t record) {
  ITC_NATIVE_RUNNING("sub_82145450 AiOther::UpdateFlags");
  uint32_t flags = AiOther(record).history_flags() & ~6u;
  const int32_t state = LoadS16(AiOther(record).state_address());
  AiOther(record).set_history_flags(uint16_t(flags));
  if (state == 3) {
    flags |= 8;
    AiOther(record).set_history_flags(uint16_t(flags));
    if ((flags & 0x10) == 0) {
      return;
    }
    flags = (flags | 4) & ~0x10u;
  } else if (state == 1) {
    flags |= 16;
    AiOther(record).set_history_flags(uint16_t(flags));
    if ((flags & 8) == 0) {
      return;
    }
    flags = (flags | 2) & ~8u;
  } else {
    return;
  }
  AiOther(record).set_history_flags(uint16_t(flags));
}

void UpdateOtherTimer(uint32_t record, uint32_t index, uint32_t limit) {
  ITC_NATIVE_RUNNING("sub_821456A0 AiOther::UpdateTimer");
  const uint32_t timer = AiOther(record).timer();
  const double gap = Lfs(AiOther(record).gap_address());
  const double thirty = Lfs(G::thirty_address());
  AiOther(record).set_timer(uint16_t(timer - 1));
  if (!(gap < thirty)) {
    return;
  }
  if (!(gap > Lfs(limit))) {
    return;
  }
  const uint32_t input = Load(G::inputs_address() + (Load(index) & 0xFF) * 4);
  if ((CarInput(input).flags() & 0x20000) == 0) {
    return;
  }
  AiOther(record).set_timer(120);
}

namespace {
// An other-racer record reset (ClearOthers' and ResetOthers' stores, in
// their order).
void ResetOtherRecord(uint32_t record, double gap, double zero, double one,
                      double minus_one) {
  Stfs(AiOther(record).gap_address(), gap);
  AiOther(record).set_flag_a(0);
  Stfs(AiOther(record).closing_rate_address(), zero);
  AiOther(record).set_flag_b(0);
  Stfs(AiOther(record).last_change_address(), zero);
  AiOther(record).set_timer(0);
  Stfs(AiOther(record).speed_address(), zero);
  AiOther(record).set_state(0);
  Stfs(AiOther(record).factor_address(), one);
  AiOther(record).set_action_flags(0);
  Stfs(AiOther(record).ticks_to_meet_address(), minus_one);
  AiOther(record).set_ring_index(0xFFFF);
  AiOther(record).set_history_flags(0);
  AiOther(record).set_unknown_36(0);
}
}  // namespace

void ClearOthers(uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_821459B0 AiDriver::ClearOthers");
  const double zero = Lfs(kZero);
  Stfs(AiDriver(driver).player_gap_address(), zero);
  AiDriver(driver).set_passes_a(0);
  const double f11 = Lfs(G::minus_one_address());
  const double one = Lfs(kOne);
  AiDriver(driver).set_passes_b(0);
  const double gap = Lfs(G::other_gap_address());
  AiDriver(driver).set_stuck_count(0);
  AiDriver(driver).set_in_corner(0);
  for (uint32_t i = 0; i < 7; i++) {
    ResetOtherRecord(driver + 172 + 40 * i, gap, zero, one, f11);
    guest::StoreU32(driver + 452 + 4 * i, 0xFFFFFFFFu);
  }
}

namespace {
// Whether racer i is in the driver's mask (+40, sign-extended) and isn't
// the driver itself (+0).
bool InMask(uint32_t driver, int32_t i) {
  if (int32_t(AiDriver(driver).index()) == i) {
    return false;
  }
  return ((1u << i) & uint32_t(LoadS16(AiDriver(driver).racer_mask_address()))) != 0;
}
}  // namespace

void PickTarget(uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82145118 AiDriver::PickTarget");
  const double zero = Lfs(kZero);
  double best = zero;
  if (!(int32_t(AiDriver(driver).watched()) < 0)) {
    return;
  }
  AiDriver(driver).set_watched(0xFFFFFFFFu);
  for (int32_t i = 0; i < 7; i++) {
    if (!InMask(driver, i)) {
      continue;
    }
    const uint32_t record = driver + 172 + 40 * uint32_t(i);
    const double gap = Lfs(AiOther(record).gap_address());
    const int32_t current = int32_t(AiDriver(driver).watched());
    bool take;
    if (gap < zero) {
      take = current < 0 || gap > best;
    } else {
      take = current < 0 || best < zero || gap < best;
    }
    if (take) {
      AiDriver(driver).set_watched(uint32_t(i));
      best = Lfs(AiOther(record).gap_address());
    }
  }
}

void PickTargetBehind(uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_821451B8 AiDriver::PickTargetBehind");
  const double zero = Lfs(kZero);
  double best = zero;
  AiDriver(driver).set_watched(0xFFFFFFFFu);
  for (int32_t i = 0; i < 7; i++) {
    if (!InMask(driver, i)) {
      continue;
    }
    const uint32_t record = driver + 172 + 40 * uint32_t(i);
    const double gap = Lfs(AiOther(record).gap_address());
    if (!(gap < zero)) {
      continue;
    }
    if (int32_t(AiDriver(driver).watched()) < 0 || gap > best) {
      AiDriver(driver).set_watched(uint32_t(i));
      best = Lfs(AiOther(record).gap_address());
    }
  }
}

uint32_t KeepsGapFrom(uint32_t driver, uint32_t other) {
  ITC_NATIVE_RUNNING("sub_82145048 AiDriver::KeepsGapFrom");
  const uint32_t state = AiDriver(driver).state();
  if (state == 0) {
    return 1;
  }
  const uint32_t index = Load(other);
  const uint32_t bit = (index & 0x20) != 0 ? 0 : 1u << (index & 0x1F);
  if ((uint32_t(LoadS16(AiDriver(driver).racer_mask_address())) & bit) != 0) {
    return 0;
  }
  if (state - 2 > 1) {
    return 1;
  }
  if (!(Lfs(driver + index * 40 + 172) > Lfs(kZero))) {
    return 1;
  }
  const uint32_t theirs = uint32_t(OfIndex(index));
  return AiDriver(theirs).state() - 2 > 1 ? 1 : 0;
}

// --- Set-up, modes and the plan ---

void SetParams(uint32_t driver, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82138D00 AiDriver::SetParams");
  AiDriver(driver).set_profile(object);
  SetSkillSource(driver + 6048, object != 0 ? object + 120 : 0);
}

void SetSkill(uint32_t driver, uint32_t word) {
  ITC_NATIVE_RUNNING("sub_821393D8 AiDriver::SetSkill");
  LoadSkill(driver + 6048);
  const uint32_t value = Load(word);
  const uint32_t vehicle = AiDriver(driver).vehicle().address;
  AiDriver(driver).set_skill_flag(value);
  const int32_t skill = LoadS16(AiDriver(driver).skill_speed_address());
  Stfs(AiDriver(driver).cruise_speed_address(), double(float(double(int64_t(skill)))));
  if (vehicle != 0) {
    StoreConstant(Vehicle(vehicle).brake_efficiency_address(), kOne);
  }
}

uint32_t ClearPlan(uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82139820 AiDriver::ClearPlan");
  const uint32_t path = driver + 512;
  const double zero = Lfs(kZero);
  const double one = Lfs(kOne);
  AiDriver(driver).set_plan_word(0xFFFFFFFFu);
  AiDriver(driver).set_state(0);
  AiDriver(driver).set_previous_state(0);
  AiDriver(driver).set_racer_mask(0);
  Stfs(AiDriver(driver).plan_a_x_address(), zero);
  Stfs(AiDriver(driver).plan_a_y_address(), zero);
  Stfs(AiDriver(driver).plan_a_z_address(), zero);
  Stfs(AiDriver(driver).plan_a_w_address(), one);
  Stfs(AiDriver(driver).plan_b_x_address(), zero);
  Stfs(AiDriver(driver).plan_b_y_address(), zero);
  Stfs(AiDriver(driver).plan_b_z_address(), zero);
  Stfs(AiDriver(driver).plan_b_w_address(), one);
  if (int32_t(AiPath(path).branch_kind()) != -1 ||
      int32_t(AiPath(path).leader_branch_kind()) != -1) {
    AiPath(path).set_branch_kind(0xFFFFFFFFu);
    AiPath(path).set_leader_branch_kind(0xFFFFFFFFu);
    return ResetPath(path);
  }
  return path;
}

double LimitSpeedBlend(uint32_t driver, uint32_t speed, uint32_t pair,
                       uint32_t t, uint32_t w, uint32_t slices, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82139B40 AiDriver::LimitSpeedBlend");
  const double target = Lfs(t);
  const double a = Lfs(pair);
  uint32_t curvature = pair;
  if (!(a < target)) {
    const double weight = Lfs(w);
    curvature = frame + 80;
    const double rest = Fsubs(Lfs(kOne), weight);
    Stfs(curvature, Fmadds(rest, a, Fmuls(weight, target)));
  }
  return LimitSpeed(driver, speed, curvature, pair + 4, slices, frame - 128);
}

uint32_t SetMode(uint32_t path, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_8213D508 AiPath::SetMode");
  const uint32_t value = Load(mode);
  if (int32_t(value) == int32_t(AiPath(path).mode())) {
    return path;
  }
  AiPath(path).set_mode(value);
  const uint32_t again = Load(mode);
  if (again == 0) {
    AiPath(path).set_normalizer(kNormalizeByBranch);
  } else if (again == 1) {
    AiPath(path).set_normalizer(kNormalizeByLane);
  }
  return ResetPath(path);
}

uint32_t ResetFor(uint32_t path, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_8213E658 AiPath::ResetFor");
  AiPath(path).set_driver(driver);
  StoreConstant(AiPath(path).largest_step_address(), kZero);
  AiPath(path).set_normalizer(kNormalizeByLane);
  AiPath(path).set_branch_kind(0xFFFFFFFFu);
  AiPath(path).set_mode(1);
  AiPath(path).set_leader_branch_kind(0xFFFFFFFFu);
  return ResetPath(path);
}

uint32_t NormalizeByLane(uint32_t entry, uint32_t position, uint32_t lane,
                         uint32_t masks) {
  ITC_NATIVE_RUNNING("sub_8213B428 AiPath::NormalizeByLane");
  const uint32_t route = track_route::kRoute;
  const uint32_t on = masks == 0
                          ? track_route::NormalizeLane(route, position, lane)
                          : track_route::NormalizeLaneMasked(route, position, lane, masks);
  if (on == 0) {
    return 0;
  }
  const uint32_t file = Load(route);
  const uint32_t section = guest::LoadU16(position + 0);
  const uint32_t table = TrackRouteFile(file).sections();
  const int32_t slice = LoadS16(position + 2);
  const uint32_t record = Load(table + section * 4) + uint32_t(slice) * 84 + 124;
  AiPathEntry(entry).set_slice_record(record);
  AiPathEntry(entry).set_section(guest::LoadU16(position + 0));
  AiPathEntry(entry).set_slice(guest::LoadU16(position + 2));
  AiPathEntry(entry).set_lane(uint32_t(LoadS16(lane)));
  SetLaneOffset(entry);
  return 1;
}

void UpdateLane(uint32_t driver, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82144EF8 AiDriver::UpdateLane");
  const uint32_t identity =
      vehicle_accessors::VehicleIdentity(AiDriver(driver).vehicle().address);
  const uint32_t lane = track_route::LaneAt(track_route::kRoute,
                                            TrackPosition(identity).point_x_address(),
                                            TrackPosition(identity).route_position_address());
  track_route::ClampPosition(frame + 80, TrackPosition(identity).route_position());
  const uint32_t clamped = Load(frame + 80);
  AiDriver(driver).set_lane(uint16_t(lane));
  AiDriver(driver).set_route_position(clamped);
}

// --- The other racers' records and the states ---

uint64_t ResetOthers(uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82145A48 AiDriver::ResetOthers");
  ClearOthers(driver);
  const double minus_one = Lfs(G::minus_one_address());
  const double one = Lfs(kOne);
  const double zero = Lfs(kZero);
  const double gap = Lfs(G::other_gap_address());
  uint64_t r3 = driver;
  for (int32_t i = 0; i < 7; i++) {
    if (i == int32_t(AiDriver(driver).index())) {
      continue;
    }
    r3 = OfIndex(uint32_t(i));
    const uint32_t record = uint32_t(r3) + AiDriver(driver).index() * 40 + 172;
    ResetOtherRecord(record, gap, zero, one, minus_one);
  }
  return r3;
}

void ClearCarState(uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82146588 AiDriver::ClearCarState");
  const double zero = Lfs(kZero);
  Stfs(AiDriver(driver).point_x_address(), zero);
  Stfs(AiDriver(driver).point_y_address(), zero);
  Stfs(AiDriver(driver).point_z_address(), zero);
  CopyWord(kOne, AiDriver(driver).point_w_address());
  AiDriver(driver).set_watched(0);
  for (uint32_t at = 96; at <= 104; at += 4) {
    Stfs(driver + at, zero);
  }
  const double far = Lfs(G::five_hundred_address());
  for (uint32_t at = 108; at <= 156; at += 4) {
    Stfs(driver + at, zero);
  }
  Stfs(AiDriver(driver).speed_limit_address(), far);
  Stfs(AiDriver(driver).speed_limit_other_address(), far);
  ClearOthers(driver);
}

void UpdateState4(uint32_t driver, uint32_t speed) {
  ITC_NATIVE_RUNNING("sub_82146220 AiDriver::UpdateState4");
  PickTarget(driver);
  const uint32_t target = AiDriver(driver).watched();
  if (int32_t(target) < 0) {
    return;
  }
  const double gap = Lfs(driver + target * 40 + 172);
  if (gap < Lfs(kZero)) {
    if (gap < Lfs(0x82008D40)) {
      return;
    }
    StoreConstant(speed, G::twenty_address());
  } else {
    if (gap > Lfs(0x8200257C)) {
      return;
    }
    StoreConstant(speed, G::five_hundred_address());
  }
}

uint64_t UpdateState5(uint32_t driver, uint32_t speed) {
  ITC_NATIVE_RUNNING("sub_82146388 AiDriver::UpdateState5");
  PickTargetBehind(driver);
  const uint32_t target = AiDriver(driver).watched();
  if (int32_t(target) < 0) {
    return driver;
  }
  if (!(Lfs(driver + target * 40 + 172) > Lfs(0x82008E48))) {
    return driver;
  }
  const uint64_t theirs = OfIndex(target);
  const double their_speed = Lfs(uint32_t(theirs) + 100);
  const double current = Lfs(speed);
  const double limit = Fmuls(their_speed, Lfs(0x82002A3C));
  if (current > limit) {
    Stfs(speed, limit);
  }
  return theirs;
}

uint32_t IsSettled(uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82146408 AiDriver::IsSettled");
  if (AiDriver(driver).state() == 0) {
    return 1;
  }
  AiDriver(driver).set_watched(0xFFFFFFFFu);
  PickTarget(driver);
  const uint32_t target = AiDriver(driver).watched();
  if (int32_t(target) < 0) {
    return 1;
  }
  const int32_t state = int32_t(AiDriver(driver).state());
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
  const double gap = Lfs(AiOther(record).gap_address());
  if (gap > Lfs(above)) {
    return 0;
  }
  return gap < Lfs(below) ? 0 : 1;
}

void MirrorOther(uint32_t out, uint32_t record, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_82145610 AiOther::Mirror");
  Stfs(AiOther(out).gap_address(),
       Fneg(Lfs(AiOther(record).gap_address())));
  Stfs(AiOther(out).closing_rate_address(),
       Fneg(Lfs(AiOther(record).closing_rate_address())));
  Stfs(AiOther(out).last_change_address(),
       Fneg(Lfs(AiOther(record).last_change_address())));
  CopyWord(AiOther(record).ticks_to_meet_address(),
           AiOther(out).ticks_to_meet_address());
  AiOther(out).set_flag_a(AiOther(record).flag_a());
  AiOther(out).set_flag_b(AiOther(record).flag_b());
  uint32_t state = AiOther(record).state();
  if (int16_t(state) == 1) {
    state = 3;
  } else if (int16_t(state) == 3) {
    state = 1;
  }
  AiOther(out).set_state(uint16_t(state));
  UpdateOtherFlags(out);
  CopyWord(value, AiOther(out).speed_address());
}

// --- The path's entries and corners ---

void BlockLeft(uint32_t entry, uint32_t at, uint32_t width, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8213C5C8 AiPathEntry::BlockLeft");
  Stfs(at, Fsubs(Lfs(at), Lfs(AiPathEntry(entry).step_address())));
  double edge = Fadds(Lfs(out), Lfs(AiPathEntry(entry).step_address()));
  Stfs(out, edge);
  edge = Fadds(Lfs(width), edge);
  Stfs(out, edge);
  const double from = Lfs(at);
  if (from < edge) {
    Stfs(out, from);
  }
  const double block = Lfs(out);
  if (!(Lfs(AiPathEntry(entry).room_low_address()) < block)) {
    return;
  }
  const double most = Lfs(AiPathEntry(entry).room_high_address());
  Stfs(AiPathEntry(entry).room_low_address(), block);
  if (block > most) {
    Stfs(AiPathEntry(entry).room_low_address(), most);
  }
}

void BlockRight(uint32_t entry, uint32_t at, uint32_t width, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8213C630 AiPathEntry::BlockRight");
  Stfs(at, Fsubs(Lfs(at), Lfs(AiPathEntry(entry).step_address())));
  double edge = Fadds(Lfs(out), Lfs(AiPathEntry(entry).step_address()));
  Stfs(out, edge);
  edge = Fadds(Lfs(width), edge);
  Stfs(out, edge);
  const double from = Lfs(at);
  if (from > edge) {
    Stfs(out, from);
  }
  const double block = Lfs(out);
  if (!(Lfs(AiPathEntry(entry).room_high_address()) > block)) {
    return;
  }
  const double least = Lfs(AiPathEntry(entry).room_low_address());
  Stfs(AiPathEntry(entry).room_high_address(), block);
  if (block < least) {
    Stfs(AiPathEntry(entry).room_high_address(), least);
  }
}

void InitCorner(uint32_t record, uint32_t data, uint32_t corner_entry,
                uint32_t count) {
  ITC_NATIVE_RUNNING("sub_8213C698 AiCorner::Init");
  AiCorner(record).set_data(data);
  AiCorner(record).set_table(data + 56);
  AiCorner(record).set_number(guest::LoadU16(corner_entry + 0));
  const uint32_t offset =
      guest::LoadU16(count) - guest::LoadU16(corner_entry + 2);
  AiCorner(record).set_entry_offset(uint16_t(offset));
  const uint32_t first = RouteCorner(data).entry_count();
  AiCorner(record).set_position(0);
  AiCorner(record).set_end(uint16_t(first + uint32_t(int32_t(int16_t(offset)))));
  const bool negative = Lfs(RouteCorner(data).direction_address()) < Lfs(kZero);
  StoreConstant(AiCorner(record).direction_address(),
                negative ? G::minus_one_address() : kOne);
  AiCorner(record).set_kind(0);
}

uint32_t WithinRoadAt(uint32_t path, uint32_t offset) {
  ITC_NATIVE_RUNNING("sub_8213C1A0 AiPath::WithinRoadAt");
  const uint32_t driver = AiPath(path).driver().address;
  const int32_t count = LoadS16(AiPath(path).entry_count_address());
  const uint32_t file = RG::route_file().address;
  const double reach = Lfs(AiDriver(driver).car_radius_address());
  const double length = Lfs(TrackRouteFile(file).slice_length_address());
  const int32_t k =
      int32_t(uint32_t(strings_and_font::steps::Fctiwz(Fdivs(reach, length))) + 1);
  if (!(k < count)) {
    return 0;
  }
  const int32_t start = LoadS16(AiPath(path).first_address());
  const double at = Lfs(offset);
  const uint32_t entry = RingEntry(path, int32_t(uint32_t(start) + uint32_t(k))) + 16;
  const uint32_t record = AiPathEntry(entry).slice_record().address;
  const double half =
      Fmuls(Lfs(RouteSlice(record).width_address()), Lfs(G::half_address()));
  if (at > Fadds(Lfs(RouteSlice(record).margin_right_address()), half)) {
    return 0;
  }
  const double left = Fsubs(Fneg(half), Lfs(RouteSlice(record).margin_left_address()));
  return at < left ? 0 : 1;
}

// --- Starts and resets ---

namespace {
// +16 bits 0 and 2 cleared.
uint32_t ClearBits0And2(uint32_t flags) {
  return flags & ~1u & ~4u;
}
}  // namespace

void ResetToMode1(uint32_t driver, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82139450 AiDriver::ResetToMode1");
  guest::StoreU32(frame + 80, 1);
  SetFlag2(driver, frame + 80);
  const uint32_t flags = AiDriver(driver).flags();
  const uint32_t vehicle = AiDriver(driver).vehicle().address;
  guest::StoreU32(frame + 84, 1);
  AiDriver(driver).set_flags(ClearBits0And2(flags));
  if (vehicle != 0) {
    SetMode(driver + 512, frame + 84);
  }
  AiDriver(driver).set_skill_flag(0);
  ResetSkill(driver + 6048);
  const uint32_t again = AiDriver(driver).vehicle().address;
  StoreConstant(AiDriver(driver).cruise_speed_address(), 0x82008948);
  if (again != 0) {
    StoreConstant(Vehicle(again).brake_efficiency_address(), kOne);
  }
}

void ResetToMode0(uint32_t driver, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_821394F0 AiDriver::ResetToMode0");
  guest::StoreU32(frame + 80, 0);
  SetFlag2(driver, frame + 80);
  const uint32_t vehicle = AiDriver(driver).vehicle().address;
  guest::StoreU32(frame + 88, 0);
  AiDriver(driver).set_flags(ClearBits0And2(AiDriver(driver).flags()));
  if (vehicle != 0) {
    SetMode(driver + 512, frame + 88);
  }
  ResetSkill(driver + 6048);
  AiDriver(driver).set_skill_flag(0);
  const int32_t skill = LoadS16(AiDriver(driver).skill_speed_address());
  Stfs(AiDriver(driver).cruise_speed_address(), double(float(double(int64_t(skill)))));
}

uint32_t StartRace(uint32_t driver, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82139DA0 AiDriver::StartRace");
  const uint32_t flags = AiDriver(driver).flags();
  const uint32_t vehicle = AiDriver(driver).vehicle().address;
  AiDriver(driver).set_flags(flags & ~1u);
  // The vehicle's flags read whatever the vehicle (+6148 of 0 without one),
  // as the original does.
  const uint32_t free = (Vehicle(vehicle).car_flags() & 129) == 0 ? 1 : 0;
  guest::StoreU32(frame + 80, free);
  SetFlag2(driver, frame + 80);
  const uint32_t set = AiDriver(driver).flags() | 4;
  guest::StoreU32(frame + 84, 0);
  const uint32_t again = AiDriver(driver).vehicle().address;
  AiDriver(driver).set_flags(set);
  if (again != 0) {
    SetMode(driver + 512, frame + 84);
  }
  guest::StoreU32(frame + 80, 1);
  SetSkill(driver, frame + 80);
  return ClearPlan(driver);
}

void ResetFollow(uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_821467B8 AiDriver::ResetFollow");
  ClearFollow(driver);
  ClearCarState(driver);
}

bool AllSettled(uint32_t frame, uint32_t* result) {
  ITC_NATIVE_RUNNING("sub_8213ADF8 AiDrivers::AllSettled");
  const int32_t count = int32_t(track_collision::RaceRacerCount());
  uint32_t table = Load(G::drivers_address());
  const double seventy = Lfs(0x82008930);
  double lowest = seventy;
  int32_t settled = 0;
  if (table != 0) {
    const uint32_t copy = frame + 80;
    for (int32_t i = 0; i < count; i++) {
      const uint32_t racer = Load(G::race().racers_address() + 4 * uint32_t(i));
      if ((MinimapRacerEntry(racer).flags() & 0x10) == 0) {
        continue;
      }
      const int32_t index = int32_t(MinimapRacerEntry(racer).vehicle_index());
      if (index < 0) {
        continue;
      }
      // memcpy (sub_825A7410) copies natively with nothing for its override
      // to record (a destination outside the physical views, which it
      // records for the renderer); else the original runs.
      if (copy >= 0xA0000000u ||
          !c_runtime::Memcpy(copy, uint32_t(index) * AiDriver::kSize + table,
                             AiDriver::kSize)) {
        return false;
      }
      settled++;
      if (IsSettled(copy) == 0) {
        *result = 0;
        return true;
      }
      const double speed = Lfs(copy + 100);
      if (lowest > speed) {
        lowest = speed;
      }
      table = Load(G::drivers_address());
    }
    if (settled == 1) {
      *result = 1;
      return true;
    }
  }
  *result = Fadds(lowest, Lfs(0x820021F4)) > Lfs(0x82008930) ? 1 : 0;
  return true;
}

// --- Pedals and steering ---

void AccumulateSteer(uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82138F00 AiDriver::AccumulateSteer");
  const double angle = Lfs(AiDriver(driver).target_angle_address());
  const double sum = Fadds(Lfs(AiDriver(driver).slide_angle_address()), angle);
  double steer = Lfs(AiDriver(driver).steer_shift_address());
  const double crossed = Fmuls(angle, steer);
  const double total = Fadds(sum, Lfs(AiDriver(driver).steer_request_address()));
  const double zero = Lfs(kZero);
  Stfs(AiDriver(driver).steer_request_address(), total);
  if (!(crossed < zero)) {
    return;
  }
  steer = Fmuls(steer, Lfs(G::fifth_address()));
  if (steer < zero) {
    steer = Fneg(steer);
  }
  const double speed = Lfs(AiDriver(driver).speed_kmh_address());
  if (speed < Lfs(G::other_gap_address())) {
    steer = Fmuls(Fmuls(speed, Lfs(0x82002A08)), steer);
  }
  steer = Fsubs(steer, Lfs(kOne));
  if (!(steer > zero)) {
    return;
  }
  const double most = Lfs(0x820021E4);
  Stfs(AiDriver(driver).steer_request_address(),
       steer < most ? Fmadds(angle, steer, total) : Fmadds(angle, most, total));
}

void ThrottleForSpeed(uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82139168 AiDriver::ThrottleForSpeed");
  const double speed = Lfs(AiDriver(driver).speed_kmh_address());
  if (!(speed > Lfs(G::twenty_address()))) {
    return;
  }
  const double over = Fsubs(speed, Lfs(AiDriver(driver).target_speed_address()));
  const double ten = Lfs(G::ten_address());
  const double zero = Lfs(kZero);
  if (over > ten) {
    Stfs(AiDriver(driver).throttle_request_address(), zero);
    StoreConstant(AiDriver(driver).brake_request_address(), 0x820029F0);
  } else if (over > zero) {
    Stfs(AiDriver(driver).throttle_request_address(), zero);
    Stfs(AiDriver(driver).brake_request_address(), zero);
  } else if (over > Lfs(0x82002824)) {
    const double eased = Fmuls(Fneg(over), Lfs(G::fifth_address()));
    if (eased < Lfs(AiDriver(driver).throttle_request_address())) {
      Stfs(AiDriver(driver).throttle_request_address(), eased);
    }
    Stfs(AiDriver(driver).brake_request_address(), zero);
  }
  if (Lfs(AiDriver(driver).speed_kmh_address()) >
      Lfs(AiDriver(driver).speed_limit_other_address())) {
    Stfs(AiDriver(driver).throttle_request_address(), zero);
  }
  if ((AiDriver(driver).flags() & 4) != 0) {
    return;
  }
  const double angle = Lfs(AiDriver(driver).target_angle_address());
  if (angle > Lfs(0x82001FB4) || angle < Lfs(0x82002A50)) {
    Stfs(AiDriver(driver).throttle_request_address(), zero);
  }
}

void EaseThrottleForSlide(uint32_t driver, uint32_t flag) {
  ITC_NATIVE_RUNNING("sub_82139BE8 AiDriver::EaseThrottleForSlide");
  const double speed = Lfs(AiDriver(driver).speed_kmh_address());
  if (speed < Lfs(G::twenty_address())) {
    return;
  }
  const uint32_t vehicle = AiDriver(driver).vehicle().address;
  uint32_t kind = 3;
  uint32_t limit = 0x82008954;
  if (vehicle != 0) {
    kind = Load(vehicle + 3548);
  }
  if (kind == 0) {
    limit = 0x8200894C;
  } else if (kind == 1) {
    limit = 0x82008950;
  }
  const double f13 = Lfs(limit);
  const double factor = Fmuls(Lfs(AiDriver(driver).steer_factor_address()),
                              Lfs(AiDriver(driver).corner_grip_address()));
  const double slide = Fabs(Lfs(AiDriver(driver).slide_angle_address()));
  double ease = Fmsubs(slide, speed, Fmuls(factor, f13));
  const double zero = Lfs(kZero);
  if (!(ease > zero)) {
    return;
  }
  ease = Fmuls(ease, Lfs(Load(flag) != 0 ? G::fifth_address() : 0x82002204));
  if (!(ease > zero)) {
    return;
  }
  const double throttle = Lfs(AiDriver(driver).throttle_request_address());
  Stfs(AiDriver(driver).throttle_request_address(),
       throttle < ease ? zero : Fsubs(throttle, ease));
}

void LimitThrottle(uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82139CB0 AiDriver::LimitThrottle");
  int32_t skill = LoadS16(AiDriver(driver).skill_throttle_cap_address());
  if (skill < 0) {
    skill = 0;
  } else if (skill > 100) {
    skill = 100;
  }
  const double most = Fmuls(double(float(double(int64_t(skill)))), Lfs(0x8200222C));
  const double zero = Lfs(kZero);
  const double throttle = Lfs(AiDriver(driver).throttle_address());
  if (throttle < zero) {
    Stfs(AiDriver(driver).throttle_address(), zero);
  } else if (throttle > most) {
    Stfs(AiDriver(driver).throttle_address(), most);
  }
  const uint32_t vehicle = AiDriver(driver).vehicle().address;
  const double limit = Lfs(vehicle + 4660);
  const double rpm = Lfs(Vehicle(vehicle).rpm_address());
  if (rpm < Fmuls(limit, Lfs(0x82002A34))) {
    return;
  }
  if (rpm > Fmuls(limit, Lfs(0x82002B90))) {
    Stfs(AiDriver(driver).throttle_address(), zero);
    return;
  }
  const bool high = rpm > Fmuls(limit, Lfs(0x82008958));
  const double now = Lfs(AiDriver(driver).throttle_address());
  if (now < zero) {
    Stfs(AiDriver(driver).throttle_address(), zero);
    return;
  }
  const double cap = Lfs(high ? 0x8200296C : G::half_address());
  if (now > cap) {
    Stfs(AiDriver(driver).throttle_address(), cap);
  }
}

void AdjustForOther(uint32_t driver, uint32_t record, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_821452A0 AiDriver::AdjustForOther");
  const double gap = Lfs(AiOther(record).gap_address());
  const double zero = Lfs(kZero);
  if (!(gap < zero)) {
    return;
  }
  const int32_t index = LoadS16(AiOther(record).ring_index_address());
  if (index < 0) {
    return;
  }
  if (AiOther(record).state() != 1) {
    return;
  }
  if (!(Lfs(AiOther(record).closing_rate_address()) > zero)) {
    return;
  }
  if ((AiOther(record).action_flags() & 4) != 0) {
    return;
  }
  double distance = Fneg(gap);
  if (Load(mode) != 0 && index > 1) {
    const double length = Lfs(RG::route_file().slice_length_address());
    const double slices = double(float(double(int64_t(index - 1))));
    distance = Fmadds(slices, length, distance);
  }
  if (distance < zero) {
    distance = zero;
  }
  double scale = Lfs(AiDriver(driver).speed_kmh_address());
  const double factor = Lfs(AiDriver(driver).distance_factor_address());
  const double hundred = Lfs(G::hundred_address());
  scale = scale > hundred ? Fdivs(hundred, scale) : Lfs(kOne);
  const double limit =
      Fmadds(Fmuls(scale, factor), distance, Lfs(AiOther(record).speed_address()));
  if (limit < Lfs(AiDriver(driver).speed_limit_other_address())) {
    Stfs(AiDriver(driver).speed_limit_other_address(), limit);
  }
  if (Lfs(AiOther(record).gap_address()) < Lfs(0x82008E48)) {
    return;
  }
  if (limit < Lfs(AiDriver(driver).speed_limit_address())) {
    Stfs(AiDriver(driver).speed_limit_address(), limit);
  }
}

uint64_t UpdateAgainstPlayer(uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82146608 AiDriver::UpdateAgainstPlayer");
  const int32_t count = int32_t(track_collision::RaceRacerCount());
  uint64_t r3 = uint32_t(count);
  StoreConstant(AiDriver(driver).player_gap_address(), kZero);
  AiDriver(driver).set_passes_a(0);
  AiDriver(driver).set_passes_b(0);
  for (int32_t i = 0; i < count; i++) {
    const uint32_t racer = Load(G::race().racers_address() + 4 * uint32_t(i));
    const uint32_t index = MinimapRacerEntry(racer).vehicle_index();
    if (int32_t(index) < 0 || AiDriver(driver).index() == index) {
      continue;
    }
    const uint32_t vehicle = vehicle_accessors::VehicleGet(index);
    r3 = vehicle;
    if ((Vehicle(vehicle).car_flags() & 129) == 0) {
      continue;
    }
    const uint32_t record = driver + index * 40 + 172;
    if (AiOther(record).flag_a() == 1) {
      if ((AiOther(record).history_flags() & 2) != 0) {
        AiDriver(driver).set_passes_a(uint16_t(AiDriver(driver).passes_a() + 1));
      }
      if ((AiOther(record).history_flags() & 4) != 0) {
        AiDriver(driver).set_passes_b(uint16_t(AiDriver(driver).passes_b() + 1));
      }
    }
    CopyWord(AiOther(record).gap_address(),
             AiDriver(driver).player_gap_address());
    break;
  }
  const double speed = Lfs(AiDriver(driver).speed_kmh_address());
  bool stuck_check = speed < Lfs(0x82008034);
  if (stuck_check) {
    r3 = IsFree(driver);
    stuck_check = r3 != 0;
  }
  if (!stuck_check) {
    AiDriver(driver).set_stuck_count(0);
    AiDriver(driver).set_flags(AiDriver(driver).flags() & ~0x20u);
    return r3;
  }
  const uint32_t flags = AiDriver(driver).flags();
  const uint32_t count492 = AiDriver(driver).stuck_count();
  if ((flags & 0x20) == 0) {
    const double held = double(float(double(int64_t(int16_t(count492)))));
    if (!(held < Lfs(0x82008238))) {
      AiDriver(driver).set_flags(flags | 32);
      return r3;
    }
    const double fifteen = Lfs(0x820029D0);
    if (!(speed < fifteen)) {
      return r3;
    }
    const uint32_t step = uint32_t(strings_and_font::steps::Fctiwz(Fsubs(speed, fifteen)));
    AiDriver(driver).set_stuck_count(uint16_t(count492 - (step & 0xFFFF)));
    return r3;
  }
  const uint32_t step = uint32_t(strings_and_font::steps::Fctiwz(speed));
  const int32_t left = int16_t(uint16_t(count492 - step));
  AiDriver(driver).set_stuck_count(uint16_t(left));
  if (left > 0) {
    return r3;
  }
  AiDriver(driver).set_stuck_count(0);
  AiDriver(driver).set_flags(flags & ~0x20u);
  return r3;
}

void UpdateOther(uint32_t record, uint32_t gap, uint32_t length) {
  ITC_NATIVE_RUNNING("sub_821454C8 AiOther::Update");
  const double old = Lfs(AiOther(record).gap_address());
  if (!(old < Lfs(0x82008E38)) && !(old > Lfs(G::other_gap_address()))) {
    const double change = Fsubs(Lfs(gap), old);
    const double last = Lfs(AiOther(record).last_change_address());
    Stfs(AiOther(record).last_change_address(), change);
    Stfs(AiOther(record).closing_rate_address(),
         Fmuls(Fadds(last, change), Lfs(G::half_address())));
    const double now = Lfs(gap);
    const double size = Lfs(length);
    const double zero = Lfs(kZero);
    double apart = Fadds(size, now);
    bool beside = false;
    if (!(apart < zero)) {
      apart = Fsubs(now, size);
      beside = !(apart > zero);
    }
    if (beside) {
      AiOther(record).set_state(2);
      Stfs(AiOther(record).ticks_to_meet_address(), zero);
    } else {
      const double rate = Lfs(AiOther(record).closing_rate_address());
      double meet;
      if (Fmuls(rate, apart) < zero) {
        const int32_t ticks = strings_and_font::steps::Fctiwz(Fneg(Fdivs(apart, rate)));
        meet = ticks < 10000 ? Fmuls(double(float(double(int64_t(ticks)))), Lfs(0x82002A3C))
                             : Lfs(G::minus_one_address());
      } else {
        meet = Lfs(G::minus_one_address());
      }
      Stfs(AiOther(record).ticks_to_meet_address(), meet);
      AiOther(record).set_state(Lfs(gap) > zero ? 3 : 1);
    }
  } else {
    const double zero = Lfs(kZero);
    Stfs(AiOther(record).closing_rate_address(), zero);
    AiOther(record).set_state(0);
    Stfs(AiOther(record).last_change_address(), zero);
    StoreConstant(AiOther(record).ticks_to_meet_address(), G::minus_one_address());
  }
  CopyWord(gap, AiOther(record).gap_address());
  UpdateOtherFlags(record);
}

// --- Resets, steering input and the path's lists ---

void ResetControl(uint32_t driver, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82139580 AiDriver::ResetControl");
  guest::StoreU32(frame + 80, 1);
  AiDriver(driver).set_flags(AiDriver(driver).flags() & ~1u);
  SetFlag2(driver, frame + 80);
  ResetToMode1(driver, frame - 112);
}

uint64_t ResetDriver(uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_8213AAA8 AiDriver::Reset");
  ResetFollow(driver);
  ResetFor(driver + 512, driver);
  const double zero = Lfs(kZero);
  Stfs(AiDriver(driver).target_speed_address(), zero);
  Stfs(AiDriver(driver).target_x_address(), zero);
  Stfs(AiDriver(driver).target_y_address(), zero);
  const double one = Lfs(kOne);
  Stfs(AiDriver(driver).target_z_address(), zero);
  Stfs(AiDriver(driver).target_w_address(), one);
  AiDriver(driver).set_handbrake(0);
  Stfs(AiDriver(driver).slide_angle_address(), zero);
  AiDriver(driver).set_update(0x8213A1B8);  // AiDriver::UpdateStarting
  for (uint32_t at : {6116u, 6120u, 6124u, 6128u, 6136u, 6140u, 6144u}) {
    Stfs(driver + at, zero);
  }
  return ResetOthers(driver);
}

uint64_t UpdateSteerInput(uint32_t driver, uint32_t stack, double* f1,
                          bool* f1_set) {
  ITC_NATIVE_RUNNING("sub_8213AC70 AiDriver::UpdateSteerInput");
  const int32_t count = LoadS16(AiDriver(driver).path_corner_count_address());
  for (int32_t i = 0; i < count; i++) {
    const uint32_t at = driver + 5676 + 44 * uint32_t(i);
    if (AiCorner(at).data().address == 0) {
      continue;
    }
    const int32_t kind = int32_t(AiCorner(at).kind());
    if (kind == 1) {
      break;
    }
    if (kind == 2) {
      return SteerIntoCorner(driver, at, stack - 96, f1, f1_set);
    }
    if (kind == 3) {
      return SteerOutOfCorner(driver, at, stack - 96);
    }
  }
  AccumulateSteer(driver);
  return driver;
}

void DropPassedSections(uint32_t path) {
  ITC_NATIVE_RUNNING("sub_8213BA48 AiPath::DropPassedSections");
  const int32_t count = LoadS16(AiPath(path).section_count_address());
  const uint32_t driver = AiPath(path).driver().address;
  if (count <= 0) {
    return;
  }
  const uint32_t list = path + 5146;
  const int32_t current = LoadS16(AiDriver(driver).route_section_address());
  if (LoadS16(list) == current) {
    return;
  }
  int32_t found = -1;
  for (int32_t i = 1; count > 1 && i < LoadS16(AiPath(path).section_count_address());
       i++) {
    if (LoadS16(list + 2 * uint32_t(i)) == current) {
      found = i;
      break;
    }
  }
  if (found < 0) {
    AiPath(path).set_section_count(0);
    for (uint32_t i = 0; i < 8; i++) {
      guest::StoreU16(list + 2 * i, 0xFFFF);
    }
    return;
  }
  int32_t moved = 0;
  if (count - found > 0) {
    int32_t j = 0;
    do {
      guest::StoreU16(list + 2 * uint32_t(j),
                      guest::LoadU16(list + 2 * uint32_t(j + found)));
      j++;
      moved = j;
    } while (j < LoadS16(AiPath(path).section_count_address()) - found);
  }
  for (int32_t i = moved; i < 8; i++) {
    guest::StoreU16(list + 2 * uint32_t(i), 0xFFFF);
  }
  AiPath(path).set_section_count(uint16_t(AiPath(path).section_count() - found));
}

uint32_t SharesSections(uint32_t path, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_8213C010 AiPath::SharesSections");
  const int32_t n = LoadS16(AiPath(path).section_count_address());
  if (n <= 0) {
    return 0;
  }
  const int32_t m = LoadS16(RouteCorner(record).section_count_address());
  const uint32_t list = path + 5146;
  for (int32_t i = 0; i < n; i++) {
    if (m <= 0) {
      continue;
    }
    const int32_t section = LoadS16(list + 2 * uint32_t(i));
    for (int32_t j = 0; j < m; j++) {
      if (section != LoadS16(record + 36 + 2 * uint32_t(j))) {
        continue;
      }
      const int32_t least = i > j ? j : i;
      int32_t a = i - least, b = j - least;
      if (!(a < n)) {
        return 1;
      }
      while (b < m) {
        if (guest::LoadU16(list + 2 * uint32_t(a)) !=
            guest::LoadU16(record + 36 + 2 * uint32_t(b))) {
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

void Widen(uint32_t entry, uint32_t margin, uint32_t shift) {
  ITC_NATIVE_RUNNING("sub_8213C3A8 AiPathEntry::Widen");
  Stfs(AiPathEntry(entry).room_low_address(),
       Fadds(Lfs(AiPathEntry(entry).range_low_address()), Lfs(margin)));
  const double m = Lfs(margin);
  const double high = Fsubs(Lfs(AiPathEntry(entry).range_high_address()), m);
  Stfs(AiPathEntry(entry).room_high_address(), high);
  const double by = Lfs(shift);
  if (by > Lfs(kZero)) {
    Stfs(AiPathEntry(entry).room_low_address(),
         Fadds(Lfs(AiPathEntry(entry).room_low_address()), by));
  } else {
    Stfs(AiPathEntry(entry).room_high_address(), Fadds(by, high));
  }
  const double lo = Lfs(AiPathEntry(entry).room_low_address());
  const double hi = Lfs(AiPathEntry(entry).room_high_address());
  if (!(lo > hi)) {
    return;
  }
  const double middle = Fmuls(Fadds(hi, lo), Lfs(G::half_address()));
  Stfs(AiPathEntry(entry).room_high_address(), middle);
  Stfs(AiPathEntry(entry).room_low_address(), middle);
}

void NarrowToOther(uint32_t entry, uint32_t other, uint32_t margin,
                   uint32_t offset) {
  ITC_NATIVE_RUNNING("sub_8213C420 AiPathEntry::NarrowToOther");
  if (other == 0) {
    return;
  }
  const uint32_t record = AiPathEntry(other).slice_record().address;
  if (record == 0) {
    return;
  }
  const double m = Lfs(margin);
  double lo = Fsubs(Lfs(AiPathEntry(other).range_low_address()), m);
  double hi = Fadds(Lfs(AiPathEntry(other).range_high_address()), m);
  const double least = Lfs(RouteSlice(record).route_bit_a_address());
  if (lo < least) {
    lo = least;
  }
  const double most = Lfs(RouteSlice(record).route_bit_b_address());
  if (hi > most) {
    hi = most;
  }
  const double by = Lfs(offset);
  double a = Fadds(by, lo);
  double b = Fadds(by, hi);
  if (a > b) {
    b = Fmuls(Fadds(b, a), Lfs(G::half_address()));
    a = b;
  }
  if (Lfs(AiPathEntry(entry).range_low_address()) < a) {
    Stfs(AiPathEntry(entry).range_low_address(), a);
    Stfs(AiPathEntry(entry).room_low_address(), a);
  }
  if (Lfs(AiPathEntry(entry).range_high_address()) > b) {
    Stfs(AiPathEntry(entry).range_high_address(), b);
    Stfs(AiPathEntry(entry).room_high_address(), b);
  }
}

bool AdvanceCorners(uint32_t path, uint32_t slices, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_8213D020 AiPath::AdvanceCorners");
  // The records' moves are memcpy's (sub_825A7410): natively only with
  // nothing for its override to record (outside the physical views) and the
  // records in one view.
  const uint32_t records = path + 5164;
  if (records >= 0xA0000000u || !c_runtime::InOneView(records, 9 * 44)) {
    return false;
  }
  *r3 = path;
  const int32_t last = LoadS16(AiPath(path).corner_count_address()) - 1;
  if (last < 0) {
    return true;
  }
  const double zero = Lfs(kZero);
  const double one = Lfs(kOne);
  for (int32_t k = last; k >= 0; k--) {
    const uint32_t record = path + 5164 + 44 * uint32_t(k);
    AiCorner(record)
        .set_end(uint16_t(AiCorner(record).end() - guest::LoadU16(slices)));
    const uint32_t step = guest::LoadU16(slices);
    const int32_t left = LoadS16(AiCorner(record).end_address());
    AiCorner(record).set_entry_offset(uint16_t(AiCorner(record).entry_offset() - step));
    if (left > 0) {
      continue;
    }
    int32_t j = k;
    if (k < LoadS16(AiPath(path).corner_count_address()) - 1) {
      uint32_t to = record;
      do {
        // Adjacent records don't share memory: c_runtime::Memcpy copies them
        // (the host's memmove, which copies them the same, otherwise).
        if (!c_runtime::Memcpy(to, to + 44, 44)) {
          std::memmove(guest::Pointer(to), guest::Pointer(to + 44), 44);
        }
        *r3 = to;  // memcpy's r3
        j++;
        to += 44;
      } while (j < LoadS16(AiPath(path).corner_count_address()) - 1);
    }
    const uint32_t cleared = path + 5164 + 44 * uint32_t(j);
    Stfs(AiCorner(cleared).direction_address(), one);
    AiCorner(cleared).set_data(0);
    Stfs(AiCorner(cleared).speed_address(), zero);
    AiCorner(cleared).set_table(0);
    Stfs(AiCorner(cleared).slope_before_address(), zero);
    AiCorner(cleared).set_number(0);
    Stfs(AiCorner(cleared).slope_after_address(), zero);
    AiCorner(cleared).set_entry_offset(0);
    AiCorner(cleared).set_end(0);
    AiCorner(cleared).set_position(0);
    AiCorner(cleared).set_unknown_24(0);
    AiCorner(cleared).set_joined_before(0);
    AiCorner(cleared).set_joined_after(0);
    AiCorner(cleared).set_kind(0);
    AiPath(path).set_corner_count(uint16_t(AiPath(path).corner_count() - 1));
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
void SetStateWord(uint32_t driver, uint32_t state) {
  const uint32_t old = AiDriver(driver).state();
  AiDriver(driver).set_state(state);
  AiDriver(driver).set_previous_state(old);
}
}  // namespace

uint32_t Restart(uint32_t driver, uint32_t mask, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_821395D0 AiDriver::Restart");
  ResetControl(driver, frame - 112);
  const uint32_t r3 = ResetPath(driver + 512);
  const uint32_t old = AiDriver(driver).state();
  AiDriver(driver).set_previous_state(old);
  AiDriver(driver).set_state(5);
  AiDriver(driver).set_racer_mask(uint16_t(Load(mask)));
  return r3;
}

uint32_t ResetAll(uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82139750 AiDrivers::ResetAll");
  const int32_t count = int32_t(track_collision::RaceRacerCount());
  uint32_t r3 = uint32_t(count);
  uint32_t mask = 0;
  guest::StoreU32(frame + 80, mask);
  uint32_t table = Load(G::drivers_address());
  if (table == 0) {
    return r3;
  }
  uint32_t race = Load(G::race_address());
  if (count <= 0) {
    return r3;
  }
  for (int32_t i = 0; i < count; i++) {
    const uint32_t racer = Load(RaceManager(race).racers_address() + 4 * uint32_t(i));
    if ((MinimapRacerEntry(racer).flags() & 0x10) != 0) {
      continue;
    }
    const uint32_t index = MinimapRacerEntry(racer).vehicle_index();
    if (int32_t(index) < 0) {
      continue;
    }
    mask |= Slw(1, index);
  }
  guest::StoreU32(frame + 80, mask);
  for (int32_t i = 0; i < count; i++) {
    const uint32_t racer = Load(RaceManager(race).racers_address() + 4 * uint32_t(i));
    if ((MinimapRacerEntry(racer).flags() & 0x10) != 0) {
      continue;
    }
    const uint32_t index = MinimapRacerEntry(racer).vehicle_index();
    if (int32_t(index) < 0) {
      continue;
    }
    r3 = Restart(index * AiDriver::kSize + table, frame + 80, frame - 112);
    table = Load(G::drivers_address());
    race = Load(G::race_address());
  }
  return r3;
}

uint32_t SetState2or3(uint32_t driver, uint32_t word, uint32_t flag,
                      uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82139E40 AiDriver::SetState2or3");
  ResetControl(driver, frame - 112);
  ClearPlan(driver);
  const bool set = Load(flag) != 0;
  const uint32_t r3 = ResetPath(driver + 512);
  SetStateWord(driver, set ? 3 : 2);
  AiDriver(driver).set_racer_mask(uint16_t(Load(word)));
  return r3;
}

uint32_t SetStateFromList(uint32_t driver, uint32_t word, uint32_t count,
                          uint32_t list, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82139EA8 AiDriver::SetStateFromList");
  ResetControl(driver, frame - 112);
  const uint32_t r3 = ClearPlan(driver);
  const int32_t n = int32_t(Load(count));
  if (n <= 1) {
    return r3;
  }
  if (n <= 2) {
    const uint32_t reset = ResetPath(driver + 512);
    const uint32_t old = AiDriver(driver).state();
    AiDriver(driver).set_previous_state(old);
    AiDriver(driver).set_state(3);
    AiDriver(driver).set_racer_mask(uint16_t(Load(word)));
    return reset;
  }
  int32_t k = 0;
  const int32_t me = int32_t(AiDriver(driver).index());
  for (; k < n; k++) {
    if (me == int32_t(Load(list + 4 * uint32_t(k)))) {
      break;
    }
  }
  if (!(k < n)) {
    return r3;
  }
  uint32_t state = 3;
  uint32_t other;
  if (k == 0) {
    other = Load(list + 4);
  } else if (k == 1) {
    other = Load(list + 0);
  } else {
    other = Load(list + 4 * uint32_t(k - 2));
    state = 2;
  }
  const uint32_t mask = Slw(1, other);
  const uint32_t reset = ResetPath(driver + 512);
  const uint32_t old = AiDriver(driver).state();
  AiDriver(driver).set_racer_mask(uint16_t(mask));
  AiDriver(driver).set_state(state);
  AiDriver(driver).set_previous_state(old);
  return reset;
}

uint32_t SetState4(uint32_t driver, uint32_t word, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82139FA0 AiDriver::SetState4");
  ResetControl(driver, frame - 112);
  ClearPlan(driver);
  const uint32_t r3 = ResetPath(driver + 512);
  const uint32_t old = AiDriver(driver).state();
  AiDriver(driver).set_previous_state(old);
  AiDriver(driver).set_state(4);
  AiDriver(driver).set_racer_mask(uint16_t(Load(word)));
  return r3;
}

uint32_t ClearAll(uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213A140 AiDrivers::ClearAll");
  const int32_t count = int32_t(track_collision::RaceRacerCount());
  uint32_t r3 = uint32_t(count);
  if (Load(G::drivers_address()) == 0 || count <= 0) {
    return r3;
  }
  for (int32_t i = 0; i < count; i++) {
    const uint32_t table = Load(G::drivers_address());
    const uint32_t racer = Load(G::race().racers_address() + 4 * uint32_t(i));
    const uint32_t driver =
        MinimapRacerEntry(racer).vehicle_index() * AiDriver::kSize + table;
    ResetControl(driver, frame - 112);
    r3 = ClearPlan(driver);
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

uint32_t JoinCorners(uint32_t a, uint32_t b, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_8213BCD0 AiPath::JoinCorners");
  const uint32_t data_b = AiCorner(b).data().address;
  uint32_t shift = 0;
  const uint32_t data_a = AiCorner(a).data().address;
  const uint32_t ends = data_b + 48;
  const int32_t have = int32_t(Load(count));
  const double f0 = Lfs(RouteCorner(data_b).length_address());
  double f13 = Fadds(Lfs(RouteCorner(data_a).speed_b_address()), f0);
  const int32_t start = LoadS16(RouteCorner(data_b).entry_count_address());
  const int32_t gap = LoadS16(ends + 6) - start;
  if (have < gap) {
    shift = uint32_t(gap - have);
    guest::StoreU32(count, uint32_t(gap));
  }
  const int32_t exit = LoadS16(RouteCorner(data_a).turn_in_address());
  const double f12a = Lfs(RouteCorner(data_b).length_address());
  const uint32_t now = Load(count);
  const uint32_t table_a = AiCorner(a).table().address;
  const uint32_t r3 = AiCorner(b).table().address;
  double one = Lfs(kOne);
  f13 = Fdivs(one, f13);
  const double f11a = Lfs(((shift << 3) & 0xFFFFFFF8u) + table_a + 4);
  const uint32_t at_b = ((now + uint32_t(start)) << 3) & 0xFFFFFFF8u;
  const int32_t enter = LoadS16(ends + 4);
  const double f10 = Fmuls(f12a, f13);
  const int32_t span = int32_t(now - uint32_t(enter) + uint32_t(start));
  double f12 = Fsubs(f11a, Lfs(at_b + r3 - 4));
  const double from = IntToFloat(exit);
  const double f9 = Fneg(f12);
  double f8 = Fadds(IntToFloat(span), from);
  f8 = Fmuls(f8, Lfs(G::half_address()));
  one = Fdivs(one, f8);
  f12 = Fmuls(Fmuls(f10, one), f12);
  Stfs(AiCorner(b).slope_after_address(), f12);
  double f13b = Fmuls(Lfs(RouteCorner(data_a).speed_b_address()), f13);
  f13b = Fmuls(f13b, f9);
  Stfs(AiCorner(a).slope_before_address(), Fmuls(f13b, one));
  AiCorner(b).set_joined_after(1);
  AiCorner(a).set_joined_before(1);
  const int32_t most = LoadS16(ends + 4);
  if (LoadS16(AiCorner(b).position_address()) > most) {
    AiCorner(b).set_position(uint16_t(most));
  }
  AiCorner(a).set_position(uint16_t(shift));
  return r3;
}

uint32_t JoinCornersShort(uint32_t a, uint32_t b, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_8213BF28 AiPath::JoinCornersShort");
  const uint32_t data_a = AiCorner(a).data().address;
  const uint32_t have = Load(count);
  const uint32_t data_b = AiCorner(b).data().address;
  const uint32_t table_a = AiCorner(a).table().address;
  const uint32_t r3 = AiCorner(b).table().address;
  double f12 = Lfs(RouteCorner(data_a).speed_b_address());
  double f0 = Lfs(RouteCorner(data_b).length_address());
  double f13 = Lfs(RouteCornerEntry(table_a).offset_address());
  const uint32_t ends = data_b + 48;
  f12 = Fadds(f12, f0);
  const int32_t exit =
      int32_t(uint32_t(LoadS16(RouteCorner(data_a).turn_in_address())) + have);
  const int32_t start = LoadS16(RouteCorner(data_b).entry_count_address());
  f13 = Fsubs(f13, Lfs(((uint32_t(start) << 3) & 0xFFFFFFF8u) + r3 - 4));
  const int32_t enter = LoadS16(ends + 4);
  AiCorner(b).set_joined_after(1);
  const double f10 = Fneg(f13);
  const int32_t span = int32_t(uint32_t(start) - uint32_t(enter) + have);
  const double f9 = IntToFloat(exit);
  f12 = Fdivs(Lfs(kOne), f12);
  f0 = Fmuls(f12, f0);
  f0 = Fmuls(f0, f13);
  f0 = Fdivs(f0, IntToFloat(span));
  Stfs(AiCorner(b).slope_after_address(), f0);
  f0 = Lfs(RouteCorner(data_a).speed_b_address());
  AiCorner(a).set_joined_before(1);
  f0 = Fmuls(f0, f12);
  f0 = Fmuls(f0, f10);
  Stfs(AiCorner(a).slope_before_address(), Fdivs(f0, f9));
  const int32_t most = LoadS16(ends + 4);
  if (LoadS16(AiCorner(b).position_address()) > most) {
    AiCorner(b).set_position(uint16_t(most));
  }
  AiCorner(a).set_position(0);
  return r3;
}

void JoinCornersByExit(uint32_t a, uint32_t b, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_8213BDF0 AiPath::JoinCornersByExit");
  const uint32_t data_b = AiCorner(b).data().address;
  const uint32_t data_a = AiCorner(a).data().address;
  const uint32_t have = Load(count);
  const int32_t exit = LoadS16(RouteCorner(data_a).turn_in_address());
  uint32_t at_b = uint32_t(LoadS16(RouteCorner(data_b).entry_count_address())) - 1;
  uint32_t at_a = have;
  if (int32_t(have) > exit) {
    at_a = uint32_t(exit);
    at_b = (uint32_t(exit) - have) + at_b;
  }
  const uint32_t span = uint32_t(LoadS16(RouteCorner(data_a).turn_in_address())) - have;
  const double to = Lfs(((at_b << 3) & 0xFFFFFFF8u) + AiCorner(b).table().address + 4);
  const double from =
      Lfs(((at_a << 3) & 0xFFFFFFF8u) + AiCorner(a).table().address + 4);
  AiCorner(a).set_position(uint16_t(at_a));
  const double rise = Fsubs(to, from);
  AiCorner(a).set_joined_before(1);
  Stfs(AiCorner(a).slope_before_address(), Fdivs(rise, IntToFloat(int32_t(span))));
}

uint32_t JoinCornersByEntry(uint32_t a, uint32_t b, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_8213BE80 AiPath::JoinCornersByEntry");
  const uint32_t data_b = AiCorner(b).data().address;
  const uint32_t data_a = AiCorner(a).data().address;
  const uint32_t have = Load(count);
  const uint32_t ends = data_b + 48;
  uint32_t at_b = uint32_t(LoadS16(RouteCorner(data_b).entry_count_address())) - 1;
  const int32_t exit = LoadS16(RouteCorner(data_a).turn_in_address());
  uint32_t at_a = have;
  if (int32_t(have) > exit) {
    at_a = uint32_t(exit);
    at_b = (uint32_t(exit) - have) + at_b;
  }
  const uint32_t table_b = AiCorner(b).table().address;
  const uint32_t r3 = AiCorner(a).table().address;
  const double from_b = Lfs(((at_b << 3) & 0xFFFFFFF8u) + table_b + 4);
  const double from_a = Lfs(((at_a << 3) & 0xFFFFFFF8u) + r3 + 4);
  const double rise = Fsubs(from_a, from_b);
  const int32_t enter = LoadS16(ends + 4);
  AiCorner(b).set_joined_after(1);
  const int32_t held = LoadS16(AiCorner(b).position_address());
  Stfs(AiCorner(b).slope_after_address(),
       Fdivs(rise, IntToFloat(int32_t(at_b - uint32_t(enter)))));
  const int32_t most = LoadS16(ends + 4);
  if (held > most) {
    AiCorner(b).set_position(uint16_t(most));
  }
  AiCorner(a).set_position(uint16_t(at_a));
  return r3;
}

bool JoinCornerPair(uint32_t a, uint32_t b, uint32_t frame, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_8213DD40 AiPath::JoinCornerPair");
  const uint32_t data_b = AiCorner(b).data().address;
  if (data_b == 0 || AiCorner(b).table().address == 0 ||
      AiCorner(a).data().address == 0 || AiCorner(a).table().address == 0) {
    return false;
  }
  const int32_t gap =
      int32_t(uint32_t(LoadS16(AiCorner(a).entry_offset_address())) -
              uint32_t(LoadS16(AiCorner(b).entry_offset_address())) -
              uint32_t(LoadS16(RouteCorner(data_b).entry_count_address())));
  guest::StoreU32(frame + 80, uint32_t(gap));
  if (gap < 0) {
    if (!(Lfs(AiCorner(a).direction_address()) ==
          Lfs(AiCorner(b).direction_address()))) {
      *r3 = JoinCorners(a, b, frame + 80);
      return true;
    }
    guest::StoreU32(frame + 80, uint32_t(-gap));
    if (Lfs(RouteCorner(data_b).length_address()) < Lfs(G::hundred_address())) {
      JoinCornersByExit(a, b, frame + 80);  // r3 kept
      return false;
    }
    *r3 = JoinCornersByEntry(a, b, frame + 80);
    return true;
  }
  const double length = Lfs(RG::route_file().slice_length_address());
  if (Fmuls(length, IntToFloat(gap)) > Lfs(G::hundred_address())) {
    return false;
  }
  *r3 = JoinCornersShort(a, b, frame + 80);
  return true;
}

void AddCornersAt(uint32_t path, uint32_t entry, uint32_t frame, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_8213E970 AiPath::AddCornersAt");
  *r3 = path;
  const uint32_t slice = AiPathEntry(entry).slice_record().address;
  uint32_t listed = slice + 60;
  const int32_t n = LoadS16(RouteSlice(slice).corner_count_address());
  if (n <= 0) {
    return;
  }
  const int32_t have = LoadS16(AiPath(path).corner_count_address());
  if (have > 0) {
    const int32_t entries = LoadS16(AiPath(path).entry_count_address());
    if (LoadS16(uint32_t(have) * 44 + path + 5132) > entries) {  // the last's +12
      return;
    }
  }
  const uint32_t route = RG::route_file().address;
  for (int32_t k = 0; k < n; k++, listed += 4) {
    if (have > 0) {
      const int32_t number = guest::LoadU16(listed + 0);
      uint32_t at = path + 5172;  // the records' +8
      int32_t i = 0;
      bool listed_already = false;
      do {
        if (number == LoadS16(at)) {
          listed_already = true;
          break;
        }
        i++;
        at += 44;
      } while (i < LoadS16(AiPath(path).corner_count_address()));
      if (listed_already) {
        continue;
      }
    }
    const uint32_t data = Load((uint32_t(guest::LoadU16(listed + 0)) << 2) +
                                         TrackRouteFile(route).corners());
    if (data == 0) {
      continue;
    }
    if (!(LoadS16(RouteCorner(data).exit_address()) >
          int32_t(guest::LoadU16(listed + 2)))) {
      continue;
    }
    const uint32_t shares = SharesSections(path, data);
    *r3 = shares;
    if (int32_t(shares) != 1) {
      continue;
    }
    if (!(have < 8)) {
      return;
    }
    const uint32_t record = uint32_t(have) * 44 + path + 5164;
    InitCorner(record, data, listed, path + 5136);
    *r3 = record;
    const int32_t now = LoadS16(AiPath(path).corner_count_address());
    if (now > 0) {
      const uint32_t before = uint32_t(now) * 44 + path + 5120;
      uint32_t joined = path;
      JoinCornerPair(record, before, frame - 112, &joined);
      *r3 = joined;
    }
    AddCornersAtFinish(path);
    return;
  }
}

void AddCornersAtFinish(uint32_t path) {
  AiPath(path).set_corner_count(uint16_t(AiPath(path).corner_count() + 1));
}

// --- The states' speeds ---

uint64_t UpdateState2(uint32_t driver, uint32_t speed) {
  ITC_NATIVE_RUNNING("sub_82145F48 AiDriver::UpdateState2");
  PickTarget(driver);
  const uint32_t target = AiDriver(driver).watched();
  if (int32_t(target) < 0) {
    return driver;
  }
  const uint32_t record = target * 40 + driver;
  const double gap = Lfs(record + 172);
  uint64_t r3 = driver;
  double f0;
  if (gap > Lfs(0x82002834)) {
    if (!(gap < Lfs(G::hundred_address()))) {
      f0 = Lfs(G::twenty_address());
    } else {
      r3 = OfIndex(target);
      f0 = Fmuls(Lfs(uint32_t(r3) + 100), Lfs(0x82002A2C));
      const double most = Lfs(0x82000F14);
      if (f0 > most) {
        f0 = most;
      }
    }
  } else {
    const double zero = Lfs(kZero);
    if (!(gap < zero)) {
      return r3;
    }
    r3 = OfIndex(target);
    const double again = Lfs(record + 172);
    if (!(again > Lfs(0x82008E54))) {
      f0 = again > Lfs(0x82008E4C)
               ? Fmuls(Lfs(uint32_t(r3) + 100), Lfs(0x820029D4))
               : Lfs(G::five_hundred_address());
      if (f0 > Lfs(speed)) {
        Stfs(speed, f0);
      }
      return r3;
    }
    const bool near = again < Lfs(0x82008E50);
    f0 = Lfs(uint32_t(r3) + 100);
    if (!near) {
      const bool slow = f0 < Lfs(0x82000F14);
      const double factor = Lfs(0x82002A2C);
      if (!slow) {
        f0 = Fmuls(f0, factor);
      } else {
        f0 = Fmuls(f0, factor) < Lfs(G::twenty_address()) ? zero
                                                                : Lfs(G::twenty_address());
      }
    }
  }
  if (f0 < Lfs(speed)) {
    Stfs(speed, f0);
  }
  return r3;
}

uint64_t UpdateState3(uint32_t driver, uint32_t speed) {
  ITC_NATIVE_RUNNING("sub_821460C8 AiDriver::UpdateState3");
  double f11 = Lfs(speed);
  PickTarget(driver);
  const uint32_t target = AiDriver(driver).watched();
  if (int32_t(target) < 0) {
    return driver;
  }
  const uint32_t record = target * 40 + driver;
  const double gap = Lfs(record + 172);
  uint64_t r3 = driver;
  if (gap > Lfs(kZero)) {
    r3 = OfIndex(target);
    const double again = Lfs(record + 172);
    double f0;
    if (again < Lfs(0x82002674)) {
      f0 = Lfs(uint32_t(r3) + 100);
    } else if (again < Lfs(G::ten_address())) {
      f0 = Fmuls(Lfs(uint32_t(r3) + 100), Lfs(0x82002A2C));
    } else if (again < Lfs(G::hundred_address())) {
      f0 = Fmuls(Lfs(uint32_t(r3) + 100), Lfs(0x82002A2C));
      const double most = Lfs(0x82000F14);
      if (!(f0 < most)) {
        f0 = most;
      }
    } else {
      f0 = Lfs(G::twenty_address());
    }
    if (f0 < Lfs(speed)) {
      Stfs(speed, f0);
    }
    return r3;
  }
  if (gap < Lfs(0x8200895C)) {
    f11 = Lfs(G::five_hundred_address());
  } else if (gap < Lfs(0x82008E58)) {
    r3 = OfIndex(target);
    f11 = Fmuls(Lfs(uint32_t(r3) + 100), Lfs(0x820029D4));
  }
  if (f11 > Lfs(speed)) {
    Stfs(speed, f11);
  }
  return r3;
}

uint64_t UpdateState6(uint32_t driver, uint32_t speed) {
  ITC_NATIVE_RUNNING("sub_821462A0 AiDriver::UpdateState6");
  PickTargetBehind(driver);
  const uint32_t target = AiDriver(driver).watched();
  if (int32_t(target) < 0) {
    return driver;
  }
  const uint64_t other = OfIndex(target);
  const uint32_t theirs = uint32_t(other);
  if (int32_t(AiDriver(theirs).state()) != 6) {
    return other;
  }
  const uint32_t again = AiDriver(driver).watched();
  const double mine = Lfs(AiDriver(driver).car_half_length_address());
  const double their_length = Lfs(AiDriver(theirs).car_half_length_address());
  double f12 = Lfs(AiDriver(theirs).speed_kmh_address());
  const double f0 = Lfs(0x820021E8);
  const double gap = Fneg(Lfs(again * 40 + driver + 172));
  const double closing = Fsubs(Fnmsubs(mine, f0, gap), their_length);
  if (!(closing < Lfs(kZero))) {
    double scale = Lfs(AiDriver(driver).speed_kmh_address());
    const double factor = Lfs(AiDriver(driver).distance_factor_address());
    const double hundred = Lfs(G::hundred_address());
    scale = scale > hundred ? Fdivs(hundred, scale) : Lfs(kOne);
    f12 = Fmadds(Fmuls(scale, factor), closing, f12);
  }
  const double limit = Fmuls(f12, Lfs(0x82002A3C));
  if (Lfs(speed) > limit) {
    Stfs(speed, limit);
  }
  return other;
}

uint64_t UpdateState(uint32_t driver, uint32_t speed) {
  ITC_NATIVE_RUNNING("sub_82139AC8 AiDriver::UpdateState");
  switch (int32_t(AiDriver(driver).state())) {
    case 2:
      return UpdateState2(driver, speed);
    case 3:
      return UpdateState3(driver, speed);
    case 4:
      UpdateState4(driver, speed);
      return driver;
    case 5:
      return UpdateState5(driver, speed);
    case 6: {
      double f0 =
          Fsubs(Lfs(AiDriver(driver).speed_kmh_address()), Lfs(G::ten_address()));
      const double least = Lfs(0x82008948);
      if (f0 < least) {
        f0 = least;
      }
      if (f0 < Lfs(AiDriver(driver).target_speed_address())) {
        Stfs(AiDriver(driver).target_speed_address(), f0);
      }
      return UpdateState6(driver, speed);
    }
    default:
      return driver;
  }
}

// --- The target speed ---

namespace {
// The ring entry k (from the path's start +5138, modulo 80) when k is below
// the entries' count +5136, else 0.
uint32_t CountedRingEntry(uint32_t path, int32_t k) {
  if (!(LoadS16(AiPath(path).entry_count_address()) > k)) {
    return 0;
  }
  const int32_t at =
      int32_t(uint32_t(LoadS16(AiPath(path).first_address())) + uint32_t(k));
  return path + uint32_t(at - (at / 80) * 80) * 64 + 16;
}
}  // namespace

uint64_t LimitForCorner(uint32_t driver, uint32_t scale, uint32_t record,
                        uint32_t frame, double* f1, bool* f1_set) {
  ITC_NATIVE_RUNNING("sub_8213A620 AiDriver::LimitForCorner");
  uint64_t r3 = driver;
  CopyWord(scale, frame + 88);
  const uint32_t ahead = driver + 5654;  // the look-ahead (the path's +5142)
  const uint32_t data = AiCorner(record).data().address;
  const double zero = Lfs(kZero);
  Stfs(frame + 84, zero);
  const uint32_t speeds = data + 12;
  const uint32_t target = data + 8;
  const int32_t offset = LoadS16(AiCorner(record).entry_offset_address());
  const uint32_t before = uint32_t(1 - offset);
  const int32_t entries = LoadS16(AiDriver(driver).path_entry_count_address());
  const int32_t start = int32_t(uint32_t(LoadS16(RouteCorner(data).turn_in_address())) +
                                uint32_t(offset));
  const int32_t middle_at = LoadS16(RouteCorner(data).apex_address());
  Stfs(AiCorner(record).speed_address(), zero);
  guest::StoreU32(frame + 92, before);
  const int32_t end = int32_t(uint32_t(middle_at) + uint32_t(offset));
  const uint32_t path = driver + 512;
  if (start >= 0 && start < entries) {
    const uint32_t entry = CountedRingEntry(path, start);
    if (entry != 0) {
      double a, b;
      if (Lfs(AiCorner(record).direction_address()) < zero) {
        a = Lfs(AiDriver(driver).road_offset_address());
        b = Lfs(AiPathEntry(entry).room_low_address());
      } else {
        a = Lfs(AiPathEntry(entry).room_high_address());
        b = Lfs(AiDriver(driver).road_offset_address());
      }
      Stfs(AiCorner(record).speed_address(), Fsubs(a, b));
    }
  }
  if (int32_t(AiDriver(driver).path_mode()) == 1) {
    AiCorner(record).set_kind(0);
  } else {
    int32_t skill = LoadS16(AiDriver(driver).skill_corner_address());
    if (skill < -100) {
      skill = -100;
    } else if (skill > 100) {
      skill = 100;
    }
    guest::StoreU16(frame + 80, uint16_t(skill));
    ClearCornerKind(record);
    r3 = record;
  }
  if (entries > 1) {
    LimitByCurve(driver, record, frame + 84, frame + 92, CountedRingEntry(path, 1));
    r3 = driver;
    const int32_t look = LoadS16(ahead);
    const int32_t next = look + 1;
    if (entries > next) {
      const uint32_t entry = CountedRingEntry(path, next);
      guest::StoreU32(frame + 92, uint32_t(look) + before);
      LimitByCurve(driver, record, frame + 84, frame + 92, entry);
    }
  }
  const auto blend = [&](uint32_t pair, uint32_t slices) {
    guest::StoreU32(frame + 92, slices);
    *f1 = LimitSpeedBlend(driver, frame + 88, pair, target, frame + 84, frame + 92,
                          frame - 96);
    *f1_set = true;
    r3 = driver;
  };
  uint32_t last;
  if (end >= 0) {
    if (start > 0) {
      blend(speeds, uint32_t(LoadS16(AiCorner(record).entry_offset_address()) -
                             LoadS16(ahead)));
    }
    if (start < end) {
      blend(speeds + 8, uint32_t(start - LoadS16(ahead)));
    }
    last = uint32_t(end - LoadS16(ahead));
  } else {
    if (int32_t(uint32_t(LoadS16(RouteCorner(data).exit_address())) +
                uint32_t(LoadS16(AiCorner(record).entry_offset_address()))) < 0) {
      return r3;
    }
    last = uint32_t(-end);
  }
  blend(speeds + 16, last);
  return r3;
}

uint64_t UpdateTargetSpeed(uint32_t driver, uint32_t scale, uint32_t frame,
                           double* f1, bool* f1_set) {
  ITC_NATIVE_RUNNING("sub_8213AB98 AiDriver::UpdateTargetSpeed");
  uint64_t r3 = driver;
  const uint32_t flags = AiDriver(driver).flags();
  const uint32_t vehicle = AiDriver(driver).vehicle().address;
  uint32_t from;
  if ((flags & 4) != 0) {
    from = driver + 12;
  } else {
    // The vehicle's flags read whatever the vehicle, as the original does.
    const uint32_t state = Vehicle(vehicle).car_flags();
    if ((state & 0x60000000u) != 0) {
      from = 0x82008930;  // 70.0
    } else if ((state & 0x1C000000u) != 0) {
      from = kZero;
    } else {
      from = 0x82008948;  // 73.0
    }
  }
  CopyWord(from, AiDriver(driver).target_speed_address());
  if (AiDriver(driver).state() != 0) {
    r3 = UpdateState(driver, driver + 6088);
  }
  if (LoadS16(AiDriver(driver).path_corner_count_address()) > 0) {
    int32_t i = 0;
    uint32_t record = driver + 5676;
    do {
      r3 = LimitForCorner(driver, scale, record, frame - 192, f1, f1_set);
      i++;
      record += 44;
    } while (i < LoadS16(AiDriver(driver).path_corner_count_address()));
  }
  return r3;
}

uint64_t UpdateSpeed(uint32_t driver, uint32_t frame, double* f1, bool* f1_set) {
  ITC_NATIVE_RUNNING("sub_8213ACD0 AiDriver::UpdateSpeed");
  int32_t skill = LoadS16(AiDriver(driver).skill_speed_scale_address());
  if (skill < -100) {
    skill = -100;
  } else if (skill > 100) {
    skill = 100;
  }
  const double f0 = Lfs(0x82002CE8);
  const double scale = Fmadds(IntToFloat(skill), f0, Lfs(kOne));
  Stfs(frame + 80, scale);
  return UpdateTargetSpeed(driver, frame + 80, frame - 128, f1, f1_set);
}

// --- Steering at the target and the pedals ---

uint32_t SteerToTarget(uint32_t driver, double* f1) {
  ITC_NATIVE_RUNNING("sub_82138D78 AiDriver::SteerToTarget");
  const uint32_t path = driver + 512;
  const uint32_t file = RG::route_file().address;
  const uint32_t slow = IsSlow(path);
  const double speed = Lfs(AiDriver(driver).speed_kmh_address());
  double reach = Fmuls(speed, Lfs(slow == 0 ? 0x82008944 : 0x82008940));
  const double least = Lfs(0x82002834);
  if (reach < least) {
    reach = least;
  }
  double more = Lfs(AiDriver(driver).slice_offset_address());
  const double zero = Lfs(kZero);
  if (more < zero) {
    more = zero;
  }
  const double ahead = Fadds(reach, more);
  *f1 = ahead;
  int32_t slices = strings_and_font::steps::Fctiwz(
      Fdivs(ahead, Lfs(TrackRouteFile(file).slice_length_address())));
  if (slices > 78) {
    slices = 78;
  }
  const uint32_t target = driver + 6096;
  AiDriver(driver).set_path_look_ahead(uint16_t(slices));
  if (TargetPoint(path, ahead, target) == 0) {
    return 0;
  }
  const double dx =
      Fsubs(Lfs(target + 0), Lfs(AiDriver(driver).point_x_address()));
  const double dz = Fsubs(Lfs(AiDriver(driver).target_z_address()),
                          Lfs(AiDriver(driver).point_z_address()));
  const uint32_t angle = driver + 6116;
  if (dx == zero && dz == zero) {
    Stfs(angle, zero);
  } else {
    const double toward = double(float(c_runtime::Atan2(dx, dz)));
    Stfs(angle, Fsubs(toward, Lfs(AiDriver(driver).heading_address())));
    WrapAngle(angle);
    Stfs(angle, Fneg(Lfs(angle)));
  }
  const uint32_t vehicle = AiDriver(driver).vehicle().address;
  const double cos = c_runtime::Cos(Lfs(angle));
  const double sin = c_runtime::Sin(Lfs(angle));
  *f1 = sin;
  const double s = double(float(sin));
  const double across = Lfs(Vehicle(vehicle).ground_velocity_address());
  const double c = double(float(cos));
  const uint32_t now = AiDriver(driver).vehicle().address;
  const double along = Lfs(Vehicle(vehicle).ground_velocity_address() + 8);
  const double turned = Lfs(angle);
  Stfs(AiDriver(driver).steer_shift_address(), Fmadds(along, c, Fmuls(s, across)));
  Stfs(AiDriver(driver).slide_angle_address(), Fadds(Lfs(now + 92), turned));
  WrapAngle(driver + 6112);
  return 1;
}

void UpdatePedals(uint32_t driver, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213A428 AiDriver::UpdatePedals");
  const uint32_t kind2 = HasKind2Ahead(driver + 512);
  ThrottleForSpeed(driver);
  const double zero = Lfs(kZero);
  uint32_t flag = 0;
  if (kind2 == 0) {
    const double yaw = Fabs(Lfs(AiDriver(driver).yaw_rate_address()));
    if (yaw > zero) {
      const double throttle = Lfs(AiDriver(driver).throttle_request_address());
      Stfs(AiDriver(driver).throttle_request_address(),
           throttle < yaw ? zero : Fsubs(throttle, yaw));
    }
    EaseThrottleForSteer(driver);
    flag = 1;
  }
  guest::StoreU32(frame + 80, flag);
  EaseThrottleForSlide(driver, frame + 80);
  const double brake = Lfs(AiDriver(driver).brake_request_address());
  if (brake > zero) {
    Stfs(AiDriver(driver).throttle_request_address(), zero);
    const double step = Lfs(0x8200296C);
    Stfs(AiDriver(driver).brake_request_address(),
         brake > step ? Fsubs(brake, step) : zero);
  }
  double steer =
      Fmuls(Lfs(AiDriver(driver).steer_shift_address()), Lfs(0x82002968));
  if (steer < zero) {
    steer = Fneg(steer);
  }
  double request = Lfs(AiDriver(driver).brake_request_address());
  const double one = Lfs(kOne);
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
    const double slide = Lfs(AiDriver(driver).steering_address());
    Stfs(AiDriver(driver).brake_request_address(), request);
    double turn = Fmuls(slide, Lfs(0x820021E8));
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
      Stfs(AiDriver(driver).brake_request_address(), request);
      kept = true;
    }
  }
  if (!kept) {
    Stfs(AiDriver(driver).brake_request_address(), zero);
  }
  const double limit = Lfs(G::fifth_address());
  const auto approach = [&](uint32_t now_at, uint32_t toward_at) {
    double move = Fsubs(Lfs(toward_at), Lfs(now_at));
    if (move > limit) {
      move = limit;
    }
    const double now = Fadds(Lfs(now_at), move);
    Stfs(now_at, now);
    if (now < zero) {
      Stfs(now_at, zero);
    } else if (now > one) {
      Stfs(now_at, one);
    }
  };
  approach(driver + 6124, driver + 6140);
  approach(driver + 6128, driver + 6144);
}

// --- The drivers' table and the path's lines and ranges ---

namespace {
// The ring entry k (from the path's start +5138, modulo 80, as divw
// truncates), without a check against the entries' count.
uint32_t RingAt(uint32_t path, int32_t k) {
  const int32_t at =
      int32_t(uint32_t(LoadS16(AiPath(path).first_address())) + uint32_t(k));
  return path + (uint32_t(at - (at / 80) * 80) << 6) + 16;
}
}  // namespace

bool Create(uint64_t* r3) {
  ITC_NATIVE_RUNNING("sub_8213B1E0 AiDrivers::Create");
  const auto block = memory::crt::OperatorNew(43120);
  if (!block) {
    return false;
  }
  const uint32_t made = *block;
  if (made != 0) {
    for (uint32_t i = 0; i < 7; i++) {
      InitPath(made + AiDriver::kSize * i);
    }
  }
  guest::StoreU32(G::drivers_address(), made);
  *r3 = made;
  if (made == 0) {
    return true;
  }
  guest::ForEachHostPart(made, 43120, [](uint8_t* host, uint32_t, uint32_t bytes) {
    std::memset(host, 0, bytes);
  });
  for (uint32_t i = 0; i < 7; i++) {
    const uint32_t vehicle = vehicle_accessors::VehicleGet(i);
    const uint32_t driver = AiDriver::kSize * i + Load(G::drivers_address());
    ClearSkill(driver + 6048);
    AiDriver(driver).set_profile(0);
    *r3 = ResetDriver(driver);
    guest::StoreU32(AiDriver::kSize * i + Load(G::drivers_address()), i);
    if (vehicle != 0) {
      guest::StoreU32(AiDriver::kSize * i + Load(G::drivers_address()) + 160, vehicle);
    }
  }
  return true;
}

uint64_t DestroyRings(uint64_t drivers) {
  ITC_NATIVE_RUNNING("sub_8213AD98 AiDrivers::DestroyRings");
  constexpr uint32_t kEmptyDestructor = 0x82106540;
  uint64_t ring = drivers + 7 * AiDriver::kSize + 528;
  for (int32_t i = 6; i >= 0; i--) {
    ring -= AiDriver::kSize;
    // The empty destructor is never called, so no guest call is needed.
    c_runtime::VectorDestruct(ring, 64, 80, kEmptyDestructor, nullptr, nullptr);
  }
  return ring;
}

bool Destroy(uint64_t* r3) {
  ITC_NATIVE_RUNNING("sub_8213B2B8 AiDrivers::Destroy");
  const uint32_t drivers = Load(G::drivers_address());
  if (!hooks::NativeHeapActive() || (drivers != 0 && !memory::IsHeapBlock(drivers))) {
    return false;
  }
  if (drivers != 0) {
    DestroyRings(drivers);
    memory::crt::OperatorDelete(drivers);
    *r3 = drivers;  // operator delete's adapter leaves r3 as the block
  }
  guest::StoreU32(G::drivers_address(), 0);
  return true;
}

void ApplyCornerLine(uint32_t path, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_8213BB58 AiPath::ApplyCornerLine");
  const uint32_t record = Load(index) * 44u + path + 5164;
  const uint32_t data = AiCorner(record).data().address;
  if (data == 0) {
    return;
  }
  const uint32_t table = AiCorner(record).table().address;
  if (table == 0) {
    return;
  }
  const int32_t count = LoadS16(RouteCorner(data).entry_count_address());
  const int32_t first = LoadS16(AiCorner(record).position_address());
  if (first >= count) {
    return;
  }
  int32_t start = int16_t(uint16_t(AiCorner(record).entry_offset() + uint32_t(first)));
  if (start >= LoadS16(AiPath(path).entry_count_address())) {
    return;
  }
  if (start < 0) {
    AiCorner(record).set_position(uint16_t(first - start));
    start = 0;
  }
  int32_t k = start;
  int32_t n = LoadS16(AiPath(path).entry_count_address());
  while (k < n) {
    const int32_t at = LoadS16(AiCorner(record).position_address());
    if (at >= count) {
      return;
    }
    const uint32_t entry = CountedRingEntry(path, k);
    CopyWord(uint32_t(at) * 8u + table + 4, AiPathEntry(entry).target_offset_address());
    if (AiCorner(record).joined_before() == 1) {
      const int32_t until = LoadS16(RouteCorner(data).turn_in_address());
      const int32_t now = LoadS16(AiCorner(record).position_address());
      if (now < until) {
        const double slope = Lfs(AiCorner(record).slope_before_address());
        const double offset = Lfs(AiPathEntry(entry).target_offset_address());
        Stfs(AiPathEntry(entry).target_offset_address(),
             Fmadds(IntToFloat(until - now), slope, offset));
      }
    }
    if (AiCorner(record).joined_after() == 1) {
      const int32_t from = LoadS16(RouteCorner(data).apex_address());
      const int32_t now = LoadS16(AiCorner(record).position_address());
      if (now > from) {
        const double slope = Lfs(AiCorner(record).slope_after_address());
        const double offset = Lfs(AiPathEntry(entry).target_offset_address());
        Stfs(AiPathEntry(entry).target_offset_address(),
             Fmadds(IntToFloat(now - from), slope, offset));
      }
    }
    AiCorner(record).set_position(uint16_t(AiCorner(record).position() + 1));
    k = int16_t(k + 1);
    n = LoadS16(AiPath(path).entry_count_address());
  }
}

void ApplyCornerLines(uint32_t path, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213CFC8 AiPath::ApplyCornerLines");
  int32_t i = 0;
  guest::StoreU32(frame + 80, 0);
  if (LoadS16(AiPath(path).corner_count_address()) <= 0) {
    return;
  }
  do {
    ApplyCornerLine(path, frame + 80);
    i++;
    guest::StoreU32(frame + 80, uint32_t(i));
  } while (i < LoadS16(AiPath(path).corner_count_address()));
}

void KeepToCornerSide(uint32_t path, uint32_t record, uint32_t count,
                      uint32_t margin, uint32_t shift) {
  ITC_NATIVE_RUNNING("sub_8213CA18 AiPath::KeepToCornerSide");
  int32_t k = LoadS16(AiCorner(record).entry_offset_address());
  if (k < 1) {
    k = 1;
  }
  const double direction = Lfs(AiCorner(record).direction_address());
  const int32_t n = int32_t(Load(count));
  double m = Lfs(margin);
  const double zero = Lfs(kZero);
  const double by = Lfs(shift);
  if (direction < zero) {
    if (by < zero) {
      m = Fadds(by, m);
    }
    for (; k < n; k++) {
      const uint32_t entry = RingAt(path, k);
      const double high = Fsubs(Lfs(AiPathEntry(entry).range_high_address()), m);
      if (high < Lfs(AiPathEntry(entry).room_high_address())) {
        Stfs(AiPathEntry(entry).room_high_address(), high);
      }
      const double low = Lfs(AiPathEntry(entry).room_low_address());
      if (Lfs(AiPathEntry(entry).room_high_address()) < low) {
        Stfs(AiPathEntry(entry).room_high_address(), low);
      }
    }
  } else {
    if (by > zero) {
      m = Fadds(by, m);
    }
    for (; k < n; k++) {
      const uint32_t entry = RingAt(path, k);
      const double low = Fadds(Lfs(AiPathEntry(entry).range_low_address()), m);
      if (low > Lfs(AiPathEntry(entry).room_low_address())) {
        Stfs(AiPathEntry(entry).room_low_address(), low);
      }
      const double high = Lfs(AiPathEntry(entry).room_high_address());
      if (Lfs(AiPathEntry(entry).room_low_address()) > high) {
        Stfs(AiPathEntry(entry).room_low_address(), high);
      }
    }
  }
}

uint32_t SetRanges(uint32_t path, uint32_t count, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213D670 AiPath::SetRanges");
  uint32_t r3 = path;
  const uint32_t driver = AiPath(path).driver().address;
  const uint32_t mode = AiPath(path).mode();
  const double steer = Lfs(AiDriver(driver).steer_shift_address());
  const double half = Lfs(AiDriver(driver).half_width_address());
  const double shift = Fmuls(steer, Lfs(G::fifth_address()));
  Stfs(frame + 80, shift);
  Stfs(frame + 84, Fadds(half, Lfs(0x82002A14)));
  if (int32_t(mode) == 1) {
    Stfs(frame + 80, Lfs(kZero));
  } else {
    const double lowest = Lfs(G::minus_one_address());
    if (shift < lowest) {
      Stfs(frame + 80, lowest);
    } else {
      const double highest = Lfs(kOne);
      if (shift > highest) {
        Stfs(frame + 80, highest);
      }
    }
  }
  for (int32_t i = 1; i < int32_t(Load(count)); i++) {
    r3 = RingAt(path, i);
    Widen(r3, frame + 84, frame + 80);
  }
  if (LoadS16(AiPath(path).corner_count_address()) > 0) {
    int32_t i = 0;
    uint32_t record = path + 5164;
    do {
      r3 = path;
      KeepToCornerSide(path, record, count, driver + 128, frame + 80);
      i++;
      record += 44;
    } while (i < LoadS16(AiPath(path).corner_count_address()));
  }
  return r3;
}

uint32_t DropOffRouteCorners(uint32_t path) {
  ITC_NATIVE_RUNNING("sub_8213DC08 AiPath::DropOffRouteCorners");
  if (LoadS16(AiPath(path).corner_count_address()) <= 0) {
    return path;
  }
  uint32_t r3 = 0;
  int32_t kept = 0;
  for (;;) {
    const uint32_t data = Load(path + 44 * uint32_t(kept) + 5164);
    r3 = data == 0 ? 0 : SharesSections(path, data);
    if (int32_t(r3) != 1) {
      break;
    }
    kept = int16_t(kept + 1);
    if (!(kept < LoadS16(AiPath(path).corner_count_address()))) {
      return r3;
    }
  }
  const double zero = Lfs(kZero);
  if (kept > 1) {
    const uint32_t last = path + 44 * uint32_t(kept) + 5120;  // record kept - 1
    if (AiCorner(last).joined_after() == 1) {
      const uint32_t data = AiCorner(last).data().address;
      AiCorner(last).set_joined_after(0);
      const int32_t at = LoadS16(AiCorner(last).position_address());
      const uint16_t most = RouteCorner(data).apex();
      if (at > int32_t(int16_t(most))) {
        AiCorner(last).set_position(most);
      }
      Stfs(AiCorner(last).slope_after_address(), zero);
    }
  }
  int32_t k = kept;
  if (k < LoadS16(AiPath(path).corner_count_address())) {
    const double one = Lfs(kOne);
    do {
      const uint32_t record = path + 44 * uint32_t(k) + 5164;
      k = int16_t(k + 1);
      Stfs(AiCorner(record).direction_address(), one);
      AiCorner(record).set_data(0);
      Stfs(AiCorner(record).speed_address(), zero);
      AiCorner(record).set_table(0);
      Stfs(AiCorner(record).slope_before_address(), zero);
      AiCorner(record).set_number(0);
      Stfs(AiCorner(record).slope_after_address(), zero);
      AiCorner(record).set_entry_offset(0);
      AiCorner(record).set_end(0);
      AiCorner(record).set_position(0);
      AiCorner(record).set_unknown_24(0);
      AiCorner(record).set_joined_before(0);
      AiCorner(record).set_joined_after(0);
      AiCorner(record).set_kind(0);
    } while (k < LoadS16(AiPath(path).corner_count_address()));
  }
  AiPath(path).set_corner_count(uint16_t(kept));
  return r3;
}

// --- The path's offsets, narrowing and branches ---

void SpreadOffset(uint32_t path, uint32_t first, uint32_t last, uint32_t amount,
                  uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213B890 AiPath::SpreadOffset");
  int32_t low = int32_t(Load(first));
  int32_t i = int32_t(Load(last));
  if (!(i > low)) {
    return;
  }
  const double share =
      Fdivs(Lfs(amount), IntToFloat(int32_t(uint32_t(i) - uint32_t(low) + 1)));
  for (;;) {
    i = int32_t(uint32_t(i) - 1);
    guest::StoreU32(frame + 80, uint32_t(i));
    if (i < low) {
      return;
    }
    const uint32_t entry = CountedRingEntry(path, i);
    const double offset = Lfs(AiPathEntry(entry).offset_address());
    const double least = Lfs(AiPathEntry(entry).range_low_address());
    const double moved =
        Fmadds(IntToFloat(int32_t(uint32_t(i) - uint32_t(low) + 1)), share, offset);
    uint32_t end;
    if (moved < least) {
      end = entry + 32;
    } else if (moved > Lfs(AiPathEntry(entry).range_high_address())) {
      end = entry + 36;
    } else {
      Stfs(AiPathEntry(entry).offset_address(), moved);
      low = int32_t(Load(first));
      i = int32_t(Load(frame + 80));
      continue;
    }
    Stfs(frame + 88,
         Fsubs(Lfs(end), Lfs(AiPathEntry(entry).offset_address())));
    SpreadOffset(path, first, frame + 80, frame + 88, frame - 128);
    const double bound = Lfs(end);
    CopyWord(end, AiPathEntry(entry).offset_address());
    int32_t high = int32_t(Load(last));
    const int32_t at = int32_t(Load(frame + 80));
    if (!(at < int32_t(uint32_t(high) - 1))) {
      return;
    }
    const double step =
        Fdivs(Fsubs(bound, moved), IntToFloat(int32_t(uint32_t(high) - uint32_t(at))));
    for (int32_t j = int32_t(uint32_t(at) + 1); j < high; j = int32_t(uint32_t(j) + 1)) {
      // The same entry each time, as the original does.
      const double now = Lfs(AiPathEntry(entry).offset_address());
      Stfs(AiPathEntry(entry).offset_address(),
           Fmadds(IntToFloat(int32_t(uint32_t(high) - uint32_t(j))), step, now));
      high = int32_t(Load(last));
    }
    return;
  }
}

void ClampOffset(uint32_t path, uint32_t entry, uint32_t last, uint32_t first,
                 uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213CF58 AiPath::ClampOffset");
  const double offset = Lfs(AiPathEntry(entry).offset_address());
  uint32_t end;
  if (offset < Lfs(AiPathEntry(entry).range_low_address())) {
    end = entry + 32;
  } else if (offset > Lfs(AiPathEntry(entry).range_high_address())) {
    end = entry + 36;
  } else {
    return;
  }
  Stfs(frame + 80, Fsubs(Lfs(end), offset));
  SpreadOffset(path, first, last, frame + 80, frame - 128);
  CopyWord(end, AiPathEntry(entry).offset_address());
}

uint32_t NarrowToOtherEx(uint32_t entry, uint32_t other, uint32_t step,
                         uint32_t offset, uint32_t low, uint32_t high, uint32_t limit) {
  ITC_NATIVE_RUNNING("sub_8213C4C0 AiPathEntry::NarrowToOtherEx");
  if (other == 0) {
    return 0;
  }
  const uint32_t record = AiPathEntry(other).slice_record().address;
  if (record == 0) {
    return 0;
  }
  double lo = Fsubs(Lfs(AiPathEntry(other).range_low_address()), Lfs(low));
  double hi = Fadds(Lfs(AiPathEntry(other).range_high_address()), Lfs(high));
  const double least = Lfs(RouteSlice(record).route_bit_a_address());
  if (lo < least) {
    lo = least;
  }
  const double most = Lfs(RouteSlice(record).route_bit_b_address());
  if (hi > most) {
    hi = most;
  }
  const double by = Lfs(offset);
  lo = Fadds(by, lo);
  hi = Fadds(by, hi);
  if (lo > hi) {
    hi = Fmuls(Fadds(hi, lo), Lfs(G::half_address()));
    lo = hi;
  }
  uint32_t narrowed = 0;
  if (Lfs(AiPathEntry(entry).range_low_address()) < lo) {
    Stfs(AiPathEntry(entry).range_low_address(), lo);
    Stfs(AiPathEntry(entry).room_low_address(), lo);
    const double margin = Lfs(low);
    if (margin < Lfs(limit)) {
      Stfs(low, Fadds(Lfs(step), margin));
    }
    narrowed = 1;
  } else {
    CopyWord(step, low);
  }
  if (Lfs(AiPathEntry(entry).range_high_address()) > hi) {
    Stfs(AiPathEntry(entry).range_high_address(), hi);
    Stfs(AiPathEntry(entry).room_high_address(), hi);
    const double margin = Lfs(high);
    if (margin < Lfs(limit)) {
      Stfs(high, Fadds(Lfs(step), margin));
    }
    return 1;
  }
  CopyWord(step, high);
  return narrowed;
}

uint32_t NarrowBehind(uint32_t path, uint32_t other, uint32_t entry,
                      uint32_t step, uint32_t limit, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213D140 AiPath::NarrowBehind");
  CopyWord(step, frame + 84);
  CopyWord(step, frame + 80);
  int32_t k = int32_t(uint32_t(LoadS16(AiPath(path).entry_count_address())) - 1);
  uint32_t r3;
  for (;;) {
    Stfs(frame + 88, Fneg(Lfs(AiPathEntry(other).step_address())));
    r3 = NarrowToOtherEx(entry, other, step, frame + 88, frame + 84, frame + 80, limit);
    if (int32_t(r3) != 1) {
      return r3;
    }
    k = int32_t(uint32_t(k) - 1);
    other = entry;
    if (k < 0) {
      return r3;
    }
    entry = CountedRingEntry(path, k);
    if (entry == 0) {
      return r3;
    }
  }
}

int32_t CompareBranches(uint32_t path, uint32_t a, uint32_t b, uint32_t a_number,
                        uint32_t b_number, uint32_t masks) {
  ITC_NATIVE_RUNNING("sub_8213C788 AiPath::CompareBranches");
  if (uint32_t((RouteSection(b).kind() & 0xF) - 1) <= 3) {
    return -1;
  }
  if (uint32_t((RouteSection(a).kind() & 0xF) - 1) <= 3) {
    return 1;
  }
  const uint32_t driver = AiPath(path).driver().address;
  const uint32_t vehicle = AiDriver(driver).vehicle().address;
  const uint32_t flag = vehicle == 0 ? 0 : (Vehicle(vehicle).car_flags() >> 2) & 1;
  if (flag == 0) {
    if (uint32_t(RouteSection(b).branch()) - 7 <= 1) {
      return -1;
    }
    if (uint32_t(RouteSection(a).branch()) - 7 <= 1) {
      return 1;
    }
  }
  uint32_t passed = 0;
  const uint32_t object = AiDriver(driver).profile().address;
  if (object != 0 &&
      uint32_t(LoadS16(AiProfile(object).branch_kind_address()) - 4) <= 1) {
    uint32_t list = PreferredSections(object + 36, object + 90);
    if (list != 0) {
      int32_t section = LoadS16(list + 0);
      if (section >= 0) {
        const int32_t first = LoadS16(a_number);
        do {
          if (section == first) {
            return -1;
          }
          if (section == LoadS16(b_number)) {
            return 1;
          }
          list += 2;
          passed++;
          section = LoadS16(list + 0);
        } while (section >= 0);
      }
    }
  }
  if (masks == 0) {
    return 0;
  }
  uint32_t at = (passed << 2) + masks;
  uint32_t mask = Load(at);
  if (int32_t(mask) == 0) {
    return 0;
  }
  const uint32_t b_bit = Slw(1, RouteSection(b).branch());
  const uint32_t a_bit = Slw(1, RouteSection(a).branch());
  do {
    if ((a_bit & mask) != 0) {
      return (b_bit & mask) == 0 ? -1 : 0;
    }
    if ((b_bit & mask) != 0) {
      return 1;
    }
    at += 4;
    mask = Load(at);
  } while (int32_t(mask) != 0);
  return 0;
}

uint32_t NormalizeByBranch(uint32_t path, uint32_t entry, uint32_t position,
                           uint32_t lane, uint32_t masks) {
  ITC_NATIVE_RUNNING("sub_8213C908 AiPath::NormalizeByBranch");
  const uint32_t route = track_route::kRoute;
  const uint32_t object = Load(AiPath(path).driver().address + 500);
  uint32_t on;
  if (object != 0 &&
      uint32_t(LoadS16(AiProfile(object).branch_kind_address()) - 4) <= 1) {
    const uint32_t list = PreferredSections(object + 36, object + 90);
    on = track_route::NormalizePath(route, position, list, masks);
  } else {
    on = track_route::Normalize(route, position, masks);
  }
  if (int32_t(on) == 0) {
    return 0;
  }
  const uint32_t file = Load(route);
  const uint32_t section = guest::LoadU16(position + 0);
  const int32_t slice = LoadS16(position + 2);
  const uint32_t table = TrackRouteFile(file).sections();
  const double zero = Lfs(kZero);
  const uint32_t record = Load((section << 2) + table) + uint32_t(slice) * 84 + 124;
  AiPathEntry(entry).set_slice_record(record);
  AiPathEntry(entry).set_section(guest::LoadU16(position + 0));
  AiPathEntry(entry).set_slice(guest::LoadU16(position + 2));
  const int32_t at = LoadS16(lane);
  Stfs(AiPathEntry(entry).offset_address(), zero);
  Stfs(AiPathEntry(entry).target_offset_address(), zero);
  AiPathEntry(entry).set_lane(uint32_t(at));
  CopyWord(RouteSlice(record).center_x_address(), AiPathEntry(entry).x_address());
  CopyWord(RouteSlice(record).center_y_address(), AiPathEntry(entry).y_address());
  CopyWord(RouteSlice(record).center_z_address(), AiPathEntry(entry).z_address());
  return 1;
}

// --- Making room for the other racers ---

uint64_t MeasureOthers(uint32_t path, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8213B650 AiPath::MeasureOthers");
  const int32_t count = int32_t(track_collision::RaceRacerCount());
  uint64_t r3 = uint32_t(count);
  const double zero = Lfs(kZero);
  int32_t at = 2;     // kept from one racer to the next, as the original does
  double sum = zero;  // the same
  for (int32_t k = 0; k < count; k++) {
    const int32_t index =
        int32_t(Load(AiPath(path).driver().address + 452 + 4 * uint32_t(k)));
    if (index < 0) {
      continue;
    }
    r3 = OfIndex(uint32_t(index));
    const uint32_t other = uint32_t(r3);
    const uint32_t driver = AiPath(path).driver().address;
    const uint32_t entry = out + 20 * uint32_t(k);
    Stfs(AiOtherSpan(entry).other_offset_address(), zero);
    Stfs(AiOtherSpan(entry).step_sum_address(), zero);
    AiOtherSpan(entry).set_end_entry(0);
    Stfs(AiOtherSpan(entry).widths_address(), zero);
    Stfs(AiOtherSpan(entry).side_offset_address(), zero);
    const uint32_t record = uint32_t(index) * 40 + driver + 172;
    AiOtherSpan(entry).set_first_entry(0);
    const uint16_t ring = AiOther(record).ring_index();
    const int32_t last = int16_t(ring);
    if (!(at > last)) {
      const int32_t n = LoadS16(AiPath(path).entry_count_address());
      do {
        if (!(at < n)) {
          break;
        }
        const int32_t m =
            int32_t(uint32_t(LoadS16(AiPath(path).first_address())) + uint32_t(at));
        at++;
        sum = Fadds(Lfs(path + (uint32_t(m - (m / 80) * 80) << 6) + 44), sum);
      } while (!(at > last));
    }
    AiOtherSpan(entry).set_first_entry(ring);
    uint16_t next;
    if ((AiOther(record).action_flags() & 4) != 0) {
      next = uint16_t(AiOther(record).ring_index() + 1);
    } else {
      const int32_t after = LoadS16(AiOther(record).ring_index_address()) + 1;
      const int32_t look = LoadS16(AiPath(path).plan_count_address());
      next = uint16_t(look < after ? after : look);
    }
    Stfs(AiOtherSpan(entry).step_sum_address(), sum);
    AiOtherSpan(entry).set_end_entry(next);
    const uint32_t me = AiPath(path).driver().address;
    Stfs(AiOtherSpan(entry).widths_address(),
         Fadds(Lfs(AiDriver(me).half_width_address()),
               Lfs(AiDriver(other).half_width_address())));
    const int32_t n = LoadS16(AiPath(path).entry_count_address());
    if (!(LoadS16(AiOther(record).ring_index_address()) < n)) {
      guest::StoreU32(AiPath(path).driver().address + 452 + 4 * uint32_t(k),
                      0xFFFFFFFFu);
      return r3;
    }
    const uint32_t file = RG::route_file().address;
    const double gap = Lfs(AiOther(record).gap_address());
    const double length = Lfs(TrackRouteFile(file).slice_length_address());
    int32_t slot;
    if (gap > length) {
      CopyWord(AiDriver(other).road_offset_address(),
               AiOtherSpan(entry).other_offset_address());
      if (LoadS16(AiPath(path).entry_count_address()) <= 0) {
        continue;
      }
      if ((AiOther(record).action_flags() & 0x10) == 0) {
        continue;
      }
      slot = LoadS16(AiPath(path).first_address());
    } else {
      int32_t ahead = int32_t(
          uint32_t(strings_and_font::steps::Fctiwz(Fdivs(Fneg(gap), length))) + 1);
      if (ahead < 0) {
        ahead = 0;
      }
      if (!(ahead < n)) {
        CopyWord(AiDriver(other).road_offset_address(),
                 AiOtherSpan(entry).other_offset_address());
        continue;
      }
      slot = int32_t(uint32_t(LoadS16(AiPath(path).first_address())) + uint32_t(ahead));
    }
    const double ox = Lfs(AiDriver(other).point_x_address());
    const double oz = Lfs(AiDriver(other).point_z_address());
    const uint32_t slice = Load(path + (uint32_t(slot - (slot / 80) * 80) << 6) + 68);
    double dx = Fsubs(ox, Lfs(RouteSlice(slice).center_x_address()));
    const double dz = Fsubs(oz, Lfs(RouteSlice(slice).center_z_address()));
    dx = Fmuls(dx, Lfs(RouteSlice(slice).side_x_address()));
    Stfs(AiOtherSpan(entry).other_offset_address(),
         Fmadds(dz, Lfs(RouteSlice(slice).side_z_address()), dx));
  }
  return r3;
}

namespace {
// AvoidOnLeft's and AvoidOnRight's spread: the width over a's +0 - 1
// entries (at frame + 80), kept within the path's step +5516.
void SpreadWidth(uint32_t path, uint32_t a, double line, double mine,
                 uint32_t frame) {
  const double zero = Lfs(kZero);
  const int32_t first = LoadS16(AiOtherSpan(a).first_entry_address());
  double width = Fsubs(line, mine);
  Stfs(frame + 80, width);
  if (first > 1) {
    width = Fdivs(width, IntToFloat(first - 1));
    Stfs(frame + 80, width);
  }
  const double step = Lfs(AiPath(path).largest_step_address());
  if (Fadds(step, width) < zero) {
    Stfs(frame + 80, Fneg(step));
  } else if (Fsubs(width, step) > zero) {
    Stfs(frame + 80, step);
  }
}
}  // namespace

uint64_t AvoidOnLeft(uint32_t path, uint32_t a, uint32_t record, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213CB30 AiPath::AvoidOnLeft");
  const uint32_t driver = AiPath(path).driver().address;
  uint64_t r3 = driver;
  const uint32_t mode = AiPath(path).mode();
  const double across = Fmuls(Lfs(AiOtherSpan(a).side_offset_address()),
                              Lfs(AiOther(record).factor_address()));
  const double zero = Lfs(kZero);
  double steer = zero;
  const double mine = Lfs(AiDriver(driver).road_offset_address());
  CopyWord(AiDriver(driver).road_offset_address(), frame + 96);
  if (int32_t(mode) != 1) {
    steer = Lfs(AiDriver(driver).steer_shift_address());
  }
  if (mine < across) {
    guest::StoreU32(frame + 88, mode == 1 ? 0 : 1);
    AdjustForOther(driver, record, frame + 88);
  }
  double line = Fadds(across, Lfs(0x82002A3C));
  if (mine < line && Lfs(AiOther(record).gap_address()) < zero) {
    line = Fmadds(IntToFloat(LoadS16(AiOtherSpan(a).end_entry_address()) + 1),
                  Fsubs(line, mine), line);
  }
  line = Fadds(line, Lfs(0x8200296C));
  Stfs(frame + 88, line);
  if (steer > zero) {
    line = Fmadds(steer, Lfs(G::fifth_address()), line);
    Stfs(frame + 88, line);
  }
  if (!(mine < line)) {
    for (int32_t j = 1; j < LoadS16(AiOtherSpan(a).end_entry_address()); j++) {
      const uint32_t entry = RingAt(path, j);
      line = Fsubs(line, Lfs(AiPathEntry(entry).step_address()));
      if (Lfs(AiPathEntry(entry).room_low_address()) < line) {
        const double most = Lfs(AiPathEntry(entry).room_high_address());
        Stfs(AiPathEntry(entry).room_low_address(), line);
        if (line > most) {
          Stfs(AiPathEntry(entry).room_low_address(), most);
        }
      }
    }
    return r3;
  }
  SpreadWidth(path, a, line, mine, frame);
  for (int32_t j = 1; j < LoadS16(AiOtherSpan(a).end_entry_address()); j++) {
    r3 = RingAt(path, j);
    BlockLeft(uint32_t(r3), frame + 88, frame + 80, frame + 96);
  }
  return r3;
}

uint64_t AvoidOnRight(uint32_t path, uint32_t a, uint32_t record,
                      uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213CD50 AiPath::AvoidOnRight");
  uint64_t r3 = path;
  const double across = Fmuls(Lfs(AiOtherSpan(a).side_offset_address()),
                              Lfs(AiOther(record).factor_address()));
  const uint32_t driver = AiPath(path).driver().address;
  const double mine = Lfs(AiDriver(driver).road_offset_address());
  CopyWord(AiDriver(driver).road_offset_address(), frame + 96);
  if (mine > across) {
    guest::StoreU32(frame + 88, AiPath(path).mode());
    AdjustForOther(driver, record, frame + 88);
    r3 = driver;
  }
  double line = Fsubs(across, Lfs(0x82002A3C));
  const double zero = Lfs(kZero);
  if (mine > line && Lfs(AiOther(record).gap_address()) < zero) {
    line = Fmadds(IntToFloat(LoadS16(AiOtherSpan(a).end_entry_address()) + 1),
                  Fsubs(line, mine), line);
  }
  const double steer = Lfs(AiDriver(driver).steer_shift_address());
  line = Fsubs(line, Lfs(0x8200296C));
  Stfs(frame + 88, line);
  if (steer < zero) {
    line = Fmadds(steer, Lfs(G::fifth_address()), line);
    Stfs(frame + 88, line);
  }
  if (!(mine > line)) {
    for (int32_t j = 1; j < LoadS16(AiOtherSpan(a).end_entry_address()); j++) {
      const uint32_t entry = RingAt(path, j);
      line = Fsubs(line, Lfs(AiPathEntry(entry).step_address()));
      if (Lfs(AiPathEntry(entry).room_high_address()) > line) {
        const double least = Lfs(AiPathEntry(entry).room_low_address());
        Stfs(AiPathEntry(entry).room_high_address(), line);
        if (line < least) {
          Stfs(AiPathEntry(entry).room_high_address(), least);
        }
      }
    }
    return r3;
  }
  SpreadWidth(path, a, line, mine, frame);
  for (int32_t j = 1; j < LoadS16(AiOtherSpan(a).end_entry_address()); j++) {
    r3 = RingAt(path, j);
    BlockRight(uint32_t(r3), frame + 88, frame + 80, frame + 96);
  }
  return r3;
}

uint64_t ChooseSideForOther(uint32_t path, uint32_t a, uint32_t record,
                            uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8213D788 AiPath::ChooseSideForOther");
  const uint32_t driver = AiPath(path).driver().address;
  const double a12 = Lfs(AiOtherSpan(a).widths_address());
  const int32_t index = LoadS16(AiOther(record).ring_index_address());
  const int32_t n = LoadS16(AiPath(path).entry_count_address());
  const double a8 = Lfs(AiOtherSpan(a).step_sum_address());
  const double width = Fadds(a12, Lfs(G::fifth_address()));
  const double reach = Fadds(Lfs(AiDriver(driver).road_offset_address()), a8);
  const double zero = Lfs(kZero);
  double left = zero, right = zero;
  if (index < n) {
    const int32_t m =
        int32_t(uint32_t(LoadS16(AiPath(path).first_address())) + uint32_t(index));
    const uint32_t entry = path + (uint32_t(m - (m / 80) * 80) << 6);
    left = Lfs(entry + 56);
    right = Lfs(entry + 60);
  }
  const int32_t kind = LoadS16(AiOther(record).state_address());
  const double other = Lfs(AiOtherSpan(a).other_offset_address());
  const double gap = Fsubs(reach, other);
  bool to_right;
  if (kind == 1) {
    const double room = Fadds(width, Lfs(kOne));
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
    Stfs(AiOtherSpan(a).side_offset_address(), Fsubs(other, width));
    return AvoidOnRight(path, a, record, stack - 160);
  }
  Stfs(AiOtherSpan(a).side_offset_address(), Fadds(other, width));
  return AvoidOnLeft(path, a, record, stack - 176);
}

uint64_t AvoidByLane(uint32_t path, uint32_t a, uint32_t record, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213D898 AiPath::AvoidByLane");
  const uint32_t driver = AiPath(path).driver().address;
  const double other = Lfs(AiOtherSpan(a).other_offset_address());
  const double one = Lfs(kOne);
  const double room = Fadds(Lfs(AiOtherSpan(a).widths_address()), one);
  const double off = Fsubs(Lfs(AiDriver(driver).road_offset_address()), other);
  if (!(off < room) || !(off > Fneg(room))) {
    return path;
  }
  if (LoadS16(AiPath(path).entry_count_address()) <= 1) {
    return path;
  }
  const uint32_t entry = RingAt(path, 1);
  if (int32_t(entry) == 0) {
    return path;
  }
  const uint32_t slice = AiPathEntry(entry).slice_record().address;
  if (slice == 0) {
    return path;
  }
  const uint32_t lanes = RouteSlice(slice).lanes() & 0xF;
  int32_t lane = 0;
  if (lanes > 1) {
    const int32_t now = int32_t(AiPathEntry(entry).lane());
    if (now <= 0) {
      lane = 1;
    } else if (off < Lfs(kZero)) {
      lane = now - 1;
    } else {
      lane = now + 1;
      if (!(uint32_t(lane) < lanes)) {
        lane = now - 1;
      }
    }
  }
  const double width = Lfs(RouteSlice(slice).width_address());
  const double half = Lfs(G::half_address());
  const double count = IntToFloat(int32_t(lanes));
  const double centered = Fnmsubs(Fsubs(count, one), half, IntToFloat(lane));
  Stfs(AiOtherSpan(a).side_offset_address(), Fmuls(Fdivs(centered, count), width));
  if (lane > int32_t(AiPathEntry(entry).lane())) {
    return AvoidOnLeft(path, a, record, frame - 176);
  }
  return AvoidOnRight(path, a, record, frame - 160);
}

uint64_t AvoidOthers(uint32_t path, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213E690 AiPath::AvoidOthers");
  const int32_t count = int32_t(track_collision::RaceRacerCount());
  const double zero = Lfs(kZero);
  for (uint32_t i = 0; i < 7; i++) {
    const uint32_t a = frame + 80 + 20 * i;
    Stfs(AiOtherSpan(a).other_offset_address(), zero);
    AiOtherSpan(a).set_first_entry(0);
    Stfs(AiOtherSpan(a).step_sum_address(), zero);
    AiOtherSpan(a).set_end_entry(0);
    Stfs(AiOtherSpan(a).widths_address(), zero);
    Stfs(AiOtherSpan(a).side_offset_address(), zero);
  }
  uint64_t r3 = MeasureOthers(path, frame + 80);
  for (int32_t k = 0; k < count; k++) {
    const uint32_t driver = AiPath(path).driver().address;
    const int32_t index = int32_t(Load(driver + 452 + 4 * uint32_t(k)));
    if (index < 0) {
      return r3;
    }
    const uint32_t record = uint32_t(index) * 40 + driver + 172;
    const uint32_t a = frame + 80 + 20 * uint32_t(k);
    const int32_t flags = LoadS16(AiOther(record).action_flags_address());
    if ((flags & 1) != 0) {
      r3 = ChooseSideForOther(path, a, record, frame);
    } else if ((flags & 6) != 0) {
      if (k == 0) {
        CopyWord(AiOtherSpan(a).other_offset_address(),
                 AiOtherSpan(a).side_offset_address());
        AvoidOnLeft(path, a, record, frame - 176);
        r3 = AvoidOnRight(path, a, record, frame - 160);
      }
    } else if ((flags & 0x10) != 0 && k == 0) {
      r3 = AvoidByLane(path, a, record, frame - 96);
    }
  }
  return r3;
}

// --- The skill's timed triggers ---

namespace {
// The race's time in seconds: RaceTime::FromTicks on the race state's +6116,
// minutes x 60 + seconds.
int32_t RaceSeconds() {
  const uint32_t time = TimeFromTicks(G::race_state().ticks());
  return int32_t(((time >> 16) & 0x7FFF) * 60 + ((time >> 10) & 0x3F));
}
// The race state's record (+2184 + 972 x i + 4) whose word matches the
// driver's +500 object's halfword +0; 0 with none.
uint32_t SkillEntry(uint32_t driver) {
  const uint32_t object = AiDriver(driver).profile().address;
  const uint32_t list = G::race_state().results_address();
  if (object == 0) {
    return 0;
  }
  const int32_t n = int32_t(Load(list));
  if (n <= 0) {
    return 0;
  }
  const int32_t key = LoadS16(object + 0);
  for (int32_t i = 0; i < n; i++) {
    if (int32_t(Load(list + 8 + 972 * uint32_t(i))) == key) {
      return list + 972 * uint32_t(i) + 4;
    }
  }
  return 0;
}
}  // namespace

uint32_t SkillEntryWord(uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_8213F7E0 AiSkill::EntryWord");
  const uint32_t entry = SkillEntry(driver);
  if (entry == 0) {
    return 0;
  }
  return uint32_t(strings_and_font::steps::Fctiwz(Lfs(entry + 824)));
}

double SkillEntryValue(uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_8213F868 AiSkill::EntryValue");
  const uint32_t entry = SkillEntry(driver);
  if (entry == 0) {
    return Lfs(kZero);
  }
  return Fmuls(IntToFloat(int32_t(Load(entry + 784))), Lfs(0x8200805C));
}

uint32_t TriggerStarts(uint32_t driver, uint32_t trigger, double* f1,
                       bool* f1_set) {
  ITC_NATIVE_RUNNING("sub_8213F958 AiSkill::TriggerStarts");
  const uint32_t kind = AiSkillTrigger(trigger).kind();
  const auto value = [&] { return LoadS16(AiSkillTrigger(trigger).value_address()); };
  switch (kind) {
    case 0:
      return Lfs(AiDriver(driver).player_gap_address()) < IntToFloat(value()) ? 0 : 1;
    case 1:
      return Lfs(AiDriver(driver).player_gap_address()) >
                     IntToFloat(int32_t(0u - uint32_t(value())))
                 ? 0
                 : 1;
    case 2:
      return RaceSeconds() < value() ? 0 : 1;
    case 3:
      return RaceSeconds() > value() ? 0 : 1;
    case 4:
      return int32_t(SkillEntryWord(driver)) < value() ? 0 : 1;
    case 5:
      return int32_t(SkillEntryWord(driver)) > value() ? 0 : 1;
    case 6:
    case 7: {
      const double got = SkillEntryValue(driver);
      *f1 = got;
      *f1_set = true;
      const double against = IntToFloat(value());
      return (kind == 6 ? got < against : got > against) ? 0 : 1;
    }
    case 8:
      return AiDriver(driver).in_corner() != 0 ? 0 : 1;
    case 9:
      return AiDriver(driver).in_corner() == 0 ? 0 : 1;
    case 10: {
      const uint32_t vehicle = AiDriver(driver).vehicle().address;
      if (vehicle == 0) {
        return 0;
      }
      return (Vehicle(vehicle).collision_flags() & 1) == 0 ? 0 : 1;
    }
    case 11:
      return LoadS16(AiDriver(driver).passes_a_address()) > 0 ? 1 : 0;
    case 12:
      return LoadS16(AiDriver(driver).passes_b_address()) > 0 ? 1 : 0;
    case 13:
      return (CarInput(Load(G::inputs_address())).flags() & 0x100) == 0 ? 0 : 1;
    case 14:
      return (CarInput(Load(G::inputs_address())).flags() & 0x20000) == 0 ? 0 : 1;
    default:
      return 0;
  }
}

uint32_t TriggerEnds(uint32_t state, uint32_t driver, uint32_t trigger,
                     double* f1, bool* f1_set) {
  ITC_NATIVE_RUNNING("sub_8213FB48 AiSkill::TriggerEnds");
  const uint32_t kind = AiSkillTrigger(trigger).kind();
  const auto value = [&] { return LoadS16(AiSkillTrigger(trigger).value_address()); };
  switch (kind) {
    case 0:
      return Lfs(AiDriver(driver).player_gap_address()) < IntToFloat(value()) ? 1 : 0;
    case 1:
      return Lfs(AiDriver(driver).player_gap_address()) >
                     IntToFloat(int32_t(0u - uint32_t(value())))
                 ? 1
                 : 0;
    case 2:
      return RaceSeconds() < value() ? 1 : 0;
    case 3:
      return RaceSeconds() > value() ? 1 : 0;
    case 4:
      return int32_t(SkillEntryWord(driver)) < value() ? 1 : 0;
    case 5:
      return int32_t(SkillEntryWord(driver)) > value() ? 1 : 0;
    case 6:
    case 7: {
      const double got = SkillEntryValue(driver);
      *f1 = got;
      *f1_set = true;
      const double against = IntToFloat(value());
      return (kind == 6 ? got < against : got > against) ? 1 : 0;
    }
    case 8:
      return AiDriver(driver).in_corner() != 0 ? 1 : 0;
    case 9:
      return AiDriver(driver).in_corner() == 0 ? 1 : 0;
    case 10:
    case 11:
    case 12:
    case 13:
    case 14: {
      const uint16_t left = uint16_t(guest::LoadU16(state + 0) - 1);
      guest::StoreU16(state + 0, left);
      return left != 0 ? 0 : 1;
    }
    default:
      return 1;
  }
}

uint32_t UpdateSkill(uint32_t skill, uint32_t driver, double* f1, bool* f1_set) {
  ITC_NATIVE_RUNNING("sub_8213FCF8 AiSkill::Update");
  if (AiSkill(skill).source().address == 0 ||
      int32_t(AiSkill(skill).runs_triggers()) != 1) {
    return 0;
  }
  uint32_t changed = 0;
  uint32_t state = skill + 24;
  for (uint32_t at = 16; at < 40; at += 8, state += 4) {
    const uint32_t trigger = at + AiSkill(skill).source().address;
    if (int32_t(AiSkillTrigger(trigger).kind()) == -1) {
      continue;
    }
    int32_t step;
    if (guest::LoadU8(state + 2) == 0) {
      step = TriggerStarts(driver, trigger, f1, f1_set) != 0 ? 1 : 0;
    } else {
      step = TriggerEnds(state, driver, trigger, f1, f1_set) != 0 ? -1 : 0;
    }
    if (step > 0) {
      guest::StoreU16(state + 0, 150);
      guest::StoreU8(state + 2, 1);
      const double now = Lfs(AiSkill(skill).percentage_address());
      Stfs(AiSkill(skill).percentage_address(),
           Fadds(IntToFloat(LoadS16(AiSkillTrigger(trigger).value_address())), now));
    } else if (step < 0) {
      guest::StoreU8(state + 2, 0);
      const double now = Lfs(AiSkill(skill).percentage_address());
      Stfs(AiSkill(skill).percentage_address(),
           Fsubs(now, IntToFloat(LoadS16(AiSkillTrigger(trigger).value_address()))));
    } else {
      continue;
    }
    changed = 1;
  }
  return changed;
}

// --- Steering through the corners and driving ---

void LimitSlide(uint32_t driver, uint32_t record, uint32_t flag) {
  ITC_NATIVE_RUNNING("sub_821392D8 AiDriver::LimitSlide");
  const double speed = Lfs(AiDriver(driver).speed_kmh_address());
  if (speed < Lfs(G::hundred_address())) {
    if (speed < Lfs(G::twenty_address())) {
      return;
    }
    if (speed <
        Fmuls(Lfs(AiDriver(driver).target_speed_address()), Lfs(0x82001070))) {
      return;
    }
  }
  const double back = Fneg(Lfs(AiCorner(record).direction_address()));
  const double steer = Lfs(AiDriver(driver).steer_request_address());
  const double zero = Lfs(kZero);
  const double side = Lfs(steer < zero ? driver + 112 : driver + 108);
  const double ratio = Fmuls(Fdivs(steer, side), back);
  const double fifth = Lfs(G::fifth_address());
  if (!(ratio > fifth)) {
    return;
  }
  const double yaw = Fmuls(Lfs(AiDriver(driver).yaw_rate_address()), back);
  if (yaw < Lfs(0x82002A10)) {
    Stfs(AiDriver(driver).throttle_request_address(), zero);
    Stfs(AiDriver(driver).steer_request_address(), Fmuls(steer, Lfs(0x820021E8)));
  }
  if (int32_t(Load(flag)) == 0) {
    return;
  }
  if (!(ratio > Lfs(0x820029D4)) || yaw < fifth) {
    return;
  }
  double most = Lfs(kOne);
  const double grip = Lfs(AiDriver(driver).corner_grip_address());
  if (grip < most) {
    most = grip;
  }
  if (!(yaw < most)) {
    return;
  }
  Stfs(AiDriver(driver).throttle_request_address(), zero);
  AiDriver(driver).set_handbrake(1);
}

namespace {
// The room on the corner's side at the first ring entry (its +40 or +44
// against the driver's +144); `valid` false without that entry or its slice.
double CornerRoom(uint32_t driver, uint32_t record, bool* valid) {
  const uint32_t path = driver + 512;
  const uint32_t entry =
      LoadS16(AiPath(path).entry_count_address()) > 1 ? RingAt(path, 1) : 0;
  *valid = entry != 0 && AiPathEntry(entry).slice_record().address != 0;
  if (!*valid) {
    return 0;
  }
  if (Lfs(AiCorner(record).direction_address()) < Lfs(kZero)) {
    return Fsubs(Lfs(AiDriver(driver).road_offset_address()),
                 Lfs(AiPathEntry(entry).room_low_address()));
  }
  return Fsubs(Lfs(AiPathEntry(entry).room_high_address()),
               Lfs(AiDriver(driver).road_offset_address()));
}
}  // namespace

uint32_t SteerByRoom(uint32_t driver, uint32_t record, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_82138FA8 AiDriver::SteerByRoom");
  uint32_t result = 0;
  if (LoadS16(AiDriver(driver).path_entry_count_address()) > 1) {
    bool valid = false;
    const double room = CornerRoom(driver, record, &valid);
    if (valid) {
      Stfs(out, room);
      if (room > Lfs(AiDriver(driver).half_width_address())) {
        const double angle = Lfs(AiDriver(driver).target_angle_address());
        if (!(Fmuls(Lfs(AiCorner(record).direction_address()), angle) >
              Lfs(kZero))) {
          result = 1;
        } else {
          const double now = Lfs(AiDriver(driver).steer_request_address());
          const double half = Fmuls(angle, Lfs(G::half_address()));
          const double sum =
              Fadds(Fadds(Lfs(AiDriver(driver).slide_angle_address()), now), half);
          Stfs(AiDriver(driver).steer_request_address(), sum);
          const double size = Lfs(AiDriver(driver).car_radius_address());
          const double left = Lfs(out);
          if (size > left) {
            Stfs(AiDriver(driver).steer_request_address(),
                 Fmadds(Fsubs(Lfs(kOne), Fdivs(left, size)), half, sum));
          }
          return 1;
        }
      }
    }
  }
  AccumulateSteer(driver);
  return result;
}

uint64_t SteerOutOfCorner(uint32_t driver, uint32_t record, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82139A08 AiDriver::SteerOutOfCorner");
  const double back = Fneg(Lfs(AiCorner(record).direction_address()));
  const double zero = Lfs(kZero);
  const double speed = Lfs(AiDriver(driver).speed_kmh_address());
  Stfs(frame + 80, zero);
  if (!(speed > Lfs(G::twenty_address()))) {
    AccumulateSteer(driver);
    return driver;
  }
  const uint32_t result = SteerByRoom(driver, record, frame + 80);
  if (result == 0) {
    return 0;
  }
  const double turn = Fmuls(Lfs(AiDriver(driver).target_angle_address()), back);
  const double target = Lfs(AiDriver(driver).target_speed_address());
  const double now = Lfs(AiDriver(driver).speed_kmh_address());
  bool cut;
  if (turn < zero) {
    cut = now > target ||
          (Fmuls(Lfs(AiDriver(driver).road_offset_address()), back) < zero &&
           Fmuls(Lfs(AiDriver(driver).yaw_rate_address()), back) < zero);
  } else {
    cut = now > target &&
          !(Fmuls(Lfs(AiDriver(driver).slide_angle_address()), back) < zero);
  }
  if (cut) {
    Stfs(AiDriver(driver).throttle_request_address(), zero);
  }
  return result;
}

uint64_t SteerAtCornerPoint(uint32_t driver, uint32_t record, uint32_t at,
                            uint32_t frame, double* f1, bool* f1_set) {
  ITC_NATIVE_RUNNING("sub_8213A200 AiDriver::SteerAtCornerPoint");
  int32_t k = int32_t(Load(at));
  const double zero = Lfs(kZero);
  const int32_t look = LoadS16(AiDriver(driver).path_look_ahead_address());
  if (k < look) {
    k = look;
  }
  if (!(k < LoadS16(AiDriver(driver).path_entry_count_address()))) {
    AccumulateSteer(driver);
    return driver;
  }
  bool valid = false;
  double room = CornerRoom(driver, record, &valid);
  if (!valid) {
    room = zero;
  }
  if (room < Lfs(AiDriver(driver).half_width_address())) {
    AiCorner(record).set_kind(3);
    return SteerOutOfCorner(driver, record, frame - 96);
  }
  const uint32_t path = driver + 512;
  const uint32_t target = CountedRingEntry(path, k);
  if (target == 0) {
    AccumulateSteer(driver);
    return driver;
  }
  const double dx =
      Fsubs(Lfs(target + 0), Lfs(AiDriver(driver).point_x_address()));
  const double dz =
      Fsubs(Lfs(target + 8), Lfs(AiDriver(driver).point_z_address()));
  *f1 = dx;  // the original works it out in f1
  *f1_set = true;
  if (dx == zero && dz == zero) {
    AccumulateSteer(driver);
    return driver;
  }
  const double toward = c_runtime::Atan2(dx, dz);
  *f1 = toward;
  *f1_set = true;
  Stfs(frame + 80,
       Fneg(Fsubs(double(float(toward)), Lfs(AiDriver(driver).heading_address()))));
  WrapAngle(frame + 80);
  const double turn = Lfs(AiDriver(driver).target_angle_address());
  const double angle = Lfs(frame + 80);
  if (Fmuls(turn, angle) != zero) {
    Stfs(AiDriver(driver).steer_request_address(),
         Fadds(Lfs(AiDriver(driver).steer_request_address()), turn));
  }
  const double sum = Fadds(Lfs(AiDriver(driver).slide_angle_address()),
                           Lfs(AiDriver(driver).steer_request_address()));
  Stfs(AiDriver(driver).steer_request_address(), sum);
  if (!(room < zero)) {
    double add = Fmuls(Fdivs(room, Lfs(AiDriver(driver).car_radius_address())), angle);
    Stfs(frame + 80, add);
    const double least = Lfs(0x82008960);
    if (add < least) {
      add = least;
      Stfs(frame + 80, add);
    } else {
      const double most = Lfs(0x82001FC4);
      if (add > most) {
        add = most;
        Stfs(frame + 80, add);
      }
    }
    Stfs(AiDriver(driver).steer_request_address(), Fadds(sum, add));
  }
  guest::StoreU32(frame + 84, 0);
  LimitSlide(driver, record, frame + 84);
  return driver;
}

uint64_t SteerIntoCorner(uint32_t driver, uint32_t record, uint32_t frame,
                         double* f1, bool* f1_set) {
  ITC_NATIVE_RUNNING("sub_8213AB40 AiDriver::SteerIntoCorner");
  const uint32_t data = AiCorner(record).data().address;
  const int32_t offset = LoadS16(AiCorner(record).entry_offset_address());
  if (int32_t(uint32_t(LoadS16(RouteCorner(data).apex_address())) + uint32_t(offset)) <
      0) {
    AiCorner(record).set_kind(3);
    return SteerOutOfCorner(driver, record, frame - 96);
  }
  guest::StoreU32(frame + 80,
      uint32_t(LoadS16(RouteCorner(data).turn_in_address())) + uint32_t(offset));
  return SteerAtCornerPoint(driver, record, frame + 80, frame - 128, f1, f1_set);
}

uint32_t UpdateSteering(uint32_t driver, uint32_t frame, double* f1,
                        bool* f1_set) {
  ITC_NATIVE_RUNNING("sub_8213AEF0 AiDriver::UpdateSteering");
  UpdateSteerInput(driver, frame, f1, f1_set);
  double scale = Lfs(0x8200296C);
  double limit = Lfs(0x82002A10);
  const uint32_t slow = IsSlow(driver + 512);
  const double fifth = Lfs(G::fifth_address());
  if (slow == 0) {
    const double yaw = Lfs(AiDriver(driver).yaw_rate_address());
    if (yaw > Lfs(0x82002FB0) || yaw < Lfs(0x82002C00)) {
      Stfs(AiDriver(driver).steer_request_address(),
           Fnmsubs(yaw, fifth, Lfs(AiDriver(driver).steer_request_address())));
    }
  } else {
    scale = fifth;
    limit = Lfs(0x82002A14);
  }
  const double yaw = Lfs(AiDriver(driver).yaw_rate_address());
  if (yaw > limit) {
    Stfs(AiDriver(driver).steer_request_address(),
         Fnmsubs(Fsubs(yaw, limit), scale,
                 Lfs(AiDriver(driver).steer_request_address())));
  } else if (yaw < Fneg(limit)) {
    Stfs(AiDriver(driver).steer_request_address(),
         Fnmsubs(Fadds(yaw, limit), scale,
                 Lfs(AiDriver(driver).steer_request_address())));
  }
  double steer = Lfs(AiDriver(driver).steer_request_address());
  const double zero = Lfs(kZero);
  steer = Fdivs(steer, Lfs(steer < zero ? driver + 112 : driver + 108));
  const double now = Lfs(AiDriver(driver).steering_address());
  Stfs(AiDriver(driver).steer_request_address(), steer);
  double move = Fsubs(steer, now);
  if (Fadds(move, fifth) < zero) {
    move = Lfs(0x82006908);
  } else if (Fsubs(move, fifth) > zero) {
    move = fifth;
  }
  const double next = Fadds(now, move);
  Stfs(AiDriver(driver).steering_address(), next);
  const double one = Lfs(kOne);
  if (Fadds(next, one) < zero) {
    Stfs(AiDriver(driver).steering_address(), Lfs(G::minus_one_address()));
  } else if (Fsubs(next, one) > zero) {
    Stfs(AiDriver(driver).steering_address(), one);
  }
  return slow;
}

uint64_t Drive(uint32_t driver, uint32_t input, uint32_t frame, double* f1) {
  ITC_NATIVE_RUNNING("sub_8213B058 AiDriver::Drive");
  const uint32_t vehicle = AiDriver(driver).vehicle().address;
  StoreConstant(AiDriver(driver).throttle_request_address(), 0x820027A8);
  AiDriver(driver).set_handbrake(0);
  const double zero = Lfs(kZero);
  Stfs(AiDriver(driver).brake_request_address(), zero);
  Stfs(AiDriver(driver).steer_request_address(), zero);
  if (SteerToTarget(driver, f1) == 0) {
    Stfs(AiDriver(driver).steering_address(), zero);
    AiDriver(driver).set_handbrake(0);
    Stfs(AiDriver(driver).brake_address(), zero);
    Stfs(AiDriver(driver).throttle_address(), zero);
  } else if (((Vehicle(vehicle).car_flags() >> 26) & 1) != 0) {
    Stfs(AiDriver(driver).steering_address(), zero);
    Stfs(AiDriver(driver).throttle_address(), zero);
    Stfs(AiDriver(driver).brake_address(), Lfs(kOne));
    CarInput(input).set_handbrake(32767);
  } else {
    double f = 0;
    bool set = false;
    UpdateSpeed(driver, frame - 96, &f, &set);
    if (LoadS16(AiDriver(driver).skill_caps_target_address()) <= 0) {
      const double most = Lfs(AiDriver(driver).speed_limit_address());
      if (Lfs(AiDriver(driver).target_speed_address()) > most) {
        Stfs(AiDriver(driver).target_speed_address(), most);
      }
    }
    const double least = Lfs(G::twenty_address());
    if (Lfs(AiDriver(driver).target_speed_address()) < least) {
      Stfs(AiDriver(driver).target_speed_address(), least);
    }
    UpdateSteering(driver, frame - 112, &f, &set);
    if (set) {
      *f1 = f;
    }
    UpdatePedals(driver, frame - 112);
    if ((AiDriver(driver).flags() & 0x20) != 0) {
      CarInput(input).set_flags(CarInput(input).flags() | 0x200);
      Stfs(AiDriver(driver).steering_address(),
           Fmuls(Lfs(AiDriver(driver).steering_address()), Lfs(0x82001BE8)));
      Stfs(AiDriver(driver).throttle_address(), zero);
    }
  }
  if (AiDriver(driver).handbrake() != 0) {
    CarInput(input).set_handbrake(32767);
  }
  LimitThrottle(driver);
  const double brake = Lfs(AiDriver(driver).brake_address());
  if (brake > zero) {
    Stfs(AiDriver(driver).throttle_address(), zero);
  }
  const double scale = Lfs(0x82007F58);
  const double steer = Fmuls(Lfs(AiDriver(driver).steering_address()), scale);
  const double throttle = Fmuls(Lfs(AiDriver(driver).throttle_address()), scale);
  const double pressed = Fmuls(brake, scale);
  CarInput(input).set_throttle(uint16_t(strings_and_font::steps::Fctiwz(throttle)));
  CarInput(input).set_steering(uint16_t(strings_and_font::steps::Fctiwz(steer)));
  CarInput(input).set_brake(uint16_t(strings_and_font::steps::Fctiwz(pressed)));
  return driver;
}

// --- The path's line through the corners ---

namespace {
// The entry's offset +24 (just set to `offset`) kept within +40 to +44.
void KeepInRange(uint32_t entry, double offset) {
  const double low = Lfs(AiPathEntry(entry).room_low_address());
  if (offset < low) {
    Stfs(AiPathEntry(entry).offset_address(), low);
    return;
  }
  const double high = Lfs(AiPathEntry(entry).room_high_address());
  if (offset > high) {
    Stfs(AiPathEntry(entry).offset_address(), high);
  }
}
// The entry's offset +24 moved from `from` by at most the path's +5516.
void LimitStep(uint32_t path, uint32_t entry, double from, double zero) {
  double step = Fsubs(Lfs(AiPathEntry(entry).offset_address()), from);
  const double most = Lfs(AiPath(path).largest_step_address());
  if (Fadds(most, step) < zero) {
    step = Fneg(most);
  } else if (Fsubs(step, most) > zero) {
    step = most;
  }
  Stfs(AiPathEntry(entry).offset_address(), Fadds(step, from));
}
// The corner steps' shared start: the end index (`end`, the corner's data's
// halfword at `point` plus the record's offset +10) and the start (the word
// at `index`); 0 to stop (`*stop` = the return value) when the end isn't
// past it. Else the start index at frame + 80, `*span` = end - start, and
// the end clamped to the look-ahead +5140 (`*reached` = 1 then).
bool CornerSpan(uint32_t path, uint32_t record, uint32_t point, uint32_t index,
                uint32_t frame, int32_t* start, int32_t* end, int32_t* span,
                uint32_t* reached) {
  *start = int32_t(Load(index));
  *end = int32_t(uint32_t(LoadS16(AiCorner(record).data().address + point)) +
                 uint32_t(LoadS16(AiCorner(record).entry_offset_address())));
  *reached = 0;
  if (!(*end > *start)) {
    return false;
  }
  guest::StoreU32(frame + 80, uint32_t(*start));
  *span = int32_t(uint32_t(*end) - uint32_t(*start));
  const int32_t look = LoadS16(AiPath(path).plan_count_address());
  if (!(*end < look)) {
    *end = look;
    *reached = 1;
  }
  return true;
}
// A corner step's last part for the entry: kept in range, the step from
// `from` limited, ClampOffset, the previous entry and the index moved on.
// True while the index is below `end`.
bool FinishCornerEntry(uint32_t path, uint32_t entry, double from, double zero,
                       uint32_t index, uint32_t previous, int32_t end, uint32_t frame) {
  KeepInRange(entry, Lfs(AiPathEntry(entry).offset_address()));
  LimitStep(path, entry, from, zero);
  ClampOffset(path, entry, index, frame + 80, frame - 96);
  guest::StoreU32(previous, entry);
  guest::StoreU32(index, Load(index) + 1);
  return int32_t(Load(index)) < end;
}
}  // namespace

void ContinueLine(uint32_t path, uint32_t entry, uint32_t index, uint32_t first,
                  uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213DB08 AiPath::ContinueLine");
  if (!(int32_t(Load(index)) < LoadS16(AiPath(path).plan_count_address()))) {
    return;
  }
  const double zero = Lfs(kZero);
  uint32_t previous = entry;
  do {
    const uint32_t at = CountedRingEntry(path, int32_t(Load(index)));
    const double offset = Fadds(Lfs(AiPathEntry(at).step_address()),
                                Lfs(AiPathEntry(previous).offset_address()));
    Stfs(AiPathEntry(at).offset_address(), offset);
    const double low = Lfs(AiPathEntry(at).room_low_address());
    bool clamp = offset < low;
    double bound = low;
    if (!clamp) {
      bound = Lfs(AiPathEntry(at).room_high_address());
      clamp = offset > bound;
    }
    if (clamp) {
      Stfs(AiPathEntry(at).offset_address(), bound);
      LimitStep(path, at, offset, zero);
    }
    ClampOffset(path, at, index, first, frame - 96);
    guest::StoreU32(index, Load(index) + 1);
    previous = at;
  } while (int32_t(Load(index)) < LoadS16(AiPath(path).plan_count_address()));
}

uint32_t ApproachCorner(uint32_t path, uint32_t record, uint32_t index,
                        uint32_t previous, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213DE30 AiPath::ApproachCorner");
  int32_t end = int32_t(uint32_t(LoadS16(AiCorner(record).entry_offset_address())) + 1);
  const int32_t start = int32_t(Load(index));
  if (!(end > start)) {
    return 0;
  }
  const uint32_t at = CountedRingEntry(path, end);
  guest::StoreU32(frame + 80, uint32_t(start));
  const double target = at != 0 ? Lfs(AiPathEntry(at).target_offset_address())
                                : Lfs(AiCorner(record).table().address + 4);
  const double gap =
      Fmsubs(target, Lfs(G::four_fifths_address()), Lfs(Load(previous) + 24));
  const double most =
      Fmuls(Lfs(AiPath(path).largest_step_address()), Lfs(G::half_address()));
  const double zero = Lfs(kZero);
  double step = Fdivs(gap, IntToFloat(int32_t(uint32_t(end) - uint32_t(start))));
  if (Fadds(most, step) < zero) {
    step = Fneg(most);
  } else if (Fsubs(step, most) > zero) {
    step = most;
  }
  uint32_t reached = 0;
  const int32_t look = LoadS16(AiPath(path).plan_count_address());
  if (!(end < look)) {
    end = look;
    reached = 1;
  }
  if (!(start < end)) {
    return reached;
  }
  bool more;
  do {
    const uint32_t entry = CountedRingEntry(path, int32_t(Load(index)));
    const double from = Fadds(Lfs(Load(previous) + 24),
                              Lfs(AiPathEntry(entry).step_address()));
    Stfs(AiPathEntry(entry).offset_address(), Fadds(from, step));
    more = FinishCornerEntry(path, entry, from, zero, index, previous, end, frame);
  } while (more);
  return reached;
}

uint32_t CornerToEntry(uint32_t path, uint32_t record, uint32_t index,
                       uint32_t previous, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213E018 AiPath::CornerToEntry");
  int32_t start, end, span;
  uint32_t reached;
  if (!CornerSpan(path, record, 50, index, frame, &start, &end, &span, &reached)) {
    return 0;
  }
  const uint32_t first = Load(previous);
  const double step = Fdivs(Fsubs(Lfs(AiPathEntry(first).target_offset_address()),
                                  Lfs(AiPathEntry(first).offset_address())),
                            IntToFloat(span));
  if (!(start < end)) {
    return reached;
  }
  const double zero = Lfs(kZero);
  bool more;
  do {
    const uint32_t entry = CountedRingEntry(path, int32_t(Load(index)));
    const uint32_t before = Load(previous);
    const double target = Lfs(AiPathEntry(entry).target_offset_address());
    const double side = Lfs(AiCorner(record).direction_address());
    const double limit = Fmuls(side, target);
    const double offset = Lfs(AiPathEntry(before).offset_address());
    double from =
        Fsubs(Fadds(target, offset), Lfs(AiPathEntry(before).target_offset_address()));
    if (Fmuls(side, from) < limit) {
      Stfs(AiPathEntry(entry).offset_address(), Fadds(step, from));
    } else {
      from = Fadds(Lfs(AiPathEntry(entry).step_address()), offset);
      if (Fmuls(from, Lfs(AiCorner(record).direction_address())) < limit) {
        Stfs(AiPathEntry(entry).offset_address(), target);
      } else {
        Stfs(AiPathEntry(entry).offset_address(), from);
      }
    }
    more = FinishCornerEntry(path, entry, from, zero, index, previous, end, frame);
  } while (more);
  return reached;
}

uint32_t CornerToApex(uint32_t path, uint32_t record, uint32_t index,
                      uint32_t previous, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213E1D0 AiPath::CornerToApex");
  int32_t start, end, span;
  uint32_t reached;
  if (!CornerSpan(path, record, 52, index, frame, &start, &end, &span, &reached)) {
    return 0;
  }
  const uint32_t first = Load(previous);
  const double step = Fdivs(Fsubs(Lfs(AiPathEntry(first).target_offset_address()),
                                  Lfs(AiPathEntry(first).offset_address())),
                            IntToFloat(span));
  if (!(start < end)) {
    return reached;
  }
  const double zero = Lfs(kZero);
  bool more;
  do {
    const uint32_t entry = CountedRingEntry(path, int32_t(Load(index)));
    const uint32_t before = Load(previous);
    const double target = Lfs(AiPathEntry(entry).target_offset_address());
    const double from = Fsubs(Fadds(Lfs(AiPathEntry(before).offset_address()), target),
                              Lfs(AiPathEntry(before).target_offset_address()));
    const double offset = Fadds(step, from);
    Stfs(AiPathEntry(entry).offset_address(), offset);
    const double side = Lfs(AiCorner(record).direction_address());
    if (Fmuls(side, offset) < Fmuls(side, target)) {
      Stfs(AiPathEntry(entry).offset_address(), target);
    }
    more = FinishCornerEntry(path, entry, from, zero, index, previous, end, frame);
  } while (more);
  return reached;
}

uint32_t CornerToExit(uint32_t path, uint32_t record, uint32_t index,
                      uint32_t previous, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213E360 AiPath::CornerToExit");
  int32_t start, end, span;
  uint32_t reached;
  if (!CornerSpan(path, record, 54, index, frame, &start, &end, &span, &reached)) {
    return 0;
  }
  const uint32_t first = Load(previous);
  const double scale = Lfs(G::four_fifths_address());
  const double step = Fdivs(Fmuls(Fsubs(Lfs(AiPathEntry(first).target_offset_address()),
                                        Lfs(AiPathEntry(first).offset_address())),
                                  scale),
                            IntToFloat(span));
  if (!(start < end)) {
    return reached;
  }
  const double zero = Lfs(kZero);
  bool more;
  do {
    const uint32_t entry = CountedRingEntry(path, int32_t(Load(index)));
    const uint32_t before = Load(previous);
    const double target = Lfs(AiPathEntry(entry).target_offset_address());
    const double from =
        Fmadds(Fsubs(target, Lfs(AiPathEntry(before).target_offset_address())), scale,
               Lfs(AiPathEntry(before).offset_address()));
    Stfs(AiPathEntry(entry).offset_address(), from);
    const double side = Lfs(AiCorner(record).direction_address());
    if (Fmuls(side, from) < Fmuls(side, target)) {
      Stfs(AiPathEntry(entry).offset_address(), Fadds(step, from));
    }
    more = FinishCornerEntry(path, entry, from, zero, index, previous, end, frame);
  } while (more);
  return reached;
}

uint32_t PlanCorners(uint32_t path, uint32_t entry, uint32_t index, uint32_t end,
                     uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213E7B8 AiPath::PlanCorners");
  const uint32_t previous = frame + 172;
  guest::StoreU32(previous, entry);
  for (int32_t k = 0; k < LoadS16(AiPath(path).corner_count_address()); k++) {
    const uint32_t record = path + 5164 + 44 * uint32_t(k);
    if (AiCorner(record).data().address == 0 || AiCorner(record).table().address == 0) {
      continue;
    }
    const int32_t offset = LoadS16(AiCorner(record).entry_offset_address());
    if (!(offset < 1) && int32_t(Load(index)) > offset) {
      guest::StoreU32(index, uint32_t(offset));
      guest::StoreU32(previous,
                      CountedRingEntry(path, int32_t(uint32_t(offset) - 1)));
    }
    const int32_t until = int32_t(Load(end));
    if (int32_t(Load(index)) < until) {
      guest::StoreU32(index, uint32_t(until));
      guest::StoreU32(previous, CountedRingEntry(path, int32_t(uint32_t(until) - 1)));
    }
    if (ApproachCorner(path, record, index, previous, frame - 160) == 1 ||
        CornerToEntry(path, record, index, previous, frame - 160) == 1 ||
        CornerToApex(path, record, index, previous, frame - 160) == 1 ||
        CornerToExit(path, record, index, previous, frame - 160) == 1) {
      return 1;
    }
    guest::StoreU32(end,
                    uint32_t(LoadS16(AiCorner(record).data().address + 54)) +
                        uint32_t(LoadS16(AiCorner(record).entry_offset_address())));
  }
  guest::StoreU32(end, Load(index));
  ContinueLine(path, Load(previous), index, end, frame - 112);
  return path;
}

uint32_t PlanLine(uint32_t path, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213F4B0 AiPath::PlanLine");
  const uint32_t entry = CountedRingEntry(path, 0);
  guest::StoreU32(frame + 80, 1);
  guest::StoreU32(frame + 84, 1);
  if (LoadS16(AiPath(path).corner_count_address()) > 0) {
    return PlanCorners(path, entry, frame + 84, frame + 80, frame - 144);
  }
  ContinueLine(path, entry, frame + 84, frame + 80, frame - 112);
  return path;
}

// --- The path's branch kinds and lines ---

namespace {
// The race data's flags ([0x826C5B2C] + 20).
uint32_t RaceFlags() { return G::race_data().branch_flags(); }
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
bool BranchNumberAt(uint32_t identity, uint32_t* number) {
  const uint32_t section = TrackPosition(identity).route_section();
  if (int16_t(section) < 0) {
    return false;
  }
  const uint32_t table = RG::route_file().sections();
  *number = guest::LoadU8(Load(table + (section << 2)) + 3);
  return true;
}
}  // namespace

uint32_t BranchMasksFor(uint32_t kind) {
  ITC_NATIVE_RUNNING("sub_8213D378 AiPath::BranchMasksFor");
  const uint32_t flags = RaceFlags();
  if (!BranchFlags(flags)) {
    return G::branch_mask_lists_address();
  }
  switch (Load(kind)) {
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

uint32_t LeaderBranchKind(uint32_t path, uint32_t kind, uint32_t best) {
  ITC_NATIVE_RUNNING("sub_8213D1F0 AiPath::LeaderBranchKind");
  const uint32_t flags = RaceFlags();
  if (!BranchFlags(flags)) {
    return path;
  }
  const uint32_t driver = AiPath(path).driver().address;
  const uint32_t vehicle = AiDriver(driver).vehicle().address;
  if (vehicle == 0) {
    return 0;
  }
  if (int32_t(Load(kind)) != -1 &&
      !(Lfs(AiDriver(driver).player_gap_address()) > Lfs(best))) {
    return vehicle;
  }
  const uint32_t identity = vehicle_accessors::VehicleIdentity(vehicle);
  uint32_t number = 0;
  if (!BranchNumberAt(identity, &number)) {
    return identity;
  }
  const int32_t found = KindForBranch(flags, number);
  if (found < 0) {
    return identity;
  }
  guest::StoreU32(kind, uint32_t(found));
  Stfs(best, Lfs(AiDriver(driver).player_gap_address()));
  return identity;
}

uint64_t FindSideSection(uint32_t path, uint32_t out, uint32_t point,
                         uint32_t masks, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213D550 AiPath::FindSideSection");
  const uint32_t identity =
      vehicle_accessors::VehicleIdentity(Load(AiPath(path).driver().address + 160));
  const uint32_t position = frame + 80;
  track_route::ClampPosition(position, TrackPosition(identity).route_position());
  uint64_t r3 = position;
  const uint32_t section = guest::LoadU16(position + 0);
  if (int16_t(section) < 0) {
    return r3;
  }
  const uint16_t slice = guest::LoadU16(position + 2);
  if (int16_t(slice) < 0) {
    return r3;
  }
  const uint32_t route = track_route::kRoute;
  uint32_t taken = Load(position);
  guest::StoreU32(frame + 84, taken);
  uint32_t current =
      Load(Load(Load(route) + 12) + ((section & 0xFFFF) << 2));
  uint32_t link = current + 52;
  for (int i = 0; i < 4; i++, link += 4) {
    const uint16_t number = guest::LoadU16(link);
    if (int16_t(number) < 0) {
      continue;
    }
    const uint32_t candidate =
        Load(Load(Load(route) + 12) + (uint32_t(number) << 2));
    const int32_t prefer =
        CompareBranches(path, current, candidate, frame + 84, link, masks);
    r3 = uint64_t(int64_t(prefer));
    if (!(prefer > 0)) {
      continue;
    }
    guest::StoreU16(position + 0, number);
    guest::StoreU16(position + 2, slice);
    r3 = track_route::Normalize(route, position, masks);
    r3 = track_route::OnSlice(route, point, position, 0, frame - 128);
    if (r3 == 0) {
      continue;
    }
    track_route::ClampPosition(frame + 88, Load(position));
    r3 = frame + 88;
    taken = Load(frame + 88);
    current = candidate;
    guest::StoreU32(frame + 84, taken);
  }
  guest::StoreU32(out, taken);
  return r3;
}

void FollowTargets(uint32_t path, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213D9D8 AiPath::FollowTargets");
  uint32_t previous = CountedRingEntry(path, 0);
  int32_t i = 1;
  guest::StoreU32(frame + 80, uint32_t(i));
  if (!(LoadS16(AiPath(path).plan_count_address()) > 1)) {
    return;
  }
  const double zero = Lfs(kZero);
  do {
    const uint32_t entry = CountedRingEntry(path, i);
    const double target = Lfs(AiPathEntry(previous).target_offset_address());
    const double before = Lfs(AiPathEntry(previous).offset_address());
    const double step = Lfs(AiPathEntry(entry).step_address());
    Stfs(AiPathEntry(entry).offset_address(), target);
    const double from = Fadds(step, before);
    KeepInRange(entry, target);
    LimitStep(path, entry, from, zero);
    guest::StoreU32(frame + 84, 1);
    ClampOffset(path, entry, frame + 80, frame + 84, frame - 96);
    i = int32_t(uint32_t(i) + 1);
    previous = entry;
    guest::StoreU32(frame + 80, uint32_t(i));
  } while (i < LoadS16(AiPath(path).plan_count_address()));
}

uint32_t FollowLeaderBranch(uint32_t path) {
  ITC_NATIVE_RUNNING("sub_8213EB48 AiPath::FollowLeaderBranch");
  const uint32_t driver = AiPath(path).driver().address;
  uint32_t r3 = path;
  int32_t kind = -1;
  if ((AiDriver(driver).flags() & 0x4) != 0) {
    const uint32_t object = AiDriver(driver).profile().address;
    const uint32_t flags = RaceFlags();
    if (BranchFlags(flags) && object != 0) {
      const int32_t mode = LoadS16(AiProfile(object).branch_kind_address());
      if ((mode < 4 || mode > 5) && !(Lfs(AiDriver(driver).player_gap_address()) >
                                      Lfs(G::leader_gap_address()))) {
        const int32_t count = int32_t(track_collision::RaceRacerCount());
        uint32_t found = 0;
        for (int32_t i = 0; i < count; i++) {
          const uint32_t racer = Load(G::race().racers_address() + 4 * uint32_t(i));
          const uint32_t index = MinimapRacerEntry(racer).vehicle_index();
          if (int32_t(index) < 0) {
            continue;
          }
          const uint32_t vehicle = vehicle_accessors::VehicleGet(index);
          if (vehicle != 0 && (Vehicle(vehicle).car_flags() & 0x81) != 0) {
            found = vehicle;
            break;
          }
        }
        r3 = 0;
        if (found != 0) {
          const uint32_t identity = vehicle_accessors::VehicleIdentity(found);
          r3 = identity;
          uint32_t number = 0;
          if (BranchNumberAt(identity, &number)) {
            kind = KindForBranch(flags, number);
          }
        }
      }
    }
  }
  if (int32_t(AiPath(path).leader_branch_kind()) != kind) {
    AiPath(path).set_leader_branch_kind(uint32_t(kind));
    r3 = ResetPath(path);
  }
  return r3;
}

// --- The drivers' updates and the pairs ---

uint64_t CompareWith(uint64_t a, uint32_t other, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82145B08 AiDriver::CompareWith");
  const uint32_t driver = uint32_t(a);
  guest::StoreU32(frame + 84, Load(other));
  const uint64_t b64 = Of(frame + 84);
  const uint32_t b = uint32_t(b64);
  const uint32_t record = uint32_t(uint64_t(Load(other)) * 40 + a + 172);
  if (AiDriver(b).vehicle().address == 0) {
    ResetOtherRecord(record, Lfs(G::other_gap_address()), Lfs(kZero),
                     Lfs(kOne), Lfs(G::minus_one_address()));
    return b64;
  }
  const uint32_t mine =
      vehicle_accessors::VehicleIdentity(AiDriver(driver).vehicle().address);
  const uint32_t theirs = vehicle_accessors::VehicleIdentity(AiDriver(b).vehicle().address);
  const double distance =
      track_route::Distance(track_route::kRoute, theirs + 4, mine + 4);
  const double length = Fadds(Lfs(AiDriver(b).car_half_length_address()),
                              Lfs(AiDriver(driver).car_half_length_address()));
  const double ahead = Fadds(Lfs(AiDriver(driver).slice_offset_address()), distance);
  Stfs(AiOther(record).speed_address(),
       Lfs(AiDriver(b).speed_kmh_address()));
  Stfs(frame + 88, Fsubs(ahead, Lfs(AiDriver(b).slice_offset_address())));
  Stfs(frame + 80, Fmuls(length, Lfs(G::four_fifths_address())));
  const bool both =
      (AiDriver(driver).flags() & 0x4) != 0 && (AiDriver(b).flags() & 0x4) != 0;
  AiOther(record).set_flag_a(both ? 1 : 0);
  AiOther(record).set_flag_b(0);
  UpdateOther(record, frame + 88, frame + 80);
  const uint64_t back64 = uint64_t(AiDriver(driver).index()) * 40 + b64 + 172;
  const uint32_t back = uint32_t(back64);
  MirrorOther(back, record, driver + 100);
  if (AiDriver(driver).lane() == AiDriver(b).lane()) {
    UpdateOtherTimer(record, other, frame + 80);
    UpdateOtherTimer(back, driver, frame + 80);
  }
  return back64;
}

uint64_t UpdateAll(uint32_t frame, DriverUpdate update, void* user) {
  ITC_NATIVE_RUNNING("sub_82139628 AiDrivers::UpdateAll");
  const uint32_t count = track_collision::RaceRacerCount();
  uint64_t r3 = count;
  if (Load(G::drivers_address()) == 0 || !(int32_t(count) > 0)) {
    return r3;
  }
  const auto index_of = [&](uint32_t i) {
    return MinimapRacerEntry(Load(G::race().racers_address() + 4 * i))
        .vehicle_index();
  };
  const auto driver_of = [&](uint32_t index) {
    return uint64_t(index) * AiDriver::kSize + Load(G::drivers_address());
  };
  for (uint32_t i = 0; i < count; i++) {
    const uint64_t driver = driver_of(index_of(i));
    r3 = update(user, driver, Load(uint32_t(driver) + 6148));
  }
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t mine = index_of(i);
    for (uint32_t j = i + 1; int32_t(j) < int32_t(count); j++) {
      const uint32_t theirs = index_of(j);
      uint64_t driver;
      if (int32_t(mine) < int32_t(theirs)) {
        guest::StoreU32(frame + 80, theirs);
        driver = driver_of(mine);
      } else {
        guest::StoreU32(frame + 80, mine);
        driver = driver_of(theirs);
      }
      r3 = CompareWith(driver, frame + 80, frame - 144);
    }
  }
  return r3;
}

// --- Reacting to the other racers ---

namespace {
// A skill percentage (a halfword) kept in 0 to 100.
int32_t Percentage(uint32_t at) {
  const int32_t value = LoadS16(at);
  if (value < 0) {
    return 0;
  }
  return value > 100 ? 100 : value;
}
void OrFlags(uint32_t record, uint16_t bits) {
  AiOther(record).set_action_flags(uint16_t(AiOther(record).action_flags() | bits));
}
uint16_t Truncated(double value) {
  return uint16_t(strings_and_font::steps::Fctiwz(value));
}
// PlanReaction's +12 from a percentage, with the flag 0x2.
void ReactionScale(uint32_t record, int32_t percentage) {
  const double scale = IntToFloat(percentage);
  OrFlags(record, 0x2);
  Stfs(AiOther(record).factor_address(),
       Fmuls(scale, Lfs(G::hundredth_address())));
}
}  // namespace

void PlanReaction(uint32_t record, uint32_t skill, uint32_t rate, uint32_t flag,
                  uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_821456F8 AiOther::PlanReaction");
  const uint32_t state = uint32_t(LoadS16(AiOther(record).state_address()));
  Stfs(AiOther(record).factor_address(), Lfs(kOne));
  if (state < 1) {
    AiOther(record).set_ring_index(0xFFFF);
    AiOther(record).set_action_flags(0);
    return;
  }
  if (state == 1) {
    const double gap = Lfs(G::react_gap_address());
    if (int32_t(flag) == 0 &&
        Lfs(AiOther(record).speed_address()) > Lfs(G::react_speed_address()) &&
        !(Lfs(AiOther(record).closing_rate_address()) >
          Lfs(G::tenth_address())) &&
        Lfs(AiOther(record).gap_address()) < gap) {
      AiOther(record).set_ring_index(0xFFFF);
      return;
    }
    if (Lfs(AiOther(record).gap_address()) > gap &&
        !(Lfs(AiOther(record).closing_rate_address()) <
          Lfs(G::react_rate_address()))) {
      AiOther(record).set_ring_index(1);
      OrFlags(record, 0x1);
      return;
    }
    const double meet = Lfs(AiOther(record).ticks_to_meet_address());
    if (meet < Lfs(kZero)) {
      AiOther(record).set_ring_index(0xFFFF);
      return;
    }
    OrFlags(record, 0x1);
    const int32_t percentage = Percentage(skill + 16);
    const double by = Lfs(rate);
    double ahead = Fadds(IntToFloat(percentage), Lfs(G::hundred_address()));
    ahead = Fmuls(ahead, by);
    ahead = Fmuls(ahead, meet);
    AiOther(record)
        .set_ring_index(Truncated(Fmuls(ahead, Lfs(G::hundredth_address()))));
    return;
  }
  if (state < 3) {
    AiOther(record).set_ring_index(1);
    if (Percentage(skill + 14) > 0) {
      return;
    }
    OrFlags(record, 0x1);
    return;
  }
  if (state != 3) {
    return;
  }
  if (int32_t(mode) == 1 && int32_t(flag) == 0 &&
      LoadS16(AiOther(record).timer_address()) > 0) {
    AiOther(record).set_ring_index(2);
    OrFlags(record, 0x10);
    return;
  }
  if (Lfs(AiOther(record).gap_address()) > Lfs(G::ten_address())) {
    const double meet = Lfs(AiOther(record).ticks_to_meet_address());
    if (meet < Lfs(kZero)) {
      AiOther(record).set_ring_index(0xFFFF);
      return;
    }
    AiOther(record).set_ring_index(Truncated(Fmuls(Lfs(rate), meet)));
    if (AiOther(record).flag_b() != 1) {
      return;
    }
    const int32_t percentage = Percentage(skill + 14);
    if (!(percentage > 0)) {
      return;
    }
    ReactionScale(record, percentage);
    return;
  }
  if (int32_t(flag) == 0) {
    AiOther(record).set_ring_index(1);
    OrFlags(record, 0x1);
    return;
  }
  AiOther(record).set_ring_index(0xFFFF);
  if (AiOther(record).flag_b() != 1) {
    return;
  }
  const int32_t percentage = Percentage(skill + 14);
  if (!(percentage > 0)) {
    return;
  }
  AiOther(record).set_ring_index(1);
  ReactionScale(record, percentage);
}

uint64_t ReactToOthers(uint32_t driver, uint32_t count, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82145CA8 AiDriver::ReactToOthers");
  guest::StoreU32(frame + 84, AiDriver(driver).index());
  const uint32_t file = RG::route_file().address;
  const uint64_t skill = Of(frame + 84) + 6048;
  const uint32_t racers = track_collision::RaceRacerCount();
  uint64_t r3 = racers;
  const double speed = Lfs(AiDriver(driver).speed_kmh_address());
  double rate = Fmuls(Lfs(G::tick_address()), Lfs(G::per_kmh_address()));
  const double length = Lfs(TrackRouteFile(file).slice_length_address());
  const double zero = Lfs(kZero);
  rate = Fdivs(rate, length);
  Stfs(frame + 80, Fmuls(rate, speed));
  uint32_t listed = 0;
  if (int32_t(racers) > 0) {
    const double ten = Lfs(G::ten_address());
    const double far = Lfs(G::react_gap_address());
    const double one = Lfs(kOne);
    uint32_t slot = driver + 452;
    for (uint32_t i = 0; i < racers; i++) {
      const uint32_t other =
          MinimapRacerEntry(Load(G::race().racers_address() + 4 * i))
              .vehicle_index();
      guest::StoreU32(frame + 88, other);
      if (int32_t(AiDriver(driver).index()) == int32_t(other)) {
        continue;
      }
      const uint64_t record64 = uint64_t(other) * 40 + driver + 172;
      const uint32_t record = uint32_t(record64);
      AiOther(record).set_action_flags(0);
      const uint32_t keeps = KeepsGapFrom(driver, frame + 88);
      r3 = keeps;
      bool react = int32_t(keeps) != 0;
      if (!react) {
        const uint32_t bit = (other & 0x20) != 0 ? 0 : 1u << (other & 0x3F);
        if ((uint32_t(LoadS16(AiDriver(driver).racer_mask_address())) & bit) != 0) {
          Stfs(AiOther(record).factor_address(), one);
          const int32_t state = int32_t(AiDriver(driver).state());
          if (state == 2) {
            const double gap = Lfs(AiOther(record).gap_address());
            if (gap < zero) {
              const double slices =
                  Fdivs(gap, Lfs(TrackRouteFile(file).slice_length_address()));
              AiOther(record).set_action_flags(4);
              AiOther(record).set_ring_index(Truncated(Fneg(slices)));
            }
          } else if (state == 3) {
            const double gap = Lfs(AiOther(record).gap_address());
            AiOther(record).set_action_flags(1);
            if (gap < far && gap > ten) {
              AiOther(record)
                  .set_ring_index(Truncated(Fdivs(
                      Fabs(gap), Lfs(TrackRouteFile(file).slice_length_address()))));
            } else {
              AiOther(record).set_ring_index(1);
            }
          } else {
            react = true;
          }
        }
      }
      if (react) {
        const uint32_t vehicle = AiDriver(driver).vehicle().address;
        const uint32_t flag7 = vehicle != 0 ? (Vehicle(vehicle).car_flags() >> 2) & 1 : 0;
        const uint32_t flag6 = (AiDriver(driver).flags() >> 2) & 1;
        PlanReaction(record, uint32_t(skill), frame + 80, flag6, flag7);
        r3 = record64;
      }
      if (AiOther(record).action_flags() != 0) {
        const int32_t at = LoadS16(AiOther(record).ring_index_address());
        if (!(at < 0) && at < int32_t(Load(count))) {
          guest::StoreU32(slot, other);
          listed++;
          slot += 4;
        }
      }
    }
  }
  guest::StoreU32(((listed + 113) << 2) + driver, 0xFFFFFFFFu);
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
      const uint32_t a = Load(at);
      const uint32_t b = Load(next);
      const uint32_t ra = uint32_t(uint64_t(a) * 40 + driver);
      const uint32_t rb = uint32_t(uint64_t(b) * 40 + driver);
      const int32_t ia = LoadS16(ra + 198);
      const int32_t ib = LoadS16(rb + 198);
      bool swap = ia > ib;
      if (ia == ib) {
        const double gb = Lfs(rb + 172);
        swap = !(gb > zero) && Lfs(ra + 172) < gb;
      }
      if (swap) {
        guest::StoreU32(at, b);
        guest::StoreU32(next, a);
      }
    }
  }
  return k;
}

// --- The drivers' starts ---

namespace {
// The race's racer i's record ([0x826D82F4] + 2308).
uint32_t RacerAt(uint32_t i) {
  return Load(G::race().racers_address() + 4 * i);
}
}  // namespace

uint64_t StartGroup(uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213A000 AiDrivers::StartGroup");
  const uint32_t count = track_collision::RaceRacerCount();
  uint64_t r3 = count;
  StoreConstant(frame + 88, G::minus_hundred_address());
  uint32_t mask = 0;
  guest::StoreU32(frame + 80, mask);
  guest::StoreU32(frame + 84, 0xFFFFFFFFu);
  if (Load(G::drivers_address()) == 0 || !(int32_t(count) > 0)) {
    return r3;
  }
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t racer = RacerAt(i);
    const uint32_t flags = MinimapRacerEntry(racer).flags();
    if ((flags & 0x10) == 0) {
      continue;
    }
    const uint32_t index = MinimapRacerEntry(racer).vehicle_index();
    if (int32_t(index) < 0) {
      continue;
    }
    if ((flags & 0xC) == 0) {
      mask |= Slw(1, index);
    }
    const uint64_t driver = uint64_t(index) * AiDriver::kSize + Load(G::drivers_address());
    r3 = LeaderBranchKind(uint32_t(driver + 512), frame + 84, frame + 88);
  }
  guest::StoreU32(frame + 80, mask);
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t racer = RacerAt(i);
    const uint32_t index = MinimapRacerEntry(racer).vehicle_index();
    if (int32_t(index) < 0) {
      continue;
    }
    const uint32_t flags = MinimapRacerEntry(racer).flags();
    const uint64_t driver = uint64_t(index) * AiDriver::kSize + Load(G::drivers_address());
    if ((flags & 0x10) == 0) {
      continue;
    }
    if ((flags & 0xC) == 0) {
      guest::StoreU32(frame + 92, 0);
      r3 = SetState2or3(uint32_t(driver), frame + 80, frame + 92, frame - 112);
    }
    const uint64_t path = driver + 512;
    r3 = path;
    const uint32_t best = Load(frame + 84);
    if (int32_t(Load(uint32_t(path) + 5524)) != int32_t(best)) {
      guest::StoreU32(uint32_t(path) + 5524, best);
      r3 = ResetPath(uint32_t(path));
    }
  }
  return r3;
}

uint64_t StartRanked(uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213A8B0 AiDrivers::StartRanked");
  const uint32_t count = track_collision::RaceRacerCount();
  uint64_t r3 = count;
  uint32_t table = Load(G::drivers_address());
  StoreConstant(frame + 92, G::minus_hundred_address());
  uint32_t mask = 0;
  uint32_t listed = 0;
  guest::StoreU32(frame + 80, mask);
  guest::StoreU32(frame + 84, listed);
  guest::StoreU32(frame + 88, 0xFFFFFFFFu);
  if (table == 0 || !(int32_t(count) > 0)) {
    return r3;
  }
  const uint32_t list = frame + 96;
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t racer = RacerAt(i);
    if ((MinimapRacerEntry(racer).flags() & 0x10) == 0) {
      continue;
    }
    const uint32_t index = MinimapRacerEntry(racer).vehicle_index();
    if (int32_t(index) < 0) {
      continue;
    }
    const uint64_t driver = uint64_t(index) * AiDriver::kSize + table;
    mask |= Slw(1, index);
    r3 = LeaderBranchKind(uint32_t(driver + 512), frame + 88, frame + 92);
    guest::StoreU32(list + 4 * listed, index);
    listed++;
    table = Load(G::drivers_address());
  }
  guest::StoreU32(frame + 84, listed);
  guest::StoreU32(frame + 80, mask);
  // Sorted only with three or more listed, as the original (its count
  // checked against 2).
  if (int32_t(listed) > 2) {
    for (uint32_t i = 0; i < listed; i++) {
      for (uint32_t j = i + 1; j < listed; j++) {
        const uint32_t at = list + 4 * i;
        const uint32_t next = list + 4 * j;
        const uint32_t a = Load(at) * AiDriver::kSize + table;
        const uint32_t b = Load(next) * AiDriver::kSize + table;
        uint32_t flags = AiDriver(a).vehicle().car_flags() & 0x81;
        r3 = flags;
        if (flags != 0) {
          continue;
        }
        flags = AiDriver(b).vehicle().car_flags() & 0x81;
        r3 = flags;
        if (flags == 0 && Lfs(AiDriver(a).player_gap_address()) >
                              Lfs(AiDriver(b).player_gap_address())) {
          continue;
        }
        const uint32_t second = Load(next);
        const uint32_t first = Load(at);
        guest::StoreU32(at, second);
        guest::StoreU32(next, first);
      }
    }
  }
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t racer = RacerAt(i);
    const uint32_t index = MinimapRacerEntry(racer).vehicle_index();
    if (int32_t(index) < 0) {
      continue;
    }
    const uint32_t flags = MinimapRacerEntry(racer).flags();
    const uint64_t driver = uint64_t(index) * AiDriver::kSize + table;
    r3 = driver;
    if ((flags & 0x10) != 0) {
      SetStateFromList(uint32_t(driver), frame + 80, frame + 84, list, frame - 128);
      const uint64_t path = driver + 512;
      r3 = path;
      const uint32_t best = Load(frame + 88);
      if (int32_t(Load(uint32_t(path) + 5524)) != int32_t(best)) {
        guest::StoreU32(uint32_t(path) + 5524, best);
        r3 = ResetPath(uint32_t(path));
      }
    } else {
      r3 = SetState4(uint32_t(driver), frame + 80, frame - 112);
    }
    table = Load(G::drivers_address());
  }
  return r3;
}

// --- Reading the vehicle ---

namespace {
// The route slice record of a route position (the halfwords at `position`:
// the section, then the slice): the section's record (the route file's +12
// table) + 124 + 84 x the slice.
uint32_t SliceRecordAt(uint32_t position) {
  const uint32_t table = RG::route_file().sections();
  const uint32_t section = guest::LoadU16(position + 0);
  const int32_t slice = LoadS16(position + 2);
  return uint32_t(slice) * 84 + Load(table + (section << 2)) + 124;
}
}  // namespace

uint64_t ReadVehicle(uint32_t driver, uint32_t slice) {
  ITC_NATIVE_RUNNING("sub_821467F0 AiDriver::ReadVehicle");
  const uint32_t identity =
      vehicle_accessors::VehicleIdentity(AiDriver(driver).vehicle().address);
  const uint32_t body = AiDriver(driver).vehicle().body().address;
  const uint32_t spin = vehicle_accessors::RigidBodyAngularVelocity(body);
  for (uint32_t i = 0; i < 16; i += 4) {  // two ld/std pairs: the bits
    CopyWord(TrackPosition(identity).point_x_address() + i, driver + 80 + i);
  }
  Stfs(AiDriver(driver).point_w_address(), Lfs(kOne));
  Stfs(AiDriver(driver).speed_kmh_address(),
       double(Vehicle_GetSpeedKmh(AiDriver(driver).vehicle().address)));
  const uint32_t vehicle = AiDriver(driver).vehicle().address;
  const double yaw =
      Fmuls(Lfs(Vehicle(vehicle).inverse_dt_address()), Lfs(spin + 4));
  StoreConstant(AiDriver(driver).speed_limit_address(), G::five_hundred_address());
  StoreConstant(AiDriver(driver).speed_limit_other_address(),
                G::five_hundred_address());
  Stfs(AiDriver(driver).yaw_rate_address(),
       Fmuls(yaw, Lfs(G::yaw_scale_address())));
  const uint32_t orientation = vehicle_accessors::RigidBodyOrientation(body);
  const double x = Lfs(orientation + 0);
  const double zero = Lfs(kZero);
  if (x == zero && Lfs(orientation + 8) == zero) {
    Stfs(AiDriver(driver).heading_address(), zero);
  } else {
    Stfs(AiDriver(driver).heading_address(),
         double(float(c_runtime::Atan2(x, Lfs(orientation + 8)))));
  }
  const uint32_t own = AiDriver(driver).vehicle().address;
  CopyWord(own + 320, AiDriver(driver).steer_limit_a_address());
  Stfs(AiDriver(driver).steer_limit_b_address(), Fneg(Lfs(own + 316)));
  if (slice != 0) {
    const double dx = Fsubs(Lfs(AiDriver(driver).point_x_address()),
                            Lfs(RouteSlice(slice).center_x_address()));
    const double dz = Fsubs(Lfs(AiDriver(driver).point_z_address()),
                            Lfs(RouteSlice(slice).center_z_address()));
    const double across = Fmuls(dx, Lfs(RouteSlice(slice).side_x_address()));
    Stfs(AiDriver(driver).road_offset_address(),
         Fmadds(dz, Lfs(RouteSlice(slice).side_z_address()), across));
    const double side = Lfs(RouteSlice(slice).side_x_address());
    const double direction = double(
        float(c_runtime::Atan2(Lfs(RouteSlice(slice).side_z_address()), Fneg(side))));
    Stfs(AiDriver(driver).road_angle_address(),
         Fsubs(direction, Lfs(AiDriver(driver).heading_address())));
    WrapAngle(driver + 148);
    UpdateHalfWidth(driver);
  } else {
    Stfs(AiDriver(driver).road_offset_address(), zero);
  }
  const uint32_t record =
      SliceRecordAt(TrackPosition(identity).route_position_address());
  if (record != 0) {
    const double dx = Fsubs(Lfs(AiDriver(driver).point_x_address()),
                            Lfs(RouteSlice(record).center_x_address()));
    const double dz = Fsubs(Lfs(AiDriver(driver).point_z_address()),
                            Lfs(RouteSlice(record).center_z_address()));
    const double along = Fmuls(dx, Lfs(RouteSlice(record).forward_x_address()));
    Stfs(AiDriver(driver).slice_offset_address(),
         Fmadds(dz, Lfs(RouteSlice(record).forward_z_address()), along));
  } else {
    Stfs(AiDriver(driver).slice_offset_address(), zero);
  }
  const uint64_t r3 = UpdateAgainstPlayer(driver);
  AiDriver(driver).set_watched(0xFFFFFFFFu);
  return r3;
}

uint64_t ReadVehicleOwnSlice(uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_821469D0 AiDriver::ReadVehicleOwnSlice");
  const uint32_t identity =
      vehicle_accessors::VehicleIdentity(AiDriver(driver).vehicle().address);
  return ReadVehicle(driver,
                     SliceRecordAt(TrackPosition(identity).route_position_address()));
}

// --- The path's branch masks ---

uint64_t BranchMasks(uint32_t path, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213E500 AiPath::BranchMasks");
  // The lists' address as the original's lis and addi leave it (sign-extended).
  const uint64_t lists = uint64_t(int64_t(int32_t(G::branch_mask_lists_address())));
  const uint32_t driver = AiPath(path).driver().address;
  const uint32_t object = AiDriver(driver).profile().address;
  const uint32_t vehicle = AiDriver(driver).vehicle().address;
  uint32_t kind = AiPath(path).branch_kind();
  if (int32_t(kind) == -1) {
    kind = AiPath(path).leader_branch_kind();
  }
  if (int32_t(kind) != -1) {
    return uint64_t((kind << 4) & 0xFFFFFFF0u) + lists;
  }
  if (object != 0) {
    guest::StoreU32(frame + 80,
                    uint32_t(int32_t(int8_t(AiProfile(object).branch_byte()))));
    return uint64_t(int64_t(int32_t(BranchMasksFor(frame + 80))));
  }
  if (vehicle != 0) {
    const uint32_t flags = Vehicle(vehicle).car_flags();
    if ((flags & 0x81) == 0) {
      if ((flags & 0x4) == 0) {
        return lists;
      }
      const uint32_t other = Vehicle(vehicle).car();
      if (other == 0) {
        return 0;
      }
      const int32_t word = int32_t(Load(other + 12));
      if (word == 66 || word == 67 || word == 70) {
        return lists + 128;
      }
      return 0;
    }
  }
  if (RaceAllowsSides() == 0) {
    return lists;
  }
  switch (G::race_data().route_course()) {
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

uint32_t KeepBranch(uint32_t path, uint32_t a, uint32_t b, uint32_t point,
                    uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213EAB0 AiPath::KeepBranch");
  const uint64_t masks = BranchMasks(path, frame - 96);
  const uint32_t table = RG::route_file().sections();
  const uint32_t section_b = Load(table + (uint32_t(guest::LoadU16(b)) << 2));
  const uint32_t section_a = Load(table + (uint32_t(guest::LoadU16(a)) << 2));
  const int32_t prefer =
      CompareBranches(path, section_a, section_b, a, b, uint32_t(masks));
  if (prefer != 0) {
    return prefer < 0 ? 1 : 0;
  }
  return track_route::LocateAt(track_route::kRoute, point, b, frame - 112) != 0 ? 1 : 0;
}

uint32_t LeftEntry(uint32_t path, uint32_t point, uint32_t entry,
                   uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213F140 AiPath::LeftEntry");
  const uint32_t at = entry + 16;
  const uint32_t driver = AiPath(path).driver().address;
  const uint32_t position = driver + 4;
  if (guest::LoadU16(at + 0) == guest::LoadU16(position + 0) &&
      guest::LoadU16(at + 2) == guest::LoadU16(position + 2)) {
    const int32_t lane = int32_t(AiPathEntry(entry).lane());
    if (lane < 0 || lane == LoadS16(AiDriver(driver).lane_address())) {
      return 0;
    }
  }
  const uint32_t record = AiPathEntry(entry).slice_record().address;
  const double dz =
      Fsubs(Lfs(point + 8), Lfs(RouteSlice(record).center_z_address()));
  const double dx =
      Fsubs(Lfs(point + 0), Lfs(RouteSlice(record).center_x_address()));
  const double length = Lfs(RG::route_file().slice_length_address());
  const double along = Fmadds(Lfs(RouteSlice(record).forward_x_address()), dx,
                              Fmuls(Lfs(RouteSlice(record).forward_z_address()), dz));
  if (along > length) {
    return 1;
  }
  if (along < Fneg(length)) {
    AiPath(path).set_entry_count(0);
    return 1;
  }
  const double half =
      Fmuls(Lfs(RouteSlice(record).width_address()), Lfs(G::half_address()));
  const double across = Fmadds(Lfs(RouteSlice(record).side_x_address()), dx,
                               Fmuls(Lfs(RouteSlice(record).side_z_address()), dz));
  Stfs(frame + 80, across);
  const double minus_half = Fneg(half);
  if (across > half || across < minus_half) {
    if (across > Fadds(Lfs(RouteSlice(record).margin_right_address()), half)) {
      return 1;
    }
    if (across < Fsubs(minus_half, Lfs(RouteSlice(record).margin_left_address()))) {
      return 1;
    }
    if (KeepBranch(path, at, position, point, frame - 128) == 0) {
      return 1;
    }
    if (WithinRoadAt(path, frame + 80) == 0) {
      return 1;
    }
  }
  int32_t lane = 0;
  if (!(int32_t(AiPathEntry(entry).lane()) < 0)) {
    lane = int16_t(track_route::LaneAt(track_route::kRoute, point, at));
    if (int32_t(AiPathEntry(entry).lane()) != lane) {
      return 1;
    }
  }
  guest::StoreU16(position + 0, guest::LoadU16(at + 0));
  guest::StoreU16(position + 2, guest::LoadU16(at + 2));
  AiDriver(driver).set_lane(uint16_t(lane));
  return 0;
}

bool CornersMovable(uint32_t path) {
  const uint32_t records = path + 5164;
  return records < 0xA0000000u && c_runtime::InOneView(records, 9 * 44);
}

bool DropPassed(uint32_t path, uint32_t point, uint32_t frame, uint64_t* r3) {
  // AdvanceCorners' condition for its native copies (it only depends on the
  // path): checked first, so the original runs whole where it fails.
  if (!CornersMovable(path)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_8213F2E0 AiPath::DropPassed");
  *r3 = path;
  if (!(AiPath(path).first() < 32768) ||
      !(LoadS16(AiPath(path).entry_count_address()) > 0)) {
    return true;
  }
  const uint32_t before = AiPath(path).entry_count();
  const auto first = [&]() {
    return path + 16 + (uint32_t(LoadS16(AiPath(path).first_address())) << 6);
  };
  uint32_t left = LeftEntry(path, point, first(), frame - 160);
  *r3 = left;
  if (left == 0) {
    return true;
  }
  guest::StoreU16(frame + 80, 0xFFFF);
  guest::StoreU16(frame + 82, 0xFFFF);
  const double one = Lfs(kOne);
  const double zero = Lfs(kZero);
  int32_t tries = 5;
  for (;;) {
    const uint32_t entry = first();
    const uint32_t none = Load(frame + 80);
    Stfs(AiPathEntry(entry).x_address(), zero);
    AiPathEntry(entry).set_route_position(none);
    Stfs(AiPathEntry(entry).y_address(), zero);
    AiPathEntry(entry).set_lane(0xFFFFFFFFu);
    Stfs(AiPathEntry(entry).z_address(), zero);
    AiPathEntry(entry).set_slice_record(0);
    Stfs(AiPathEntry(entry).w_address(), one);
    Stfs(AiPathEntry(entry).offset_address(), zero);
    Stfs(AiPathEntry(entry).target_offset_address(), zero);
    for (uint32_t at = 28; at <= 44; at += 4) {
      Stfs(entry + at, zero);
    }
    const int16_t count = int16_t(AiPath(path).entry_count() - 1);
    AiPath(path).set_entry_count(uint16_t(count));
    if (!(count > 0) || --tries < 0) {
      *r3 = ResetPath(path);
      return true;
    }
    const int32_t next = LoadS16(AiPath(path).first_address()) + 1;
    const int16_t wrapped = int16_t(next - (next / 80) * 80);
    AiPath(path).set_first(uint16_t(wrapped));
    left = LeftEntry(path, point, first(), frame - 160);
    *r3 = left;
    if (left == 0) {
      break;
    }
  }
  if (!(LoadS16(AiPath(path).entry_count_address()) > 0)) {
    return true;
  }
  const uint32_t entry = first();
  guest::StoreU16(frame + 80, uint16_t(before - AiPath(path).entry_count()));
  uint32_t ignored = 0;
  AdvanceCorners(path, frame + 80, &ignored);  // can't fail: checked above
  DropPassedSections(path);
  *r3 = path;
  const uint32_t record = AiPathEntry(entry).slice_record().address;
  const double dx =
      Fsubs(Lfs(point + 0), Lfs(RouteSlice(record).center_x_address()));
  const double dz =
      Fsubs(Lfs(point + 8), Lfs(RouteSlice(record).center_z_address()));
  const double offset = Fmadds(dz, Lfs(RouteSlice(record).side_z_address()),
                               Fmuls(dx, Lfs(RouteSlice(record).side_x_address())));
  Stfs(AiPathEntry(entry).offset_address(), offset);
  Stfs(AiPathEntry(entry).x_address(),
       Fmadds(Lfs(RouteSlice(record).side_x_address()), offset,
              Lfs(RouteSlice(record).center_x_address())));
  CopyWord(RouteSlice(record).center_y_address(), AiPathEntry(entry).y_address());
  Stfs(AiPathEntry(entry).z_address(),
       Fmadds(Lfs(RouteSlice(record).side_z_address()), offset,
              Lfs(RouteSlice(record).center_z_address())));
  return true;
}

// --- The path's update ---


uint64_t Extend(uint32_t path, uint32_t point, uint32_t frame,
                const PathCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8213ECF8 AiPath::Extend");
  const uint32_t file = RG::route_file().address;
  const double length = Lfs(TrackRouteFile(file).slice_length_address());
  const double slices = Fdivs(Lfs(G::other_gap_address()), length);
  Stfs(frame + 92, Fmuls(length, Lfs(G::hundredth_address())));
  Stfs(frame + 96, Fmuls(length, Lfs(G::tenth_address())));
  guest::StoreU32(frame + 84, uint32_t(strings_and_font::steps::Fctiwz(slices)));
  int32_t ahead = int32_t(Load(frame + 84));
  if (!(ahead < 80)) {
    ahead = 80;
  }
  const int32_t count = LoadS16(AiPath(path).entry_count_address());
  int32_t limit = ahead;
  if (!(count + 10 > ahead)) {
    limit = count + 10;
  }
  guest::StoreU16(frame + 80, 0);
  guest::StoreU32(frame + 84, 0);
  const uint64_t masks = BranchMasks(path, frame - 96);
  uint64_t r3 = masks;
  const auto ring_at = [&](int32_t at) {
    return path + 16 + (uint32_t(at - (at / 80) * 80) << 6);
  };
  uint32_t previous = 0;
  if (count > 0) {
    previous = ring_at(LoadS16(AiPath(path).first_address()) + (count - 1));
    guest::StoreU16(frame + 84, AiPathEntry(previous).section());
    guest::StoreU16(frame + 86, uint16_t(AiPathEntry(previous).slice() + 1));
    guest::StoreU16(frame + 80, uint16_t(AiPathEntry(previous).lane()));
  } else {
    r3 = FindSideSection(path, frame + 84, point, uint32_t(masks), frame - 208);
    if (int32_t(AiPath(path).mode()) == 1) {
      guest::StoreU16(frame + 80,
                      guest::LoadU16(AiPath(path).driver().address + 8));
    } else {
      guest::StoreU16(frame + 80, 0xFFFF);
    }
  }
  if (LoadS16(AiPath(path).entry_count_address()) < limit) {
    guest::StoreU16(frame + 88, 0xFFFF);
    guest::StoreU16(frame + 90, 0xFFFF);
    const double one = Lfs(kOne);
    const double zero = Lfs(kZero);
    for (;;) {
      const uint32_t entry = ring_at(LoadS16(AiPath(path).first_address()) +
                                     LoadS16(AiPath(path).entry_count_address()));
      const uint32_t shift = entry + 28;
      const uint32_t none = Load(frame + 88);
      Stfs(AiPathEntry(entry).x_address(), zero);
      AiPathEntry(entry).set_lane(0xFFFFFFFFu);
      Stfs(AiPathEntry(entry).w_address(), one);
      AiPathEntry(entry).set_route_position(none);
      Stfs(AiPathEntry(entry).target_offset_address(), zero);
      AiPathEntry(entry).set_slice_record(0);
      Stfs(AiPathEntry(entry).y_address(), zero);
      Stfs(AiPathEntry(entry).z_address(), zero);
      Stfs(AiPathEntry(entry).offset_address(), zero);
      Stfs(shift, zero);
      for (uint32_t at = 32; at <= 44; at += 4) {
        Stfs(entry + at, zero);
      }
      const uint32_t normalizer = AiPath(path).normalizer();
      if (normalizer == kNormalizeByLane) {
        r3 = NormalizeByLane(entry, frame + 84, frame + 80, uint32_t(masks));
      } else if (normalizer == kNormalizeByBranch) {
        r3 = NormalizeByBranch(path, entry, frame + 84, frame + 80, uint32_t(masks));
      } else {
        r3 = calls.normalize(calls.user, normalizer, frame, path, entry, frame + 84, frame + 80,
                             masks);
      }
      if (int32_t(uint32_t(r3)) != 1) {
        break;
      }
      guest::StoreU16(frame + 84, AiPathEntry(entry).section());
      guest::StoreU16(frame + 86, uint16_t(AiPathEntry(entry).slice() + 1));
      const auto add_section = [&](uint16_t section) {
        const uint32_t at =
            uint32_t(LoadS16(AiPath(path).section_count_address()) + 2573) << 1;
        guest::StoreU16(path + at, section);
        AiPath(path).set_section_count(uint16_t(AiPath(path).section_count() + 1));
        r3 = DropOffRouteCorners(path);
      };
      if (previous != 0) {
        uint32_t record = AiPathEntry(entry).slice_record().address;
        if (AiPathEntry(entry).section() == AiPathEntry(previous).section()) {
          Stfs(shift, zero);
        } else {
          const uint32_t before = AiPathEntry(previous).slice_record().address;
          const double dx =
              Fsubs(Lfs(RouteSlice(before).center_x_address()), Lfs(record + 0));
          const double dz =
              Fsubs(Lfs(RouteSlice(before).center_z_address()), Lfs(record + 8));
          Stfs(shift, Fmadds(dz, Lfs(record + 40), Fmuls(dx, Lfs(record + 36))));
          add_section(AiPathEntry(entry).section());
          record = AiPathEntry(entry).slice_record().address;
        }
        CopyWord(record + 24, AiPathEntry(entry).range_low_address());
        CopyWord(record + 28, AiPathEntry(entry).range_high_address());
        CopyWord(record + 28, AiPathEntry(entry).room_high_address());
        CopyWord(AiPathEntry(entry).range_low_address(),
                 AiPathEntry(entry).room_low_address());
        NarrowToOther(entry, previous, frame + 92, shift);
        r3 = NarrowBehind(path, entry, previous, frame + 92, frame + 96, frame - 128);
      } else if (AiPath(path).entry_count() == 0) {
        const uint32_t record = AiPathEntry(entry).slice_record().address;
        const uint16_t section = AiPathEntry(entry).section();
        CopyWord(record + 24, AiPathEntry(entry).range_low_address());
        CopyWord(record + 28, AiPathEntry(entry).range_high_address());
        CopyWord(AiPathEntry(entry).range_low_address(),
                 AiPathEntry(entry).room_low_address());
        CopyWord(AiPathEntry(entry).range_high_address(),
                 AiPathEntry(entry).room_high_address());
        const double dx = Fsubs(Lfs(point + 0), Lfs(record + 0));
        const double dz = Fsubs(Lfs(point + 8), Lfs(record + 8));
        const double offset = Fmadds(dz, Lfs(record + 40), Fmuls(dx, Lfs(record + 36)));
        Stfs(AiPathEntry(entry).offset_address(), offset);
        Stfs(AiPathEntry(entry).x_address(),
             Fmadds(Lfs(record + 36), offset, Lfs(record + 0)));
        CopyWord(record + 4, AiPathEntry(entry).y_address());
        Stfs(AiPathEntry(entry).z_address(),
             Fmadds(Lfs(record + 40), offset, Lfs(record + 8)));
        add_section(section);
      }
      uint32_t corners_r3 = 0;
      AddCornersAt(path, entry, frame - 144, &corners_r3);
      r3 = corners_r3;
      previous = entry;
      const int16_t added = int16_t(AiPath(path).entry_count() + 1);
      AiPath(path).set_entry_count(uint16_t(added));
      if (!(added < limit)) {
        break;
      }
    }
  }
  if (int32_t(AiPath(path).mode()) != 1) {
    ApplyCornerLines(path, frame - 112);
    r3 = path;
  }
  const uint32_t driver = AiPath(path).driver().address;
  if (driver == 0) {
    return r3;
  }
  uint32_t inside = 0;
  if (LoadS16(AiPath(path).corner_count_address()) > 0) {
    uint32_t record = path + 5164;
    int32_t i = 0;
    do {
      const uint32_t data = Load(record + 0);
      if (data != 0) {
        const int32_t offset = LoadS16(record + 10);
        const int32_t look = LoadS16(AiPath(path).look_ahead_address());
        if (!(look < LoadS16(RouteCorner(data).start_address()) + offset) &&
            !(look > LoadS16(RouteCorner(data).exit_address()) + offset)) {
          inside = 1;
          break;
        }
      }
      i++;
      record += 44;
    } while (i < LoadS16(AiPath(path).corner_count_address()));
  }
  AiDriver(driver).set_in_corner(inside);
  return r3;
}

bool UpdatePath(uint32_t path, uint32_t frame, const PathCalls& calls,
                uint64_t* r3) {
  if (!CornersMovable(path)) {  // DropPassed's condition, checked first
    return false;
  }
  ITC_NATIVE_RUNNING("sub_8213F530 AiPath::Update");
  *r3 = path;
  guest::StoreU32(frame + 80, 0);
  const uint32_t file = RG::route_file().address;
  const uint32_t driver = AiPath(path).driver().address;
  const uint32_t point = driver + 80;
  if (!(AiDriver(driver).lane() < 32768)) {
    return true;
  }
  FollowLeaderBranch(path);
  uint64_t dropped = 0;
  DropPassed(path, point, frame - 160, &dropped);  // can't fail: checked above
  *r3 = Extend(path, point, frame - 208, calls);
  const int16_t entries = int16_t(AiPath(path).entry_count());
  const int32_t count = entries;
  const int32_t reach = strings_and_font::steps::Fctiwz(
      Fdivs(Lfs(G::look_ahead_address()),
            Lfs(TrackRouteFile(file).slice_length_address())));
  const int16_t look = int16_t(AiPath(path).look_ahead() - uint16_t(reach) + 1);
  AiPath(path).set_plan_count(uint16_t(look));
  if (look > count) {
    AiPath(path).set_plan_count(uint16_t(entries));
  }
  if (!(count > 1)) {
    return true;
  }
  const uint32_t own = AiPath(path).driver().address;
  if ((Load(own + 16) & 0x1) != 0) {
    return true;
  }
  const double speed = Fadds(Lfs(own + 100), Lfs(G::twenty_address()));
  const double per = Fdivs(Lfs(RG::route_file().slice_length_address()), speed);
  int32_t span = int32_t(uint32_t(LoadS16(AiPath(path).plan_count_address())) << 2);
  guest::StoreU32(frame + 80, uint32_t(span));
  Stfs(AiPath(path).largest_step_address(), Fmuls(per, Lfs(G::twelve_address())));
  if (span > count) {
    guest::StoreU32(frame + 80, uint32_t(count));
  }
  SetRanges(path, frame + 80, frame - 128);
  ReactToOthers(AiPath(path).driver().address, frame + 80, frame - 240);
  const uint32_t mine = AiPath(path).driver().address;
  const int32_t first = int32_t(Load(mine + 452));
  int32_t ring = -1;
  if (!(first < 0)) {
    ring = LoadS16(uint32_t(first) * 40 + mine + 198);
  }
  if (!(ring < 0)) {
    AvoidOthers(path, frame - 272);
  }
  if (int32_t(AiPath(path).mode()) == 1) {
    FollowTargets(path, frame - 128);
  } else {
    PlanLine(path, frame - 96);
  }
  RefreshPoints(path);
  *r3 = path;
  return true;
}

// --- The drivers' updates ---

bool UpdateDriver(uint32_t driver, uint32_t input, uint32_t flag, uint32_t frame,
                  const PathCalls& calls, uint64_t* r3, double* f1) {
  if (!CornersMovable(driver + 512)) {  // AiPath::Update's condition, checked first
    return false;
  }
  ITC_NATIVE_RUNNING("sub_8213B310 AiDriver::Update");
  const uint32_t flags = AiDriver(driver).flags();
  if ((flags & 0x2) == 0 || int32_t(Load(flag)) == int32_t((flags >> 2) & 1)) {
    if (Load(flag) != 0) {
      ResetControl(driver, frame - 112);
    } else {
      StartRace(driver, frame - 112);
    }
  }
  uint64_t path_r3 = 0;
  UpdatePath(driver + 512, frame - 144, calls, &path_r3);  // can't fail: checked above
  *r3 = Drive(driver, input, frame - 160, f1);
  return true;
}

bool DriveCar(uint32_t input, uint32_t index, uint32_t flag, uint32_t frame,
              const PathCalls& calls, uint64_t* r3, double* f1) {
  const uint32_t table = Load(G::drivers_address());
  if (table != 0 && !CornersMovable(uint32_t(uint64_t(index) * AiDriver::kSize + table) + 512)) {
    return false;  // AiPath::Update's condition, checked first
  }
  ITC_NATIVE_RUNNING("sub_8215F678 AiDriver::DriveCar");
  const uint32_t vehicle = vehicle_accessors::VehicleGet(index);
  const uint32_t identity = vehicle_accessors::VehicleIdentity(vehicle);
  track_route::ClampPosition(frame + 88, TrackPosition(identity).route_position());
  Vehicle_GetSpeedKmh(vehicle);  // not read (its f1 Drive's later)
  if (Vehicle(vehicle).automatic() == 0) {
    Vehicle(vehicle).set_automatic(1);
  }
  if ((Vehicle(vehicle).car_flags() & 0x81) != 0 &&
      Vehicle(vehicle).nitrous_on() != 0) {
    Vehicle(vehicle).set_nitrous_on(0);
  }
  ClearCarInput(input);
  guest::StoreU32(frame + 84, flag);
  guest::StoreU32(frame + 80, index);
  CarInput(input).set_kind(2);
  const uint32_t driver = uint32_t(Of(frame + 80));
  uint64_t ignored = 0;
  UpdateDriver(driver, input, frame + 84, frame - 112, calls, &ignored, f1);
  *r3 = input;
  return true;
}

uint64_t UpdateIdle(uint32_t driver, uint32_t frame, const PathCalls& calls,
                    bool* done) {
  *done = CornersMovable(driver + 512);  // AiPath::Update's condition, checked first
  if (!*done) {
    return 0;
  }
  ITC_NATIVE_RUNNING("sub_821399A8 AiDriver::UpdateIdle");
  if ((AiDriver(driver).flags() & 0x2) != 0) {
    ResetToMode0(driver, frame - 128);
  }
  uint64_t r3 = 0;
  UpdatePath(driver + 512, frame - 144, calls, &r3);
  const double zero = Lfs(kZero);
  Stfs(AiDriver(driver).steering_address(), zero);
  AiDriver(driver).set_handbrake(0);
  Stfs(AiDriver(driver).throttle_address(), zero);
  Stfs(AiDriver(driver).brake_address(), zero);
  return r3;
}

}  // namespace ai_drivers
