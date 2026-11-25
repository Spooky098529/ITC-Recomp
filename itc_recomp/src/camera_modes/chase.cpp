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

uint32_t Load(uint32_t a) { return guest::LoadU32(a); }
uint64_t Load64(uint32_t a) {
  return (uint64_t(guest::LoadU32(a)) << 32) | guest::LoadU32(a + 4);
}
void Store(uint32_t a, uint32_t v) { guest::StoreU32(a, v); }
void Store64(uint32_t a, uint64_t v) {
  guest::StoreU32(a, uint32_t(v >> 32));
  guest::StoreU32(a + 4, uint32_t(v));
}
// lfs then stfs: the bits as the translated code keeps them.
void CopyWords(uint32_t to, uint32_t from, uint32_t words) {
  for (uint32_t i = 0; i < words; i++) {
    Store(to + 4 * i, Load(from + 4 * i));
  }
}
// ld/std copies of `doublewords`.
void CopyDoublewords(uint32_t to, uint32_t from, uint32_t doublewords) {
  for (uint32_t i = 0; i < doublewords; i++) {
    Store64(to + 8 * i, Load64(from + 8 * i));
  }
}

// --- The vector unit, as the translated code does it (word 0 in the top lane) ---

simde__m128 Lvx(uint32_t address) {
  const uint32_t at = address & ~0xFu;
  return VectorOf(Load(at), Load(at + 4), Load(at + 8), Load(at + 12));
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
void NormalizeQuat(uint32_t at) {
  const simde__m128 q = Lvx(at);
  simde__m128 n;
  {
    FlushToZero ftz;
    n = Normalized(q, Dot4(q, q));
  }
  Stvx(at, n);
}

// The identity matrix as the originals store it (vupkd3d128 of 0 and four
// vpermwi128s): rows (1, 0, 0, 0) to (0, 0, 0, 1).
void StoreIdentity(uint32_t at) {
  constexpr uint32_t kOneBits = 0x3F800000;
  for (uint32_t row = 0; row < 4; row++) {
    for (uint32_t i = 0; i < 4; i++) {
      Store((at & ~0xFu) + 16 * row + 4 * i, row == i ? kOneBits : 0);
    }
  }
}

// The zoom from the float at `degrees` at the display's width (the video
// output's +296, an integer, kept as a doubleword at `slot` as the
// originals' conversion does): Camera::ZoomFromFov (ChaseReady checked that
// its tan stands in).
double Zoom(uint32_t degrees, uint32_t slot, uint32_t frame, uint32_t back,
            const VideoOutputCall& video_output) {
  const uint32_t video = video_output(frame, back);
  const double fov = Lfs(degrees);
  const int32_t width = int32_t(schema::VideoOutput(video).display_width());
  Store64(slot, uint64_t(int64_t(width)));
  return ZoomFromFov(fov, Single(double(int64_t(width)))).value();
}

// The zoom, near plane and roll every view's camera sets: SetZoom(zoom),
// SetNear(+1868 / +1864 x zoom), SetRoll(roll).
void SetLens(uint32_t camera, double zoom, double roll) {
  const schema::CameraObject c(camera);
  cameras::ViewportSetZoom(c.viewport().address, zoom);
  const uint32_t viewport = c.viewport().address;
  const double ratio = Fdivs(double(schema::Viewport(viewport).near()),
                             double(schema::Viewport(viewport).zoom()));
  cameras::SetNear(Fmuls(ratio, zoom));
  cameras::ViewportSetRoll(c.viewport().address, roll);
}
// The same with a given near plane.
void SetLensNear(uint32_t camera, double zoom, double near, double roll) {
  const schema::CameraObject c(camera);
  cameras::ViewportSetZoom(c.viewport().address, zoom);
  cameras::SetNear(near);
  cameras::ViewportSetRoll(c.viewport().address, roll);
}

// The tracker's w words (1.0), as every view camera's constructor sets them.
void TrackerOnes(uint32_t tracker, double one) {
  for (uint32_t at = 12; at <= 220; at += 16) {
    Stfs(tracker + at, one);
  }
  for (uint32_t at : {252u, 268u, 284u, 300u}) {
    Stfs(tracker + at, one);
  }
}

// A static vector made once: (x, y, z, w) at `at` with guard bit `bit` of
// `guard` set, then atexit(`function`). `guard_first`: the guard stored
// before the floats (as some originals do). Returns the guard as reloaded.
uint32_t MakeStatic(uint32_t guard_address, uint32_t guard, uint32_t bit,
                    uint32_t at, const double (&v)[4], uint32_t function, bool guard_first,
                    const c_runtime::AtexitCall& atexit) {
  if ((guard & bit) != 0) {
    return guard;
  }
  guard |= bit;
  if (guard_first) {
    Store(guard_address, guard);
  }
  for (uint32_t i = 0; i < 4; i++) {
    Stfs(at + 4 * i, v[i]);
  }
  if (!guard_first) {
    Store(guard_address, guard);
  }
  c_runtime::AtexitFrom(function, atexit);
  return Load(guard_address);
}

// The look point every in-car view sets: +1648 = the eye + the view's unit
// direction (target less eye, normalized on the vector unit at `f` + 80),
// +1660 = 1.0.
void LookPoint(uint32_t camera, uint32_t f, double one) {
  const schema::CameraObject c(camera);
  uint32_t viewport = c.viewport().address;
  Stfs(f + 92, one);
  const double ex = double(schema::Viewport(viewport).eye_x());
  Stfs(f + 80, Fsubs(double(schema::Viewport(viewport).target_x()), ex));
  const double ty = double(schema::Viewport(viewport).target_y()),
               tz = double(schema::Viewport(viewport).target_z());
  const double ey = double(schema::Viewport(viewport).eye_y()),
               ez = double(schema::Viewport(viewport).eye_z());
  const double dy = Fsubs(ty, ey);
  Stfs(f + 88, Fsubs(tz, ez));
  Stfs(f + 84, dy);
  const simde__m128 v = Lvx(f + 80);
  simde__m128 n;
  {
    FlushToZero ftz;
    n = Normalized(v, Dot3(v, v));
  }
  Stvx(f + 80, n);
  schema::Viewport(viewport).set_look_x(float(Fadds(ex, Lfs(f + 80))));
  schema::Viewport(viewport)
      .set_look_y(
          float(Fadds(double(schema::Viewport(viewport).eye_y()), Lfs(f + 84))));
  schema::Viewport(viewport)
      .set_look_z(
          float(Fadds(double(schema::Viewport(viewport).eye_z()), Lfs(f + 88))));
  viewport = c.viewport().address;
  schema::Viewport(viewport).set_look_w(float(one));
}

// The nearest human car's body (Racers::NearestHumanVehicle(the tracker's
// position at `point`, 1), its frame below `f`): the originals then take
// its matrix or position and leave it unused.
void NearestHuman(uint32_t camera, uint32_t point, uint32_t f) {
  const schema::CameraObject c(camera);
  CopyWords(point, c.tracker_address(), 4);
  vehicle::RacersNearestHumanVehicle(point, 1, f - 304);
}

constexpr uint32_t kChaseDegrees = G::chase_degrees_address();    // the constructor's (55.0)
constexpr uint32_t kFollowDegrees = G::follow_degrees_address();  // Follow's (55.0)

}  // namespace

