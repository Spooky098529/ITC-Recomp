// The camera modes' views (docs/functions/camera-modes.md): the target
// tracker every camera updates, the shake and the viewports' moves in the
// view's axes, the race start's intro camera and its shots, the director's
// start and release, and the cutscene's shots. Plain C++ over guest memory,
// the originals' loads and stores in their order, their float steps as the
// translated code's (src/camera_modes/steps.h); the vector unit's work with
// flush-to-zero on, the rest with it off. Stack frames the originals write
// (their stack's leftovers) are written the same way, at the frames the
// adapters hand in.

#include "camera_modes/camera_modes.h"

#include "camera_modes/steps.h"

#include "base/log.h"
#include "c_runtime/math.h"
#include "cameras/cameras.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "hooks/vehicle.h"
#include "interpolation/blend_state.h"
#include "matrix_math/matrix_math.h"
#include "race_overlays/race_overlays.h"
#include "track_collision/track_collision.h"
#include "vehicle/vehicle.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"
#include "schema/camera_modes.h"
#include "schema/car_physics.h"
#include "schema/minimap.h"
#include "schema/race_event.h"
#include "schema/race_states.h"
#include "schema/race_world.h"

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
void Store64(uint32_t a, uint64_t v) {
  guest::StoreU32(a, uint32_t(v >> 32));
  guest::StoreU32(a + 4, uint32_t(v));
}
void Store(uint32_t a, uint32_t v) { guest::StoreU32(a, v); }
int32_t LoadS16(uint32_t a) { return int16_t(guest::LoadU16(a)); }
// lfs then stfs: the bits as the translated code keeps them (its
// conversions there are folded away; Matrix::FromFloats' note).
void CopyWord(uint32_t to, uint32_t from) { Store(to, Load(from)); }
void CopyWords(uint32_t to, uint32_t from, uint32_t words) {
  for (uint32_t i = 0; i < words; i++) {
    CopyWord(to + 4 * i, from + 4 * i);
  }
}
// ld/std copies of `doublewords`.
void CopyDoublewords(uint32_t to, uint32_t from, uint32_t doublewords) {
  for (uint32_t i = 0; i < doublewords; i++) {
    Store64(to + 8 * i, Load64(from + 8 * i));
  }
}
// std of a 64-bit integer (the fcfid conversions' stack slot).
void StoreDoubleword(uint32_t a, uint64_t v) { Store64(a, v); }

// --- The vector unit, as the translated code does it (word 0 in the top lane) ---

simde__m128 Lvx(uint32_t address) {
  const uint32_t at = address & ~0xFu;
  return VectorOf(Load(at), Load(at + 4), Load(at + 8), Load(at + 12));
}
template <int kImm>
simde__m128 Shuffle(simde__m128 v) {
  return simde_mm_castsi128_ps(simde_mm_shuffle_epi32(simde_mm_castps_si128(v), kImm));
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
// vmsum3fp128: x, y, z (words 0-2) into every lane.
simde__m128 Dot3(simde__m128 a) { return simde_mm_dp_ps(a, a, 0xEF); }
// vrsqrtefp: 1 / sqrt.
simde__m128 Rsqrt(simde__m128 v) {
  return simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(v));
}
simde__m128 CmpEq(simde__m128 a, simde__m128 b) { return simde_mm_cmpeq_ps(a, b); }
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

// The identity matrix as the originals store it (vupkd3d128 of 0 and four
// vpermwi128s, the translation's lanes), four stvx from `at`.
void StoreIdentity(uint32_t at) {
  const simde__m128 v = simde_mm_set_ps(3.0f, 3.0f, 0.0f, 1.0f);  // lanes 3..0
  Stvx(at + 0, Shuffle<0x15>(v));
  Stvx(at + 16, Shuffle<0x45>(v));
  Stvx(at + 32, Shuffle<0x51>(v));
  Stvx(at + 48, Shuffle<0x54>(v));
}

// The node `steps` steps from a 16-slot list's head (+68), as the originals
// walk it.
uint32_t Walk16(uint32_t list, uint32_t steps) {
  uint32_t node = (uint32_t(int16_t(schema::List16(list).head())) << 2) + list;
  for (uint32_t n = steps; n != 0; n--) {
    node = (uint32_t(LoadS16(node)) << 2) + list;
  }
  return node;
}

// A 16-slot list reset (List16's, inline in the constructors): node i's +2
// = i, +0 = i + 1; the free head (+64) and count (+72) 0; node 15's next
// (+60), the head (+68), +70 and +66 = -1.
void ResetList16(uint32_t list) {
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreU16(list + 4 * i + 2, uint16_t(i));
    guest::StoreU16(list + 4 * i + 0, uint16_t(i + 1));
  }
  const schema::List16 l(list);
  l.set_free(0);
  l.set_count(0);
  guest::StoreU16(l.nodes_address() + 60, 0xFFFF);
  l.set_head(0xFFFF);
  l.set_unknown_70(0xFFFF);
  l.set_unknown_66(0xFFFF);
}

}  // namespace

