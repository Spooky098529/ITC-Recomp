// The vehicle code above the car physics (docs/functions/vehicle.md): the
// leaves, the route position's step, the track positions, the vehicles'
// lifetime. See vehicle.h.

#include "vehicle/vehicle.h"

#include "car_physics/car_physics.h"
#include "car_physics/float_ops.h"

#include "ai_drivers/ai_drivers.h"
#include "base/log.h"
#include "c_runtime/arrays.h"
#include "c_runtime/math.h"
#include "c_runtime/strings.h"
#include "collision/collision.h"
#include "matrix_math/matrix_math.h"
#include "minimap/minimap.h"
#include "strings_and_font/steps.h"
#include "memory/c_runtime_heap.h"
#include "track_collision/track_collision.h"
#include "schema/vehicle.h"
#include "schema/car_physics.h"
#include "schema/collision_world.h"
#include "schema/race_event.h"
#include "schema/race_setup.h"
#include "schema/track_route.h"
#include "schema/race_states.h"
#include "schema/ai_drivers.h"
#include "track_route/track_route.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include "base/float_mode.h"
#include <simde/x86/sse4.1.h>

#include <cstring>

namespace vehicle {

using namespace car_physics::ops;
namespace cp = car_physics;
namespace co = collision;
namespace tc = track_collision;
namespace va = vehicle_accessors;
namespace vl = vector_library;

namespace {

using G = schema::VehicleGlobals;

constexpr uint32_t kEmptyDestructor = 0x82106540;  // sub_82106540

// Whether Vehicle::DestroyPhysics would run natively for the vehicle (its
// own checks: the pairs' removal, the shape a box the layer gave, the body
// a block the layer gave).
bool DestroyPhysicsFits(uint32_t vehicle) {
  const schema::Vehicle veh(vehicle);
  const uint32_t object = veh.collision_object();
  const uint32_t shape = veh.box();
  const uint32_t body = veh.body().address;
  return !((object != 0 && !cp::CollisionWorldPairsMovable()) ||
           (shape != 0 && (!co::IsBox(shape) || !co::Deletable(shape))) ||
           !co::Deletable(body));
}

}  // namespace

// --- Leaves ---

void MatrixFromRows(uint32_t out, uint32_t a, uint32_t b, uint32_t c,
                    uint32_t d) {
  ITC_NATIVE_RUNNING("sub_820BFBB8 Matrix::FromRows");
  const uint32_t rows[4] = {a, b, c, d};
  for (uint32_t row = 0; row < 4; row++) {
    const schema::VehicleMatrixRow from(rows[row]);
    const uint32_t to = out + 16 * row;
    guest::StoreU32(to + 0, from.x_bits());
    guest::StoreU32(to + 4, from.y_bits());
    guest::StoreU32(to + 8, from.z_bits());
    guest::StoreU32(to + 12, from.w_bits());
  }
}

void TrackPositionCopy(uint32_t out, uint32_t from) {
  ITC_NATIVE_RUNNING("sub_820C2150 TrackPosition::Copy");
  const schema::VehicleTrackPositionSource src(from);
  const schema::TrackPosition to(out);
  to.set_section(src.section());
  to.set_slice(src.slice());
  to.set_route_section(src.route_section());
  to.set_route_slice(src.route_slice());
  // ld and std, as the original: each doubleword's bits.
  to.set_point_xy(src.point_xy());
  to.set_point_zw(src.point_zw());
  to.set_color_xy(src.vector2_xy());
  to.set_color_zw(src.vector2_zw());
  to.set_normal_xy(src.vector3_xy());
  to.set_normal_zw(src.vector3_zw());
  to.set_unknown_64(src.vector4_xy());
  to.set_unknown_72(src.vector4_zw());
  to.set_surface_kind(src.half_80());
  to.set_flags(src.half_82());
}

void CameraCurveLoad(uint32_t object, uint32_t index, uint32_t word) {
  ITC_NATIVE_RUNNING("sub_820BFCE8 CameraCurve::Load");
  const schema::VehicleCameraCurve curve(object);
  const uint32_t table = G::camera_curves_address() + uint32_t(uint64_t(index) * 360);
  const double zero = Lfs(kZero);
  for (uint32_t i = 0; i < 90; i++) {
    Stfs(curve.cleared_address() + 4 * i, zero);
    // lfs then stfs with only an addi between: the word's bits.
    CopyWord(table + 4 * i, curve.values_address() + 4 * i);
  }
  curve.set_unknown_720(word);
}

// --- The route position ---

uint32_t RoutePositionStep(uint32_t position, uint32_t step, uint32_t kind,
                           uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82170F90 RoutePosition::Step");
  guest::StoreU32(frame + 80, uint32_t(int32_t(int16_t(kind))));
  const uint32_t masks = ai_drivers::BranchMasksFor(frame + 80);
  const schema::RoutePosition p(position);
  p.set_slice(uint16_t(p.slice() + step));
  return track_route::Normalize(G::route_address(), position, masks);
}

// --- The vehicle's track positions and world ---

void VehicleUpdateTrackPositions(uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820C1D78 Vehicle::UpdateTrackPositions");
  const schema::Vehicle veh(vehicle);
  const schema::TrackPosition car(veh.own_track_position_address());
  const schema::VehicleBodyPosition position(va::RigidBodyPosition(veh.body().address));
  CopyWord(position.x_address(), car.point_x_address());
  CopyWord(position.y_address(), car.point_y_address());
  CopyWord(position.z_address(), car.point_z_address());
  tc::TrackPositionUpdate(car.address, frame - 144);
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t mount = schema::VehicleWheel(veh.wheels_address() + 368 * i).mount_point_address();
    const schema::TrackPosition wheel(veh.wheel_track_positions_address() + 96 * i);
    CopyWord(mount + 0, wheel.point_x_address());
    CopyWord(mount + 4, wheel.point_y_address());
    CopyWord(mount + 8, wheel.point_z_address());
    tc::TrackPositionUpdate(wheel.address, frame - 144);
  }
}

bool VehicleLinkTrackPositions(uint32_t vehicle, uint32_t* r3) {
  uint32_t shape = 0;
  // Current asks the template's type (nothing written): first, then the
  // copy's slot 20 must be the box's.
  const schema::Vehicle veh(vehicle);
  if (!co::CollisionShapeCurrent(veh.collision_object() + 32, &shape) ||
      !co::IsBox(shape)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820C1D08 Vehicle::LinkTrackPositions");
  veh.set_track_position(veh.own_track_position_address());
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t position = veh.wheel_track_positions_address() + 96 * i;
    schema::VehicleWheel(veh.wheels_address() + 368 * i).set_track_position(position);
    co::BoxSetCacheWord(shape, i, position);
  }
  *r3 = shape;
  return true;
}

bool VehicleRefresh(uint32_t vehicle, uint32_t frame, double* f1, uint32_t* r3) {
  if (!cp::CollisionWorldCallbackKnown()) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820C0118 Vehicle::Refresh");
  *f1 = double(G::step_seconds());
  *r3 = cp::VehicleRefreshFromBody(vehicle, frame);
  return true;
}

bool VehicleRemoveFromWorld(uint32_t vehicle, uint32_t* r3) {
  const schema::Vehicle veh(vehicle);
  const uint32_t object = veh.collision_object();
  const uint32_t shape = veh.box();
  if ((object != 0 && !cp::CollisionWorldPairsMovable()) ||
      (shape != 0 && (!co::IsBox(shape) || !co::Deletable(shape)))) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820C20E0 Vehicle::RemoveFromWorld");
  veh.set_car_flags(0);
  if (object != 0) {
    uint32_t ignored = 0;
    cp::CollisionWorldRemoveObject(object, &ignored);
    veh.set_collision_object(0);
  }
  // +344 reloaded after the removal, as the original.
  const uint32_t current = veh.box();
  if (current != 0) {
    co::ShapeDeletingDestructor(current, 1);  // slot 0 (a box's)
  }
  veh.set_box(0);
  *r3 = current;
  return true;
}

