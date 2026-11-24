// The camera modes' views' cameras (docs/functions/camera-modes.md): the
// chase camera (view 0, and the TV camera's fallback at +43136) and views 1
// and 2's cameras (probably the in-car views). Plain C++
// over guest memory, the originals' loads and stores in their order, their
// float steps as the translated code's (src/camera_modes/steps.h); the
// vector unit's work with flush-to-zero on, the rest with it off. Stack
// frames the originals write are written the same way, at the frames the
// adapters hand in.

#include "camera_modes/camera_modes.h"

#include "camera_modes/steps.h"

#include "base/log.h"
#include "c_runtime/math.h"
#include "cameras/cameras.h"
#include "car_physics/car_physics.h"
#include "vehicle/vehicle.h"
#include "guest.h"
#include "hooks/vehicle.h"
#include "matrix_math/matrix_math.h"
#include "track_zones/track_zones.h"
#include "vehicle_accessors/vehicle_accessors.h"
#include "schema/camera_modes.h"
#include "schema/car_draw.h"
#include "schema/car_physics.h"
#include "schema/cameras.h"
#include "schema/car_scene_draw.h"
#include "schema/frame_passes.h"
#include "schema/game_modes.h"
#include "schema/race_event.h"
#include "schema/race_states.h"
#include "schema/scene_models.h"
#include "schema/type6_type7_modes.h"
#include "schema/video_output.h"

namespace camera_modes {

using namespace steps;

namespace {

using G = schema::CameraModesGlobals;

constexpr uint32_t kOne = G::one_address();    // 1.0
constexpr uint32_t kZero = G::zero_address();  // 0.0

uint32_t Load(uint8_t* base, uint32_t a) { return guest::LoadU32(base, a); }
uint64_t Load64(uint8_t* base, uint32_t a) {
  return (uint64_t(guest::LoadU32(base, a)) << 32) | guest::LoadU32(base, a + 4);
}
void Store(uint8_t* base, uint32_t a, uint32_t v) { guest::StoreU32(base, a, v); }
void Store64(uint8_t* base, uint32_t a, uint64_t v) {
  guest::StoreU32(base, a, uint32_t(v >> 32));
  guest::StoreU32(base, a + 4, uint32_t(v));
}
// lfs then stfs: the bits as the translated code keeps them.
void CopyWords(uint8_t* base, uint32_t to, uint32_t from, uint32_t words) {
  for (uint32_t i = 0; i < words; i++) {
    Store(base, to + 4 * i, Load(base, from + 4 * i));
  }
}
// ld/std copies of `doublewords`.
void CopyDoublewords(uint8_t* base, uint32_t to, uint32_t from, uint32_t doublewords) {
  for (uint32_t i = 0; i < doublewords; i++) {
    Store64(base, to + 8 * i, Load64(base, from + 8 * i));
  }
}

// --- The vector unit, as the translated code does it (word 0 in the top lane) ---

simde__m128 Lvx(uint8_t* base, uint32_t address) {
  const uint32_t at = address & ~0xFu;
  return VectorOf(Load(base, at), Load(base, at + 4), Load(base, at + 8), Load(base, at + 12));
}
simde__m128 Mul(simde__m128 a, simde__m128 b) { return simde_mm_mul_ps(a, b); }
// vmaddfp: a b + c (not fused, as the translation).
simde__m128 MulAdd(simde__m128 a, simde__m128 b, simde__m128 c) {
  return simde_mm_add_ps(simde_mm_mul_ps(a, b), c);
}
// vnmsubfp: -(a b - c), the sign flipped.
simde__m128 NegMulSub(simde__m128 a, simde__m128 b, simde__m128 c) {
  return simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(a, b), c),
                         simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000))));
}
// vmsum4fp128 (all four words) and vmsum3fp128 (words 0-2), into every lane.
simde__m128 Dot4(simde__m128 a, simde__m128 b) { return simde_mm_dp_ps(a, b, 0xFF); }
simde__m128 Dot3(simde__m128 a, simde__m128 b) { return simde_mm_dp_ps(a, b, 0xEF); }
// vsel: b where the mask is set, else a.
simde__m128 Select(simde__m128 mask, simde__m128 a, simde__m128 b) {
  const simde__m128i m = simde_mm_castps_si128(mask);
  return simde_mm_castsi128_ps(
      simde_mm_or_si128(simde_mm_andnot_si128(m, simde_mm_castps_si128(a)),
                        simde_mm_and_si128(m, simde_mm_castps_si128(b))));
}
// vcfsx of (1, 1, 1, 1) by 1: 0.5 in every lane.
simde__m128 Half() {
  return simde_mm_mul_ps(simde_mm_cvtepi32_ps(simde_mm_set1_epi32(1)),
                         simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x3F000000))));
}
// The originals' normalize of `v` by its squared length `dot` (the
// estimate, vrsqrtefp, and one Newton step; the squared length left where
// it's 0). Flush-to-zero on (the caller's).
simde__m128 Normalized(simde__m128 v, simde__m128 dot) {
  const simde__m128 half = Half();
  const simde__m128 estimate = simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(dot));
  const simde__m128 halved = Mul(dot, half);
  const simde__m128 zero = simde_mm_cmpeq_ps(dot, simde_mm_setzero_ps());
  const simde__m128 squared = Mul(estimate, estimate);
  const simde__m128 step = NegMulSub(halved, squared, half);
  const simde__m128 refined = MulAdd(estimate, step, estimate);
  return Select(zero, Mul(v, refined), dot);
}
// The quaternion at `at` normalized in place (four words), as the chase
// camera's functions do it (lvx and stvx: the aligned 16 bytes).
void NormalizeQuat(uint8_t* base, uint32_t at) {
  const simde__m128 q = Lvx(base, at);
  simde__m128 n;
  {
    FlushToZero ftz;
    n = Normalized(q, Dot4(q, q));
  }
  Stvx(base, at, n);
}

// The identity matrix as the originals store it (vupkd3d128 of 0 and four
// vpermwi128s): rows (1, 0, 0, 0) to (0, 0, 0, 1).
void StoreIdentity(uint8_t* base, uint32_t at) {
  constexpr uint32_t kOneBits = 0x3F800000;
  for (uint32_t row = 0; row < 4; row++) {
    for (uint32_t i = 0; i < 4; i++) {
      Store(base, (at & ~0xFu) + 16 * row + 4 * i, row == i ? kOneBits : 0);
    }
  }
}