// --- The target tracker ---

void TargetTrack(uint32_t tracker, uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82153290 CameraTarget::Track");
  const schema::CameraTarget t(tracker);
  if (vehicle == 0) {
    return;
  }
  const uint32_t turn = t.turn_address();
  Store64(t.last_position_address(), Load64(t.position_address()));
  Store64(t.last_position_address() + 8, Load64(t.position_z_address()));
  Store64(t.last_point_address(), Load64(t.point_address()));
  Store64(t.last_point_address() + 8, Load64(t.point_z_address()));
  matrix_math::Copy(t.last_turn_address(), turn);
  interpolation::BlendState_OnCopy(t.last_turn_address(), turn, 0x821532E4);
  CopyWords(t.last_rotation_address(), t.rotation_address(), 4);
  CopyDoublewords(t.last_rotation_address(), t.rotation_address(), 2);
  // The body's matrix on the stack; its fourth row the position.
  const uint32_t body_matrix = vehicle_accessors::RigidBodyMatrix(schema::Vehicle(vehicle).body().address);
  CopyDoublewords(frame + 112, body_matrix, 8);
  CopyWord(t.position_address(), frame + 160);
  CopyWord(t.position_y_address(), frame + 164);
  const uint32_t one = Load(kOne);
  CopyWord(t.position_z_address(), frame + 168);
  Store(t.position_w_address(), one);
  // The two vectors (+1216, +1584) summed from 0.0 and halved.
  const double zero = Lfs(kZero);
  Stfs(frame + 92, zero);
  double x = zero, y = zero, z = zero;
  for (uint32_t i = 0; i < 2; i++) {
    vector_library::Vec4Copy(frame + 80, vehicle + 1216 + 368 * i);
    x = Fadds(x, Lfs(frame + 80));
    y = Fadds(y, Lfs(frame + 84));
    z = Fadds(z, Lfs(frame + 88));
  }
  Store(t.point_w_address(), one);
  const double half = Lfs(0x82001BE4);
  t.set_point_x(float(Fmuls(x, half)));
  t.set_point_y(float(Fmuls(y, half)));
  t.set_point_z(float(Fmuls(z, half)));
  // The body's rotation (+80) into +192 and the turn at +64.
  const uint32_t rotation = vehicle_accessors::RigidBodyField80(schema::Vehicle(vehicle).body().address);
  CopyDoublewords(frame + 96, rotation, 2);
  CopyWords(t.rotation_address(), frame + 96, 4);
  StoreIdentity(turn);
  const uint64_t r4 = Load64(t.rotation_address());
  const uint64_t r5 = Load64(t.rotation_z_address());
  // Matrix::FromQuat's parameter save words (its caller's r1 + 32).
  Store64(frame + 32, r4);
  Store64(frame + 40, r5);
  const uint32_t q[4] = {uint32_t(r4 >> 32), uint32_t(r4), uint32_t(r5 >> 32), uint32_t(r5)};
  MatrixFromQuat(turn, q, (frame - 16) & ~0xFu);
  vehicle::TrackPositionCopy(t.track_position_address(),
                             vehicle_accessors::VehicleIdentity(vehicle));
}

// --- The viewports' moves in the view's axes ---