bool ChaseReady() {
  return ZoomFromFov(Lfs(kChaseDegrees), 1.0).has_value() &&
         ZoomFromFov(Lfs(kFollowDegrees), 1.0).has_value();
}

// --- The chase camera ---

void ChasePlace(uint32_t camera, uint32_t eye, uint32_t target, uint32_t up,
                uint32_t look, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82153478 ChaseCamera::Place");
  const schema::ChaseCamera c(camera);
  const schema::CameraTarget t(c.tracker_address());
  const double zero = Lfs(kZero);
  uint32_t viewport = c.viewport().address;
  CopyWords(schema::Viewport(viewport).base_eye_address(), eye, 4);
  viewport = c.viewport().address;
  CopyWords(schema::Viewport(viewport).base_target_address(), target, 4);
  viewport = c.viewport().address;
  CopyWords(schema::Viewport(viewport).base_up_address(), up, 4);
  viewport = c.viewport().address;
  CopyWords(schema::Viewport(viewport).base_look_address(), look, 4);
  viewport = c.viewport().address;
  CopyWords(schema::Viewport(viewport).view1_target_address(),
            schema::Viewport(viewport).base_look_address(), 4);
  const uint32_t vehicle = c.vehicle();
  c.set_unknown_368(float(zero));
  if (vehicle == 0) {
    return;
  }
  TargetTrack(c.tracker_address(), vehicle, frame - 256);
  // The tracker's rotation (+224) normalized, kept at +240.
  const uint32_t rotation = t.rotation_address();
  NormalizeQuat(rotation);
  const double one = Lfs(kOne);
  CopyWords(t.last_rotation_address(), rotation, 4);
  CopyDoublewords(t.last_rotation_address(), rotation, 2);
  for (uint32_t i = 0; i < 3; i++) {
    Stfs(c.viewport().base_target_address() + 4 * i, zero);
  }
  Stfs(c.viewport().base_target_w_address(), one);
  viewport = c.viewport().address;
  schema::Viewport(viewport)
      .set_base_target_y(float(
          Fadds(double(schema::Viewport(viewport).base_target_y()), Lfs(0x820029F0))));
  // The track position's section and slice (+260, +262) into +376.
  const uint16_t section = guest::LoadU16(t.track_position_address() + 4);
  const uint16_t slice = guest::LoadU16(t.track_position_address() + 6);
  c.set_section(section);
  c.set_slice(slice);
}