// The zoom from the float at `degrees` at the display's width (the video
// output's +296, an integer, kept as a doubleword at `slot` as the
// originals' conversion does): Camera::ZoomFromFov (ChaseReady checked that
// its tan stands in).
double Zoom(uint8_t* base, uint32_t degrees, uint32_t slot, uint32_t frame, uint32_t back,
            const VideoOutputCall& video_output) {
  const uint32_t video = video_output(frame, back);
  const double fov = Lfs(base, degrees);
  const int32_t width = int32_t(schema::VideoOutput(base, video).display_width());
  Store64(base, slot, uint64_t(int64_t(width)));
  return ZoomFromFov(base, fov, Single(double(int64_t(width)))).value();
}

// The zoom, near plane and roll every view's camera sets: SetZoom(zoom),
// SetNear(+1868 / +1864 x zoom), SetRoll(roll).
void SetLens(uint8_t* base, uint32_t camera, double zoom, double roll) {
  const schema::CameraObject c(base, camera);
  cameras::ViewportSetZoom(base, c.viewport().address, zoom);
  const uint32_t viewport = c.viewport().address;
  const double ratio = Fdivs(double(schema::Viewport(base, viewport).near()),
                             double(schema::Viewport(base, viewport).zoom()));
  cameras::SetNear(base, Fmuls(ratio, zoom));
  cameras::ViewportSetRoll(base, c.viewport().address, roll);
}
// The same with a given near plane.
void SetLensNear(uint8_t* base, uint32_t camera, double zoom, double near, double roll) {
  const schema::CameraObject c(base, camera);
  cameras::ViewportSetZoom(base, c.viewport().address, zoom);
  cameras::SetNear(base, near);
  cameras::ViewportSetRoll(base, c.viewport().address, roll);
}

// The tracker's w words (1.0), as every view camera's constructor sets them.
void TrackerOnes(uint8_t* base, uint32_t tracker, double one) {
  for (uint32_t at = 12; at <= 220; at += 16) {
    Stfs(base, tracker + at, one);
  }
  for (uint32_t at : {252u, 268u, 284u, 300u}) {
    Stfs(base, tracker + at, one);
  }
}

// A static vector made once: (x, y, z, w) at `at` with guard bit `bit` of
// `guard` set, then atexit(`function`). `guard_first`: the guard stored
// before the floats (as some originals do). Returns the guard as reloaded.
uint32_t MakeStatic(uint8_t* base, uint32_t guard_address, uint32_t guard, uint32_t bit,
                    uint32_t at, const double (&v)[4], uint32_t function, bool guard_first,
                    const c_runtime::AtexitCall& atexit) {
  if ((guard & bit) != 0) {
    return guard;
  }
  guard |= bit;
  if (guard_first) {
    Store(base, guard_address, guard);
  }
  for (uint32_t i = 0; i < 4; i++) {
    Stfs(base, at + 4 * i, v[i]);
  }
  if (!guard_first) {
    Store(base, guard_address, guard);
  }
  c_runtime::AtexitFrom(base, function, atexit);
  return Load(base, guard_address);
}

// The look point every in-car view sets: +1648 = the eye + the view's unit
// direction (target less eye, normalized on the vector unit at `f` + 80),
// +1660 = 1.0.
void LookPoint(uint8_t* base, uint32_t camera, uint32_t f, double one) {
  const schema::CameraObject c(base, camera);
  uint32_t viewport = c.viewport().address;
  Stfs(base, f + 92, one);
  const double ex = double(schema::Viewport(base, viewport).eye_x());
  Stfs(base, f + 80, Fsubs(double(schema::Viewport(base, viewport).target_x()), ex));
  const double ty = double(schema::Viewport(base, viewport).target_y()),
               tz = double(schema::Viewport(base, viewport).target_z());
  const double ey = double(schema::Viewport(base, viewport).eye_y()),
               ez = double(schema::Viewport(base, viewport).eye_z());
  const double dy = Fsubs(ty, ey);
  Stfs(base, f + 88, Fsubs(tz, ez));
  Stfs(base, f + 84, dy);
  const simde__m128 v = Lvx(base, f + 80);
  simde__m128 n;
  {
    FlushToZero ftz;
    n = Normalized(v, Dot3(v, v));
  }
  Stvx(base, f + 80, n);
  schema::Viewport(base, viewport).set_look_x(float(Fadds(ex, Lfs(base, f + 80))));
  schema::Viewport(base, viewport)
      .set_look_y(
          float(Fadds(double(schema::Viewport(base, viewport).eye_y()), Lfs(base, f + 84))));
  schema::Viewport(base, viewport)
      .set_look_z(
          float(Fadds(double(schema::Viewport(base, viewport).eye_z()), Lfs(base, f + 88))));
  viewport = c.viewport().address;
  schema::Viewport(base, viewport).set_look_w(float(one));
}

// The nearest human car's body (Racers::NearestHumanVehicle(the tracker's
// position at `point`, 1), its frame below `f`): the originals then take
// its matrix or position and leave it unused.
void NearestHuman(uint8_t* base, uint32_t camera, uint32_t point, uint32_t f) {
  const schema::CameraObject c(base, camera);
  CopyWords(base, point, c.tracker_address(), 4);
  vehicle::RacersNearestHumanVehicle(base, point, 1, f - 304);
}

constexpr uint32_t kChaseDegrees = G::chase_degrees_address();    // the constructor's (55.0)
constexpr uint32_t kFollowDegrees = G::follow_degrees_address();  // Follow's (55.0)

}  // namespace

bool ChaseReady(uint8_t* base) {
  return ZoomFromFov(base, Lfs(base, kChaseDegrees), 1.0).has_value() &&
         ZoomFromFov(base, Lfs(base, kFollowDegrees), 1.0).has_value();
}

// --- The chase camera ---