void ViewMove(uint32_t viewport, uint32_t moved, double amount, uint32_t offset,
              uint32_t sp) {
  const double zero = Lfs(kZero);
  const double one = Lfs(kOne);
  if (amount < zero) {
    amount = zero;
  } else if (amount > one) {
    amount = one;
  }
  Stfs(sp - 48, zero);
  Stfs(sp - 40, zero);
  Stfs(sp - 16, zero);
  Stfs(sp - 12, zero);
  Stfs(sp - 8, zero);
  Stfs(sp - 44, one);
  Stfs(sp - 36, one);
  const schema::Viewport vp(viewport);
  const schema::CameraVec3 by(offset);
  const double from = Lfs(moved);
  const double dx = Fsubs(double(vp.target_x()), double(vp.eye_x()));
  const double dy = Fsubs(double(vp.target_y()), double(vp.eye_y()));
  const double dz = Fsubs(double(vp.target_z()), double(vp.eye_z()));
  Stfs(sp - 20, one);
  Stfs(sp - 32, dx);
  const simde__m128 up = Lvx(sp - 48);
  Stfs(sp - 28, dy);
  Stfs(sp - 24, dz);
  const double ox = Fmuls(double(by.x()), amount);
  const double oy = Fmuls(double(by.y()), amount);
  const double oz = Fmuls(double(by.z()), amount);
  const simde__m128 dir = Lvx(sp - 32);
  // The vector unit: right = dir x up normalized, up' = right x dir
  // normalized, dir normalized (each left as its squared length where
  // that's 0), with the originals' estimate and one Newton step.
  simde__m128 v0, v1, v2, v3, v5, v6, v8, v9, v10, v11, v12, v13 = dir, v29, v30, v31, v7;
  simde__m128 right_raw;
  {
    FlushToZero ftz;
    const simde__m128 zeros = simde_mm_setzero_ps();
    v12 = Half();
    v31 = Half();
    v7 = Half();
    v11 = Shuffle<0x78>(up);
    v9 = Shuffle<0x9C>(up);
    v8 = Shuffle<0x9C>(v13);
    v10 = Shuffle<0x78>(v13);
    v0 = Mul(v8, v11);
    v0 = NegMulSub(v10, v9, v0);
    v9 = Shuffle<0x9C>(v0);
    v11 = Dot3(v0);
    v6 = Shuffle<0x78>(v0);
    right_raw = v0;
    v9 = Mul(v9, v10);
    v2 = NegMulSub(v6, v8, v9);
    v8 = Dot3(v13);
    v10 = Rsqrt(v11);
    v5 = Mul(v11, v12);
    v3 = CmpEq(v11, zeros);
    v9 = Dot3(v2);
    v6 = Mul(v10, v10);
    v29 = Mul(v8, v7);
    v12 = NegMulSub(v5, v6, v12);
    v5 = Rsqrt(v8);
    v6 = Rsqrt(v9);
    v30 = Mul(v9, v31);
    v1 = CmpEq(v9, zeros);
    v12 = MulAdd(v10, v12, v10);
    v10 = Mul(v6, v6);
    v0 = Mul(v0, v12);
    v12 = Mul(v5, v5);
    v31 = NegMulSub(v30, v10, v31);
    v3 = Select(v3, v0, v11);
    v0 = MulAdd(v6, v31, v6);
    v0 = Mul(v2, v0);
    v1 = Select(v1, v0, v9);
    v0 = CmpEq(v8, zeros);
    v7 = NegMulSub(v29, v12, v7);
    v12 = MulAdd(v5, v7, v5);
    v13 = Mul(v13, v12);
    v0 = Select(v0, v13, v8);
  }
  Stvx(sp - 16, right_raw);
  Stvx(sp - 16, v3);
  Stvx(sp - 48, v1);
  Stvx(sp - 32, v0);
  // The moved point (at `moved`) plus the offset along the three axes.
  const uint32_t m = moved;
  Stfs(m + 0, Fadds(from, Fmuls(Lfs(sp - 16), ox)));
  {
    const double f10 = Lfs(m + 4);
    Stfs(m + 4, Fadds(f10, Fmuls(Lfs(sp - 12), ox)));
  }
  {
    const double f0 = Fmuls(Lfs(sp - 8), ox);
    Stfs(m + 8, Fadds(Lfs(m + 8), f0));
  }
  for (uint32_t i = 0; i < 3; i++) {
    const double f0 = Fmuls(Lfs(sp - 48 + 4 * i), oy);
    Stfs(m + 4 * i, Fadds(Lfs(m + 4 * i), f0));
  }
  for (uint32_t i = 0; i < 3; i++) {
    const double f0 = Fmuls(Lfs(sp - 32 + 4 * i), oz);
    Stfs(m + 4 * i, Fadds(Lfs(m + 4 * i), f0));
  }
}

void ViewportOffset(uint32_t viewport, double amount, uint32_t offset,
                    uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821529B0 Viewport::Offset");
  ViewMove(viewport, schema::Viewport(viewport).target_address(), amount, offset, sp);
}

void ViewportPullBack(uint32_t viewport, double amount, uint32_t offset,
                      uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82152788 Viewport::PullBack");
  ViewMove(viewport, schema::Viewport(viewport).eye_address(), amount, offset, sp);
}

// --- The shake ---

void ShakeRandom(uint32_t viewport, double amount, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82152BD8 Camera::ShakeRandom");
  constexpr uint32_t kGuard = G::shake_guard_address(), kStep = G::shake_step_address();
  const uint32_t guard = Load(kGuard);
  const double zero = Lfs(kZero);
  Stfs(frame + 96, zero);
  Stfs(frame + 100, zero);
  Stfs(frame + 104, zero);
  const double one = Lfs(kOne);
  Stfs(frame + 108, one);
  if ((guard & 1) == 0) {
    Store(kGuard, guard | 1);
    CopyWord(kStep, 0x8200939C);
  }
  if (amount < zero) {
    amount = zero;
  } else if (amount > one) {
    amount = one;
  }
  for (uint32_t i = 0; i < 2; i++) {
    const uint32_t drawn = race_overlays::Rand() % 100;
    StoreDoubleword(frame + 80, drawn);
    const double n = Single(double(int64_t(drawn)));
    Stfs(frame + 96 + 4 * i, Fmuls(n, Lfs(kStep)));
  }
  ViewportOffset(viewport, amount, frame + 96, frame);
}

void Shake(uint32_t camera, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82152CD8 Camera::Shake");
  const schema::ChaseCamera c(camera);
  const double speed = double(Vehicle_GetSpeedKmh(c.vehicle()));
  const uint32_t viewport = c.viewport().address;
  const double over = Fsubs(speed, Lfs(0x82001FB8));
  ShakeRandom(viewport, Fmuls(over, Lfs(0x8200222C)), frame - 160);
}