// --- The seven vehicles ---

void VehicleConstructAll(uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820C30D8 Vehicle::Construct");
  double ignored = 0.0;
  cp::VehicleConstruct(vehicle, frame - 112, &ignored);
  const schema::Vehicle veh(vehicle);
  cp::CarDataConstruct(veh.car_data_block_address(), frame - 96);
  const double one = Lfs(kOne);
  // The track positions' w words (+28, +44, +60, +76: no f32 fields there).
  const uint32_t own = veh.own_track_position_address();
  Stfs(own + 28, one);
  Stfs(own + 44, one);
  Stfs(own + 60, one);
  Stfs(own + 76, one);
  for (uint32_t k = 0; k < 4; k++) {
    const uint32_t at = veh.wheel_track_positions_address() + 44 + 96 * k;
    Stfs(at - 16, one);
    Stfs(at + 0, one);
    Stfs(at + 16, one);
    Stfs(at + 32, one);
  }
}

std::optional<uint32_t> VehiclesCreate(uint32_t frame) {
  const auto block = memory::crt::OperatorNew(kVehiclesBytes);
  if (!block) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820C3BD0 Vehicles::Create");
  if (*block != 0) {
    for (uint32_t i = 0; i < 7; i++) {
      VehicleConstructAll(*block + kVehicleBytes * i, frame - 96);
    }
  }
  G::set_vehicles(*block);
  guest::ForEachHostPart(*block, kVehiclesBytes,
                         [](uint8_t* host, uint32_t, uint32_t bytes) {
                           std::memset(host, 0, bytes);
                         });
  return *block;
}

bool VehiclesDestroy() {
  const uint32_t block = G::vehicles();
  if (block != 0) {
    if (!co::Deletable(block)) {
      return false;
    }
    for (uint32_t i = 0; i < 7; i++) {
      if (!DestroyPhysicsFits(block + kVehicleBytes * i)) {
        return false;
      }
    }
  }
  ITC_NATIVE_RUNNING("sub_820C3B78 Vehicles::Destroy");
  if (block != 0) {
    ITC_NATIVE_RUNNING("sub_820C21E0 Vehicles::DestroyAll");
    for (int32_t i = 6; i >= 0; i--) {
      const uint32_t vehicle = block + kVehicleBytes * uint32_t(i);
      // The empty destructor on each: the iterator calls nothing.
      c_runtime::VectorDestruct(schema::Vehicle(vehicle).wheel_track_positions_address(), 96, 4,
          kEmptyDestructor,
          [](void*, uint64_t, uint64_t) {
            ITC_LOG_ERROR("vehicle: the track positions' destructor isn't the empty one");
          },
          nullptr);
      uint32_t ignored = 0;
      cp::VehicleDestroyPhysics(vehicle, &ignored);
    }
    memory::crt::OperatorDelete(block);
  }
  G::set_vehicles(0);
  return true;
}

// --- The vehicle's input and queries ---

namespace {

constexpr uint32_t kTwentieth = 0x82002204;    // 0.05
constexpr uint32_t kInputScale = 0x82002BD4;   // 1/32,767
constexpr uint32_t kOneAndHalf = 0x820027A8;   // 1.5
constexpr uint32_t kHalf = 0x82002BC0;         // 0.5
constexpr uint32_t kFour = 0x820021F8;         // the handbrake ramp's rate
constexpr uint32_t kPerKmh = 0x82002BD8;       // 1/3.6

// std then lfd then fcfid then frsp: a 64-bit integer through the stack
// (its doubleword written) to a single.
double IntThroughStack(uint32_t at, uint64_t value) {
  guest::StoreU32(at, uint32_t(value >> 32));
  guest::StoreU32(at + 4, uint32_t(value));
  return double(float(double(int64_t(value))));
}
// extsb, addi 10, clrldi 32: a rate byte's word.
uint64_t RateOf(uint8_t byte) {
  return uint32_t(int32_t(int8_t(byte)) + 10);
}
// The brake's and throttle's curve: x = the input / 32,767, x^2 + (x - x^2
// + x - x^2) x (rate x 0.05) (as fmadds).
using InputHalf = uint16_t (schema::CarInput::*)() const;
using InputByte = uint8_t (schema::CarInput::*)() const;
double PedalCurve(const schema::CarInput& in, InputHalf pedal, InputByte rate_of,
                  uint32_t frame) {
  const double x0 = IntThroughStack(frame + 88, uint64_t(int64_t(int16_t((in.*pedal)()))));
  const double rate = IntThroughStack(frame + 80, RateOf((in.*rate_of)()));
  const double x = Fmuls(x0, Lfs(kInputScale));
  const double k = Fmuls(rate, Lfs(kTwentieth));
  const double x2 = Fmuls(x, x);
  const double a = Fsubs(x, x2);
  const double b = Fsubs(Fadds(a, x), x2);
  return Fmadds(b, k, x2);
}

}  // namespace

bool VehicleApplyInput(uint32_t vehicle, uint32_t input, uint32_t frame,
                       const SteerOriginal& steer_original) {
  const uint32_t v = vehicle;
  const schema::Vehicle veh(v);
  const schema::CarInput in(input);
  const uint32_t kind = uint32_t(int32_t(int8_t(in.kind())));
  const uint32_t flags = in.flags();
  // The callees never seen in play: decided first (the setters before them
  // don't write the flags or +2249).
  const bool shifts = veh.automatic() != 0 ? (flags & 0x2000) != 0
                                                          : (flags & 0x3) != 0;
  if (kind == 1 || (flags & 0x20000000) != 0 || (flags & 0x08000000) != 0 || shifts) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820BFD30 Vehicle::ApplyInput");
  const double dt = double(G::step_seconds());
  const double zero = Lfs(kZero);
  const double one = Lfs(kOne);
  const double twentieth = Lfs(kTwentieth);
  const double scale = Lfs(kInputScale);
  if (kind == 0) {
    // The pad: SteerTowards with a rate from byte +16.
    const double rate = IntThroughStack(frame + 88, RateOf(in.steering_rate()));
    const double f3 = Fmadds(Fnmsubs(rate, twentieth, one), Lfs(kOneAndHalf), Lfs(kHalf));
    const int32_t steer = int16_t(in.steering());
    double target = zero;
    uint32_t left = 0, right = 0;
    if ((flags & 0x02000000) != 0) {
      if (steer == -32767) {
        left = 1;
      } else if (steer == 32767) {
        right = 1;
      }
    } else {
      target = Fmuls(IntThroughStack(frame + 88, uint64_t(int64_t(steer))), scale);
    }
    double f1 = 0.0;
    if (!cp::VehicleSteerTowards(v, target, dt, f3, left, right, &f1)) {
      steer_original(target, dt, f3, left, right);
    }
  } else if (kind == 2) {
    // An AI driver's.
    const int64_t steer = int16_t(in.steering());
    cp::VehicleSetSteering(v, Fmuls(IntThroughStack(frame + 80, uint64_t(steer)), scale));
  }
  cp::VehicleSetBrake(v,
                      PedalCurve(in, &schema::CarInput::brake,
                                 &schema::CarInput::brake_rate, frame));
  cp::VehicleSetThrottle(v,
                         PedalCurve(in, &schema::CarInput::throttle,
                                    &schema::CarInput::throttle_rate, frame));
  if ((flags & 0x80000000) != 0) {
    const uint32_t on = in.handbrake() == 32767 ? 1 : 0;
    cp::VehicleRampHandbrake(v, on, Fmuls(dt, Lfs(kFour)));
  } else {
    const int64_t value = int16_t(in.handbrake());
    cp::VehicleSetHandbrake(v, IntThroughStack(frame + 88, uint64_t(value)));
  }
  cp::VehicleRampReverseThrottle(v, (flags & 0x200) != 0 ? 1 : 0, dt);
  if ((flags & 0x10000) != 0) {
    uint8_t on = 0;
    if (veh.nitrous_on() == 0 && double(veh.nitrous_value()) > zero) {
      on = 1;
    }
    veh.set_nitrous_on(on);
  }
  return true;
}