void ChasePlace(uint8_t* base, uint32_t camera, uint32_t eye, uint32_t target, uint32_t up,
                uint32_t look, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82153478 ChaseCamera::Place");
  const schema::ChaseCamera c(base, camera);
  const schema::CameraTarget t(base, c.tracker_address());
  const double zero = Lfs(base, kZero);
  uint32_t viewport = c.viewport().address;
  CopyWords(base, schema::Viewport(base, viewport).base_eye_address(), eye, 4);
  viewport = c.viewport().address;
  CopyWords(base, schema::Viewport(base, viewport).base_target_address(), target, 4);
  viewport = c.viewport().address;
  CopyWords(base, schema::Viewport(base, viewport).base_up_address(), up, 4);
  viewport = c.viewport().address;
  CopyWords(base, schema::Viewport(base, viewport).base_look_address(), look, 4);
  viewport = c.viewport().address;
  CopyWords(base, schema::Viewport(base, viewport).view1_target_address(),
            schema::Viewport(base, viewport).base_look_address(), 4);
  const uint32_t vehicle = c.vehicle();
  c.set_unknown_368(float(zero));
  if (vehicle == 0) {
    return;
  }
  TargetTrack(base, c.tracker_address(), vehicle, frame - 256);
  // The tracker's rotation (+224) normalized, kept at +240.
  const uint32_t rotation = t.rotation_address();
  NormalizeQuat(base, rotation);
  const double one = Lfs(base, kOne);
  CopyWords(base, t.last_rotation_address(), rotation, 4);
  CopyDoublewords(base, t.last_rotation_address(), rotation, 2);
  for (uint32_t i = 0; i < 3; i++) {
    Stfs(base, c.viewport().base_target_address() + 4 * i, zero);
  }
  Stfs(base, c.viewport().base_target_w_address(), one);
  viewport = c.viewport().address;
  schema::Viewport(base, viewport)
      .set_base_target_y(float(
          Fadds(double(schema::Viewport(base, viewport).base_target_y()), Lfs(base, 0x820029F0))));
  // The track position's section and slice (+260, +262) into +376.
  const uint16_t section = guest::LoadU16(base, t.track_position_address() + 4);
  const uint16_t slice = guest::LoadU16(base, t.track_position_address() + 6);
  c.set_section(section);
  c.set_slice(slice);
}

void ChaseFollow(uint8_t* base, uint32_t camera, double rate, double least, uint32_t frame,
                 const VideoOutputCall& video_output) {
  ITC_NATIVE_RUNNING("sub_82153640 ChaseCamera::Follow");
  const schema::ChaseCamera c(base, camera);
  const schema::CameraTarget t(base, c.tracker_address());
  const uint32_t f = frame;
  const double one = Lfs(base, kOne);
  const double zero = Lfs(base, kZero);
  // (0, 0, 0, 1) at +96, +112, +128 and +144; the identity at +160.
  for (uint32_t at : {96u, 112u, 144u, 128u}) {
    Stfs(base, f + at + 0, zero);
    Stfs(base, f + at + 4, zero);
    Stfs(base, f + at + 8, zero);
    Stfs(base, f + at + 12, one);
  }
  StoreIdentity(base, f + 160);
  const uint32_t vehicle = c.vehicle();
  if (vehicle == 0) {
    return;
  }
  const uint32_t tracker = c.tracker_address();
  TargetTrack(base, tracker, vehicle, f - 256);
  SetLens(base, camera, Zoom(base, kFollowDegrees, f + 80, f, 0x82153730, video_output), zero);
  // Forward (1, 0, 0, 1) through the turn now (+160) and before (+96): on
  // which side the turn has gone.
  Stfs(base, f + 112, one);
  Stfs(base, f + 96, one);
  Stfs(base, f + 116, zero);
  Stfs(base, f + 100, zero);
  Stfs(base, f + 120, zero);
  Stfs(base, f + 104, zero);
  Stfs(base, f + 124, one);
  Stfs(base, f + 108, one);
  matrix_math::Vec4Transform(base, f + 96, t.last_turn_address(), f + 96);
  const uint32_t turn = t.turn_address();
  matrix_math::Vec4Transform(base, f + 112, turn, f + 112);
  {
    const simde__m128 now = Lvx(base, f + 112);
    const simde__m128 before = Lvx(base, f + 96);
    simde__m128 dot;
    {
      FlushToZero ftz;
      dot = Dot3(before, now);
    }
    Stvx(base, f + 80, dot);
  }
  // The rate: 1.0 when (d < 0) d^4 is 0 (in single steps), else the float
  // at 0x82001BE0; times `rate`, at least `least`.
  const double d = Lfs(base, f + 80);
  const int64_t behind = d < zero ? 1 : 0;
  Store64(base, f + 80, uint64_t(behind));
  double p = Single(double(behind));
  p = Fmuls(p, d);
  p = Fmuls(p, d);
  p = Fmuls(p, d);
  p = Fmuls(p, d);
  double w = p == zero ? one : Lfs(base, 0x82001BE0);
  w = Fmuls(w, rate);
  if (w < least) {
    w = least;
  }
  const double keep = Fsubs(one, w);
  // The rotation (+224) blended toward the last (+240) by `keep`.
  const uint32_t rotation = t.rotation_address();
  const double l0 = double(t.last_rotation_x()), l1 = double(t.last_rotation_y());
  const double l2 = double(t.last_rotation_z()), l3 = double(t.last_rotation_w());
  const double r1 = Fmuls(Lfs(base, rotation + 4), w);
  const double r2 = Fmuls(Lfs(base, rotation + 8), w);
  const double r3 = Lfs(base, rotation + 12);
  const double k0 = Fmuls(l0, keep), k1 = Fmuls(l1, keep), k2 = Fmuls(l2, keep);
  const double k3 = Fmuls(l3, keep);
  const double r0 = Fmuls(Lfs(base, rotation + 0), w);
  const double b1 = Fadds(r1, k1);
  Stfs(base, f + 84, b1);
  const double b2 = Fadds(r2, k2);
  Stfs(base, f + 88, b2);
  const double b0 = Fadds(r0, k0);
  Stfs(base, f + 80, b0);
  const double b3 = Fadds(Fmuls(r3, w), k3);
  Stfs(base, f + 92, b3);
  CopyDoublewords(base, rotation, f + 80, 2);
  NormalizeQuat(base, rotation);
  // The eye's offset from the target (+1680 less +1664) turned by it.
  uint32_t viewport = c.viewport().address;
  for (uint32_t i = 0; i < 3; i++) {
    Stfs(base, f + 128 + 4 * i,
         Fsubs(Lfs(base, schema::Viewport(base, viewport).base_eye_address() + 4 * i),
               Lfs(base, schema::Viewport(base, viewport).view1_target_address() + 4 * i)));
  }
  QuatRotate(base, f + 80, rotation, f + 128);
  CopyDoublewords(base, f + 128, f + 80, 2);
  // The eye: the target point (+1664) through the turn about the tracked
  // point (+64), plus the offset; the target through it from +1696.
  const uint32_t point = t.point_address();
  car_physics::MatrixMultiplyTranslation(base, f + 160, turn, point, f - 192);
  matrix_math::Vec4Transform(base, f + 144, f + 160, c.viewport().view1_target_address());
  viewport = c.viewport().address;
  for (uint32_t i = 0; i < 3; i++) {
    Stfs(base, schema::Viewport(base, viewport).eye_address() + 4 * i,
         Fadds(Lfs(base, f + 128 + 4 * i), Lfs(base, f + 144 + 4 * i)));
  }
  Stfs(base, c.viewport().eye_w_address(), one);
  car_physics::MatrixMultiplyTranslation(base, f + 160, turn, point, f - 192);
  viewport = c.viewport().address;
  matrix_math::Vec4Transform(base, schema::Viewport(base, viewport).target_address(), f + 160,
                             schema::Viewport(base, viewport).base_target_address());
  viewport = c.viewport().address;
  CopyWords(base, schema::Viewport(base, viewport).up_address(),
            schema::Viewport(base, viewport).base_up_address(), 4);
  viewport = c.viewport().address;
  CopyWords(base, schema::Viewport(base, viewport).look_address(), tracker, 4);
}