// --- The intro camera ---

uint32_t IntroStepShot(uint32_t shot, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82152E78 IntroCamera::StepShot");
  const schema::IntroShot s(shot);
  StoreIdentity(frame + 80);
  const uint32_t body_matrix = vehicle_accessors::RigidBodyMatrix(schema::Vehicle(s.vehicle()).body().address);
  CopyDoublewords(frame + 176, body_matrix, 8);
  uint32_t viewport = s.viewport().address;
  const int32_t phase = int32_t(s.phase());
  CopyWords(frame + 80, frame + 176, 16);
  CopyWords(frame + 160, schema::Viewport(viewport).base_eye_address(), 4);
  CopyWords(frame + 144, schema::Viewport(viewport).base_target_address(), 4);
  if (phase == 0 && s.frames() >= 29) {
    s.set_phase(3);
  }
  matrix_math::Vec4Transform(schema::Viewport(viewport).eye_address(), frame + 80,
                             frame + 160);
  viewport = s.viewport().address;
  matrix_math::Vec4Transform(schema::Viewport(viewport).target_address(), frame + 80,
                             frame + 144);
  viewport = s.viewport().address;
  CopyWords(schema::Viewport(viewport).up_address(),
            schema::Viewport(viewport).base_up_address(), 4);
  viewport = s.viewport().address;
  CopyWords(schema::Viewport(viewport).view1_target_address(),
            schema::Viewport(viewport).base_look_address(), 4);
  if (int32_t(s.phase()) == 3) {
    return 0;
  }
  if (int32_t(G::paused()) == 0) {
    s.set_frames(s.frames() + 1);
  }
  return 1;
}

void IntroMakeShot(uint32_t shot, uint32_t viewport, uint32_t vehicle,
                   uint32_t frame, const c_runtime::AtexitCall& atexit) {
  ITC_NATIVE_RUNNING("sub_82154108 IntroCamera::MakeShot");
  const schema::IntroShot s(shot);
  constexpr uint32_t kGuard = G::intro_guard_address();
  constexpr uint32_t kUp = G::intro_up_address();          // (0, 1, 0, 1)
  constexpr uint32_t kOffset = G::intro_offset_address();  // (0.75, 1.8, 0, 1), probably
  const double zero = Lfs(kZero);
  s.set_length(float(zero));
  s.set_viewport(viewport);
  s.set_vehicle(vehicle);
  s.set_frames(0);
  s.set_phase(0);
  const uint32_t spec = vehicle_accessors::ObjectRecord1888(schema::Vehicle(vehicle).car());
  const double f0 = Lfs(0x820029D8);
  const uint32_t r10 = schema::CarSpecCopies(spec).unknown_80();
  const double angle = std::bit_cast<double>(Load64(0x820093A8));
  const uint32_t r11 = schema::CarSpecCopies(spec).unknown_76();
  StoreDoubleword(frame + 88, r10);
  StoreDoubleword(frame + 80, r11);
  const double f12 = Single(double(int64_t(r10)));
  const double f11 = Single(double(int64_t(r11)));
  const double half = Lfs(0x82001BE4);
  const double len = Fmuls(f12, half);
  const double wid = Fmuls(f11, half);
  const double f29 = Fmadds(wid, f0, Lfs(0x82008230));
  const double f28 = Fmsubs(len, f0, Lfs(0x82002208));
  const double sin = Single(c_runtime::Sin(angle));
  const double cos = Single(c_runtime::Cos(angle));
  const double f27 = Fmadds(sin, f29, f28);
  uint32_t guard = Load(kGuard);
  const double one = Lfs(kOne);
  const double x = Fmuls(cos, f29);
  if ((guard & 1) == 0) {
    guard |= 1;
    Stfs(kUp + 0, zero);
    Stfs(kUp + 4, one);
    Stfs(kUp + 8, zero);
    Stfs(kUp + 12, one);
    Store(kGuard, guard);
    c_runtime::AtexitFrom(0x825DB150, atexit);
    guard = Load(kGuard);
  }
  if ((guard & 2) == 0) {
    Store(kGuard, guard | 2);
    CopyWord(kOffset + 0, 0x8200295C);
    CopyWord(kOffset + 4, 0x82008290);
    Stfs(kOffset + 8, zero);
    Stfs(kOffset + 12, one);
    c_runtime::AtexitFrom(0x825DB148, atexit);
  }
  uint32_t vp = s.viewport().address;
  schema::Viewport(vp).set_base_eye_x(float(x));
  schema::Viewport(vp).set_base_eye_y(float(f27));
  schema::Viewport(vp).set_base_eye_z(float(zero));
  schema::Viewport(vp).set_base_eye_w(float(one));
  vp = s.viewport().address;
  schema::Viewport(vp).set_base_target_x(float(zero));
  schema::Viewport(vp).set_base_target_y(float(f28));
  schema::Viewport(vp).set_base_target_z(float(zero));
  schema::Viewport(vp).set_base_target_w(float(one));
  vp = s.viewport().address;
  CopyWords(schema::Viewport(vp).base_up_address(), kUp, 4);
  vp = s.viewport().address;
  CopyWords(schema::Viewport(vp).base_look_address(), kOffset, 4);
  const uint32_t r88 = schema::CarSpecCopies(spec).model_year();
  StoreDoubleword(frame + 88, r88);
  const double n = Single(double(int64_t(r88)));
  s.set_length(float(Fmuls(n, Lfs(0x82002CCC))));
}

