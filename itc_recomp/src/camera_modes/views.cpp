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

namespace camera_modes {

using namespace steps;

namespace {

constexpr uint32_t kOne = 0x82000A30;   // 1.0
constexpr uint32_t kZero = 0x82000A34;  // 0.0

uint32_t Load(uint8_t* base, uint32_t a) { return guest::LoadU32(base, a); }
uint64_t Load64(uint8_t* base, uint32_t a) {
  return (uint64_t(guest::LoadU32(base, a)) << 32) | guest::LoadU32(base, a + 4);
}
void Store64(uint8_t* base, uint32_t a, uint64_t v) {
  guest::StoreU32(base, a, uint32_t(v >> 32));
  guest::StoreU32(base, a + 4, uint32_t(v));
}
void Store(uint8_t* base, uint32_t a, uint32_t v) { guest::StoreU32(base, a, v); }
int32_t LoadS16(uint8_t* base, uint32_t a) { return int16_t(guest::LoadU16(base, a)); }
// lfs then stfs: the bits as the translated code keeps them (its
// conversions there are folded away; Matrix::FromFloats' note).
void CopyWord(uint8_t* base, uint32_t to, uint32_t from) { Store(base, to, Load(base, from)); }
void CopyWords(uint8_t* base, uint32_t to, uint32_t from, uint32_t words) {
  for (uint32_t i = 0; i < words; i++) {
    CopyWord(base, to + 4 * i, from + 4 * i);
  }
}
// ld/std copies of `doublewords`.
void CopyDoublewords(uint8_t* base, uint32_t to, uint32_t from, uint32_t doublewords) {
  for (uint32_t i = 0; i < doublewords; i++) {
    Store64(base, to + 8 * i, Load64(base, from + 8 * i));
  }
}
// std of a 64-bit integer (the fcfid conversions' stack slot).
void StoreDoubleword(uint8_t* base, uint32_t a, uint64_t v) { Store64(base, a, v); }

// --- The vector unit, as the translated code does it (word 0 in the top lane) ---

simde__m128 Lvx(uint8_t* base, uint32_t address) {
  const uint32_t at = address & ~0xFu;
  return VectorOf(Load(base, at), Load(base, at + 4), Load(base, at + 8), Load(base, at + 12));
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
void StoreIdentity(uint8_t* base, uint32_t at) {
  const simde__m128 v = simde_mm_set_ps(3.0f, 3.0f, 0.0f, 1.0f);  // lanes 3..0
  Stvx(base, at + 0, Shuffle<0x15>(v));
  Stvx(base, at + 16, Shuffle<0x45>(v));
  Stvx(base, at + 32, Shuffle<0x51>(v));
  Stvx(base, at + 48, Shuffle<0x54>(v));
}

// The node `steps` steps from a 16-slot list's head (+68), as the originals
// walk it.
uint32_t Walk16(uint8_t* base, uint32_t list, uint32_t steps) {
  uint32_t node = (uint32_t(LoadS16(base, list + 68)) << 2) + list;
  for (uint32_t n = steps; n != 0; n--) {
    node = (uint32_t(LoadS16(base, node)) << 2) + list;
  }
  return node;
}

// A 16-slot list reset (List16's, inline in the constructors): node i's +2
// = i, +0 = i + 1; the free head (+64) and count (+72) 0; node 15's next
// (+60), the head (+68), +70 and +66 = -1.
void ResetList16(uint8_t* base, uint32_t list) {
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreU16(base, list + 4 * i + 2, uint16_t(i));
    guest::StoreU16(base, list + 4 * i + 0, uint16_t(i + 1));
  }
  guest::StoreU16(base, list + 64, 0);
  guest::StoreU16(base, list + 72, 0);
  guest::StoreU16(base, list + 60, 0xFFFF);
  guest::StoreU16(base, list + 68, 0xFFFF);
  guest::StoreU16(base, list + 70, 0xFFFF);
  guest::StoreU16(base, list + 66, 0xFFFF);
}

}  // namespace

// --- The target tracker ---

void TargetTrack(uint8_t* base, uint32_t tracker, uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82153290 CameraTarget::Track");
  if (vehicle == 0) {
    return;
  }
  const uint32_t turn = tracker + 64;
  Store64(base, tracker + 16, Load64(base, tracker + 0));
  Store64(base, tracker + 24, Load64(base, tracker + 8));
  Store64(base, tracker + 48, Load64(base, tracker + 32));
  Store64(base, tracker + 56, Load64(base, tracker + 40));
  matrix_math::Copy(base, tracker + 128, turn);
  interpolation::BlendState_OnCopy(base, tracker + 128, turn, 0x821532E4);
  CopyWords(base, tracker + 208, tracker + 192, 4);
  CopyDoublewords(base, tracker + 208, tracker + 192, 2);
  // The body's matrix on the stack; its fourth row the position.
  const uint32_t body_matrix = vehicle_accessors::RigidBodyMatrix(Load(base, vehicle + 336));
  CopyDoublewords(base, frame + 112, body_matrix, 8);
  CopyWord(base, tracker + 0, frame + 160);
  CopyWord(base, tracker + 4, frame + 164);
  const uint32_t one = Load(base, kOne);
  CopyWord(base, tracker + 8, frame + 168);
  Store(base, tracker + 12, one);
  // The two vectors (+1216, +1584) summed from 0.0 and halved.
  const double zero = Lfs(base, kZero);
  Stfs(base, frame + 92, zero);
  double x = zero, y = zero, z = zero;
  for (uint32_t i = 0; i < 2; i++) {
    vector_library::Vec4Copy(base, frame + 80, vehicle + 1216 + 368 * i);
    x = Fadds(x, Lfs(base, frame + 80));
    y = Fadds(y, Lfs(base, frame + 84));
    z = Fadds(z, Lfs(base, frame + 88));
  }
  Store(base, tracker + 44, one);
  const double half = Lfs(base, 0x82001BE4);
  Stfs(base, tracker + 32, Fmuls(x, half));
  Stfs(base, tracker + 36, Fmuls(y, half));
  Stfs(base, tracker + 40, Fmuls(z, half));
  // The body's rotation (+80) into +192 and the turn at +64.
  const uint32_t rotation = vehicle_accessors::RigidBodyField80(Load(base, vehicle + 336));
  CopyDoublewords(base, frame + 96, rotation, 2);
  CopyWords(base, tracker + 192, frame + 96, 4);
  StoreIdentity(base, turn);
  const uint64_t r4 = Load64(base, tracker + 192);
  const uint64_t r5 = Load64(base, tracker + 200);
  // Matrix::FromQuat's parameter save words (its caller's r1 + 32).
  Store64(base, frame + 32, r4);
  Store64(base, frame + 40, r5);
  const uint32_t q[4] = {uint32_t(r4 >> 32), uint32_t(r4), uint32_t(r5 >> 32), uint32_t(r5)};
  MatrixFromQuat(base, turn, q, (frame - 16) & ~0xFu);
  vehicle::TrackPositionCopy(base, tracker + 224, vehicle_accessors::VehicleIdentity(vehicle));
}

// --- The viewports' moves in the view's axes ---

void ViewMove(uint8_t* base, uint32_t viewport, uint32_t moved, double amount, uint32_t offset,
              uint32_t sp) {
  const double zero = Lfs(base, kZero);
  const double one = Lfs(base, kOne);
  if (amount < zero) {
    amount = zero;
  } else if (amount > one) {
    amount = one;
  }
  Stfs(base, sp - 48, zero);
  Stfs(base, sp - 40, zero);
  Stfs(base, sp - 16, zero);
  Stfs(base, sp - 12, zero);
  Stfs(base, sp - 8, zero);
  Stfs(base, sp - 44, one);
  Stfs(base, sp - 36, one);
  const double from = Lfs(base, viewport + moved);
  const double dx = Fsubs(Lfs(base, viewport + 1616), Lfs(base, viewport + 1600));
  const double dy = Fsubs(Lfs(base, viewport + 1620), Lfs(base, viewport + 1604));
  const double dz = Fsubs(Lfs(base, viewport + 1624), Lfs(base, viewport + 1608));
  Stfs(base, sp - 20, one);
  Stfs(base, sp - 32, dx);
  const simde__m128 up = Lvx(base, sp - 48);
  Stfs(base, sp - 28, dy);
  Stfs(base, sp - 24, dz);
  const double ox = Fmuls(Lfs(base, offset + 0), amount);
  const double oy = Fmuls(Lfs(base, offset + 4), amount);
  const double oz = Fmuls(Lfs(base, offset + 8), amount);
  const simde__m128 dir = Lvx(base, sp - 32);
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
  Stvx(base, sp - 16, right_raw);
  Stvx(base, sp - 16, v3);
  Stvx(base, sp - 48, v1);
  Stvx(base, sp - 32, v0);
  // The moved point (+moved) plus the offset along the three axes.
  const uint32_t m = viewport + moved;
  Stfs(base, m + 0, Fadds(from, Fmuls(Lfs(base, sp - 16), ox)));
  {
    const double f10 = Lfs(base, m + 4);
    Stfs(base, m + 4, Fadds(f10, Fmuls(Lfs(base, sp - 12), ox)));
  }
  {
    const double f0 = Fmuls(Lfs(base, sp - 8), ox);
    Stfs(base, m + 8, Fadds(Lfs(base, m + 8), f0));
  }
  for (uint32_t i = 0; i < 3; i++) {
    const double f0 = Fmuls(Lfs(base, sp - 48 + 4 * i), oy);
    Stfs(base, m + 4 * i, Fadds(Lfs(base, m + 4 * i), f0));
  }
  for (uint32_t i = 0; i < 3; i++) {
    const double f0 = Fmuls(Lfs(base, sp - 32 + 4 * i), oz);
    Stfs(base, m + 4 * i, Fadds(Lfs(base, m + 4 * i), f0));
  }
}

void ViewportOffset(uint8_t* base, uint32_t viewport, double amount, uint32_t offset,
                    uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821529B0 Viewport::Offset");
  ViewMove(base, viewport, 1616, amount, offset, sp);
}

void ViewportPullBack(uint8_t* base, uint32_t viewport, double amount, uint32_t offset,
                      uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82152788 Viewport::PullBack");
  ViewMove(base, viewport, 1600, amount, offset, sp);
}

// --- The shake ---

void ShakeRandom(uint8_t* base, uint32_t viewport, double amount, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82152BD8 Camera::ShakeRandom");
  constexpr uint32_t kGuard = 0x826D81C4, kStep = 0x826D81C0;
  const uint32_t guard = Load(base, kGuard);
  const double zero = Lfs(base, kZero);
  Stfs(base, frame + 96, zero);
  Stfs(base, frame + 100, zero);
  Stfs(base, frame + 104, zero);
  const double one = Lfs(base, kOne);
  Stfs(base, frame + 108, one);
  if ((guard & 1) == 0) {
    Store(base, kGuard, guard | 1);
    CopyWord(base, kStep, 0x8200939C);
  }
  if (amount < zero) {
    amount = zero;
  } else if (amount > one) {
    amount = one;
  }
  for (uint32_t i = 0; i < 2; i++) {
    const uint32_t drawn = race_overlays::Rand(base) % 100;
    StoreDoubleword(base, frame + 80, drawn);
    const double n = Single(double(int64_t(drawn)));
    Stfs(base, frame + 96 + 4 * i, Fmuls(n, Lfs(base, kStep)));
  }
  ViewportOffset(base, viewport, amount, frame + 96, frame);
}

void Shake(uint8_t* base, uint32_t camera, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82152CD8 Camera::Shake");
  const double speed = double(Vehicle_GetSpeedKmh(base, Load(base, camera + 16)));
  const uint32_t viewport = Load(base, camera + 20);
  const double over = Fsubs(speed, Lfs(base, 0x82001FB8));
  ShakeRandom(base, viewport, Fmuls(over, Lfs(base, 0x8200222C)), frame - 160);
}

// --- The intro camera ---

uint32_t IntroStepShot(uint8_t* base, uint32_t shot, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82152E78 IntroCamera::StepShot");
  StoreIdentity(base, frame + 80);
  const uint32_t body_matrix =
      vehicle_accessors::RigidBodyMatrix(Load(base, Load(base, shot + 4) + 336));
  CopyDoublewords(base, frame + 176, body_matrix, 8);
  uint32_t viewport = Load(base, shot + 0);
  const int32_t phase = int32_t(Load(base, shot + 16));
  CopyWords(base, frame + 80, frame + 176, 16);
  CopyWords(base, frame + 160, viewport + 1680, 4);
  CopyWords(base, frame + 144, viewport + 1696, 4);
  if (phase == 0 && Load(base, shot + 8) >= 29) {
    Store(base, shot + 16, 3);
  }
  matrix_math::Vec4Transform(base, viewport + 1600, frame + 80, frame + 160);
  viewport = Load(base, shot + 0);
  matrix_math::Vec4Transform(base, viewport + 1616, frame + 80, frame + 144);
  viewport = Load(base, shot + 0);
  CopyWords(base, viewport + 1632, viewport + 1712, 4);
  viewport = Load(base, shot + 0);
  CopyWords(base, viewport + 1664, viewport + 1744, 4);
  if (int32_t(Load(base, shot + 16)) == 3) {
    return 0;
  }
  if (int32_t(Load(base, 0x826A50D4)) == 0) {
    Store(base, shot + 8, Load(base, shot + 8) + 1);
  }
  return 1;
}

void IntroMakeShot(uint8_t* base, uint32_t shot, uint32_t viewport, uint32_t vehicle,
                   uint32_t frame, const c_runtime::AtexitCall& atexit) {
  ITC_NATIVE_RUNNING("sub_82154108 IntroCamera::MakeShot");
  constexpr uint32_t kGuard = 0x826D81F0;
  constexpr uint32_t kUp = 0x826D81E0;      // (0, 1, 0, 1)
  constexpr uint32_t kOffset = 0x826D81D0;  // (0.75, 1.8, 0, 1), probably
  const double zero = Lfs(base, kZero);
  Stfs(base, shot + 12, zero);
  Store(base, shot + 0, viewport);
  Store(base, shot + 4, vehicle);
  Store(base, shot + 8, 0);
  Store(base, shot + 16, 0);
  const uint32_t spec = vehicle_accessors::ObjectRecord1888(Load(base, vehicle + 5712));
  const double f0 = Lfs(base, 0x820029D8);
  const uint32_t r10 = Load(base, spec + 80);
  const double angle = std::bit_cast<double>(Load64(base, 0x820093A8));
  const uint32_t r11 = Load(base, spec + 76);
  StoreDoubleword(base, frame + 88, r10);
  StoreDoubleword(base, frame + 80, r11);
  const double f12 = Single(double(int64_t(r10)));
  const double f11 = Single(double(int64_t(r11)));
  const double half = Lfs(base, 0x82001BE4);
  const double len = Fmuls(f12, half);
  const double wid = Fmuls(f11, half);
  const double f29 = Fmadds(wid, f0, Lfs(base, 0x82008230));
  const double f28 = Fmsubs(len, f0, Lfs(base, 0x82002208));
  const double sin = Single(c_runtime::Sin(angle));
  const double cos = Single(c_runtime::Cos(angle));
  const double f27 = Fmadds(sin, f29, f28);
  uint32_t guard = Load(base, kGuard);
  const double one = Lfs(base, kOne);
  const double x = Fmuls(cos, f29);
  if ((guard & 1) == 0) {
    guard |= 1;
    Stfs(base, kUp + 0, zero);
    Stfs(base, kUp + 4, one);
    Stfs(base, kUp + 8, zero);
    Stfs(base, kUp + 12, one);
    Store(base, kGuard, guard);
    c_runtime::AtexitFrom(base, 0x825DB150, atexit);
    guard = Load(base, kGuard);
  }
  if ((guard & 2) == 0) {
    Store(base, kGuard, guard | 2);
    CopyWord(base, kOffset + 0, 0x8200295C);
    CopyWord(base, kOffset + 4, 0x82008290);
    Stfs(base, kOffset + 8, zero);
    Stfs(base, kOffset + 12, one);
    c_runtime::AtexitFrom(base, 0x825DB148, atexit);
  }
  uint32_t vp = Load(base, shot + 0);
  Stfs(base, vp + 1680, x);
  Stfs(base, vp + 1684, f27);
  Stfs(base, vp + 1688, zero);
  Stfs(base, vp + 1692, one);
  vp = Load(base, shot + 0);
  Stfs(base, vp + 1696, zero);
  Stfs(base, vp + 1700, f28);
  Stfs(base, vp + 1704, zero);
  Stfs(base, vp + 1708, one);
  vp = Load(base, shot + 0);
  CopyWords(base, vp + 1712, kUp, 4);
  vp = Load(base, shot + 0);
  CopyWords(base, vp + 1744, kOffset, 4);
  const uint32_t r88 = Load(base, spec + 88);
  StoreDoubleword(base, frame + 88, r88);
  const double n = Single(double(int64_t(r88)));
  Stfs(base, shot + 12, Fmuls(n, Lfs(base, 0x82002CCC)));
}

uint32_t IntroConstruct(uint8_t* base, uint32_t camera, uint32_t viewport, uint32_t vehicle,
                        uint32_t frame, const StagesCopyCall& stages_copy,
                        const c_runtime::AtexitCall& atexit) {
  ITC_NATIVE_RUNNING("sub_82158F70 IntroCamera::IntroCamera");
  const uint32_t tracker = camera + 32;
  const uint32_t list = camera + 352;
  const double one = Lfs(base, kOne);
  for (uint32_t at : {12u, 28u, 44u, 60u, 76u, 92u, 108u, 124u, 140u, 156u, 172u, 188u, 204u,
                      220u, 252u, 268u, 284u, 300u}) {
    Stfs(base, tracker + at, one);
  }
  Store(base, camera + 0, 0x820093C4);
  Store(base, camera + 748, 0);
  Store(base, camera + 752, 0);
  Store(base, camera + 756, 0);
  Store(base, camera + 16, vehicle);
  Store(base, camera + 20, viewport);
  ResetList16(base, list);
  // The stage's racers: their vehicles at frame + 80 (the count at +96),
  // the ones whose item is of kind 1 or 3 moved to the front.
  const uint32_t race = Load(base, kRaceData);
  for (uint32_t i = 0; i < 4; i++) {
    Store(base, frame + 80 + 4 * i, 0);
  }
  uint32_t count = 0;
  Store(base, frame + 96, count);
  const uint32_t stages = race + 0x59620;
  const uint32_t stage = Load(base, stages + 804);
  if (!vehicle_accessors::StagesCopy(base, frame + 144, stages, stage)) {
    stages_copy(frame + 144, stages, stage);
  }
  for (int32_t s = 0; s < int32_t(Load(base, frame + 220)); s++) {
    const uint32_t racer = Load(base, frame + 188 + 8 * s);
    const uint32_t record = uint32_t(uint64_t(racer) * 3936) + Load(base, kRaceData);
    if (((Load(base, record + 3936) >> 1) & 1) == 0) {
      continue;
    }
    uint32_t found_vehicle = 0;
    bool kind_flag = false;
    for (int32_t n = 0; n < int32_t(track_collision::RaceRacerCount(base)); n++) {
      const uint32_t entry = Load(base, Load(base, 0x826D82F4) + 2308 + 4 * uint32_t(n));
      if (int32_t(racer) == int32_t(Load(base, entry + 24))) {
        found_vehicle = vehicle_accessors::VehicleGet(base, Load(base, entry + 20));
        kind_flag = (Load(base, entry + 0) & 0x400) != 0;
        break;
      }
    }
    Store(base, ((count << 2) & 0xFFFFFFFCu) + frame + 80, found_vehicle);
    uint32_t r11 = 0;
    bool front = false;
    if (kind_flag) {
      const int32_t id = int32_t(Load(base, record + 3960));
      if (id >= 0) {
        const int32_t kind = LoadS16(base, data_tables::Item(base, uint32_t(id)) + 88);
        if (kind == 1 || kind == 3) {
          front = true;
          r11 = Load(base, frame + 96);
          int32_t k = int32_t(r11);
          if (k > 0) {
            uint32_t at = ((r11 << 2) & 0xFFFFFFFCu) + frame + 80;
            for (; k > 0; k--) {
              Store(base, at, Load(base, at - 4));
              at -= 4;
            }
            r11 = Load(base, frame + 96);
          }
          Store(base, frame + 80, found_vehicle);
        }
      }
    }
    if (!front) {
      r11 = Load(base, frame + 96);
    }
    count = r11 + 1;
    Store(base, frame + 96, count);
  }
  // A shot made and added for each.
  if (int32_t(count) > 0) {
    uint32_t at = frame + 80;
    for (uint32_t n = count; n != 0; n--) {
      IntroMakeShot(base, frame + 112, viewport, Load(base, at), frame - 192, atexit);
      List16Insert20(base, list, frame + 112, uint32_t(LoadS16(base, list + 72)));
      at += 4;
    }
  }
  return camera;
}

// --- The director's start and release (handlers) ---

void DirectorStart(uint8_t* base, uint32_t director, const std::function<void()>& viewports_init) {
  ITC_NATIVE_RUNNING("sub_82152338 CameraDirector::Start");
  viewports_init();
  const uint32_t viewports = Load(base, kViewports);
  const uint32_t first = Load(base, viewports + 0);
  const uint32_t second = Load(base, viewports + 4);
  cameras::ViewportUpdate(base, first);
  const double near = Lfs(base, 0x8200295C);
  cameras::SetNear(base, near);
  const double zoom = Lfs(base, kOne);
  cameras::ViewportSetZoom(base, first, zoom);
  const double far = Lfs(base, 0x82001FCC);
  Stfs(base, first + 72, far);
  cameras::ViewportUpdate(base, second);
  cameras::SetNear(base, near);
  cameras::ViewportSetZoom(base, second, zoom);
  Stfs(base, second + 72, far);
  Store(base, director + kFollowed, 0);
  Store(base, director + 13348, 2);
  Store(base, director + kFollowedInput, 0);
  AnimsClear(base);
}

void DirectorRelease(uint8_t* base, uint32_t director, const std::function<void()>& free_all,
                     const std::function<void()>& release_viewports) {
  ITC_NATIVE_RUNNING("sub_82153098 CameraDirector::Release");
  free_all();
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0) + 131072 - 1956;
  const int32_t kind = int32_t(Load(base, Load(base, kRaceData) + 0xA3AF0));
  if (kind == 1 || kind == 2 || kind == 3) {
    uint32_t view = Load(base, director + kView);
    if (int32_t(view) == 4 || int32_t(view) == 5) {
      view = Load(base, director + kViewKept);
    }
    if (view < 3) {
      Store(base, save + 24, view == 0 ? 2 : view == 1 ? 1 : 0);
    }
    Store(base, director + 9624, view);
  }
  release_viewports();
}