uint32_t ChaseConstruct(uint8_t* base, uint32_t camera, uint32_t viewport, uint32_t vehicle,
                        uint32_t frame, const VideoOutputCall& video_output,
                        const c_runtime::AtexitCall& atexit) {
  ITC_NATIVE_RUNNING("sub_82154548 ChaseCamera::ChaseCamera");
  const schema::ChaseCamera c(base, camera);
  constexpr uint32_t kGuard = G::chase_guard_address();
  constexpr uint32_t kUp = G::chase_up_address();      // (0, 1, 0, 1)
  constexpr uint32_t kLook = G::chase_look_address();  // (0x8200295C's, 0x82008290's, 0, 1)
  const uint32_t f = frame;
  const uint32_t tracker = c.tracker_address();
  const double one = Lfs(base, kOne);
  for (uint32_t at = 12; at <= 220; at += 16) {
    Stfs(base, tracker + at, one);
  }
  for (uint32_t at : {252u, 268u, 284u, 300u}) {
    Stfs(base, tracker + at, one);
  }
  c.set_vtable(0x82006B24);
  c.set_unknown_364(float(one));
  c.set_vehicle(vehicle);
  c.set_viewport(viewport);
  // The offsets from the car's spec block: its +76 and +80 halved, an angle
  // (the table at 0x8200930C by the word at +372, in degrees).
  const uint32_t spec = vehicle_accessors::ObjectRecord1888(schema::Vehicle(base, vehicle).car());
  const double scale = Lfs(base, 0x820029D8);
  const uint32_t index = c.angle();
  const uint32_t r80 = schema::CarSpecCopies(base, spec).unknown_80();
  const double degrees = Lfs(base, (index << 2) + 0x8200930C);
  Store64(base, f + 96, r80);
  const uint32_t r76 = schema::CarSpecCopies(base, spec).unknown_76();
  const double angle = Fmuls(degrees, Lfs(base, 0x82002948));
  Store64(base, f + 80, r76);
  const double half = Lfs(base, 0x82001BE4);
  const double wid = Fmuls(Single(double(int64_t(r76))), half);
  const double len = Fmuls(Single(double(int64_t(r80))), half);
  const double reach = Fmadds(wid, scale, Lfs(base, 0x82002C40));
  const double height = Fmuls(len, scale);
  const double sin = Single(c_runtime::Sin(angle));
  const double cos = c_runtime::Cos(angle);
  Stfs(base, f + 116, Fmadds(sin, reach, height));
  Stfs(base, f + 124, one);
  Stfs(base, f + 84, height);
  Stfs(base, f + 92, one);
  uint32_t guard = Load(base, kGuard);
  const double x = Fneg(Fmuls(Single(cos), reach));
  const double zero = Lfs(base, kZero);
  Stfs(base, f + 120, zero);
  Stfs(base, f + 80, zero);
  Stfs(base, f + 88, zero);
  Stfs(base, f + 112, x);
  if ((guard & 1) == 0) {
    guard |= 1;
    Stfs(base, kUp + 0, zero);
    Stfs(base, kUp + 4, one);
    Stfs(base, kUp + 8, zero);
    Stfs(base, kUp + 12, one);
    Store(base, kGuard, guard);
    c_runtime::AtexitFrom(base, 0x825DB100, atexit);
    guard = Load(base, kGuard);
  }
  if ((guard & 2) == 0) {
    Store(base, kGuard, guard | 2);
    Stfs(base, kLook + 0, Lfs(base, 0x8200295C));
    Stfs(base, kLook + 4, Lfs(base, 0x82008290));
    Stfs(base, kLook + 8, zero);
    Stfs(base, kLook + 12, one);
    c_runtime::AtexitFrom(base, 0x825DB0F8, atexit);
  }
  SetLens(base, camera, Zoom(base, kChaseDegrees, f + 96, f, 0x82154734, video_output), zero);
  ChasePlace(base, camera, f + 112, f + 80, kUp, kLook, f - 112);
  TargetTrack(base, tracker, c.vehicle(), f - 256);
  ChaseFollow(base, camera, Lfs(base, 0x82002CBC), Lfs(base, 0x82001FBC), f - 320, video_output);
  CopyWords(base, c.eye_address(), c.viewport().eye_address(), 4);
  return camera;
}

// --- The TV camera ---