void ChaseFollow(uint32_t camera, double rate, double least, uint32_t frame,
                 const VideoOutputCall& video_output) {
  ITC_NATIVE_RUNNING("sub_82153640 ChaseCamera::Follow");
  const schema::ChaseCamera c(camera);
  const schema::CameraTarget t(c.tracker_address());
  const uint32_t f = frame;
  const double one = Lfs(kOne);
  const double zero = Lfs(kZero);
  // (0, 0, 0, 1) at +96, +112, +128 and +144; the identity at +160.
  for (uint32_t at : {96u, 112u, 144u, 128u}) {
    Stfs(f + at + 0, zero);
    Stfs(f + at + 4, zero);
    Stfs(f + at + 8, zero);
    Stfs(f + at + 12, one);
  }
  StoreIdentity(f + 160);
  const uint32_t vehicle = c.vehicle();
  if (vehicle == 0) {
    return;
  }
  const uint32_t tracker = c.tracker_address();
  TargetTrack(tracker, vehicle, f - 256);
  SetLens(camera, Zoom(kFollowDegrees, f + 80, f, 0x82153730, video_output), zero);
  // Forward (1, 0, 0, 1) through the turn now (+160) and before (+96): on
  // which side the turn has gone.
  Stfs(f + 112, one);
  Stfs(f + 96, one);
  Stfs(f + 116, zero);
  Stfs(f + 100, zero);
  Stfs(f + 120, zero);
  Stfs(f + 104, zero);
  Stfs(f + 124, one);
  Stfs(f + 108, one);
  matrix_math::Vec4Transform(f + 96, t.last_turn_address(), f + 96);
  const uint32_t turn = t.turn_address();
  matrix_math::Vec4Transform(f + 112, turn, f + 112);
  {
    const simde__m128 now = Lvx(f + 112);
    const simde__m128 before = Lvx(f + 96);
    simde__m128 dot;
    {
      FlushToZero ftz;
      dot = Dot3(before, now);
    }
    Stvx(f + 80, dot);
  }
  // The rate: 1.0 when (d < 0) d^4 is 0 (in single steps), else the float
  // at 0x82001BE0; times `rate`, at least `least`.
  const double d = Lfs(f + 80);
  const int64_t behind = d < zero ? 1 : 0;
  Store64(f + 80, uint64_t(behind));
  double p = Single(double(behind));
  p = Fmuls(p, d);
  p = Fmuls(p, d);
  p = Fmuls(p, d);
  p = Fmuls(p, d);
  double w = p == zero ? one : Lfs(0x82001BE0);
  w = Fmuls(w, rate);
  if (w < least) {
    w = least;
  }
  const double keep = Fsubs(one, w);
  // The rotation (+224) blended toward the last (+240) by `keep`.
  const uint32_t rotation = t.rotation_address();
  const double l0 = double(t.last_rotation_x()), l1 = double(t.last_rotation_y());
  const double l2 = double(t.last_rotation_z()), l3 = double(t.last_rotation_w());
  const double r1 = Fmuls(Lfs(rotation + 4), w);
  const double r2 = Fmuls(Lfs(rotation + 8), w);
  const double r3 = Lfs(rotation + 12);
  const double k0 = Fmuls(l0, keep), k1 = Fmuls(l1, keep), k2 = Fmuls(l2, keep);
  const double k3 = Fmuls(l3, keep);
  const double r0 = Fmuls(Lfs(rotation + 0), w);
  const double b1 = Fadds(r1, k1);
  Stfs(f + 84, b1);
  const double b2 = Fadds(r2, k2);
  Stfs(f + 88, b2);
  const double b0 = Fadds(r0, k0);
  Stfs(f + 80, b0);
  const double b3 = Fadds(Fmuls(r3, w), k3);
  Stfs(f + 92, b3);
  CopyDoublewords(rotation, f + 80, 2);
  NormalizeQuat(rotation);
  // The eye's offset from the target (+1680 less +1664) turned by it.
  uint32_t viewport = c.viewport().address;
  for (uint32_t i = 0; i < 3; i++) {
    Stfs(f + 128 + 4 * i,
         Fsubs(Lfs(schema::Viewport(viewport).base_eye_address() + 4 * i),
               Lfs(schema::Viewport(viewport).view1_target_address() + 4 * i)));
  }
  QuatRotate(f + 80, rotation, f + 128);
  CopyDoublewords(f + 128, f + 80, 2);
  // The eye: the target point (+1664) through the turn about the tracked
  // point (+64), plus the offset; the target through it from +1696.
  const uint32_t point = t.point_address();
  car_physics::MatrixMultiplyTranslation(f + 160, turn, point, f - 192);
  matrix_math::Vec4Transform(f + 144, f + 160, c.viewport().view1_target_address());
  viewport = c.viewport().address;
  for (uint32_t i = 0; i < 3; i++) {
    Stfs(schema::Viewport(viewport).eye_address() + 4 * i,
         Fadds(Lfs(f + 128 + 4 * i), Lfs(f + 144 + 4 * i)));
  }
  Stfs(c.viewport().eye_w_address(), one);
  car_physics::MatrixMultiplyTranslation(f + 160, turn, point, f - 192);
  viewport = c.viewport().address;
  matrix_math::Vec4Transform(schema::Viewport(viewport).target_address(), f + 160,
                             schema::Viewport(viewport).base_target_address());
  viewport = c.viewport().address;
  CopyWords(schema::Viewport(viewport).up_address(),
            schema::Viewport(viewport).base_up_address(), 4);
  viewport = c.viewport().address;
  CopyWords(schema::Viewport(viewport).look_address(), tracker, 4);
}