// --- The cutscene's shots ---

void CutsceneInit(uint8_t* base, uint32_t shot, uint32_t entry, uint32_t frame,
                  const CutsceneCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82156B68 CutsceneShot::Init");
  const uint32_t list = shot + 12;
  const uint32_t kind = Load(base, entry + 0);
  Store(base, frame + 80, 22824);
  Store(base, frame + 84, 22825);
  Store(base, shot + 0, kind);
  for (uint32_t i = 2; i < 16; i++) {
    Store(base, frame + 80 + 4 * i, 22824 + i);
  }
  Store(base, shot + 4, Load(base, ((kind << 2) & 0xFFFFFFFCu) + frame + 80));
  Store(base, shot + 8, Load(base, entry + 4));
  ResetList16(base, list);
  Store(base, shot + 600, 0);
  Store(base, shot + 604, 1);
  for (uint32_t i = 0; i < uint32_t(LoadS16(base, entry + 80)); i++) {
    uint32_t from = 0;
    if (!List16Copy32(base, frame + 80, entry + 8, i, &from)) {
      calls.copy32(frame + 80, from, 32);
    }
    uint32_t to = 0;
    if (!List16Insert(base, list, frame + 80, uint32_t(LoadS16(base, list + 72)), 32, &to)) {
      calls.insert32(to, frame + 80, 32);
    }
  }
  calls.load(Load(base, shot + 4));
}