uint32_t TvConstruct(uint8_t* base, uint32_t camera, uint32_t viewport, uint32_t vehicle,
                     uint32_t frame, const VideoOutputCall& video_output,
                     const c_runtime::AtexitCall& atexit) {
  ITC_NATIVE_RUNNING("sub_82155288 TvCamera::TvCamera");
  const schema::TvCamera c(base, camera);
  const schema::ChaseCamera fallback(base, c.fallback_address());
  const uint32_t f = frame;
  const double one = Lfs(base, kOne);
  c.set_vehicle(0);
  c.set_viewport(0);
  TrackerOnes(base, c.tracker_address(), one);
  c.set_vtable(0x820093BC);
  c.set_eye_w(float(one));
  c.set_target_w(float(one));
  c.set_unknown_416_w(float(one));
  // The four camera path buffers (+43016, Buffer28s): no path (-1), empty.
  for (uint32_t k = 0; k < 4; k++) {
    const schema::CameraPathBuffer buffer(base, c.buffers_address() + schema::CameraPathBuffer::kSize * k);
    buffer.set_index(0xFFFFFFFF);
    buffer.set_vtable(0x82009390);
    buffer.set_serial(0xFFFFFFFF);
    buffer.set_block(0);
    buffer.set_state(0);
  }
  // The fallback chase camera's fields its constructor doesn't set first.
  const uint32_t chase = c.fallback_address();
  fallback.set_vehicle(0);
  fallback.set_viewport(0);
  Stfs(base, fallback.tracker_address() + 12, one);
  fallback.set_vtable(0x82006B24);
  for (uint32_t at = 28; at <= 220; at += 16) {
    Stfs(base, fallback.tracker_address() + at, one);
  }
  for (uint32_t at : {252u, 268u, 284u, 300u}) {
    Stfs(base, fallback.tracker_address() + at, one);
  }
  fallback.set_unknown_364(float(one));
  c.set_vehicle(vehicle);
  c.set_viewport(viewport);
  // Zoom and near plane (no roll).
  const double zoom = Zoom(base, kChaseDegrees, f + 80, f, 0x821553E0, video_output);
  cameras::ViewportSetZoom(base, c.viewport().address, zoom);
  {
    const uint32_t v = c.viewport().address;
    const double ratio =
        Fdivs(double(schema::Viewport(base, v).near()), double(schema::Viewport(base, v).zoom()));
    cameras::SetNear(base, Fmuls(ratio, zoom));
  }
  TargetTrack(base, c.tracker_address(), vehicle, f - 256);
  c.set_paused(0);
  fallback.set_angle(2);  // the chase camera's +372 (its angle)
  if (chase != 0) {
    ChaseConstruct(base, chase, viewport, vehicle, f - 224, video_output, atexit);
  }
  // The route ranges (+456, 1,064 bytes apart, their count +452): each from
  // its camera path's record ([0x82618D30 + 4 x i]: +4 the start's section,
  // +8 the end's, its last slice), on the stack at f + 80; the forks to take
  // of some of them.
  static constexpr uint16_t k8[] = {247, 253, 258, 261, 269, 272, 277, 278, 285, 288, 280, 282};
  static constexpr uint16_t k9[] = {181, 188, 194, 195, 205, 207, 210, 213, 217, 220, 221, 225};
  static constexpr uint16_t k13[] = {103, 106};
  static constexpr uint16_t k14[] = {91};
  static constexpr uint16_t k16[] = {9, 11, 12, 15, 17, 19};
  static constexpr uint16_t k17[] = {26};
  static constexpr uint16_t k19[] = {2, 6};
  static constexpr uint16_t k20[] = {152};
  static constexpr uint16_t k21[] = {139, 142, 143, 146};
  static constexpr uint16_t k22[] = {113, 116, 118};
  static constexpr uint16_t k25[] = {31, 35, 37, 44, 42, 46};
  static constexpr uint16_t k28[] = {57, 69, 71, 73, 74};
  struct Branches {
    uint32_t range;
    const uint16_t* sections;
    uint32_t count;
  };
  static constexpr Branches kBranches[] = {
      {8, k8, 12},  {9, k9, 12},   {13, k13, 2}, {14, k14, 1}, {16, k16, 6}, {17, k17, 1},
      {19, k19, 2}, {20, k20, 1}, {21, k21, 4}, {22, k22, 3}, {25, k25, 6}, {28, k28, 5},
  };
  if (c.range_count() > 0) {
    uint32_t i = 0;
    do {
      const uint32_t record = Load(base, G::path_assets_address() + (i << 2));
      const uint32_t range = c.ranges_address() + i * 1064;
      guest::StoreU16(base, f + 82, 0);
      guest::StoreU16(base, f + 80, uint16_t(Load(base, record + 4)));
      guest::StoreU16(base, f + 84, uint16_t(Load(base, record + 8)));
      const uint32_t end = Load(base, record + 8);
      const uint32_t table = Load(base, G::route(base) + 12);
      const uint32_t section = Load(base, table + ((end << 2) & 0x3FFFC));
      guest::StoreU16(base, f + 86, uint16_t(guest::LoadU16(base, section) - 1));
      track_zones::RangeInit(base, range,
                             {guest::LoadU16(base, f + 80), guest::LoadU16(base, f + 82)},
                             {guest::LoadU16(base, f + 84), guest::LoadU16(base, f + 86)});
      for (const Branches& b : kBranches) {
        if (b.range == i) {
          for (uint32_t k = 0; k < b.count; k++) {
            track_zones::RangeAddBranch(base, range, b.sections[k]);
          }
        }
      }
      i++;
    } while (i < c.range_count());
  }
  c.set_filter_state(0);
  return camera;
}