uint32_t ChaseConstruct(uint32_t camera, uint32_t viewport, uint32_t vehicle,
                        uint32_t frame, const VideoOutputCall& video_output,
                        const c_runtime::AtexitCall& atexit) {
  ITC_NATIVE_RUNNING("sub_82154548 ChaseCamera::ChaseCamera");
  const schema::ChaseCamera c(camera);
  constexpr uint32_t kGuard = G::chase_guard_address();
  constexpr uint32_t kUp = G::chase_up_address();      // (0, 1, 0, 1)
  constexpr uint32_t kLook = G::chase_look_address();  // (0x8200295C's, 0x82008290's, 0, 1)
  const uint32_t f = frame;
  const uint32_t tracker = c.tracker_address();
  const double one = Lfs(kOne);
  for (uint32_t at = 12; at <= 220; at += 16) {
    Stfs(tracker + at, one);
  }
  for (uint32_t at : {252u, 268u, 284u, 300u}) {
    Stfs(tracker + at, one);
  }
  c.set_vtable(0x82006B24);
  c.set_unknown_364(float(one));
  c.set_vehicle(vehicle);
  c.set_viewport(viewport);
  // The offsets from the car's spec block: its +76 and +80 halved, an angle
  // (the table at 0x8200930C by the word at +372, in degrees).
  const uint32_t spec = vehicle_accessors::ObjectRecord1888(schema::Vehicle(vehicle).car());
  const double scale = Lfs(0x820029D8);
  const uint32_t index = c.angle();
  const uint32_t r80 = schema::CarSpecCopies(spec).unknown_80();
  const double degrees = Lfs((index << 2) + 0x8200930C);
  Store64(f + 96, r80);
  const uint32_t r76 = schema::CarSpecCopies(spec).unknown_76();
  const double angle = Fmuls(degrees, Lfs(0x82002948));
  Store64(f + 80, r76);
  const double half = Lfs(0x82001BE4);
  const double wid = Fmuls(Single(double(int64_t(r76))), half);
  const double len = Fmuls(Single(double(int64_t(r80))), half);
  const double reach = Fmadds(wid, scale, Lfs(0x82002C40));
  const double height = Fmuls(len, scale);
  const double sin = Single(c_runtime::Sin(angle));
  const double cos = c_runtime::Cos(angle);
  Stfs(f + 116, Fmadds(sin, reach, height));
  Stfs(f + 124, one);
  Stfs(f + 84, height);
  Stfs(f + 92, one);
  uint32_t guard = Load(kGuard);
  const double x = Fneg(Fmuls(Single(cos), reach));
  const double zero = Lfs(kZero);
  Stfs(f + 120, zero);
  Stfs(f + 80, zero);
  Stfs(f + 88, zero);
  Stfs(f + 112, x);
  if ((guard & 1) == 0) {
    guard |= 1;
    Stfs(kUp + 0, zero);
    Stfs(kUp + 4, one);
    Stfs(kUp + 8, zero);
    Stfs(kUp + 12, one);
    Store(kGuard, guard);
    c_runtime::AtexitFrom(0x825DB100, atexit);
    guard = Load(kGuard);
  }
  if ((guard & 2) == 0) {
    Store(kGuard, guard | 2);
    Stfs(kLook + 0, Lfs(0x8200295C));
    Stfs(kLook + 4, Lfs(0x82008290));
    Stfs(kLook + 8, zero);
    Stfs(kLook + 12, one);
    c_runtime::AtexitFrom(0x825DB0F8, atexit);
  }
  SetLens(camera, Zoom(kChaseDegrees, f + 96, f, 0x82154734, video_output), zero);
  ChasePlace(camera, f + 112, f + 80, kUp, kLook, f - 112);
  TargetTrack(tracker, c.vehicle(), f - 256);
  ChaseFollow(camera, Lfs(0x82002CBC), Lfs(0x82001FBC), f - 320, video_output);
  CopyWords(c.eye_address(), c.viewport().eye_address(), 4);
  return camera;
}

// --- The TV camera ---

uint32_t TvConstruct(uint32_t camera, uint32_t viewport, uint32_t vehicle,
                     uint32_t frame, const VideoOutputCall& video_output,
                     const c_runtime::AtexitCall& atexit) {
  ITC_NATIVE_RUNNING("sub_82155288 TvCamera::TvCamera");
  const schema::TvCamera c(camera);
  const schema::ChaseCamera fallback(c.fallback_address());
  const uint32_t f = frame;
  const double one = Lfs(kOne);
  c.set_vehicle(0);
  c.set_viewport(0);
  TrackerOnes(c.tracker_address(), one);
  c.set_vtable(0x820093BC);
  c.set_eye_w(float(one));
  c.set_target_w(float(one));
  c.set_unknown_416_w(float(one));
  // The four camera path buffers (+43016, Buffer28s): no path (-1), empty.
  for (uint32_t k = 0; k < 4; k++) {
    const schema::CameraPathBuffer buffer(c.buffers_address() + schema::CameraPathBuffer::kSize * k);
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
  Stfs(fallback.tracker_address() + 12, one);
  fallback.set_vtable(0x82006B24);
  for (uint32_t at = 28; at <= 220; at += 16) {
    Stfs(fallback.tracker_address() + at, one);
  }
  for (uint32_t at : {252u, 268u, 284u, 300u}) {
    Stfs(fallback.tracker_address() + at, one);
  }
  fallback.set_unknown_364(float(one));
  c.set_vehicle(vehicle);
  c.set_viewport(viewport);
  // Zoom and near plane (no roll).
  const double zoom = Zoom(kChaseDegrees, f + 80, f, 0x821553E0, video_output);
  cameras::ViewportSetZoom(c.viewport().address, zoom);
  {
    const uint32_t v = c.viewport().address;
    const double ratio =
        Fdivs(double(schema::Viewport(v).near()), double(schema::Viewport(v).zoom()));
    cameras::SetNear(Fmuls(ratio, zoom));
  }
  TargetTrack(c.tracker_address(), vehicle, f - 256);
  c.set_paused(0);
  fallback.set_angle(2);  // the chase camera's +372 (its angle)
  if (chase != 0) {
    ChaseConstruct(chase, viewport, vehicle, f - 224, video_output, atexit);
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
      const uint32_t record = Load(G::path_assets_address() + (i << 2));
      const uint32_t range = c.ranges_address() + i * 1064;
      guest::StoreU16(f + 82, 0);
      guest::StoreU16(f + 80, uint16_t(Load(record + 4)));
      guest::StoreU16(f + 84, uint16_t(Load(record + 8)));
      const uint32_t end = Load(record + 8);
      const uint32_t table = Load(G::route() + 12);
      const uint32_t section = Load(table + ((end << 2) & 0x3FFFC));
      guest::StoreU16(f + 86, uint16_t(guest::LoadU16(section) - 1));
      track_zones::RangeInit(range,
                             {guest::LoadU16(f + 80), guest::LoadU16(f + 82)},
                             {guest::LoadU16(f + 84), guest::LoadU16(f + 86)});
      for (const Branches& b : kBranches) {
        if (b.range == i) {
          for (uint32_t k = 0; k < b.count; k++) {
            track_zones::RangeAddBranch(range, b.sections[k]);
          }
        }
      }
      i++;
    } while (i < c.range_count());
  }
  c.set_filter_state(0);
  return camera;
}