uint32_t CutsceneStep(uint8_t* base, uint32_t shot, uint32_t out, uint32_t frame,
                      const std::function<void(uint32_t object)>& camera_original, double* f1) {
  ITC_NATIVE_RUNNING("sub_82155800 CutsceneShot::Step");
  const uint32_t one = Load(base, kOne);
  Store(base, frame + 156, one);
  Store(base, frame + 172, one);
  std::optional<double> camera_f1;
  const uint32_t found = AnimsEvaluate(base, frame + 144, Load(base, shot + 4),
                                       Load(base, shot + 600), camera_original, &camera_f1);
  if (camera_f1) {
    *f1 = *camera_f1;
  }
  if (found == 0) {
    Store(base, shot + 604, 4);
    return 4;
  }
  Store(base, shot + 604, (Load(base, shot + 604) & 0xFFFFFFDAu) | 8);
  const uint32_t body_matrix =
      vehicle_accessors::RigidBodyMatrix(Load(base, Load(base, shot + 8) + 336));
  CopyDoublewords(base, frame + 192, body_matrix, 8);
  CopyWords(base, frame + 80, frame + 192, 16);
  const double flip = Lfs(base, 0x82001BE0);
  Stfs(base, frame + 168, Fmuls(Lfs(base, frame + 168), flip));
  Stfs(base, frame + 152, Fmuls(Lfs(base, frame + 152), flip));
  matrix_math::Vec3SwapXZ(base, frame + 160);
  matrix_math::Vec3SwapXZ(base, frame + 144);
  matrix_math::Vec4Transform(base, out + 16, frame + 80, frame + 160);
  matrix_math::Vec4Transform(base, out + 32, frame + 80, frame + 144);
  Stfs(base, out + 64, Fmuls(Lfs(base, frame + 176), Lfs(base, 0x82002948)));
  CopyWord(base, out + 68, frame + 180);
  // The cues (the list at +12, its count +84).
  const uint32_t list = shot + 12;
  for (uint32_t i = 0; i < uint32_t(LoadS16(base, shot + 84)); i++) {
    const uint32_t node = Walk16(base, list, i);
    const uint32_t slot = uint32_t(LoadS16(base, node + 2));
    FadeCueStep(base, ((slot << 5) & 0xFFFFFFE0u) + list + 76);
  }
  if ((Load(base, shot + 604) & 0x10) == 0) {
    Store(base, shot + 600, Load(base, shot + 600) + 1);
  }
  uint32_t r3 = 0;
  const double duration = AnimsDuration(base, Load(base, shot + 4), &r3);
  *f1 = duration;
  const int32_t time = int32_t(Load(base, shot + 600));
  StoreDoubleword(base, frame + 256, uint64_t(int64_t(time)));
  if (!(duration > Single(double(int64_t(time))))) {
    Store(base, shot + 604, 32);
  }
  return Load(base, shot + 604);
}

}  // namespace camera_modes