void ChaseUpdate(uint8_t* base, uint32_t camera, uint32_t frame,
                 const VideoOutputCall& video_output) {
  ITC_NATIVE_RUNNING("sub_82154808 ChaseCamera::Update");
  const schema::ChaseCamera c(base, camera);
  constexpr uint32_t kSpeed = G::last_speed_address();  // the speed last seen
  constexpr uint32_t kPull = G::pull_back_address();    // the pull back
  const uint32_t f = frame;
  const double least = Lfs(base, 0x82001FBC);
  ChaseFollow(base, camera, Lfs(base, 0x82002CBC), least, f - 320, video_output);
  Shake(base, camera, f - 96);
  // The pull back follows the speed's change, clamped.
  const double speed = double(Vehicle_GetSpeedKmh(base, c.vehicle()));
  const double change = Fsubs(speed, Lfs(base, kSpeed));
  Stfs(base, kSpeed, speed);
  const double gain = Lfs(base, 0x820029F8);
  double pull;
  if (change < gain) {
    pull = Fadds(Lfs(base, kPull), Lfs(base, 0x8200256C));
  } else if (change > least) {
    pull = Fsubs(Lfs(base, kPull), Lfs(base, 0x8200256C));
  } else {
    pull = Lfs(base, kPull);
  }
  double step = Fmuls(pull, gain);
  const double low = Lfs(base, 0x82002C00);
  if (step < low) {
    step = low;
  } else {
    const double high = Lfs(base, 0x82002FB0);
    if (step > high) {
      step = high;
    }
  }
  double amount = Fadds(pull, step);
  Stfs(base, kPull, amount);
  {
    const double low_end = Lfs(base, 0x820093B4);
    if (amount < low_end) {
      amount = low_end;
      Stfs(base, kPull, amount);
    } else {
      const double high_end = Lfs(base, 0x82001070);
      if (amount > high_end) {
        amount = high_end;
        Stfs(base, kPull, amount);
      }
    }
  }
  const double zero = Lfs(base, kZero);
  const double one = Lfs(base, kOne);
  Stfs(base, f + 104, amount);
  Stfs(base, f + 96, zero);
  Stfs(base, f + 100, zero);
  Stfs(base, f + 108, one);
  ViewportPullBack(base, c.viewport().address, one, f + 96, f);
  // A state (+380): the eye moved once off the view's line, then cleared.
  if (c.state() != 0) {
    const uint32_t viewport = c.viewport().address;
    Stfs(base, f + 92, one);
    for (uint32_t i = 0; i < 3; i++) {
      Stfs(base, f + 80 + 4 * i,
           Fsubs(Lfs(base, schema::Viewport(base, viewport).eye_address() + 4 * i),
                 Lfs(base, schema::Viewport(base, viewport).target_address() + 4 * i)));
    }
    const double tx = double(schema::Viewport(base, viewport).target_x());
    const simde__m128 v = Lvx(base, f + 80);
    simde__m128 n;
    {
      FlushToZero ftz;
      n = Normalized(v, Dot3(v, v));
    }
    Stvx(base, f + 80, n);
    const double dx = Fneg(Lfs(base, f + 80));
    const double dy = Lfs(base, f + 84);
    const double side = Lfs(base, 0x820093B0);
    schema::Viewport(base, viewport).set_eye_x(float(Fadds(tx, Fmuls(dx, side))));
    const double up = Fmuls(dy, Lfs(base, 0x82002CAC));
    schema::Viewport(base, viewport)
        .set_eye_y(float(Fadds(double(schema::Viewport(base, viewport).target_y()), up)));
    const double dz = Fneg(Lfs(base, f + 88));
    const double tz = double(schema::Viewport(base, viewport).target_z());
    schema::Viewport(base, viewport).set_eye_z(float(Fadds(tz, Fmuls(dz, side))));
    c.set_state(0);
  }
  CopyWords(base, c.eye_address(), c.viewport().eye_address(), 4);
}

// --- Views 1 and 2 ---

uint32_t View1Construct(uint8_t* base, uint32_t camera, uint32_t viewport, uint32_t car,
                        uint32_t frame, const VideoOutputCall& video_output,
                        const c_runtime::AtexitCall& atexit) {
  ITC_NATIVE_RUNNING("sub_82154A48 ViewCamera1::ViewCamera1");
  const schema::ViewCamera1 c(base, camera);
  const schema::CameraTarget t(base, c.tracker_address());
  constexpr uint32_t kGuard = G::view1_guard_address();
  const uint32_t f = frame;
  const uint32_t tracker = c.tracker_address();
  c.set_vehicle(0);
  const double one = Lfs(base, kOne);
  c.set_viewport(0);
  TrackerOnes(base, tracker, one);
  uint32_t guard = Load(base, kGuard);
  c.set_vtable(0x82006B2C);
  const double depth = Lfs(base, 0x82002A00);
  c.set_rotation_w(float(one));
  guard = MakeStatic(base, kGuard, guard, 1, G::view1_statics_address() + 48,
                     {Lfs(base, 0x820093B8), one, depth, one}, 0x825DB120, true, atexit);
  guard = MakeStatic(base, kGuard, guard, 2, G::view1_statics_address() + 32,
                     {one, one, depth, one}, 0x825DB118, false, atexit);
  const double zero = Lfs(base, kZero);
  guard = MakeStatic(base, kGuard, guard, 4, G::view1_statics_address() + 16,
                     {zero, one, zero, one}, 0x825DB110, false, atexit);
  MakeStatic(base, kGuard, guard, 8, G::view1_statics_address(), {zero, zero, zero, one},
             0x825DB108, false, atexit);
  c.set_vehicle(car);
  c.set_viewport(viewport);
  SetLensNear(base, camera, Zoom(base, kChaseDegrees, f + 80, f, 0x82154BF4, video_output), zero,
              zero);
  CopyWords(base, c.viewport().base_eye_address(), G::view1_statics_address() + 48, 4);
  CopyWords(base, c.viewport().base_target_address(), G::view1_statics_address() + 32, 4);
  CopyWords(base, c.viewport().base_up_address(), G::view1_statics_address() + 16, 4);
  CopyWords(base, c.viewport().base_look_address(), G::view1_statics_address(), 4);
  TargetTrack(base, tracker, c.vehicle(), f - 256);
  vehicle::CameraCurveLoad(base, c.curve_address(), 0, 30);
  CopyWords(base, c.rotation_address(), t.rotation_address(), 4);
  CopyDoublewords(base, c.rotation_address(), t.rotation_address(), 2);
  c.set_look_angle(float(zero));
  return camera;
}