double VehicleSlipWheelValue(uint32_t vehicle, double speed, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820C0310 Vehicle::SlipWheelValue");
  const uint32_t v = vehicle, f = frame;
  const uint32_t once = G::slip_flag();
  if ((once & 1) == 0) {
    G::set_slip_flag(once | 1);
    G::set_slip_threshold(float(Fmuls(speed, Lfs(kPerKmh))));
  }
  const schema::Vehicle veh(v);
  const double zero = Lfs(kZero);
  double best = zero, most = zero;
  for (uint32_t i = 0; i < 4; i++) {
    const schema::VehicleWheel wheel(veh.wheels_address() + 368 * i);
    if (wheel.on_ground() == 0) {
      continue;
    }
    // Each wheel's axle record in the car data (+2160 + 4 i).
    const schema::CarDataAxle data(guest::LoadU32(veh.wheel_axle_data_address() + 4 * i));
    Stfs(f + 124, zero);
    Stfs(f + 92, zero);
    Stfs(f + 108, zero);
    const double spin = double(wheel.spin());
    const uint32_t body = veh.body().address;
    const double rolled = Fmuls(double(data.radius()), spin);
    Stfs(f + 84, zero);
    Stfs(f + 80, rolled);
    Stfs(f + 88, zero);
    va::RigidBodyPointVelocity(f + 144, body, wheel.mount_point_address());
    vl::Vec4Copy(f + 112, f + 144);
    Stfs(f + 96, double(vl::Vec3DotOf(f + 112, wheel.forward_axis_address())));
    Stfs(f + 100, zero);
    Stfs(f + 104, double(vl::Vec3DotOf(f + 112, wheel.side_axis_address())));
    vl::Vec3Difference(f + 128, f + 96, f + 80);
    const double slip = double(vl::Sqrtf(double(vl::Vec3DotOf(f + 128, f + 128))));
    // bge on both: on only when the threshold and the most so far are below
    // it (a NaN keeps the wheel so far).
    if (double(G::slip_threshold()) < slip && most < slip) {
      best = double(wheel.slip_long());
      most = slip;
    }
  }
  return best;
}

uint32_t RacersNearestHumanVehicle(uint32_t point, uint32_t n, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820C2248 Racers::NearestHumanVehicle");
  const uint32_t f = frame;
  const int32_t count = int32_t(tc::RaceRacerCount());
  const double zero = Lfs(kZero);
  for (uint32_t k = 0; k < 7; k++) {
    Stfs(f + 96 + 8 * k, zero);
    guest::StoreU32(f + 100 + 8 * k, 0);
  }
  Stfs(f + 92, zero);
  CopyWord(point + 8, f + 88);
  CopyWord(point + 0, f + 80);
  CopyWord(point + 4, f + 84);
  uint32_t found = 0;
  for (int32_t racer = 0; racer < count; racer++) {
    const uint32_t flags = schema::Vehicle(minimap::RaceRacerVehicle(racer)).car_flags();
    if ((flags & 0x1) != 0 || (flags & 0x80) != 0) {
      guest::StoreU32(f + 160 + 4 * found, uint32_t(racer));
      found++;
    }
  }
  for (uint32_t j = 0; j < found; j++) {
    const uint32_t racer = guest::LoadU32(f + 160 + 4 * j);
    const schema::Vehicle vehicle(minimap::RaceRacerVehicle(racer));
    const uint32_t body = vehicle.body().address;
    vl::Vec3Difference(f + 208, f + 80, va::RigidBodyPosition(body));
    vl::Vec3Difference(f + 192, f + 80, va::RigidBodyPosition(vehicle.body().address));
    Stfs(f + 96 + 8 * j, double(vl::Vec3DotOf(f + 192, f + 208)));
    guest::StoreU32(f + 100 + 8 * j, racer);
  }
  // The selection sort: a pass per racer found, each over the racer count.
  int32_t next = 1;
  for (uint32_t pass = 0, at = f + 96; pass < found; pass++, at += 8, next++) {
    uint32_t least = at;
    if (next < count) {
      uint32_t other = at + 8;
      for (int32_t k = count - next; k > 0; k--, other += 8) {
        if (Lfs(least) > Lfs(other)) {
          least = other;
        }
      }
    }
    const uint32_t a0 = guest::LoadU32(at);
    const uint32_t m0 = guest::LoadU32(least), m1 = guest::LoadU32(least + 4);
    guest::StoreU32(least, a0);
    guest::StoreU32(least + 4, guest::LoadU32(at + 4));
    guest::StoreU32(at, m0);
    guest::StoreU32(at + 4, m1);
  }
  const uint32_t racer = guest::LoadU32(f + 100 + ((n << 3) & 0xFFFFFFF8u));
  return minimap::RaceRacerVehicle(racer);
}

// --- The engine's power curve ---

namespace {

constexpr uint32_t kRpmStep = 0x82003314;     // 400.0
constexpr uint32_t kHundred = 0x82002A0C;     // 100.0
constexpr uint32_t kPowerScale = 0x82003380;  // torque x rpm to power
constexpr uint32_t kTopRpm = 0x82003384;      // 800.0 (the range's start)
constexpr uint32_t kTwo = 0x820021E8;         // 2.0
constexpr uint32_t kHundredth = 0x8200222C;   // 0.01

}  // namespace

void PowerCurveShiftPeak(uint32_t curve, uint32_t shift, uint32_t r1) {
  ITC_NATIVE_RUNNING("sub_820CE1D8 PowerCurve::ShiftPeak");
  const schema::PowerCurve pc(curve);
  const uint32_t peak = pc.peak_index();
  double low = double(pc.low_rpm());
  const double step = Lfs(kRpmStep);
  const double shifted = IntThroughStack(r1 - 576, uint64_t(int64_t(int32_t(shift))));
  const double by = Fmuls(shifted, Lfs(kHundred));
  const double index =
      IntThroughStack(r1 - 576, uint64_t(int64_t(int32_t(peak + 1))));
  double rpm = Fmadds(index, step, by);
  pc.set_peak_rpm(float(rpm));
  if (rpm < low) {
    rpm = low;
  } else {
    const double high = double(pc.high_rpm());
    if (rpm > high) {
      rpm = high;
    }
  }
  const uint32_t count = pc.count();
  pc.set_peak_rpm(float(rpm));
  const double top = Fsubs(rpm, step);  // the new peak's entry
  const double zero = Lfs(kZero);
  pc.set_most_power(float(zero));
  pc.set_most_power_rpm(float(zero));
  // The rpm axis remapped below r1: the points below the peak scaled to end
  // at it, those after stretched from it to the end.
  const uint32_t table = r1 - 528;
  if (int32_t(count) > 0) {
    for (int32_t i = 0; i < int32_t(count); i++) {
      const uint32_t at = table + 16 * uint32_t(i);
      if (i < int32_t(peak)) {
        const double a = IntThroughStack(r1 - 576, uint64_t(int64_t(i)));
        const double b = IntThroughStack(r1 - 544, uint64_t(int64_t(int32_t(peak))));
        Stfs(at, Fdivs(Fmuls(a, top), b));
      } else if (i > int32_t(peak)) {
        const double n = IntThroughStack(r1 - 568, uint64_t(int64_t(int32_t(count))));
        const double k =
            IntThroughStack(r1 - 560, uint64_t(int64_t(int32_t(uint32_t(i) - peak))));
        const double rest =
            IntThroughStack(r1 - 552, uint64_t(int64_t(int32_t(count - peak))));
        Stfs(at, Fadds(Fdivs(Fmuls(Fmsubs(n, step, top), k), rest), top));
      } else {
        Stfs(at, top);
      }
      const double remapped = Lfs(at);
      const uint32_t torque = guest::LoadU32(pc.entries_address() + 4 + 16 * uint32_t(i));
      Stfs(at, Fadds(remapped, step));
      guest::StoreU32(at + 4, torque);  // lfs then stfs: its bits
    }
  }
  // Each point's torque looked up at its rpm on the remapped axis.
  if (int32_t(count) > 0) {
    const double scale = Lfs(kPowerScale);
    for (uint32_t j = 0; int32_t(j) < int32_t(pc.count()); j++) {
      const uint32_t out = pc.shifted_address() + 4 + 16 * j;
      const int32_t n = int32_t(pc.count());
      const double x = Lfs(pc.entries_address() + 16 * j);
      double torque = Lfs(pc.entries_address() + 4 + 16 * j);
      if (n > 1) {
        for (int32_t k = 1; k < int32_t(pc.count()); k++) {
          const uint32_t e = table + 16 * uint32_t(k);
          if (!(Lfs(e - 16) > x) && x < Lfs(e)) {
            const double x0 = Lfs(e - 16);
            const double x1 = Lfs(e);
            const double dx = Fsubs(x, x0);
            const double span = Fsubs(x1, x0);
            const double t0 = Lfs(e - 12);
            const double t1 = Lfs(e + 4);
            const double dt = Fsubs(t1, t0);
            torque = Fmadds(Fdivs(dx, span), dt, t0);
            break;
          }
        }
      }
      const double product = Fmuls(torque, x);
      Stfs(out, torque);
      Stfs(out - 4, x);
      const double power = Fmuls(product, scale);
      Stfs(out + 4, power);
      if (double(pc.most_power()) < power) {
        pc.set_most_power(float(power));
        CopyWord(out - 4, pc.most_power_rpm_address());
      }
    }
  }
}