void ChaseUpdate(uint32_t camera, uint32_t frame,
                 const VideoOutputCall& video_output) {
  ITC_NATIVE_RUNNING("sub_82154808 ChaseCamera::Update");
  const schema::ChaseCamera c(camera);
  constexpr uint32_t kSpeed = G::last_speed_address();  // the speed last seen
  constexpr uint32_t kPull = G::pull_back_address();    // the pull back
  const uint32_t f = frame;
  const double least = Lfs(0x82001FBC);
  ChaseFollow(camera, Lfs(0x82002CBC), least, f - 320, video_output);
  Shake(camera, f - 96);
  // The pull back follows the speed's change, clamped.
  const double speed = double(Vehicle_GetSpeedKmh(c.vehicle()));
  const double change = Fsubs(speed, Lfs(kSpeed));
  Stfs(kSpeed, speed);
  const double gain = Lfs(0x820029F8);
  double pull;
  if (change < gain) {
    pull = Fadds(Lfs(kPull), Lfs(0x8200256C));
  } else if (change > least) {
    pull = Fsubs(Lfs(kPull), Lfs(0x8200256C));
  } else {
    pull = Lfs(kPull);
  }
  double step = Fmuls(pull, gain);
  const double low = Lfs(0x82002C00);
  if (step < low) {
    step = low;
  } else {
    const double high = Lfs(0x82002FB0);
    if (step > high) {
      step = high;
    }
  }
  double amount = Fadds(pull, step);
  Stfs(kPull, amount);
  {
    const double low_end = Lfs(0x820093B4);
    if (amount < low_end) {
      amount = low_end;
      Stfs(kPull, amount);
    } else {
      const double high_end = Lfs(0x82001070);
      if (amount > high_end) {
        amount = high_end;
        Stfs(kPull, amount);
      }
    }
  }
  const double zero = Lfs(kZero);
  const double one = Lfs(kOne);
  Stfs(f + 104, amount);
  Stfs(f + 96, zero);
  Stfs(f + 100, zero);
  Stfs(f + 108, one);
  ViewportPullBack(c.viewport().address, one, f + 96, f);
  // A state (+380): the eye moved once off the view's line, then cleared.
  if (c.state() != 0) {
    const uint32_t viewport = c.viewport().address;
    Stfs(f + 92, one);
    for (uint32_t i = 0; i < 3; i++) {
      Stfs(f + 80 + 4 * i,
           Fsubs(Lfs(schema::Viewport(viewport).eye_address() + 4 * i),
                 Lfs(schema::Viewport(viewport).target_address() + 4 * i)));
    }
    const double tx = double(schema::Viewport(viewport).target_x());
    const simde__m128 v = Lvx(f + 80);
    simde__m128 n;
    {
      FlushToZero ftz;
      n = Normalized(v, Dot3(v, v));
    }
    Stvx(f + 80, n);
    const double dx = Fneg(Lfs(f + 80));
    const double dy = Lfs(f + 84);
    const double side = Lfs(0x820093B0);
    schema::Viewport(viewport).set_eye_x(float(Fadds(tx, Fmuls(dx, side))));
    const double up = Fmuls(dy, Lfs(0x82002CAC));
    schema::Viewport(viewport)
        .set_eye_y(float(Fadds(double(schema::Viewport(viewport).target_y()), up)));
    const double dz = Fneg(Lfs(f + 88));
    const double tz = double(schema::Viewport(viewport).target_z());
    schema::Viewport(viewport).set_eye_z(float(Fadds(tz, Fmuls(dz, side))));
    c.set_state(0);
  }
  CopyWords(c.eye_address(), c.viewport().eye_address(), 4);
}