void IntroUpdate(uint32_t camera, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82155E58 IntroCamera::Update");
  const schema::IntroCamera cam(camera);
  constexpr uint32_t kKept = G::intro_kept_address();  // the frame the shots ended at
  const uint32_t f = frame;
  TargetTrack(cam.tracker_address(), cam.vehicle(), f - 256);
  const uint32_t list = cam.shots_address();
  if (schema::List16(list).count() == 0) {
    // No shots: the race's flag word (+0x59AE4) bit 1, phase 2.
    const uint32_t flags = G::race_data() + 0x59AE4;
    Store(flags, Load(flags) | 1);
    cam.set_phase(2);
    return;
  }
  const uint32_t phase = cam.phase();
  if (phase < 1) {
    // The shots in turn: the current one stepped, the next when it ends;
    // after the last, phase 1 from this frame.
    const uint32_t node = Walk16(list, cam.shot());
    const int32_t slot = LoadS16(node + 2);
    const uint32_t shot = uint32_t(int64_t(slot) * 20) + list + schema::List16::kSize;
    if ((IntroStepShot(shot, f - 272) & 0xFF) == 0) {
      cam.set_shot(cam.shot() + 1);
    }
    const uint32_t count = uint32_t(int16_t(schema::List16(list).count()));
    if (!(cam.shot() < count)) {
      const uint32_t frames = cam.frames();
      cam.set_phase(1);
      Store(kKept, frames);
    }
  } else if (phase < 3) {
    // The orbit about the car (from its spec block's size) over the frames
    // since the shots ended, at 1/60 s a frame, t clamped to 0-1.
    const uint32_t spec = vehicle_accessors::ObjectRecord1888(schema::Vehicle(cam.vehicle()).car());
    const double c = Lfs(0x82002CCC);
    const uint32_t frames = cam.frames();
    const uint32_t r84 = schema::CarSpecCopies(spec).unknown_84();
    StoreDoubleword(f + 80, r84);
    const double f11 = Single(double(int64_t(r84)));
    const int32_t since = int32_t(frames - Load(kKept));
    const uint32_t r80 = schema::CarSpecCopies(spec).unknown_80();
    StoreDoubleword(f + 240, r80);
    StoreDoubleword(f + 96, uint64_t(int64_t(since)));
    const double f12 = Single(double(int64_t(since)));
    const uint32_t r76 = schema::CarSpecCopies(spec).unknown_76();
    StoreDoubleword(f + 96, r76);
    double t = Fmuls(f12, Lfs(0x820029E8));
    const double zero = Lfs(kZero);
    const double f25 = Fmuls(Single(double(int64_t(r80))), c);
    const double f27 = Fmadds(Single(double(int64_t(r76))), c, Lfs(0x82008230));
    const double f26 = Fmadds(f11, c, Lfs(0x82002A14));
    const double one = Lfs(kOne);
    if (t < zero) {
      t = zero;
    } else if (t > one) {
      t = one;
    }
    const double angle = std::bit_cast<double>(Load64(0x820093A8));
    const double f28 = Fmadds(Single(c_runtime::Sin(angle)), f27, f25);
    const double f9 = Fmuls(Single(c_runtime::Cos(angle)), f27);
    for (uint32_t at = f + 188; at <= f + 236; at += 16) {
      Stfs(at, one);
    }
    const double u = Fsubs(one, t);
    const double f10a = Fneg(f9);
    Stfs(f + 92, one);
    const double tt = Fmuls(t, t);
    const double f11n = Fneg(f26);
    const double f8a = Fmuls(u, t);
    const double uu = Fmuls(u, u);
    const double f12b = Fmuls(f8a, Lfs(0x820021E8));
    const double f8b = Fmuls(f10a, f12b);
    const double f7a = Fadds(tt, f12b);
    const double f10b = Fmadds(f10a, tt, f8b);
    const double f8c = Fmuls(f11n, uu);
    const double f7b = Fadds(f7a, uu);
    Stfs(f + 80, Fmadds(uu, f9, f10b));
    const double f12c = Fmadds(f11n, f12b, f8c);
    Stfs(f + 84, Fmuls(f7b, f28));
    Stfs(f + 88, Fmadds(tt, zero, f12c));
    const uint32_t body_matrix =
        vehicle_accessors::RigidBodyMatrix(schema::Vehicle(cam.vehicle()).body().address);
    CopyDoublewords(f + 176, body_matrix, 8);
    CopyWords(f + 112, f + 176, 16);
    matrix_math::Vec4Transform(cam.viewport().eye_address(), f + 112, f + 80);
    Stfs(f + 80, zero);
    Stfs(f + 84, f25);
    Stfs(f + 88, zero);
    Stfs(f + 92, one);
    matrix_math::Vec4Transform(cam.viewport().target_address(), f + 112, f + 80);
    if (int32_t(cam.phase()) == 1 && !(t < one)) {
      cam.set_phase(2);
      const uint32_t flags = G::race_data() + 0x59AE4;
      Store(flags, Load(flags) | 1);
    }
  }
  // The frames counted unless the screen fade holds them.
  if (int32_t(G::paused()) == 0) {
    cam.set_frames(cam.frames() + 1);
  }
}