void View1Update(uint8_t* base, uint32_t camera, uint32_t frame,
                 const VideoOutputCall& video_output) {
  ITC_NATIVE_RUNNING("sub_82153A50 ViewCamera1::Update");
  const schema::ViewCamera1 c(base, camera);
  const schema::CameraTarget t(base, c.tracker_address());
  const uint32_t f = frame;
  const uint32_t tracker = c.tracker_address();
  const double one = Lfs(base, kOne);
  for (uint32_t at : {220u, 236u, 252u, 268u}) {
    Stfs(base, f + at, one);
  }
  TargetTrack(base, tracker, c.vehicle(), f - 256);
  const double zoom = Zoom(base, kChaseDegrees, f + 80, f, 0x82153AB8, video_output);
  const double zero = Lfs(base, kZero);
  SetLensNear(base, camera, zoom, Lfs(base, 0x82001FBC), zero);
  // The car's model: the second in some races (kind 6 with +0x59614 2 and
  // [0x826C7824] 0), else the first; its +48 picks the eye and target
  // offsets (32 bytes each at 0x82618DC0).
  uint32_t n = 0;
  const uint32_t race = G::race_data(base);
  if (int32_t(schema::RaceData(base, race).kind()) == 6 &&
      int32_t(schema::RaceData(base, race).added_count()) == 2) {
    n = G::two_player(base) == 0 ? 1 : 0;
  }
  const uint32_t model =
      vehicle_accessors::ScenePoolsFindModel(base, schema::CarDrawGlobals::scene(base).address, 2, n);
  const uint32_t entry = (schema::SceneModel(base, model).block() << 5) & 0xFFFFFFE0u;
  CopyWords(base, f + 80, G::view1_offsets_address() + entry, 4);
  CopyWords(base, f + 112, G::view1_offsets_address() + 16 + entry, 4);
  const uint32_t turn = t.turn_address();
  car_physics::MatrixMultiplyTranslation(base, f + 208, turn, tracker, f - 192);
  matrix_math::Vec4Transform(base, c.viewport().eye_address(), f + 208, f + 80);
  matrix_math::Vec4Transform(base, c.viewport().target_address(), f + 208, f + 112);
  uint32_t viewport = c.viewport().address;
  CopyWords(base, schema::Viewport(base, viewport).view1_target_address(),
            schema::Viewport(base, viewport).target_address(), 4);
  viewport = c.viewport().address;
  matrix_math::Vec4Transform(base, schema::Viewport(base, viewport).up_address(), turn,
                             schema::Viewport(base, viewport).base_up_address());
  // The look turned by +1104 (Matrix::RotateY of the identity at +144).
  Stfs(base, f + 96, zero);
  Stfs(base, f + 100, zero);
  Stfs(base, f + 104, zero);
  Stfs(base, f + 108, one);
  StoreIdentity(base, f + 144);
  viewport = c.viewport().address;
  Stfs(base, f + 96,
       Fsubs(double(schema::Viewport(base, viewport).target_x()),
             double(schema::Viewport(base, viewport).eye_x())));
  Stfs(base, f + 100,
       Fsubs(double(schema::Viewport(base, viewport).target_y()),
             double(schema::Viewport(base, viewport).eye_y())));
  Stfs(base, f + 104,
       Fsubs(double(schema::Viewport(base, viewport).target_z()),
             double(schema::Viewport(base, viewport).eye_z())));
  {
    const simde__m128 v = Lvx(base, f + 96);
    simde__m128 nv;
    {
      FlushToZero ftz;
      nv = Normalized(v, Dot3(v, v));
    }
    Stvx(base, f + 96, nv);
  }
  matrix_math::RotateY(base, f + 144, f + 144, c.look_angle_address(), f - 80);
  matrix_math::Vec4Transform(base, f + 96, f + 144, f + 96);
  viewport = c.viewport().address;
  for (uint32_t i = 0; i < 3; i++) {
    Stfs(base, schema::Viewport(base, viewport).target_address() + 4 * i,
         Fadds(Lfs(base, schema::Viewport(base, viewport).eye_address() + 4 * i),
               Lfs(base, f + 96 + 4 * i)));
  }
  NearestHuman(base, camera, f + 128, f);
  LookPoint(base, camera, f, one);
}

uint32_t View2Construct(uint8_t* base, uint32_t camera, uint32_t viewport, uint32_t car,
                        uint32_t frame, const VideoOutputCall& video_output,
                        const c_runtime::AtexitCall& atexit) {
  ITC_NATIVE_RUNNING("sub_82154D38 ViewCamera2::ViewCamera2");
  const schema::ViewCamera2 c(base, camera);
  const schema::CameraTarget t(base, c.tracker_address());
  constexpr uint32_t kGuard = G::view2_guard_address();
  const uint32_t f = frame;
  const uint32_t tracker = c.tracker_address();
  const double one = Lfs(base, kOne);
  TrackerOnes(base, tracker, one);
  uint32_t guard = Load(base, kGuard);
  c.set_vehicle(0);
  c.set_viewport(0);
  c.set_vtable(0x82006B34);
  const double height = Lfs(base, 0x82002CB0);
  c.set_rotation_w(float(one));
  const double zero = Lfs(base, kZero);
  guard = MakeStatic(base, kGuard, guard, 1, G::view2_statics_address() + 48,
                     {one, height, zero, one}, 0x825DB140, false, atexit);
  guard = MakeStatic(base, kGuard, guard, 2, G::view2_statics_address() + 32,
                     {Lfs(base, 0x82001FD8), height, zero, one}, 0x825DB138, true, atexit);
  guard = MakeStatic(base, kGuard, guard, 4, G::view2_statics_address() + 16,
                     {zero, one, zero, one}, 0x825DB130, false, atexit);
  MakeStatic(base, kGuard, guard, 8, G::view2_statics_address(),
             {Lfs(base, 0x820029C8), height, zero, one}, 0x825DB128, true, atexit);
  c.set_vehicle(car);
  c.set_viewport(viewport);
  SetLens(base, camera, Zoom(base, kChaseDegrees, f + 80, f, 0x82154EF0, video_output), zero);
  CopyWords(base, c.viewport().base_eye_address(), G::view2_statics_address() + 48, 4);
  CopyWords(base, c.viewport().base_target_address(), G::view2_statics_address() + 32, 4);
  CopyWords(base, c.viewport().base_up_address(), G::view2_statics_address() + 16, 4);
  CopyWords(base, c.viewport().base_look_address(), G::view2_statics_address(), 4);
  const uint32_t the_vehicle = c.vehicle();
  if (the_vehicle != 0) {
    // The eye and target from the car's size: its spec's +76 / 10 (x the
    // floats at 0x82001BE0 and 0x820029D8) and +76, +80 halved.
    const uint32_t spec = vehicle_accessors::ObjectRecord1888(schema::Vehicle(base, the_vehicle).car());
    const double scale = Lfs(base, 0x820029D8);
    const uint32_t tenth = schema::CarSpecCopies(base, spec).unknown_76() / 10;
    Store64(base, f + 80, tenth);
    double x = Fmuls(Single(double(int64_t(tenth))), Lfs(base, 0x82001BE0));
    Stfs(base, c.viewport().base_eye_address(), Fmuls(x, scale));
    // The spec's word, and the viewport's field it sets: base_eye_y,
    // base_target_x, base_target_y.
    const schema::Viewport at0(base, 0);
    const uint32_t fields[3][2] = {{80, at0.base_eye_y_address()},
                                   {76, at0.base_target_x_address()},
                                   {80, at0.base_target_y_address()}};
    for (const auto& field : fields) {
      const uint32_t half = Load(base, spec + field[0]) >> 1;
      const uint32_t at = c.viewport().address + field[1];
      Store64(base, f + 80, half);
      Stfs(base, at, Fmuls(Single(double(int64_t(half))), scale));
    }
  }
  vehicle::CameraCurveLoad(base, c.curve_address(), 0, 30);
  TargetTrack(base, tracker, c.vehicle(), f - 256);
  CopyWords(base, c.rotation_address(), t.rotation_address(), 4);
  CopyDoublewords(base, c.rotation_address(), t.rotation_address(), 2);
  return camera;
}