void PowerCurveBuild(uint32_t curve, uint32_t points, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820CE430 PowerCurve::Build");
  const schema::PowerCurve pc(curve);
  const schema::PowerCurvePoints pts(points);
  const double zero = Lfs(kZero);
  for (uint32_t i = 0; i < 32; i++) {
    const uint32_t at = pc.entries_address() + 12 + 16 * i;
    for (uint32_t k = 0; k < 4; k++) {
      Stfs(at + 512 - 4 * k, zero);
      Stfs(at - 4 * k, zero);
    }
  }
  CopyWord(pts.torque_scale_address(), pc.torque_scale_address());
  pc.set_low_rpm(float(Lfs(kTopRpm)));
  const double step = Lfs(kRpmStep);
  pc.set_count(0);
  const double scale = Lfs(kPowerScale);
  const double two = Lfs(kTwo);
  const double least = Lfs(kHundredth);
  double most = zero;
  for (uint32_t i = 0; i < 32; i++) {
    const uint32_t e = pc.entries_address() + 16 * i;  // i = 0: e - 28, e - 12 back_2_torque, back_1_torque
    const double point = Lfs(pts.points_address() + 4 * i);
    if (point < least) {
      // From the two entries before it (before the curve for the first).
      const double t2 = Lfs(e - 28);
      double torque = Fmsubs(Lfs(e - 12), two, t2);
      const double rpm =
          Fmuls(IntThroughStack(frame + 80, uint64_t(int64_t(int32_t(i + 1)))), step);
      Stfs(e + 0, rpm);
      if (!(torque > zero)) {
        torque = zero;
      }
      const double product = Fmuls(rpm, torque);
      Stfs(e + 4, torque);
      Stfs(e + 8, Fmuls(product, scale));
    } else {
      pc.set_count(pc.count() + 1);
      const double rpm =
          Fmuls(IntThroughStack(frame + 88, uint64_t(int64_t(int32_t(i + 1)))), step);
      Stfs(e + 0, rpm);
      const double torque = Fmuls(double(pc.torque_scale()), Lfs(pts.points_address() + 4 * i));
      Stfs(e + 4, torque);
      Stfs(e + 8, Fmuls(Fmuls(torque, rpm), scale));
      if (most < torque) {
        most = torque;
        pc.set_peak_index(i);
      }
    }
  }
  const uint32_t peak = (pc.peak_index() << 4) & 0xFFFFFFF0u;
  const double start = double(pc.low_rpm());
  const double end = double(pc.high_rpm());
  pc.set_peak_below(int32_t(strings_and_font::steps::Fctiwz(
      Fmuls(Fsubs(start, Lfs(pc.entries_address() + peak)), least))));
  int32_t below = pc.peak_below();
  pc.set_peak_above(int32_t(strings_and_font::steps::Fctiwz(
      Fmuls(Fsubs(end, Lfs(pc.entries_address() + peak)), least))));
  if (below < -20) {
    below = -20;
  }
  int32_t above = pc.peak_above();
  pc.set_peak_below(below);
  if (above > 20) {
    above = 20;
  }
  pc.set_peak_above(above);
  PowerCurveShiftPeak(curve, 0, frame);
}

void PowerCurveFromEngine(uint32_t curve, uint32_t engine, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820CE7D8 PowerCurve::FromEngine");
  const schema::PowerCurveEngine from(engine);
  CopyWord(from.high_rpm_address(), schema::PowerCurve(curve).high_rpm_address());
  PowerCurveBuild(curve, from.torque_points_address(), frame);
}

uint32_t VehicleStatusReset(uint32_t status, uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820BFC48 VehicleStatus::Reset");
  const schema::VehicleStatus st(status);
  const float zero = guest::LoadFloat(kZero);
  st.set_unknown_76(zero);
  st.set_speed_kmh(zero);
  st.set_landing_ticks(0);
  st.set_trip_km(zero);
  st.set_unknown_80(0);
  st.set_total_km(zero);
  st.set_wrong_way(0);
  const float eighty = guest::LoadFloat(0x82001E8C);
  st.set_water_temperature(eighty);
  st.set_unknown_12(0);
  st.set_oil_temperature(eighty);
  st.set_route_section(0xFFFF);
  st.set_route_slice(0xFFFF);
  st.set_engine_output(guest::LoadFloat(kOne));
  const schema::PowerCurve curve(frame + 80);  // on its stack
  // The car data's engine record (its +1268, the vehicle's +4644).
  const schema::CarData data(schema::Vehicle(vehicle).car_data_block_address());
  PowerCurveFromEngine(curve.address, data.gear_count_address(), frame - 112);
  PowerCurveShiftPeak(curve.address, 0, frame);
  CopyWord(curve.torque_scale_address(), st.torque_scale_address());
  return frame + 80;
}

// --- The vehicle set up from a car ---