uint32_t IntroConstruct(uint32_t camera, uint32_t viewport, uint32_t vehicle,
                        uint32_t frame, const StagesCopyCall& stages_copy,
                        const c_runtime::AtexitCall& atexit) {
  ITC_NATIVE_RUNNING("sub_82158F70 IntroCamera::IntroCamera");
  const schema::IntroCamera c(camera);
  const uint32_t tracker = c.tracker_address();
  const uint32_t list = c.shots_address();
  const double one = Lfs(kOne);
  for (uint32_t at : {12u, 28u, 44u, 60u, 76u, 92u, 108u, 124u, 140u, 156u, 172u, 188u, 204u,
                      220u, 252u, 268u, 284u, 300u}) {
    Stfs(tracker + at, one);  // the tracker's w words
  }
  c.set_vtable(0x820093C4);
  c.set_shot(0);
  c.set_frames(0);
  c.set_phase(0);
  c.set_vehicle(vehicle);
  c.set_viewport(viewport);
  ResetList16(list);
  // The stage's racers: their vehicles at frame + 80 (the count at +96),
  // the ones whose item is of kind 1 or 3 moved to the front.
  const uint32_t race = G::race_data();
  for (uint32_t i = 0; i < 4; i++) {
    Store(frame + 80 + 4 * i, 0);
  }
  uint32_t count = 0;
  Store(frame + 96, count);
  const uint32_t stages = schema::RaceData(race).stages_address();
  const uint32_t stage = schema::RaceData(race).current_stage();
  if (!vehicle_accessors::StagesCopy(frame + 144, stages, stage)) {
    stages_copy(frame + 144, stages, stage);
  }
  for (int32_t s = 0; s < int32_t(Load(frame + 220)); s++) {
    const uint32_t racer = Load(frame + 188 + 8 * s);
    const uint32_t record = uint32_t(uint64_t(racer) * 3936) + G::race_data();
    if (((schema::RaceData(record).record0_flags() >> 1) & 1) == 0) {
      continue;
    }
    uint32_t found_vehicle = 0;
    bool kind_flag = false;
    for (int32_t n = 0; n < int32_t(track_collision::RaceRacerCount()); n++) {
      const uint32_t entry = Load(schema::RaceManager(G::racers_manager()).racers_address() + 4 * uint32_t(n));
      if (int32_t(racer) == int32_t(schema::MinimapRacerEntry(entry).record())) {
        found_vehicle = vehicle_accessors::VehicleGet(schema::MinimapRacerEntry(entry).vehicle_index());
        kind_flag = (schema::MinimapRacerEntry(entry).flags() & 0x400) != 0;
        break;
      }
    }
    Store(((count << 2) & 0xFFFFFFFCu) + frame + 80, found_vehicle);
    uint32_t r11 = 0;
    bool front = false;
    if (kind_flag) {
      const int32_t id = int32_t(schema::RaceData(record).record0_racer_id());
      if (id >= 0) {
        const int32_t kind = int16_t(schema::OpponentItem(data_tables::Item(uint32_t(id))).race_kind());
        if (kind == 1 || kind == 3) {
          front = true;
          r11 = Load(frame + 96);
          int32_t k = int32_t(r11);
          if (k > 0) {
            uint32_t at = ((r11 << 2) & 0xFFFFFFFCu) + frame + 80;
            for (; k > 0; k--) {
              Store(at, Load(at - 4));
              at -= 4;
            }
            r11 = Load(frame + 96);
          }
          Store(frame + 80, found_vehicle);
        }
      }
    }
    if (!front) {
      r11 = Load(frame + 96);
    }
    count = r11 + 1;
    Store(frame + 96, count);
  }
  // A shot made and added for each.
  if (int32_t(count) > 0) {
    uint32_t at = frame + 80;
    for (uint32_t n = count; n != 0; n--) {
      IntroMakeShot(frame + 112, viewport, Load(at), frame - 192, atexit);
      List16Insert20(list, frame + 112,
                     uint32_t(int16_t(schema::List16(list).count())));
      at += 4;
    }
  }
  return camera;
}