// --- Views 1 and 2 ---

uint32_t View1Construct(uint32_t camera, uint32_t viewport, uint32_t car,
                        uint32_t frame, const VideoOutputCall& video_output,
                        const c_runtime::AtexitCall& atexit) {
  ITC_NATIVE_RUNNING("sub_82154A48 ViewCamera1::ViewCamera1");
  const schema::ViewCamera1 c(camera);
  const schema::CameraTarget t(c.tracker_address());
  constexpr uint32_t kGuard = G::view1_guard_address();
  const uint32_t f = frame;
  const uint32_t tracker = c.tracker_address();
  c.set_vehicle(0);
  const double one = Lfs(kOne);
  c.set_viewport(0);
  TrackerOnes(tracker, one);
  uint32_t guard = Load(kGuard);
  c.set_vtable(0x82006B2C);
  const double depth = Lfs(0x82002A00);
  c.set_rotation_w(float(one));
  guard = MakeStatic(kGuard, guard, 1, G::view1_statics_address() + 48,
                     {Lfs(0x820093B8), one, depth, one}, 0x825DB120, true, atexit);
  guard = MakeStatic(kGuard, guard, 2, G::view1_statics_address() + 32,
                     {one, one, depth, one}, 0x825DB118, false, atexit);
  const double zero = Lfs(kZero);
  guard = MakeStatic(kGuard, guard, 4, G::view1_statics_address() + 16,
                     {zero, one, zero, one}, 0x825DB110, false, atexit);
  MakeStatic(kGuard, guard, 8, G::view1_statics_address(), {zero, zero, zero, one},
             0x825DB108, false, atexit);
  c.set_vehicle(car);
  c.set_viewport(viewport);
  SetLensNear(camera, Zoom(kChaseDegrees, f + 80, f, 0x82154BF4, video_output), zero,
              zero);
  CopyWords(c.viewport().base_eye_address(), G::view1_statics_address() + 48, 4);
  CopyWords(c.viewport().base_target_address(), G::view1_statics_address() + 32, 4);
  CopyWords(c.viewport().base_up_address(), G::view1_statics_address() + 16, 4);
  CopyWords(c.viewport().base_look_address(), G::view1_statics_address(), 4);
  TargetTrack(tracker, c.vehicle(), f - 256);
  vehicle::CameraCurveLoad(c.curve_address(), 0, 30);
  CopyWords(c.rotation_address(), t.rotation_address(), 4);
  CopyDoublewords(c.rotation_address(), t.rotation_address(), 2);
  c.set_look_angle(float(zero));
  return camera;
}

void View1Update(uint32_t camera, uint32_t frame,
                 const VideoOutputCall& video_output) {
  ITC_NATIVE_RUNNING("sub_82153A50 ViewCamera1::Update");
  const schema::ViewCamera1 c(camera);
  const schema::CameraTarget t(c.tracker_address());
  const uint32_t f = frame;
  const uint32_t tracker = c.tracker_address();
  const double one = Lfs(kOne);
  for (uint32_t at : {220u, 236u, 252u, 268u}) {
    Stfs(f + at, one);
  }
  TargetTrack(tracker, c.vehicle(), f - 256);
  const double zoom = Zoom(kChaseDegrees, f + 80, f, 0x82153AB8, video_output);
  const double zero = Lfs(kZero);
  SetLensNear(camera, zoom, Lfs(0x82001FBC), zero);
  // The car's model: the second in some races (kind 6 with +0x59614 2 and
  // [0x826C7824] 0), else the first; its +48 picks the eye and target
  // offsets (32 bytes each at 0x82618DC0).
  uint32_t n = 0;
  const uint32_t race = G::race_data();
  if (int32_t(schema::RaceData(race).kind()) == 6 &&
      int32_t(schema::RaceData(race).added_count()) == 2) {
    n = G::two_player() == 0 ? 1 : 0;
  }
  const uint32_t model =
      vehicle_accessors::ScenePoolsFindModel(schema::CarDrawGlobals::scene().address, 2, n);
  const uint32_t entry = (schema::SceneModel(model).block() << 5) & 0xFFFFFFE0u;
  CopyWords(f + 80, G::view1_offsets_address() + entry, 4);
  CopyWords(f + 112, G::view1_offsets_address() + 16 + entry, 4);
  const uint32_t turn = t.turn_address();
  car_physics::MatrixMultiplyTranslation(f + 208, turn, tracker, f - 192);
  matrix_math::Vec4Transform(c.viewport().eye_address(), f + 208, f + 80);
  matrix_math::Vec4Transform(c.viewport().target_address(), f + 208, f + 112);
  uint32_t viewport = c.viewport().address;
  CopyWords(schema::Viewport(viewport).view1_target_address(),
            schema::Viewport(viewport).target_address(), 4);
  viewport = c.viewport().address;
  matrix_math::Vec4Transform(schema::Viewport(viewport).up_address(), turn,
                             schema::Viewport(viewport).base_up_address());
  // The look turned by +1104 (Matrix::RotateY of the identity at +144).
  Stfs(f + 96, zero);
  Stfs(f + 100, zero);
  Stfs(f + 104, zero);
  Stfs(f + 108, one);
  StoreIdentity(f + 144);
  viewport = c.viewport().address;
  Stfs(f + 96,
       Fsubs(double(schema::Viewport(viewport).target_x()),
             double(schema::Viewport(viewport).eye_x())));
  Stfs(f + 100,
       Fsubs(double(schema::Viewport(viewport).target_y()),
             double(schema::Viewport(viewport).eye_y())));
  Stfs(f + 104,
       Fsubs(double(schema::Viewport(viewport).target_z()),
             double(schema::Viewport(viewport).eye_z())));
  {
    const simde__m128 v = Lvx(f + 96);
    simde__m128 nv;
    {
      FlushToZero ftz;
      nv = Normalized(v, Dot3(v, v));
    }
    Stvx(f + 96, nv);
  }
  matrix_math::RotateY(f + 144, f + 144, c.look_angle_address(), f - 80);
  matrix_math::Vec4Transform(f + 96, f + 144, f + 96);
  viewport = c.viewport().address;
  for (uint32_t i = 0; i < 3; i++) {
    Stfs(schema::Viewport(viewport).target_address() + 4 * i,
         Fadds(Lfs(schema::Viewport(viewport).eye_address() + 4 * i),
               Lfs(f + 96 + 4 * i)));
  }
  NearestHuman(camera, f + 128, f);
  LookPoint(camera, f, one);
}