uint32_t VehicleSetUpSurfaceGrip(uint32_t vehicle, uint32_t car, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820C1E00 Vehicle::SetUpSurfaceGrip");
  const uint32_t f = frame;
  const schema::Vehicle veh(vehicle);
  const schema::Car car_object(car);
  const double one = Lfs(kOne);
  // The four stack tables (nine floats each), in the original's order.
  Stfs(f + 112, one);
  const uint32_t flags = car_object.flags();
  Stfs(f + 116, one);
  Stfs(f + 120, one);
  Stfs(f + 124, one);
  const double base_factor = Lfs(0x82001070);
  Stfs(f + 128, base_factor);
  Stfs(f + 144, base_factor);
  const double a2c = Lfs(0x82002A2C);
  Stfs(f + 132, a2c);
  Stfs(f + 140, a2c);
  const double a08 = Lfs(0x82002A08);
  for (uint32_t at = 208; at <= 232; at += 4) {
    Stfs(f + at, a08);
  }
  const double bb4 = Lfs(0x82002BB4);
  const double bbc = Lfs(0x82002BBC);
  Stfs(f + 136, bbc);
  Stfs(f + 240, bb4);
  Stfs(f + 160, bb4);
  Stfs(f + 164, bb4);
  const double zero = Lfs(kZero);
  for (uint32_t at = 256; at <= 284; at += 4) {
    Stfs(f + at, zero);
  }
  Stfs(f + 288, Lfs(0x82002CC4));
  Stfs(f + 168, bb4);
  Stfs(f + 172, bb4);
  Stfs(f + 180, bb4);
  const double cc0 = Lfs(0x82002CC0);
  Stfs(f + 184, bb4);
  Stfs(f + 188, bb4);
  const double hundredth = Lfs(kHundredth);
  Stfs(f + 236, hundredth);
  const double cbc = Lfs(0x82002CBC);
  Stfs(f + 176, cc0);
  Stfs(f + 192, cbc);
  veh.set_tire_wear_mode((flags & 0x2) != 0 || (flags & 0x4) != 0 ? 0 : 1);
  const double scale = Lfs(0x8200256C);
  const double quarter = Lfs(0x82002968);
  uint32_t r3 = 0;
  for (uint32_t axle = 0; axle < 2; axle++) {
    const uint32_t type = axle == 0 ? car_object.front_tire_type() : car_object.rear_tire_type();
    uint32_t grade = axle == 0 ? car_object.front_tire_grade() : car_object.rear_tire_grade();
    if (type == 0) {
      grade = 0;
    }
    const double g =
        Fmuls(IntThroughStack(f + 296, uint64_t(int64_t(int32_t(grade)))), quarter);
    const uint32_t records =
        axle == 0 ? veh.front_grip_records_address() : veh.rear_grip_records_address();
    const uint32_t pair = (type << 3) & 0xFFFFFFF8u;
    const double soften = Fadds(g, one);
    const double a0 = Lfs(G::grip_pairs_a_address() + pair),
                 a1 = Lfs(G::grip_pairs_a_address() + 4 + pair);
    const double b0 = Lfs(G::grip_pairs_b_address() + pair);
    const double da = Fsubs(a1, a0);
    const double b1 = Lfs(G::grip_pairs_b_address() + 4 + pair);
    const double db = Fsubs(b1, b0);
    const double a = Fmadds(da, g, a0);
    const double b = Fmadds(db, g, b0);
    const double first = Fmadds(a, scale, base_factor);
    const double second = Fmadds(b, scale, base_factor);
    const uint32_t nine = type * 9;
    for (uint32_t j = 0; j < 9; j++) {
      const uint32_t at = ((nine + j) << 2) & 0xFFFFFFFCu;
      const double factor = (j == 1 || j == 8) ? second : first;
      const double grip = Fmuls(Fmuls(Lfs(G::grip_records_address() + at), factor),
                                Lfs(G::grip_scale_address()));
      const uint32_t record = records + 24 * j;
      Stfs(f + 92, grip);
      const double squared = Fmuls(grip, grip);
      Stfs(f + 84, Fmuls(Lfs(f + 112 + 4 * j), hundredth));
      CopyWord(f + 160 + 4 * j, f + 80);
      const double ratio = Fdivs(squared, soften);
      CopyWord(f + 208 + 4 * j, f + 88);
      CopyWord(f + 256 + 4 * j, f + 100);
      Stfs(f + 96, Fmuls(ratio, quarter));
      if (!c_runtime::Memcpy(record, f + 80, 24)) {
        // A stack range and the vehicle's never share memory.
        ITC_LOG_ERROR("vehicle: SetUpSurfaceGrip's record copy refused");
        for (uint32_t i = 0; i < 24; i++) {
          guest::StoreU8(record + i, guest::LoadU8(f + 80 + i));
        }
      }
      const uint32_t surfaces =
          axle == 0 ? veh.front_surfaces_address() : veh.rear_surfaces_address();
      guest::StoreU32(surfaces + 4 * j, record);
      r3 = record;
    }
  }
  return r3;
}

uint32_t VehicleSetUp(uint32_t vehicle, uint32_t car, uint32_t frame, bool reset,
                      bool native_heap, const CalleeOriginal& original, double* f1) {
  if (reset) {
    ITC_NATIVE_RUNNING("sub_820C33B8 Vehicle::ResetForCar");
  } else {
    ITC_NATIVE_RUNNING("sub_820C3150 Vehicle::SetUp");
  }
  const uint32_t v = vehicle, f = frame;
  const schema::Vehicle veh(v);
  const schema::Car car_object(car);
  const schema::CarSpecCopies block2(va::ObjectRecord1888(car));
  const uint32_t data = veh.car_data_block_address();
  veh.set_collision_flags(0);
  const uint32_t flags = car_object.flags();
  veh.set_car(car);
  veh.set_car_flags(flags);
  double ignored = 0.0;
  if (!cp::VehicleSetupFromCar(data, car, f - 368, &ignored)) {
    original(0x820C27E8, data, car, 0, 0);
  }
  VehicleStatusReset(veh.status_address(), v, f - 1168);
  uint32_t r3 = 0;
  if (reset) {
    if (!native_heap || !cp::VehicleResetPhysics(v, data, 7, f - 208, &ignored, &r3)) {
      original(0x820B5B08, v, data, 7, 0);
    }
  } else {
    if (!native_heap || !cp::VehicleSetUpPhysics(v, data, 1, 7, f - 208, &ignored, &r3)) {
      original(0x820B5670, v, data, 1, 7);
    }
  }
  if (!VehicleLinkTrackPositions(v, &r3)) {
    original(0x820C1D08, v, 0, 0, 0);
  }
  veh.set_surfaces_on(1);
  const int8_t extra = int8_t(car_object.setting_76());
  double value = 0.0;
  uint32_t on = 0;
  if (extra == 0) {
    value = Lfs(kZero);
  } else {
    on = 1;
    value = Fmuls(IntThroughStack(f + 80, uint64_t(int64_t(extra))), Lfs(0x82002BBC));
  }
  cp::VehicleSetValue2388(v, value, on);
  veh.set_abs(int8_t(car_object.setting_77()) != 0 ? 1 : 0);
  uint32_t mode = 0;
  uint8_t boosted = 0;
  if (block2.list_kind() == 1) {
    mode = 6;
    boosted = 1;
  } else {
    mode = car_object.power_part_b();
    boosted = int32_t(mode) >= 3 ? 1 : 0;
  }
  veh.set_unknown_2512(boosted);
  veh.set_unknown_2524(mode);
  if (block2.turbo() == 1) {
    const schema::CarData car_data(data);
    if (int32_t(car_object.engine_stage()) >= 8) {
      car_data.set_blow_off(1);
      if (!reset) {
        veh.set_unknown_2512(1);
        veh.set_unknown_2524(int32_t(mode) < 3 ? 3 : mode);
      }
    } else {
      car_data.set_blow_off(0);
    }
  }
  veh.set_flags(veh.flags() | 2);
  veh.set_automatic(car_object.unknown_3892() != 0 ? 1 : 0);
  VehicleSetUpSurfaceGrip(v, car, f - 480);
  veh.set_unknown_2380(1);
  veh.set_forced_rest(0);
  // The nitrous' tables (probably), in the original's order.
  guest::StoreU32(f + 96, 0);
  const uint32_t box = veh.box();
  Stfs(f + 128, Lfs(kOne));
  guest::StoreU32(f + 112, 570);
  Stfs(f + 132, Lfs(kOneAndHalf));
  guest::StoreU32(f + 116, 630);
  Stfs(f + 136, Lfs(0x82002CF4));
  guest::StoreU32(f + 100, 390);
  Stfs(f + 140, Lfs(0x82002C40));
  Stfs(f + 144, Lfs(0x82002C1C));
  guest::StoreU32(f + 104, 450);
  guest::StoreU32(f + 120, 690);
  guest::StoreU32(f + 108, 510);
  const uint32_t level = (car_object.nitrous() << 2) & 0xFFFFFFFCu;
  Stfs(f + 148, Lfs(0x82002CF0));
  Stfs(f + 152, Lfs(0x82002CEC));
  CopyWord(f + 128 + level, veh.nitrous_factor_address());
  const int64_t extra_rpm = int32_t(guest::LoadU32(f + 96 + level));
  veh.set_nitrous_value(float(IntThroughStack(f + 80, uint64_t(extra_rpm))));
  *f1 = value;
  if (box != 0) {
    const uint32_t car_flags = car_object.flags();
    const uint32_t half = (car_flags & 0x4) != 0 ? 2 : ((car_flags >> 1) & 1);
    va::ObjectSetHalf112(box, half & 0xFFFF);
  }
  return box;
}