// --- The director's start and release (handlers) ---

void DirectorStart(uint32_t director, const std::function<void()>& viewports_init) {
  ITC_NATIVE_RUNNING("sub_82152338 CameraDirector::Start");
  const schema::CameraDirector d(director);
  viewports_init();
  const schema::Viewports viewports = G::viewports();
  const uint32_t first = viewports.first().address;
  const uint32_t second = viewports.second().address;
  cameras::ViewportUpdate(first);
  const double near = Lfs(0x8200295C);
  cameras::SetNear(near);
  const double zoom = Lfs(kOne);
  cameras::ViewportSetZoom(first, zoom);
  const double far = Lfs(0x82001FCC);
  schema::ViewportEntry(first).set_far(float(far));
  cameras::ViewportUpdate(second);
  cameras::SetNear(near);
  cameras::ViewportSetZoom(second, zoom);
  schema::ViewportEntry(second).set_far(float(far));
  d.set_followed(0);
  d.set_view0_angle(2);
  d.set_followed_input(0);
  AnimsClear();
}

void DirectorRelease(uint32_t director, const std::function<void()>& free_all,
                     const std::function<void()>& release_viewports) {
  ITC_NATIVE_RUNNING("sub_82153098 CameraDirector::Release");
  const schema::CameraDirector d(director);
  free_all();
  const schema::SaveData save(vehicle_accessors::SaveDataOf(0));
  const int32_t kind = int32_t(schema::RaceData(G::race_data()).kind());
  if (kind == 1 || kind == 2 || kind == 3) {
    uint32_t view = d.view();
    if (int32_t(view) == 4 || int32_t(view) == 5) {
      view = d.view_kept();
    }
    if (view < 3) {
      save.set_camera_view(view == 0 ? 2 : view == 1 ? 1 : 0);
    }
    d.set_start_view(view);
  }
  release_viewports();
}

// --- The cutscene's shots ---

void CutsceneInit(uint32_t shot, uint32_t entry, uint32_t frame,
                  const CutsceneCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82156B68 CutsceneShot::Init");
  const schema::CutsceneShot s(shot);
  const schema::CutsceneScriptEntry e(entry);
  const uint32_t list = s.cues_address();
  const uint32_t kind = e.kind();
  Store(frame + 80, 22824);
  Store(frame + 84, 22825);
  s.set_kind(kind);
  for (uint32_t i = 2; i < 16; i++) {
    Store(frame + 80 + 4 * i, 22824 + i);
  }
  s.set_anim(Load(((kind << 2) & 0xFFFFFFFCu) + frame + 80));
  s.set_vehicle(e.vehicle());
  ResetList16(list);
  s.set_time(0);
  s.set_flags(1);
  for (uint32_t i = 0; i < uint32_t(LoadS16(e.cue_count_address())); i++) {
    uint32_t from = 0;
    if (!List16Copy32(frame + 80, e.cues_address(), i, &from)) {
      calls.copy32(frame + 80, from, 32);
    }
    uint32_t to = 0;
    if (!List16Insert(list, frame + 80, uint32_t(int16_t(schema::List16(list).count())),
                      32, &to)) {
      calls.insert32(to, frame + 80, 32);
    }
  }
  calls.load(s.anim());
}

uint32_t CutsceneStep(uint32_t shot, uint32_t out, uint32_t frame,
                      const std::function<void(uint32_t object)>& camera_original, double* f1) {
  ITC_NATIVE_RUNNING("sub_82155800 CutsceneShot::Step");
  const schema::CutsceneShot s(shot);
  const schema::CameraShotOut o(out);
  const uint32_t one = Load(kOne);
  Store(frame + 156, one);
  Store(frame + 172, one);
  std::optional<double> camera_f1;
  const uint32_t found =
      AnimsEvaluate(frame + 144, s.anim(), s.time(), camera_original, &camera_f1);
  if (camera_f1) {
    *f1 = *camera_f1;
  }
  if (found == 0) {
    s.set_flags(4);
    return 4;
  }
  s.set_flags((s.flags() & 0xFFFFFFDAu) | 8);
  const uint32_t body_matrix = vehicle_accessors::RigidBodyMatrix(schema::Vehicle(s.vehicle()).body().address);
  CopyDoublewords(frame + 192, body_matrix, 8);
  CopyWords(frame + 80, frame + 192, 16);
  const double flip = Lfs(0x82001BE0);
  Stfs(frame + 168, Fmuls(Lfs(frame + 168), flip));
  Stfs(frame + 152, Fmuls(Lfs(frame + 152), flip));
  matrix_math::Vec3SwapXZ(frame + 160);
  matrix_math::Vec3SwapXZ(frame + 144);
  matrix_math::Vec4Transform(o.eye_address(), frame + 80, frame + 160);
  matrix_math::Vec4Transform(o.target_address(), frame + 80, frame + 144);
  o.set_roll(float(Fmuls(Lfs(frame + 176), Lfs(0x82002948))));
  CopyWord(o.zoom_address(), frame + 180);
  // The cues (the list at +12, its count +84).
  const uint32_t list = s.cues_address();
  for (uint32_t i = 0; i < uint32_t(int16_t(schema::List16(s.cues_address()).count())); i++) {
    const uint32_t node = Walk16(list, i);
    const uint32_t slot = uint32_t(LoadS16(node + 2));
    FadeCueStep(((slot << 5) & 0xFFFFFFE0u) + list + schema::List16::kSize);
  }
  if ((s.flags() & 0x10) == 0) {
    s.set_time(s.time() + 1);
  }
  uint32_t r3 = 0;
  const double duration = AnimsDuration(s.anim(), &r3);
  *f1 = duration;
  const int32_t time = int32_t(s.time());
  StoreDoubleword(frame + 256, uint64_t(int64_t(time)));
  if (!(duration > Single(double(int64_t(time))))) {
    s.set_flags(32);
  }
  return s.flags();
}