uint32_t View2Construct(uint32_t camera, uint32_t viewport, uint32_t car,
                        uint32_t frame, const VideoOutputCall& video_output,
                        const c_runtime::AtexitCall& atexit) {
  ITC_NATIVE_RUNNING("sub_82154D38 ViewCamera2::ViewCamera2");
  const schema::ViewCamera2 c(camera);
  const schema::CameraTarget t(c.tracker_address());
  constexpr uint32_t kGuard = G::view2_guard_address();
  const uint32_t f = frame;
  const uint32_t tracker = c.tracker_address();
  const double one = Lfs(kOne);
  TrackerOnes(tracker, one);
  uint32_t guard = Load(kGuard);
  c.set_vehicle(0);
  c.set_viewport(0);
  c.set_vtable(0x82006B34);
  const double height = Lfs(0x82002CB0);
  c.set_rotation_w(float(one));
  const double zero = Lfs(kZero);
  guard = MakeStatic(kGuard, guard, 1, G::view2_statics_address() + 48,
                     {one, height, zero, one}, 0x825DB140, false, atexit);
  guard = MakeStatic(kGuard, guard, 2, G::view2_statics_address() + 32,
                     {Lfs(0x82001FD8), height, zero, one}, 0x825DB138, true, atexit);
  guard = MakeStatic(kGuard, guard, 4, G::view2_statics_address() + 16,
                     {zero, one, zero, one}, 0x825DB130, false, atexit);
  MakeStatic(kGuard, guard, 8, G::view2_statics_address(),
             {Lfs(0x820029C8), height, zero, one}, 0x825DB128, true, atexit);
  c.set_vehicle(car);
  c.set_viewport(viewport);
  SetLens(camera, Zoom(kChaseDegrees, f + 80, f, 0x82154EF0, video_output), zero);
  CopyWords(c.viewport().base_eye_address(), G::view2_statics_address() + 48, 4);
  CopyWords(c.viewport().base_target_address(), G::view2_statics_address() + 32, 4);
  CopyWords(c.viewport().base_up_address(), G::view2_statics_address() + 16, 4);
  CopyWords(c.viewport().base_look_address(), G::view2_statics_address(), 4);
  const uint32_t the_vehicle = c.vehicle();
  if (the_vehicle != 0) {
    // The eye and target from the car's size: its spec's +76 / 10 (x the
    // floats at 0x82001BE0 and 0x820029D8) and +76, +80 halved.
    const uint32_t spec = vehicle_accessors::ObjectRecord1888(schema::Vehicle(the_vehicle).car());
    const double scale = Lfs(0x820029D8);
    const uint32_t tenth = schema::CarSpecCopies(spec).unknown_76() / 10;
    Store64(f + 80, tenth);
    double x = Fmuls(Single(double(int64_t(tenth))), Lfs(0x82001BE0));
    Stfs(c.viewport().base_eye_address(), Fmuls(x, scale));
    // The spec's word, and the viewport's field it sets: base_eye_y,
    // base_target_x, base_target_y.
    const schema::Viewport at0(0);
    const uint32_t fields[3][2] = {{80, at0.base_eye_y_address()},
                                   {76, at0.base_target_x_address()},
                                   {80, at0.base_target_y_address()}};
    for (const auto& field : fields) {
      const uint32_t half = Load(spec + field[0]) >> 1;
      const uint32_t at = c.viewport().address + field[1];
      Store64(f + 80, half);
      Stfs(at, Fmuls(Single(double(int64_t(half))), scale));
    }
  }
  vehicle::CameraCurveLoad(c.curve_address(), 0, 30);
  TargetTrack(tracker, c.vehicle(), f - 256);
  CopyWords(c.rotation_address(), t.rotation_address(), 4);
  CopyDoublewords(c.rotation_address(), t.rotation_address(), 2);
  return camera;
}