void View2Update(uint8_t* base, uint32_t camera, uint32_t frame,
                 const VideoOutputCall& video_output) {
  ITC_NATIVE_RUNNING("sub_82153E58 ViewCamera2::Update");
  const schema::ViewCamera2 c(base, camera);
  const schema::CameraTarget t(base, c.tracker_address());
  const uint32_t f = frame;
  const uint32_t tracker = c.tracker_address();
  const double one = Lfs(base, kOne);
  for (uint32_t at : {124u, 140u, 156u, 172u}) {
    Stfs(base, f + at, one);
  }
  StoreIdentity(base, f + 112);
  TargetTrack(base, tracker, c.vehicle(), f - 256);
  SetLens(base, camera, Zoom(base, kChaseDegrees, f + 80, f, 0x82153EF0, video_output),
          Lfs(base, kZero));
  const uint32_t turn = t.turn_address();
  car_physics::MatrixMultiplyTranslation(base, f + 112, turn, tracker, f - 192);
  uint32_t viewport = c.viewport().address;
  matrix_math::Vec4Transform(base, schema::Viewport(base, viewport).eye_address(), f + 112,
                             schema::Viewport(base, viewport).base_eye_address());
  viewport = c.viewport().address;
  matrix_math::Vec4Transform(base, schema::Viewport(base, viewport).target_address(), f + 112,
                             schema::Viewport(base, viewport).base_target_address());
  viewport = c.viewport().address;
  matrix_math::Vec4Transform(base, schema::Viewport(base, viewport).view1_target_address(), f + 112,
                             schema::Viewport(base, viewport).base_look_address());
  viewport = c.viewport().address;
  matrix_math::Vec4Transform(base, schema::Viewport(base, viewport).up_address(), turn,
                             schema::Viewport(base, viewport).base_up_address());
  NearestHuman(base, camera, f + 96, f);
  LookPoint(base, camera, f, one);
}

// --- The garage camera (the front end's) ---

namespace {

constexpr uint32_t kGarageRadians = 0x82002948;  // pi / 180
constexpr uint32_t kPassZero = schema::CamerasGlobals::passes_address();  // camera pass 0

// GarageUpdate's tan argument: the field of view (+224) in radians.
double GarageAngle(uint8_t* base, uint32_t camera) {
  return Fmuls(double(schema::GarageCamera(base, camera).fov()), Lfs(base, kGarageRadians));
}

}  // namespace

bool GarageReady(uint8_t* base, uint32_t camera) {
  double t;
  return (Fpscr::getcsr() & Fpscr::RoundMaskVal) == 0 &&
         c_runtime::Tan(GarageAngle(base, camera), &t);
}

void GarageUpdate(uint8_t* base, uint32_t camera, uint32_t frame, const GarageCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8217BE98 GarageCamera::Update");
  const uint32_t f = frame;
  const auto [queue, list] = calls.queue_and_list(f);
  schema::FramePassesGlobals::set_command_list(base, list);
  schema::FramePassesGlobals::set_render_queue(base, queue);
  CopyWords(base, schema::FramePassesGlobals::lod_bias_address(), kZero, 1);
  // The target (+32) at f + 96 and the eye's start (+16) at f + 80, read
  // before they're written (the viewport loaded between).
  const schema::GarageCamera cam(base, camera);
  const uint64_t target0 = Load64(base, cam.target_address()),
                 target1 = Load64(base, cam.target_address() + 8);
  // [kViewports]'s +0, the block a host object (E5 step 5).
  const uint32_t viewport = schema::CameraModesGlobals::viewports(base).first().address;
  const uint64_t eye0 = Load64(base, cam.eye_address()), eye1 = Load64(base, cam.eye_address() + 8);
  Store64(base, f + 96, target0);
  Store64(base, f + 104, target1);
  Store64(base, f + 80, eye0);
  Store64(base, f + 88, eye1);
  // The orbit: a = +100, b = +176, the distance +180.
  Stfs(base, f + 84, Fmuls(Single(c_runtime::Sin(double(cam.tilt()))),
                           double(cam.distance())));
  Stfs(base, f + 88, Fmuls(Single(c_runtime::Cos(double(cam.tilt()))),
                           double(cam.distance())));
  Stfs(base, f + 80, Fmuls(Single(c_runtime::Sin(double(cam.orbit()))), Lfs(base, f + 88)));
  const double cos_b = c_runtime::Cos(double(cam.orbit()));
  const double angle = GarageAngle(base, camera);
  Stfs(base, f + 88, Fmuls(Single(cos_b), Lfs(base, f + 88)));
  // tan as its adapter gives it with nearest rounding set (GarageReady
  // checked it stands in).
  double tangent = 0.0;
  c_runtime::Tan(angle, &tangent);
  const double zoom = Fdivs(Lfs(base, 0x82009EB8), Single(tangent));
  cam.set_zoom(float(zoom));
  cameras::ViewportSetZoom(base, viewport, zoom);
  cameras::SetNear(base, Lfs(base, 0x8200222C));
  cameras::ViewportSetRoll(base, viewport, double(cam.angle_z()));
  // The identity at f + 112, copied to f + 176 (ld/std), its translation
  // row (f + 224 to + 232) = +256 to +264.
  StoreIdentity(base, f + 112);
  CopyDoublewords(base, f + 176, f + 112, 8);
  CopyWords(base, f + 224, cam.translation_address(), 3);
  matrix_math::Vec4Transform(base, f + 80, f + 176, f + 80);
  matrix_math::Vec4Transform(base, f + 96, f + 176, f + 96);
  cameras::ViewportLookAt(base, viewport, f + 80, f + 96);
  cameras::FromViewport(base, kPassZero, 0, 0);
  calls.pass_update(kPassZero, f);
  cameras::Enable(base, kPassZero);
  CopyDoublewords(base, schema::CarSceneDrawGlobals::view_eye_address(), f + 80, 2);
}

}  // namespace camera_modes