uint32_t CutsceneTick(uint32_t cutscene, uint32_t frame,
                      const CutsceneTickCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82155A28 Cutscene::Step");
  const schema::Cutscene c(cutscene);
  const uint32_t logo = c.rival_logo_address();
  uint32_t r29 = 1;
  const uint32_t one = Load(kOne);
  const schema::RivalLogo rival(logo);
  const uint32_t flags = rival.flags();
  Store(frame + 108, one);
  Store(frame + 124, one);
  Store(frame + 140, one);
  if ((flags & 1) == 0) {
    if (calls.play_original) {
      calls.play_original(logo);
    } else {
      // RivalLogo::Play's frame (144 bytes): the back chain its original
      // leaves there (CutsceneShot::Step's frame reads that word as its
      // target's x when no animation object writes it).
      Store(frame - 144, frame);
      RivalLogoPlay(logo, frame - 144 + 80);
    }
    if ((rival.flags() & 8) == 0) {
      return 1;
    }
  }
  const uint32_t index = c.shot();
  if (!(uint32_t(int16_t(schema::List16(c.shots_address()).count())) > index)) {
    return r29;
  }
  if (c.unknown_10756() != 0) {
    return r29;
  }
  // The shots: a 16-slot list at +952 of 608-byte shots, its count +1024.
  const uint32_t list = c.shots_address();
  const uint32_t node = Walk16(list, index);
  const uint32_t shot =
      uint32_t(int64_t(LoadS16(node + 2)) * 608) + list + schema::List16::kSize;
  double f1 = 0;
  const uint32_t r30 = CutsceneStep(shot, frame + 80, frame - 304,
      [&](uint32_t object) { calls.camera_original(shot, object); }, &f1);
  if ((r30 & 0x20) != 0) {
    const uint32_t next = c.shot() + 1;
    const uint32_t count = uint32_t(int16_t(schema::List16(c.shots_address()).count()));
    c.set_shot(next);
    if (!(count > next)) {
      r29 = 0;
      RivalLogoClose(logo, frame - 128, calls.close);
    }
  }
  if ((r30 & 4) != 0) {
    return r29;
  }
  // The viewport (+20): the zoom, the near plane, the roll, then the eye
  // and target (the frame's +96, 8 words), +1712's four words into +1632,
  // and the cutscene's +32 into +1648.
  cameras::ViewportSetZoom(c.viewport().address, Lfs(frame + 148));
  uint32_t viewport = c.viewport().address;
  const double at1864 = double(schema::Viewport(viewport).zoom());
  const double at1868 = double(schema::Viewport(viewport).near());
  cameras::SetNear(Fmuls(Fdivs(at1868, at1864), Lfs(frame + 148)));
  cameras::ViewportSetRoll(c.viewport().address, Lfs(frame + 144));
  viewport = c.viewport().address;
  CopyWords(schema::Viewport(viewport).eye_address(), frame + 96, 8);
  viewport = c.viewport().address;
  CopyWords(schema::Viewport(viewport).up_address(),
            schema::Viewport(viewport).base_up_address(), 4);
  viewport = c.viewport().address;
  CopyWords(schema::Viewport(viewport).look_address(), c.tracker_address(), 4);
  if ((rival.flags() & 8) != 0) {
    calls.draw(logo);
  }
  c.set_frame(c.frame() + 1);
  return r29;
}

uint32_t CutsceneUpdate(uint32_t cutscene, uint32_t frame,
                        const CutsceneTickCalls& calls) {
  ITC_NATIVE_RUNNING("sub_821561F8 Cutscene::Update");
  const schema::Cutscene c(cutscene);
  uint32_t r3 = cutscene;
  if (int32_t(c.script()) != -1) {
    r3 = CutsceneTick(cutscene, frame - 192, calls);
    if ((r3 & 0xFF) != 0) {
      return r3;
    }
  }
  const uint32_t done = G::race_data() + 0x59AE4;
  Store(done, Load(done) | 1);
  return r3;
}

}  // namespace camera_modes