void View2Update(uint32_t camera, uint32_t frame,
                 const VideoOutputCall& video_output) {
  ITC_NATIVE_RUNNING("sub_82153E58 ViewCamera2::Update");
  const schema::ViewCamera2 c(camera);
  const schema::CameraTarget t(c.tracker_address());
  const uint32_t f = frame;
  const uint32_t tracker = c.tracker_address();
  const double one = Lfs(kOne);
  for (uint32_t at : {124u, 140u, 156u, 172u}) {
    Stfs(f + at, one);
  }
  StoreIdentity(f + 112);
  TargetTrack(tracker, c.vehicle(), f - 256);
  SetLens(camera, Zoom(kChaseDegrees, f + 80, f, 0x82153EF0, video_output),
          Lfs(kZero));
  const uint32_t turn = t.turn_address();
  car_physics::MatrixMultiplyTranslation(f + 112, turn, tracker, f - 192);
  uint32_t viewport = c.viewport().address;
  matrix_math::Vec4Transform(schema::Viewport(viewport).eye_address(), f + 112,
                             schema::Viewport(viewport).base_eye_address());
  viewport = c.viewport().address;
  matrix_math::Vec4Transform(schema::Viewport(viewport).target_address(), f + 112,
                             schema::Viewport(viewport).base_target_address());
  viewport = c.viewport().address;
  matrix_math::Vec4Transform(schema::Viewport(viewport).view1_target_address(), f + 112,
                             schema::Viewport(viewport).base_look_address());
  viewport = c.viewport().address;
  matrix_math::Vec4Transform(schema::Viewport(viewport).up_address(), turn,
                             schema::Viewport(viewport).base_up_address());
  NearestHuman(camera, f + 96, f);
  LookPoint(camera, f, one);
}

// --- The garage camera (the front end's) ---

namespace {

constexpr uint32_t kGarageRadians = 0x82002948;  // pi / 180
constexpr uint32_t kPassZero = schema::CamerasGlobals::passes_address();  // camera pass 0

// GarageUpdate's tan argument: the field of view (+224) in radians.
double GarageAngle(uint32_t camera) {
  return Fmuls(double(schema::GarageCamera(camera).fov()), Lfs(kGarageRadians));
}

}  // namespace

bool GarageReady(uint32_t camera) {
  double t;
  return (Fpscr::getcsr() & Fpscr::RoundMaskVal) == 0 &&
         c_runtime::Tan(GarageAngle(camera), &t);
}

void GarageUpdate(uint32_t camera, uint32_t frame, const GarageCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8217BE98 GarageCamera::Update");
  const uint32_t f = frame;
  const auto [queue, list] = calls.queue_and_list(f);
  schema::FramePassesGlobals::set_command_list(list);
  schema::FramePassesGlobals::set_render_queue(queue);
  CopyWords(schema::FramePassesGlobals::lod_bias_address(), kZero, 1);
  // The target (+32) at f + 96 and the eye's start (+16) at f + 80, read
  // before they're written (the viewport loaded between).
  const schema::GarageCamera cam(camera);
  const uint64_t target0 = Load64(cam.target_address()),
                 target1 = Load64(cam.target_address() + 8);
  // [kViewports]'s +0, the block a host object (E5 step 5).
  const uint32_t viewport = schema::CameraModesGlobals::viewports().first().address;
  const uint64_t eye0 = Load64(cam.eye_address()), eye1 = Load64(cam.eye_address() + 8);
  Store64(f + 96, target0);
  Store64(f + 104, target1);
  Store64(f + 80, eye0);
  Store64(f + 88, eye1);
  // The orbit: a = +100, b = +176, the distance +180.
  Stfs(f + 84, Fmuls(Single(c_runtime::Sin(double(cam.tilt()))),
                           double(cam.distance())));
  Stfs(f + 88, Fmuls(Single(c_runtime::Cos(double(cam.tilt()))),
                           double(cam.distance())));
  Stfs(f + 80, Fmuls(Single(c_runtime::Sin(double(cam.orbit()))), Lfs(f + 88)));
  const double cos_b = c_runtime::Cos(double(cam.orbit()));
  const double angle = GarageAngle(camera);
  Stfs(f + 88, Fmuls(Single(cos_b), Lfs(f + 88)));
  // tan as its adapter gives it with nearest rounding set (GarageReady
  // checked it stands in).
  double tangent = 0.0;
  c_runtime::Tan(angle, &tangent);
  const double zoom = Fdivs(Lfs(0x82009EB8), Single(tangent));
  cam.set_zoom(float(zoom));
  cameras::ViewportSetZoom(viewport, zoom);
  cameras::SetNear(Lfs(0x8200222C));
  cameras::ViewportSetRoll(viewport, double(cam.angle_z()));
  // The identity at f + 112, copied to f + 176 (ld/std), its translation
  // row (f + 224 to + 232) = +256 to +264.
  StoreIdentity(f + 112);
  CopyDoublewords(f + 176, f + 112, 8);
  CopyWords(f + 224, cam.translation_address(), 3);
  matrix_math::Vec4Transform(f + 80, f + 176, f + 80);
  matrix_math::Vec4Transform(f + 96, f + 176, f + 96);
  cameras::ViewportLookAt(viewport, f + 80, f + 96);
  cameras::FromViewport(kPassZero, 0, 0);
  calls.pass_update(kPassZero, f);
  cameras::Enable(kPassZero);
  CopyDoublewords(schema::CarSceneDrawGlobals::view_eye_address(), f + 80, 2);
}

}  // namespace camera_modes