// --- The vehicle's tick ---

namespace {

constexpr uint32_t kKmh = 0x820029E0;     // 3.6

}  // namespace

bool VehicleStatusUpdate(uint32_t status, uint32_t vehicle, uint32_t car,
                         uint32_t flag, uint32_t frame) {
  const uint32_t v = vehicle, f = frame;
  const schema::VehicleStatus st(status);
  const schema::Vehicle veh(v);
  if (!c_runtime::Memcpy(f + 208, veh.own_track_position_address(), 96)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820C23E8 VehicleStatus::Update");
  guest::StoreU32(f + 80, guest::LoadU32(f + 212));
  const uint32_t body = veh.body().address;
  const schema::VehicleBodyPosition position(va::RigidBodyPosition(body));
  guest::StoreU32(f + 128, guest::LoadU32(position.x_address()));
  guest::StoreU32(f + 132, guest::LoadU32(position.y_address()));
  guest::StoreU32(f + 136, guest::LoadU32(position.z_address()));
  guest::StoreU32(f + 140, guest::LoadU32(position.w_address()));
  RoutePositionStep(f + 80, 1, 0xFFFFFFFFu, f - 112);
  auto lha = [&](uint32_t at) { return uint32_t(int32_t(int16_t(guest::LoadU16(at)))); };
  track_route::SlicePoint(f + 112, 0, lha(f + 212), lha(f + 214));
  track_route::SlicePoint(f + 176, 0, lha(f + 80), lha(f + 82));
  const double zero = Lfs(kZero);
  Stfs(f + 132, zero);
  Stfs(f + 116, zero);
  Stfs(f + 180, zero);
  vl::Vec3Sub(f + 160, f + 176, f + 112);
  const uint32_t direction = st.direction_address();
  const double length = double(vl::Vec3Normalize(direction, f + 160));
  vl::Vec3Sub(f + 192, f + 128, f + 112);
  if (length == zero) {
    st.set_along(float(zero));
  } else {
    st.set_along(float(Fdivs(double(vl::Vec3Dot(f + 160, f + 192)), length)));
  }
  const uint32_t point = st.slice_point_address();
  vl::Vec4ScaleTo(point, direction, double(st.along()));
  vl::Vec3Add(point, point, f + 112);
  st.set_across(vl::Vec3Direction(f + 96, point, f + 128));
  vl::Vec3CrossW1(f + 96, f + 160, f + 192);
  if (Lfs(f + 100) < zero) {
    st.set_across(float(Fmuls(double(st.across()), Lfs(0x82001BE0))));
  }
  const uint32_t axis = st.heading_address();
  matrix_math::QuaternionXAxis(axis, va::RigidBodyField80(veh.body().address),
                               f - 96);
  st.set_heading_y(float(zero));
  vl::Vec3Normalize(axis, axis);
  const double along = double(vl::Vec3Dot(axis, direction));
  // Probably wrong way, with hysteresis.
  if (st.wrong_way() != 0) {
    const double limit = Fneg(double(float(c_runtime::Cos(
        std::bit_cast<double>((uint64_t(guest::LoadU32(0x82002CE0)) << 32) |
                              guest::LoadU32(0x82002CE4))))));
    if (along > limit) {
      st.set_wrong_way(0);
    }
  } else {
    const double limit = Fneg(double(float(c_runtime::Cos(
        std::bit_cast<double>((uint64_t(guest::LoadU32(0x82002CD8)) << 32) |
                              guest::LoadU32(0x82002CDC))))));
    if (along < limit) {  // bge: a NaN leaves it
      st.set_wrong_way(1);
    }
  }
  const double per_tick = Lfs(0x820029EC);  // 1/30
  if (flag != 0) {
    const double moved = Fmuls(Fmuls(double(veh.speed()), per_tick), Lfs(0x820029D8));
    const double trip = double(st.trip_km());
    st.set_total_km(float(Fadds(double(st.total_km()), moved)));
    st.set_trip_km(float(Fadds(trip, moved)));
  }
  const double hundredth = Lfs(kHundredth);
  if (car != 0) {
    // The engine's temperatures (probably), eased toward targets.
    const schema::VehicleCarObject car_object(car);
    const int64_t stage = int32_t(car_object.engine_stage());
    const double top =
        double(schema::CarData(veh.car_data_block_address()).max_rpm());
    const int64_t part = int32_t(car_object.engine_part());
    const double rpm = double(veh.unknown_2224());
    const double ratio = Fdivs(rpm, top);
    const double water = double(st.water_temperature());
    const double oil = double(st.oil_temperature());
    guest::StoreU32(f + 144, uint32_t(uint64_t(stage) >> 32));
    guest::StoreU32(f + 148, uint32_t(uint64_t(stage)));
    const double f9a = Lfs(0x8200257C);
    guest::StoreU32(f + 96, uint32_t(uint64_t(part) >> 32));
    guest::StoreU32(f + 100, uint32_t(uint64_t(part)));
    const double part_f = double(float(double(part)));
    const double eighty = Lfs(0x82001E8C);
    const double f7a = Lfs(0x82002CD0);
    const double f6a = Lfs(0x82002CCC);
    const double stage_d = double(stage);
    double x = Fmuls(ratio, Lfs(kHundred));
    const double stage_f = double(float(stage_d));
    x = Fmuls(x, x);
    const double level = Fsubs(stage_f, part_f);
    double load = Fmuls(x, hundredth);
    const double level2 = Fadds(level, Lfs(0x82002CAC));
    const double f10b = Lfs(0x82002578);
    load = Fmuls(load, hundredth);
    const double rate_a = Fmadds(level2, f7a, f10b);
    const double rate_b = Fmadds(level2, f6a, f10b);
    const double f12c = Lfs(0x82002A20);
    const double target_a = Fmadds(load, f9a, f12c);
    const double target_b = Fmadds(load, eighty, f12c);
    const double da = Fmuls(Fsubs(target_a, water), rate_a);
    const double db = Fmuls(Fsubs(target_b, oil), rate_b);
    st.set_water_temperature(float(Fmadds(da, per_tick, water)));
    st.set_oil_temperature(float(Fmadds(db, per_tick, oil)));
    const double rpm2 = double(veh.rpm());
    const double curve = Fadds(Fnmsubs(rpm2, rpm2, rpm2), rpm2);
    const double root = double(float(std::sqrt(Fmuls(curve, Lfs(0x82002814)))));
    const double torque = cp::VehicleEngineTorque(v, rpm2, f);
    const double heat = Fsubs(Lfs(0x820027A4), double(st.oil_temperature()));
    const double ratio2 = double(float(std::sqrt(Fdivs(torque, double(st.torque_scale())))));
    double value = Fmuls(Fmuls(ratio2, root), Lfs(0x82002BA4));
    value = Fmuls(Fmuls(value, heat), hundredth);
    st.set_engine_output(float(value));
  }
  // The brakes' efficiency (+2432) and +2428, lowered.
  const schema::CarData car_data(veh.car_data_block_address());
  const double speed = Fmuls(double(veh.speed()), per_tick);
  const double a = double(car_data.brake_heat_start());
  const double fade = Fsubs(double(car_data.brake_heat_limit()), a);
  const double brake = double(veh.brake());
  const double efficiency = double(veh.brake_efficiency());
  double lowered = Fmuls(Fmuls(Fmuls(fade, hundredth), Lfs(0x82002A44)), speed);
  const double least = Lfs(kHalf);
  lowered = Fsubs(efficiency, Fmuls(Fmuls(lowered, brake), hundredth));
  if (lowered < least) {
    lowered = least;
  }
  veh.set_brake_efficiency(float(lowered));
  double wear = Fmuls(double(st.oil_temperature()), Lfs(0x82002CC8));
  wear = Fsubs(double(veh.oil_factor()), Fmuls(wear, hundredth));
  const double floor = Lfs(0x82002A3C);
  if (wear < floor) {
    wear = floor;
  }
  veh.set_oil_factor(float(wear));
  return true;
}

void VehicleTick(uint32_t vehicle, uint32_t racer, uint32_t car, uint32_t flag,
                 uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_820C3020 Vehicle::Tick");
  const uint32_t v = vehicle, f = frame;
  const schema::Vehicle veh(v);
  const uint32_t s = veh.status_address();
  const schema::VehicleStatus st(s);
  const double kmh = Fmuls(double(veh.speed()), Lfs(kKmh));
  const uint32_t input = guest::LoadU32(G::input_records_address() + ((racer << 2) & 0x3FCu));
  st.set_speed_kmh(float(kmh));
  const schema::TrackPosition own(veh.own_track_position_address());
  st.set_route_section(own.route_section());
  st.set_route_slice(own.route_slice());
  for (int step = 0; step < 2; step++) {
    const uint32_t input_frame = f - 176;
    auto steer = [&](double target, double dt, double f3, uint32_t left, uint32_t right) {
      GuestRegisters r;
      r.r3 = v;
      r.r4 = left;
      r.r5 = right;
      r.f1 = target;
      r.f2 = dt;
      r.f3 = f3;
      call(0x820B5FD8, input_frame, r);
    };
    if (!VehicleApplyInput(v, input, input_frame, steer)) {
      GuestRegisters r;
      r.r3 = v;
      r.r4 = input;
      call(0x820BFD30, f, r);
    }
    {
      const schema::CarInput in(input);
      in.set_flags(in.flags() & 0xFFFFFFFCu & ~0x10000u);
    }
    const double dt = double(G::step_seconds());
    if (cp::CollisionWorldCallbackKnown()) {
      cp::VehiclePhysicsStep(v, dt, f - 112, [&](uint32_t address) {
        GuestRegisters r;
        r.r3 = v;
        call(address, f - 112, r);
      });
    } else {
      GuestRegisters r;
      r.r3 = v;
      r.f1 = dt;
      call(0x820B6498, f, r);
    }
    VehicleUpdateTrackPositions(v, f - 112);
  }
  if (!VehicleStatusUpdate(s, v, car, flag, f - 384)) {
    GuestRegisters r;
    r.r3 = s;
    r.r4 = v;
    r.r5 = car;
    r.r6 = flag;
    call(0x820C23E8, f, r);
  }
}

// --- The vehicle placed on the route ---

namespace {

using Fpscr = ::base::FloatMode;

constexpr uint32_t kRaise = 0x8200296C;        // 0.3
constexpr uint32_t kThousand = 0x82001BDC;     // 1000.0
constexpr uint32_t kPerHour = 0x82002CF8;      // 1/3,600

uint32_t S16At(uint32_t address) {
  return uint32_t(int32_t(int16_t(guest::LoadU16(address))));
}

// lvx128 and stvx as the translation does them: the 16 bytes at the address
// rounded down, word 0 in the top lane.
simde__m128 LoadLanes(uint32_t address) {
  const uint32_t a = address & ~0xFu;
  return simde_mm_castsi128_ps(
      simde_mm_set_epi32(int(guest::LoadU32(a + 0)), int(guest::LoadU32(a + 4)),
                         int(guest::LoadU32(a + 8)), int(guest::LoadU32(a + 12))));
}
void StoreLanes(uint32_t address, simde__m128 v) {
  alignas(16) uint32_t lanes[4];
  simde_mm_store_si128(reinterpret_cast<simde__m128i*>(lanes), simde_mm_castps_si128(v));
  const uint32_t a = address & ~0xFu;
  guest::StoreU32(a + 0, lanes[3]);
  guest::StoreU32(a + 4, lanes[2]);
  guest::StoreU32(a + 8, lanes[1]);
  guest::StoreU32(a + 12, lanes[0]);
}

// The vector unit's part of the orientation (flush-to-zero on, as the
// translated code), from the forward row at `frame` + 112 and the across
// row at + 128 (as loaded before any of these is stored): `cross` their
// cross product (across x forward, as vnmsubfp leaves it), `forward` the
// forward row normalized, `up` forward x cross normalized and `side` the
// cross product normalized (each its squared length where that's 0).
struct Rows {
  simde__m128 cross, forward, up, side;
};
Rows OrientationRows(simde__m128 v13, simde__m128 v0) {
  const uint32_t csr = Fpscr::getcsr();
  Fpscr::setcsr(csr | uint32_t(Fpscr::FlushMask));
  vl::PinInMode(v13, v0);
  const simde__m128i one = simde_mm_set1_epi32(1);
  const simde__m128 zero = simde_mm_castsi128_ps(simde_mm_set1_epi32(0));
  const simde__m128 sign = simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)));
  auto half = [&] {
    return simde_mm_mul_ps(simde_mm_cvtepi32_ps(one),
                           simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x3F000000))));
  };
  auto sel = [](simde__m128 a, simde__m128 b, simde__m128 mask) {
    return simde_mm_castsi128_ps(
        simde_mm_or_si128(simde_mm_andnot_si128(simde_mm_castps_si128(mask), simde_mm_castps_si128(a)),
                          simde_mm_and_si128(simde_mm_castps_si128(mask), simde_mm_castps_si128(b))));
  };
  auto rsqrte = [](simde__m128 x) { return simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(x)); };
  auto nmsub = [&](simde__m128 a, simde__m128 b, simde__m128 c) {
    return simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(a, b), c), sign);
  };
  auto madd = [](simde__m128 a, simde__m128 b, simde__m128 c) {
    return simde_mm_add_ps(simde_mm_mul_ps(a, b), c);
  };
  auto perm = [](simde__m128 v, int which) {
    const simde__m128i i = simde_mm_castps_si128(v);
    return simde_mm_castsi128_ps(which == 0x78 ? simde_mm_shuffle_epi32(i, 0x78)
                                               : simde_mm_shuffle_epi32(i, 0x9C));
  };
  Rows rows;
  simde__m128 v12 = simde_mm_dp_ps(v13, v13, 0xEF);
  simde__m128 v5 = perm(v13, 0x78);
  simde__m128 v7 = half();
  simde__m128 v6 = perm(v13, 0x9C);
  simde__m128 v4 = perm(v0, 0x9C);
  simde__m128 v3 = perm(v0, 0x78);
  v0 = simde_mm_mul_ps(v4, v5);
  simde__m128 v11 = rsqrte(v12);
  simde__m128 v2 = simde_mm_mul_ps(v12, v7);
  simde__m128 v8 = simde_mm_cmpeq_ps(v12, zero);
  v0 = nmsub(v3, v6, v0);
  v4 = simde_mm_mul_ps(v11, v11);
  v3 = perm(v0, 0x9C);
  rows.cross = v0;
  v7 = nmsub(v2, v4, v7);
  v4 = perm(v0, 0x78);
  v6 = simde_mm_mul_ps(v6, v4);
  v11 = madd(v11, v7, v11);
  v7 = half();
  v6 = nmsub(v5, v3, v6);
  v5 = half();
  v4 = simde_mm_mul_ps(v13, v11);
  v11 = simde_mm_dp_ps(v0, v0, 0xEF);
  v13 = simde_mm_dp_ps(v6, v6, 0xEF);
  rows.forward = sel(v4, v12, v8);
  simde__m128 v10 = rsqrte(v11);
  v3 = simde_mm_mul_ps(v11, v5);
  v12 = rsqrte(v13);
  v4 = simde_mm_mul_ps(v13, v7);
  v8 = simde_mm_cmpeq_ps(v13, zero);
  simde__m128 v9 = simde_mm_cmpeq_ps(v11, zero);
  simde__m128 v1 = simde_mm_mul_ps(v10, v10);
  v2 = simde_mm_mul_ps(v12, v12);
  v5 = nmsub(v3, v1, v5);
  v7 = nmsub(v4, v2, v7);
  v12 = madd(v12, v7, v12);
  v12 = simde_mm_mul_ps(v6, v12);
  rows.up = sel(v12, v13, v8);
  v13 = madd(v10, v5, v10);
  v0 = simde_mm_mul_ps(v0, v13);
  rows.side = sel(v0, v11, v9);
  vl::PinInMode(rows.cross, rows.forward, rows.up, rows.side);
  Fpscr::setcsr(csr);
  return rows;
}

}  // namespace

bool VehiclePlaceOnRoute(uint32_t vehicle, uint32_t position, double speed,
                         double offset, int32_t lane, uint32_t direction, uint32_t frame,
                         const GuestCall& call) {
  const double zero = Lfs(kZero);
  if (!(offset == zero)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820C3608 Vehicle::PlaceOnRoute");
  const uint32_t v = vehicle, f = frame;
  const schema::Vehicle veh(v);
  const uint32_t car = veh.own_track_position_address();
  auto reset_transform = [&] {
    const uint32_t object = veh.collision_object();
    if (!cp::CollisionObjectResetTransform(object, f - 112)) {
      GuestRegisters r;
      r.r3 = object;
      call(0x820B7998, f, r);
    }
  };
  // The route positions: back (+80), on (+144) and its own (+176).
  const schema::RoutePosition route_position(position);
  const uint32_t word = route_position.word();
  guest::StoreU32(f + 176, word);
  guest::StoreU32(f + 80, word);
  guest::StoreU32(f + 144, word);
  RoutePositionStep(f + 80, uint32_t(-1), uint32_t(-1), f - 112);
  RoutePositionStep(f + 144, 1, uint32_t(-1), f - 112);
  const schema::TrackPosition car_position(car);
  car_position.set_route_section(route_position.section());
  car_position.set_route_slice(route_position.slice());
  tc::TrackPositionInit(car, f - 128);
  // The slices' points: the centers back (+240) and on (+256), the edges
  // (+192, +208).
  const double one = Lfs(kOne);
  Stfs(f + 252, one);
  Stfs(f + 268, one);
  Stfs(f + 204, one);
  Stfs(f + 220, one);
  track_route::SlicePoint(f + 240, 0, S16At(f + 80), S16At(f + 82));
  track_route::SlicePoint(f + 256, 0, S16At(f + 144), S16At(f + 146));
  const uint32_t slice = S16At(f + 178), section = S16At(f + 176);
  track_route::SlicePoint(f + 192, 1, section, slice);
  track_route::SlicePoint(f + 208, 2, section, slice);
  // Forward (+112) and across (+128), each built at +96 or +160 and copied
  // (ld and std: their bits).
  Stfs(f + 108, one);
  Stfs(f + 172, one);
  uint32_t forward_at, across_at;
  if (direction != 0) {
    Stfs(f + 96, Fsubs(Lfs(f + 256), Lfs(f + 240)));
    Stfs(f + 100, Fsubs(Lfs(f + 260), Lfs(f + 244)));
    Stfs(f + 104, Fsubs(Lfs(f + 264), Lfs(f + 248)));
    Stfs(f + 160, Fsubs(Lfs(f + 192), Lfs(f + 208)));
    Stfs(f + 164, Fsubs(Lfs(f + 196), Lfs(f + 212)));
    Stfs(f + 168, Fsubs(Lfs(f + 200), Lfs(f + 216)));
    forward_at = f + 96;
    across_at = f + 160;
  } else {
    Stfs(f + 160, Fsubs(Lfs(f + 240), Lfs(f + 256)));
    Stfs(f + 164, Fsubs(Lfs(f + 244), Lfs(f + 260)));
    Stfs(f + 168, Fsubs(Lfs(f + 248), Lfs(f + 264)));
    Stfs(f + 96, Fsubs(Lfs(f + 208), Lfs(f + 192)));
    Stfs(f + 100, Fsubs(Lfs(f + 212), Lfs(f + 196)));
    Stfs(f + 104, Fsubs(Lfs(f + 216), Lfs(f + 200)));
    forward_at = f + 160;
    across_at = f + 96;
  }
  for (uint32_t i = 0; i < 16; i += 4) {
    guest::StoreU32(f + 112 + i, guest::LoadU32(forward_at + i));
  }
  for (uint32_t i = 0; i < 16; i += 4) {
    guest::StoreU32(f + 128 + i, guest::LoadU32(across_at + i));
  }
  // The rows: +160 forward, +128 up, +112 the side, as the original leaves
  // its stores (the vector unit's work in between).
  Stfs(f + 124, one);
  Stfs(f + 140, one);
  Stfs(f + 108, one);
  const Rows rows = OrientationRows(LoadLanes(f + 112), LoadLanes(f + 128));
  StoreLanes(f + 96, rows.cross);
  StoreLanes(f + 112, rows.forward);
  Stfs(f + 172, Lfs(f + 124));
  const double fx = Lfs(f + 112), fy = Lfs(f + 116), fz = Lfs(f + 120);
  Stfs(f + 160, fx);
  Stfs(f + 164, fy);
  Stfs(f + 168, fz);
  StoreLanes(f + 128, rows.up);
  Stfs(f + 112, Lfs(f + 128));
  Stfs(f + 116, Lfs(f + 132));
  Stfs(f + 120, Lfs(f + 136));
  Stfs(f + 124, Lfs(f + 140));
  StoreLanes(f + 96, rows.side);
  for (uint32_t i = 0; i < 16; i += 4) {
    Stfs(f + 128 + i, Lfs(f + 96 + i));
  }
  MatrixFromRows(f + 352, f + 160, f + 128, f + 112, G::last_row_address());
  matrix_math::QuaternionFromMatrix(f + 320, f + 352);
  cp::RigidBodySetOrientation(veh.body().address, f + 320, f - 96);
  cp::VehicleSyncFromBody(v, f - 368);
  reset_transform();
  // The velocity: forward x the speed x 1000 / 3600.
  Stfs(f + 272, fx);
  Stfs(f + 276, fy);
  Stfs(f + 280, fz);
  Stfs(f + 284, one);
  vl::Vec4Scaled(f + 160, f + 272, speed);
  vl::Vec4Scaled(f + 96, f + 160, Lfs(kThousand));
  vl::Vec4Scaled(f + 336, f + 96, Lfs(kPerHour));
  cp::RigidBodySetVelocity(veh.body().address, f + 336, f - 112);
  // The position: the lane's point or the slice back's center, 0.3 higher.
  Stfs(f + 236, zero);
  if (lane >= 0) {
    track_route::LanePoint(G::route_address(), f + 224, f + 80, lane);
  } else {
    const uint32_t sections = schema::TrackRouteFile(G::route()).sections();
    // The section's slice records start at its +124 (TrackRouteSection::slices).
    const schema::TrackRouteSlice record(guest::LoadU32((uint32_t(guest::LoadU16(f + 80)) << 2) + sections) +
            S16At(f + 82) * 84u + 124);
    Stfs(f + 224, double(record.center_x()));
    Stfs(f + 228, double(record.center_y()));
    Stfs(f + 232, double(record.center_z()));
    Stfs(f + 236, one);
  }
  Stfs(f + 288, Lfs(f + 224));
  const double y = Lfs(f + 228);
  Stfs(f + 300, zero);
  Stfs(f + 292, Fadds(y, Lfs(kRaise)));
  Stfs(f + 296, Lfs(f + 232));
  cp::RigidBodySetPosition(veh.body().address, f + 288, f - 192);
  cp::VehicleSyncFromBody(v, f - 368);
  reset_transform();
  // The wheels' track positions: copies of the car's.
  for (uint32_t i = 0; i < 4; i++) {
    if (!c_runtime::Memcpy(f + 352, car, 96)) {
      GuestRegisters r;
      r.r3 = f + 352;
      r.r4 = car;
      r.r5 = 96;
      call(0x825A7410, f, r);
    }
    TrackPositionCopy(veh.wheel_track_positions_address() + 96 * i, f + 352);
  }
  Stfs(f + 304, zero);
  Stfs(f + 308, zero);
  Stfs(f + 312, zero);
  Stfs(f + 316, zero);
  cp::RigidBodySetAngularVelocity(veh.body().address, f + 304, f - 160);
  return true;
}

}  // namespace vehicle
