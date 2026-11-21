#include "cameras/cameras.h"

#include "guest.h"

#include "base/log.h"
#include "c_runtime/exit.h"
#include "c_runtime/math.h"
#include "c_runtime/strings.h"
#include "command_list/command_list.h"
#include "graphics/resources.h"
#include "graphics/video_output.h"
#include "interpolation/blend_state.h"
#include "matrix_math/matrix_math.h"
#include "memory/heap.h"
#include "schema/camera_modes.h"
#include "schema/cameras.h"
#include "schema/command_list.h"
#include "schema/post_effects.h"
#include "schema/video_output.h"

#include "base/float_mode.h"
#include <simde/x86/avx.h>
#include <simde/x86/sse4.1.h>

#include <bit>
#include <climits>
#include <cmath>

namespace cameras {

namespace {

using Fpscr = ::base::FloatMode;
// A camera's fields (+0 to +399) are CameraPass's first ones: the view serves
// any camera (a pass or AppCameraCamera).
using Cam = schema::CameraPass;
using G = schema::CamerasGlobals;
// Other areas' objects: the viewports (camera-modes.toml), the command list
// (command-list.toml), the clear item (post-effects.toml), the video output
// (video-output.toml).
using Viewport = schema::Viewport;
using Entry = schema::ViewportEntry;
using List = schema::CommandList;
using ListG = schema::CommandListGlobals;
using ClearItem = schema::ClearItem;
using VideoOutput = schema::VideoOutput;

constexpr uint32_t kOne = 0x82000A30;       // the game's float 1.0
constexpr uint32_t kHalf = 0x82001BE4;      // 0.5
constexpr uint32_t kTwoPi = 0x82001FA8;     // 2 pi
constexpr uint32_t kOverTwoPi = 0x82001FA4;  // 1 / (2 pi)
constexpr uint32_t kRadiansPerDegree = 0x82002948;

// --- Scalar steps, as the translated code's doubles ---

double Lfs(uint8_t* base, uint32_t address) {
  return double(guest::LoadFloat(base, address));
}
void Stfs(uint8_t* base, uint32_t address, double value) {
  guest::StoreFloat(base, address, float(value));
}
uint32_t Bits(double value) {
  return std::bit_cast<uint32_t>(float(value));
}
double Fmuls(double a, double b) {
  return double(float(a * b));
}
double Fadds(double a, double b) {
  return double(float(a + b));
}
double Fsubs(double a, double b) {
  return double(float(a - b));
}
double Fdivs(double a, double b) {
  return double(float(a / b));
}
double Fnmsubs(double a, double b, double c) {
  return double(float(-std::fma(a, b, -c)));
}
// fneg: the sign bit flipped.
double Fneg(double a) {
  return std::bit_cast<double>(std::bit_cast<uint64_t>(a) ^ 0x8000000000000000ull);
}

// An angle into [-pi, pi], as the library does it everywhere: pi with the
// angle's sign (its stored float's sign bit ORed into 0x40490FDB), added,
// times 1 / (2 pi), truncated (fctiwz: NaN gives INT_MIN, INT_MAX from
// INT_MAX up), that many 2 pi taken off (fnmsubs).
double WrapAngle(uint8_t* base, double x) {
  const double two_pi = Lfs(base, kTwoPi);
  const double pi = double(std::bit_cast<float>((Bits(x) & 0x80000000u) | 0x40490FDBu));
  const double turns = Fmuls(Fadds(pi, x), Lfs(base, kOverTwoPi));
  int32_t n;
  if (std::isnan(turns)) {
    n = INT32_MIN;
  } else if (turns >= double(INT_MAX)) {
    n = INT32_MAX;
  } else {
    n = simde_mm_cvttsd_si32(simde_mm_set_sd(turns));
  }
  return Fnmsubs(double(float(double(int64_t(n)))), two_pi, x);
}

// --- The vector unit, as the translation does it ---

// Vectors as the translated code holds them: word 0 in the top lane.
simde__m128 VectorOf(uint32_t w0, uint32_t w1, uint32_t w2, uint32_t w3) {
  return simde_mm_castsi128_ps(
      simde_mm_set_epi32(int32_t(w0), int32_t(w1), int32_t(w2), int32_t(w3)));
}
// lvx: the aligned 16 bytes holding `address`.
simde__m128 Lvx(uint8_t* base, uint32_t address) {
  const uint32_t at = address & ~0xFu;
  return VectorOf(guest::LoadU32(base, at + 0), guest::LoadU32(base, at + 4),
                  guest::LoadU32(base, at + 8), guest::LoadU32(base, at + 12));
}
// Word i (0 the top lane).
uint32_t WordOf(simde__m128 v, int i) {
  alignas(16) uint32_t lanes[4];
  simde_mm_store_si128(reinterpret_cast<simde__m128i*>(lanes), simde_mm_castps_si128(v));
  return lanes[3 - i];
}
double Word0(simde__m128 v) {
  return double(std::bit_cast<float>(WordOf(v, 0)));
}
// stvx: to the aligned 16 bytes holding `address`.
void Stvx(uint8_t* base, uint32_t address, simde__m128 v) {
  const uint32_t at = address & ~0xFu;
  for (int i = 0; i < 4; i++) {
    guest::StoreU32(base, at + 4 * i, WordOf(v, i));
  }
}
// vspltw: one word in every lane (the shuffle's immediate).
template <int kImm>
simde__m128 Splat(simde__m128 v) {
  return simde_mm_castsi128_ps(simde_mm_shuffle_epi32(simde_mm_castps_si128(v), kImm));
}
simde__m128 Mul(simde__m128 a, simde__m128 b) {
  return simde_mm_mul_ps(a, b);
}
simde__m128 Dot(simde__m128 a, simde__m128 b) {
  return simde_mm_dp_ps(a, b, 0xFF);
}

simde__m128 Sub(simde__m128 a, simde__m128 b) {
  return simde_mm_sub_ps(a, b);
}
simde__m128 Add(simde__m128 a, simde__m128 b) {
  return simde_mm_add_ps(a, b);
}
// vnmsubfp128 as the translation does it: -((a * b) - c).
simde__m128 NegMulSub(simde__m128 a, simde__m128 b, simde__m128 c) {
  return simde_mm_xor_ps(Sub(Mul(a, b), c), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000))));
}
// vmsum3fp128: x, y and z multiplied and summed, in every lane.
simde__m128 Dot3(simde__m128 a, simde__m128 b) {
  return simde_mm_dp_ps(a, b, 0xEF);
}
// The 0.5 the translation makes from vspltisw 1 (vcfsx's scale).
simde__m128 Half() {
  return simde_mm_mul_ps(simde_mm_cvtepi32_ps(simde_mm_set1_epi32(1)),
                         simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x3F000000))));
}
template <int kImm>
simde__m128 Shuffle(simde__m128 v) {
  return simde_mm_castsi128_ps(simde_mm_shuffle_epi32(simde_mm_castps_si128(v), kImm));
}
// The library's vector normalize, as the translation does it: the length
// squared (x, y, z) d, r = 1 / sqrt(d) (vrsqrtefp), one Newton step (r +=
// r (0.5 - d r r / 2)... as -((d / 2) r r - 0.5) r + r), v r; where d is 0,
// d itself (vsel on vcmpeqfp).
simde__m128 Normalize(simde__m128 v) {
  const simde__m128 d = Dot3(v, v);
  simde__m128 r = simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(d));
  const simde__m128 t = Mul(d, Half());
  const simde__m128 zero = simde_mm_cmpeq_ps(d, simde_mm_setzero_ps());
  const simde__m128 k = NegMulSub(t, Mul(r, r), Half());
  r = Add(Mul(r, k), r);
  const simde__m128 n = Mul(v, r);
  return simde_mm_castsi128_ps(simde_mm_or_si128(
      simde_mm_andnot_si128(simde_mm_castps_si128(zero), simde_mm_castps_si128(n)),
      simde_mm_and_si128(simde_mm_castps_si128(zero), simde_mm_castps_si128(d))));
}
// A word through lfs and stfs (a float register): its bits, as the
// hardware keeps them (the translated code's conversions there are folded
// away, a signaling NaN kept).
uint32_t Through(uint32_t word) {
  return word;
}

// Flush-to-zero on while alive (the vector unit's mode), put back after.
class FlushToZero {
 public:
  FlushToZero() : csr_(Fpscr::getcsr()) { Fpscr::setcsr(csr_ | uint32_t(Fpscr::FlushMask)); }
  ~FlushToZero() { Fpscr::setcsr(csr_); }

 private:
  uint32_t csr_;
};

}  // namespace

void SetFieldOfView(uint8_t* base, uint32_t camera, double degrees) {
  ITC_NATIVE_RUNNING("sub_82593278 Camera::SetFieldOfView");
  const double radians = Fmuls(degrees, Lfs(base, kRadiansPerDegree));
  Stfs(base, Cam(base, camera).field_of_view_address(), WrapAngle(base, radians));
}

void BuildProjection(uint8_t* base, uint32_t camera, uint32_t fov, uint32_t aspect, uint32_t far,
                     uint32_t near) {
  ITC_NATIVE_RUNNING("sub_825932E8 Camera::BuildProjection");
  const uint32_t p = Cam(base, camera).projection_address();
  const simde__m128 zero = simde_mm_setzero_ps();
  // vupkd3d128's (3.0, 3.0, 0.0, 1.0), turned into (0, 0, 0, 1).
  simde__m128 v13 = simde_mm_castsi128_ps(
      simde_mm_shuffle_epi32(simde_mm_castps_si128(simde_mm_set_ps(3.0f, 3.0f, 0.0f, 1.0f)), 0x54));
  Stvx(base, p, zero);
  Stvx(base, p + 16, zero);
  Stvx(base, p + 48, zero);
  const uint32_t one = Bits(Lfs(base, kOne));
  Stvx(base, p + 32, v13);
  const double half = Fmuls(Lfs(base, fov), Lfs(base, kHalf));
  const double a = Lfs(base, aspect);
  const double x = WrapAngle(base, half);
  const double x2 = Fmuls(x, x);
  const double x3 = Fmuls(x2, x);
  const double n = Lfs(base, near);
  const double f = Lfs(base, far);
  // The polynomials: (1, x, x^2, x^3) and its powers dotted with the
  // coefficients, the translation's steps in its order.
  simde__m128 v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12;
  {
    FlushToZero flush;
    v11 = VectorOf(one, Bits(x), Bits(x2), Bits(x3));
    v12 = Mul(v11, v11);
    v9 = Splat<0x55>(v11);
    v11 = Splat<0xAA>(v11);
    v6 = Lvx(base, 0x82000A00);
    v8 = Splat<0x00>(v12);
    v10 = Mul(v12, v11);
    v5 = Lvx(base, 0x820009D0);
    v11 = Mul(v8, v9);
    v4 = Lvx(base, 0x82000A10);
    v3 = Lvx(base, 0x820009E0);
    v9 = Mul(v12, v11);
    v2 = Lvx(base, 0x820009F0);
    v8 = Mul(v10, v11);
    v7 = Mul(v9, v11);
    v6 = Dot(v9, v6);
    v11 = Mul(v8, v11);
    v8 = Dot(v8, v5);
    v9 = Lvx(base, 0x820009C0);
    v7 = Dot(v7, v4);
    v12 = Dot(v12, v2);
    v10 = Dot(v10, v9);
  }
  const double depth = Fdivs(n, Fsubs(n, f));
  simde__m128 v12b;
  {
    FlushToZero flush;
    v12b = Dot(v11, v3);
  }
  const double w = Fneg(Fmuls(f, depth));
  const double sine_sum = Fadds(Fadds(Word0(v7), Word0(v6)), Word0(v12));
  const double cosine_sum = Fadds(Fadds(Word0(v12b), Word0(v8)), Word0(v10));
  const double y_scale = Fdivs(sine_sum, cosine_sum);
  const double x_scale = Fdivs(y_scale, a);
  // The four scales put into the rows (vrlimi128).
  const simde__m128 s = VectorOf(Bits(x_scale), Bits(y_scale), Bits(depth), Bits(w));
  const simde__m128 row0 = simde_mm_blend_ps(zero, simde_mm_permute_ps(s, 228), 8);
  const simde__m128 row1 = simde_mm_blend_ps(zero, simde_mm_permute_ps(s, 228), 4);
  v13 = simde_mm_blend_ps(v13, simde_mm_permute_ps(s, 228), 2);
  const simde__m128 row3 = simde_mm_blend_ps(zero, simde_mm_permute_ps(s, 147), 2);
  Stvx(base, p, row0);
  Stvx(base, p + 16, row1);
  Stvx(base, p + 32, v13);
  Stvx(base, p + 48, row3);
}

void SetViewportSize(uint8_t* base, uint32_t camera, uint32_t width, uint32_t height) {
  ITC_NATIVE_RUNNING("sub_82593500 Camera::SetViewportSize");
  const double w = double(float(double(int64_t(uint64_t(width)))));
  const Cam c(base, camera);
  Stfs(base, c.width_address(), w);
  const double h = double(float(double(int64_t(uint64_t(height)))));
  Stfs(base, c.height_address(), h);
  Stfs(base, c.aspect_address(), Fdivs(w, h));
  BuildProjection(base, camera, c.field_of_view_address(), c.aspect_address(), c.far_address(),
                  c.near_address());
}

namespace {

simde__m128 Unpackhi(simde__m128 a, simde__m128 b) {
  return simde_mm_castsi128_ps(
      simde_mm_unpackhi_epi32(simde_mm_castps_si128(a), simde_mm_castps_si128(b)));
}
simde__m128 Unpacklo(simde__m128 a, simde__m128 b) {
  return simde_mm_castsi128_ps(
      simde_mm_unpacklo_epi32(simde_mm_castps_si128(a), simde_mm_castps_si128(b)));
}

}  // namespace

void Update(uint8_t* base, uint32_t camera, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82593568 Camera::Update");
  const Cam c(base, camera);
  // The identity at frame + 112 (vupkd3d128's (0, 0, 0, 1) shuffled into
  // each row), turned by the negated angles.
  const uint32_t m = frame + 112;
  for (uint32_t row = 0; row < 4; row++) {
    for (uint32_t column = 0; column < 4; column++) {
      guest::StoreU32(base, m + 16 * row + 4 * column, row == column ? 0x3F800000u : 0);
    }
  }
  const uint32_t angle = frame + 80, turn = frame - 80;
  Stfs(base, angle, Fneg(Lfs(base, c.angles_y_address())));
  matrix_math::RotateY(base, m, m, angle, turn);
  Stfs(base, angle, Fneg(Lfs(base, c.angles_x_address())));
  matrix_math::RotateX(base, m, m, angle, turn);
  Stfs(base, angle, Fneg(Lfs(base, c.angles_z_address())));
  matrix_math::RotateZ(base, m, m, angle, turn);
  // The position (+272) dotted with the turned matrix's columns, as the
  // translation's merges and dot products do it, at frame + 96; its x, y
  // and z negated into the fourth row.
  {
    simde__m128 v9 = Lvx(base, m + 48), v11 = Lvx(base, m + 32), v8 = Lvx(base, m + 16),
                v10 = Lvx(base, m);
    simde__m128 v12 = Unpackhi(v9, v8);
    simde__m128 v13 = Unpackhi(v11, v10);
    v11 = Unpacklo(v11, v10);
    v10 = Unpacklo(v9, v8);
    v9 = Unpackhi(v12, v13);
    v13 = Unpacklo(v12, v13);
    simde__m128 v0 = Lvx(base, c.position_x_address());
    v12 = Unpackhi(v10, v11);
    v11 = Unpacklo(v10, v11);
    {
      FlushToZero flush;
      v10 = Dot(v0, v9);
      v13 = Dot(v0, v13);
      v12 = Dot(v0, v12);
      v0 = Dot(v0, v11);
    }
    v12 = Unpackhi(v12, v10);
    v0 = Unpackhi(v0, v13);
    v0 = Unpackhi(v0, v12);
    Stvx(base, frame + 96, v0);
  }
  for (uint32_t i = 0; i < 12; i += 4) {
    Stfs(base, m + 48 + i, Fneg(Lfs(base, frame + 96 + i)));
  }
  matrix_math::Copy(base, c.view_built_address(), m);
  interpolation::BlendState_OnCopy(base, c.view_built_address(), m, 0x825936C4);
  matrix_math::Copy(base, c.view_address(), c.view_built_address());
  interpolation::BlendState_OnCopy(base, c.view_address(), c.view_built_address(), 0x825936D4);
  Stfs(base, c.aspect_address(), Fdivs(Lfs(base, c.width_address()), Lfs(base, c.height_address())));
  BuildProjection(base, camera, c.field_of_view_address(), c.aspect_address(), c.far_address(),
                  c.near_address());
  matrix_math::Multiply(base, c.view_projection_address(), c.view_address(), c.projection_address());
  interpolation::BlendState_OnMultiply(base, c.view_projection_address(), c.view_address(),
                                       c.projection_address(), 0x82593798);
  // +372: the half field of view's polynomials, as BuildProjection's (the
  // translation's steps in its order; the original's stack words held here).
  const double half = Lfs(base, kHalf);
  const uint32_t one = Bits(Lfs(base, kOne));
  const double x = WrapAngle(base, Fmuls(Lfs(base, c.field_of_view_address()), half));
  const double x2 = Fmuls(x, x);
  const double x3 = Fmuls(x2, x);
  const simde__m128 ca00 = Lvx(base, 0x82000A00), ca10 = Lvx(base, 0x82000A10),
                    c9f0 = Lvx(base, 0x820009F0), c9c0 = Lvx(base, 0x820009C0),
                    c9d0 = Lvx(base, 0x820009D0), c9e0 = Lvx(base, 0x820009E0);
  simde__m128 s80, s96, s176, s192, s208, s224;
  {
    FlushToZero flush;
    simde__m128 v13 = VectorOf(one, Bits(x), Bits(x2), Bits(x3));
    simde__m128 v0 = Mul(v13, v13);
    const simde__m128 v5 = Splat<0x55>(v13);
    v13 = Splat<0xAA>(v13);
    simde__m128 v12 = Mul(v0, v13);
    s208 = Dot(v0, c9f0);
    const simde__m128 v4 = Splat<0x00>(v0);
    s192 = Dot(v12, c9c0);
    v13 = Mul(v4, v5);
    v0 = Mul(v0, v13);
    v12 = Mul(v12, v13);
    const simde__m128 v9 = Mul(v0, v13);
    s96 = Dot(v0, ca00);
    v13 = Mul(v12, v13);
    s224 = Dot(v12, c9d0);
    s176 = Dot(v13, c9e0);
    s80 = Dot(v9, ca10);
  }
  double f13 = Fadds(Word0(s80), Word0(s96));
  f13 = Fadds(f13, Word0(s208));
  double f12 = Fadds(Word0(s176), Word0(s224));
  const double h = Fmuls(Lfs(base, c.height_address()), half);
  f12 = Fadds(f12, Word0(s192));
  Stfs(base, c.view_scale_address(), Fmuls(Fdivs(h, f12), f13));
}

void Construct(uint8_t* base, uint32_t camera, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82593908 Camera::Camera");
  const double zero = Lfs(base, 0x82000A34);
  const double one = Lfs(base, kOne);
  const Cam c(base, camera);
  c.set_vtable(0x82001EC0);
  // The fourth word of every row from the view (+16) to +367.
  for (uint32_t at = c.view_address() + 12; at < c.field_of_view_address(); at += 16) {
    Stfs(base, at, one);
  }
  // The position and the angles (through the original's stack, as words).
  const uint32_t position[4] = {Bits(zero), Bits(Lfs(base, 0x8200277C)),
                                Bits(Lfs(base, 0x82008E48)), Bits(one)};
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU32(base, c.position_x_address() + 4 * i, position[i]);
    guest::StoreU32(base, c.angles_x_address() + 4 * i, i == 3 ? Bits(one) : Bits(zero));
  }
  Stfs(base, c.near_address(), Lfs(base, 0x82001FBC));
  Stfs(base, c.far_address(), Lfs(base, 0x82001FC8));
  Stfs(base, c.width_address(), Lfs(base, 0x82009EB8));
  Stfs(base, c.height_address(), Lfs(base, 0x8200B5CC));
  Stfs(base, c.aspect_address(), Lfs(base, 0x82052CFC));
  Stfs(base, c.field_of_view_address(), WrapAngle(base, Lfs(base, 0x82001FC4)));
  BuildProjection(base, camera, c.field_of_view_address(), c.aspect_address(), c.far_address(),
                  c.near_address());
  Update(base, camera, frame - 320);
}

uint32_t Mode(uint8_t* base, int32_t which) {
  ITC_NATIVE_RUNNING("sub_8209F718 CameraPasses::Mode");
  return which == 1 ? G::mode(base) : 0xFFFFFFFFu;
}

bool SetMode(uint8_t* base, int32_t which, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_8209F738 CameraPasses::SetMode");
  if (which != 1) {
    return true;
  }
  const uint32_t pass = Pass(4);
  const bool sets_view = mode == 0 || mode == 1;
  if (sets_view && Cam(base, pass).vtable().set_field_of_view() != 0x82593278) {
    return false;
  }
  G::set_mode(base, mode);
  if (sets_view) {
    SetFieldOfView(base, pass, Lfs(base, mode == 0 ? 0x82001E8C : 0x82001E88));
  }
  return true;
}

uint32_t RenderTarget(uint8_t* base, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_8209F7A8 CameraPasses::RenderTarget");
  static constexpr uint32_t kTextures[] = {
      G::render_target_0_address(),  0,
      G::render_target_2_address(),  G::render_target_3_address(),
      G::render_target_4_address(),  G::render_target_5_address(),
      G::render_target_6_address(),  G::render_target_7_address(),
      G::render_target_8_address(),  G::render_target_9_address(),
      G::render_target_10_address(), G::render_target_11_address(),
      G::render_target_12_address(),
  };
  if (id > 12) {
    return 0;
  }
  if (id == 1) {
    const uint32_t mode = G::mode(base);
    return mode == 0 ? G::render_target_1_2d(base)
           : mode == 1 ? G::render_target_1_cube(base)
                       : 0;
  }
  return guest::LoadU32(base, kTextures[id]);
}

uint32_t MakeRenderTarget(uint8_t* base, uint32_t frame, uint32_t id, const DeviceCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8209FBF0 CameraPasses::MakeRenderTarget");
  struct Target {
    uint32_t size;
    uint32_t format;
    uint32_t global;
  };
  // Ids 0 and 2 to 12 (id 0 is 512 x 128; id 1 below).
  static constexpr Target kTargets[] = {
      {512, 0x182801B6, G::render_target_0_address()},
      {0, 0, 0},
      {1024, 0x1A220197, G::render_target_2_address()},
      {512, 0x18280186, G::render_target_3_address()},
      {1024, 0x18280186, G::render_target_4_address()},
      {512, 0x1A220197, G::render_target_5_address()},
      {256, 0x18280186, G::render_target_6_address()},
      {256, 0x18280186, G::render_target_7_address()},
      {512, 0x1A220197, G::render_target_8_address()},
      {256, 0x18280186, G::render_target_9_address()},
      {256, 0x18280186, G::render_target_10_address()},
      {256, 0x18280186, G::render_target_11_address()},
      {1024, 0x1A220197, G::render_target_12_address()},
  };
  constexpr uint32_t kCubeSize = 0x82001EF0;  // id 1's size (a word)
  const uint32_t local = frame + 80;
  guest::StoreU32(base, local,
                  graphics::video_output::LockDevice(base, calls.video_output(0)));
  if (const uint32_t old = calls.render_target(id); old != 0) {
    calls.release(old);
  }
  if (id == 1) {
    const int32_t mode = int32_t(G::mode(base));
    const uint32_t size = guest::LoadU32(base, kCubeSize);
    if (mode == 0) {
      G::set_render_target_1_2d(
          base, graphics::CreateTexture(base, size, size, 1, 1, 1, 0x182801B6, 3));
    } else if (mode == 1) {
      G::set_render_target_1_cube(
          base, graphics::CreateTexture(base, size, size, 6, 1, 1, 0x182801B6, 18));
    }
  } else if (id <= 12) {
    const Target& t = kTargets[id];
    const uint32_t height = id == 0 ? 128 : t.size;
    guest::StoreU32(base, t.global,
                    graphics::CreateTexture(base, t.size, height, 1, 1, 1, t.format, 3));
  }
  const uint32_t video_output = calls.video_output(1);
  graphics::video_output::UnlockDevice(base, video_output, local);
  return video_output;
}

void SetUpSurfaces(uint8_t* base, uint32_t frame, uint32_t pass, const SurfaceCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820A2F40 CameraPass::SetUpSurfaces");
  namespace gr = graphics;
  namespace vo = graphics::video_output;
  const uint32_t local = frame + 80;
  guest::StoreU32(base, local, 0);
  calls.release_surfaces();
  const Cam p(base, pass);
  const uint32_t type = p.type();
  uint32_t color = 0;
  uint32_t depth = 0;
  if (type <= 2) {
    guest::StoreU32(base, local, vo::LockDevice(base, calls.video_output(0)));
    color = gr::GetRenderTarget(base, guest::LoadU32(base, local), 0);
    depth = gr::GetDepthStencilSurface(base, guest::LoadU32(base, local));
    vo::UnlockDevice(base, calls.video_output(1), local);
  } else if (type >= 5 && type <= 9) {
    if (Mode(base, 1) == 1) {
      const uint32_t surface = calls.cube_surface(calls.render_target(1), type - 4);
      const Cam pass4(base, Pass(4));
      color = type == 5 ? pass4.color_surface().address : surface;
      depth = pass4.depth_surface().address;
    }
  } else if (type <= 20) {
    // Types 3, 4 and 10 to 20: the render target's id, and whether the
    // depth surface's parameters are {0, -1, 0} (else after the color one).
    static constexpr uint32_t kIds[21] = {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2,
                                          3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    const bool fixed = type == 10 || type == 13 || type == 16 || type == 20;
    const int32_t samples = type == 3 || type == 4 || type == 19 ? 2 : 0;
    const uint32_t format = type == 4 ? 0x1A2201BF : 0x18280186;
    const uint32_t id = kIds[type];
    calls.make_render_target(id);
    if (type == 4) {
      const uint32_t mode = Mode(base, 1);
      if (mode == 0) {
        gr::GetSurfaceLevel(base, calls.render_target(1), 0);
      } else if (mode == 1) {
        calls.cube_surface(calls.render_target(1), 0);
      }
    } else {
      gr::GetSurfaceLevel(base, calls.render_target(id), 0);
    }
    guest::StoreU32(base, local, vo::LockDevice(base, calls.video_output(0)));
    const uint32_t color_parameters = frame + 440;
    const uint32_t depth_parameters = frame + 120;
    for (uint32_t i = 0; i < 12; i += 4) {
      guest::StoreU32(base, color_parameters + i, 0);
    }
    color = gr::CreateSurface(base, p.viewport_width(), p.viewport_height(), format, samples,
                              color_parameters);
    for (uint32_t i = 0; i < 12; i += 4) {
      guest::StoreU32(base, depth_parameters + i, 0);
    }
    if (fixed) {
      guest::StoreU32(base, depth_parameters + 4, 0xFFFFFFFF);
    } else {
      guest::StoreU32(base, depth_parameters + 0,
                      schema::CameraSurface(base, color).edram_bytes() / 5120);
    }
    depth = gr::CreateSurface(base, p.viewport_width(), p.viewport_height(), 0x1A220197, samples,
                              depth_parameters);
    vo::UnlockDevice(base, calls.video_output(1), local);
  }
  p.set_color_surface(color);
  p.set_depth_surface(depth);
}

uint32_t InitPass(uint8_t* base, uint32_t pass, uint32_t type, const PassInitCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820A3DB0 CameraPass::Init");
  // The game's float constants (words at 0x82000000 + offset), copied as
  // the original's lfs and stfs do (none is a NaN).
  auto constant = [&](uint32_t offset) { return guest::LoadU32(base, 0x82000000 + offset); };
  // A word into a field (the constants' bits into the float ones, as words).
  auto store = [&](uint32_t field, uint32_t value) { guest::StoreU32(base, field, value); };
  const Cam p(base, pass);
  p.set_color_surface(0);
  p.set_depth_surface(0);
  calls.construct();
  if (type <= 20) {
    uint32_t width = 256, height = 256;
    uint32_t y = 0;
    uint32_t settings_544 = constant(2608);
    uint32_t far_548 = constant(7820);
    uint32_t settings_524 = constant(2608);
    uint32_t settings_552 = 0;
    uint32_t clear_color = 0xFF000000;
    bool field_of_view = true;
    uint32_t fov = constant(8116);
    if (type == 0) {
      width = VideoOutput(base, calls.video_output(0)).display_width();
      height = VideoOutput(base, calls.video_output(1)).display_height();
      settings_524 = constant(8144);
      far_548 = constant(8140);
      field_of_view = false;
    } else if (type <= 3) {
      width = type == 3 ? 512 : 1024;
      height = type == 3 ? 128 : 288;
      y = type == 2 ? 288 : 0;
      far_548 = constant(type == 3 ? 8128 : 8136);
      settings_552 = 1;
      fov = constant(8132);
    } else if (type <= 9) {
      settings_544 = constant(8124);
      if (type == 4) {
        far_548 = constant(Mode(base, 1) == 0 ? 8120 : 7820);
      } else if (type == 7) {
        far_548 = constant(8112);
      }
    } else if (type <= 18) {
      static constexpr uint32_t kSizes[9] = {1024, 512, 1024, 512, 256, 256, 512, 256, 256};
      width = height = kSizes[type - 10];
    } else if (type == 19) {
      far_548 = constant(8108);
      clear_color = 0;
    } else {
      width = height = 1024;
    }
    p.set_viewport_x(0);
    p.set_viewport_y(y);
    p.set_viewport_width(width);
    p.set_viewport_height(height);
    p.set_unknown_512(0);
    store(p.zoom_address(), constant(8156));
    store(p.unknown_520_address(), constant(2608));
    store(p.unknown_524_address(), settings_524);
    store(p.unknown_528_address(), constant(2612));
    store(p.unknown_532_address(), constant(8152));
    store(p.unknown_536_address(), constant(8148));
    store(p.unknown_540_address(), constant(8148));
    store(p.init_near_address(), settings_544);
    store(p.init_far_address(), far_548);
    p.set_unknown_552(settings_552);
    p.set_unknown_556(0);
    p.set_unknown_560(1);
    p.set_clear_color(clear_color);
    p.set_unknown_568(1);
    p.set_clear_depth(0);
    if (field_of_view) {
      store(p.field_of_view_address(), fov);
    }
  }
  // lfs and stfs through a double, as the original.
  p.set_near(float(double(p.init_near())));
  p.set_far(float(double(p.init_far())));
  p.set_type(type);
  p.set_enabled(0);
  // fcfid of the sign-extended words, then frsp.
  p.set_width(float(double(int64_t(int32_t(p.viewport_width())))));
  p.set_height(float(double(int64_t(int32_t(p.viewport_height())))));
  calls.update();
  calls.set_up_surfaces();
  return 0;
}

uint32_t InitPasses(uint8_t* base, const PassesInitCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8209F960 CameraPasses::InitPasses");
  for (uint32_t i = 0; i < kPassCount; i++) {
    calls.init_pass(Pass(i), i, G::init_argument(base));
  }
  const uint32_t mode = G::mode(base);
  if (!SetMode(base, 1, mode)) {
    calls.set_mode(mode);
  }
  return mode;
}

uint32_t InitAll(uint8_t* base, const PassesInitCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8209F9B8 CameraPasses::Init");
  for (uint32_t at : {G::init_argument_address(), G::mode_address(),
                      G::render_target_0_address(), G::render_target_1_2d_address(),
                      G::render_target_1_cube_address(), G::render_target_2_address(),
                      G::render_target_3_address(), G::render_target_4_address(),
                      G::render_target_5_address(), G::render_target_6_address(),
                      G::render_target_7_address(), G::render_target_8_address(),
                      G::render_target_9_address(), G::render_target_10_address(),
                      G::render_target_11_address()}) {
    guest::StoreU32(base, at, 0);
  }
  return InitPasses(base, calls);
}

void Enable(uint8_t* base, uint32_t pass) {
  ITC_NATIVE_RUNNING("sub_820A0968 CameraPass::Enable");
  Cam(base, pass).set_enabled(1);
}

void Disable(uint8_t* base, uint32_t pass) {
  ITC_NATIVE_RUNNING("sub_820A0958 CameraPass::Disable");
  Cam(base, pass).set_enabled(0);
}

uint32_t EnableForView(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820AA1A8 CameraPasses::EnableForView");
  if (G::viewports(base).split() == 0) {
    for (uint32_t i : {0, 10, 11, 12, 19, 20}) {
      Enable(base, Pass(i));
    }
    const uint32_t view_mode = G::viewports(base).first().view_mode();
    if (view_mode == 2 || view_mode == 1) {
      Enable(base, Pass(3));
    }
  } else {
    for (uint32_t i : {1, 2, 13, 14, 15, 16, 17, 18, 20}) {
      Enable(base, Pass(i));
    }
  }
  if (Mode(base, 1) == 1) {
    for (uint32_t i = 4; i <= 9; i++) {
      Enable(base, Pass(i));
    }
    return Pass(9);
  }
  Enable(base, Pass(4));
  return Pass(4);
}

void SetNear(uint8_t* base, double near) {
  ITC_NATIVE_RUNNING("sub_820AA368 CameraPasses::SetNear");
  Stfs(base, Cam(base, Pass(0)).near_address(), near);
}

bool ReleaseSurfaces(uint8_t* base, uint32_t pass,
                     const std::function<void(uint32_t surface, uint32_t field)>& release) {
  ITC_NATIVE_RUNNING("sub_820A0C40 CameraPass::ReleaseSurfaces");
  const Cam p(base, pass);
  const int32_t type = int32_t(p.type());
  if (type < 3 || (type > 4 && uint32_t(type - 10) > 10)) {
    return false;
  }
  for (uint32_t field : {p.color_surface_address(), p.depth_surface_address()}) {
    const uint32_t surface = guest::LoadU32(base, field);
    if (surface != 0) {
      release(surface, field - pass);
      guest::StoreU32(base, field, 0);
    }
  }
  return true;
}

void FromViewport(uint8_t* base, uint32_t pass, uint32_t player, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_820AA4A8 CameraPass::FromViewport");
  const Cam p(base, pass);
  const Entry at(base, guest::LoadU32(base, G::viewports(base).first_address() + (player << 2)) +
                          Entry::kSize * entry);
  for (uint32_t i = 0; i < 48; i += 8) {
    const uint32_t high = guest::LoadU32(base, at.eye_x_address() + i);
    const uint32_t low = guest::LoadU32(base, at.eye_y_address() + i);
    guest::StoreU32(base, p.eye_x_address() + i, high);
    guest::StoreU32(base, p.eye_y_address() + i, low);
  }
  PassSetZoom(base, pass, Lfs(base, at.zoom_address()));
  guest::StoreU32(base, p.far_address(), Through(guest::LoadU32(base, at.far_address())));
  guest::StoreU32(base, p.roll_address(), Through(guest::LoadU32(base, at.roll_address())));
}

void PassSetZoom(uint8_t* base, uint32_t pass, double zoom) {
  ITC_NATIVE_RUNNING("sub_820AA378 CameraPass::SetZoom");
  const Cam p(base, pass);
  Stfs(base, p.zoom_address(), zoom);
  const int32_t height = int32_t(p.viewport_height());
  const double h = double(float(double(int64_t(height))));
  const double ratio = Fdivs(Fmuls(h, Lfs(base, kHalf)), zoom);
  const double angle = double(float(c_runtime::Atan(ratio)));
  Stfs(base, p.field_of_view_address(), WrapAngle(base, angle));
}

bool ReleaseViewports(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820AA430 Viewports::Release");
  const schema::Viewports block = G::viewports(base);
  if (!memory::IsPages(block.address) || !memory::IsPages(block.first().address) ||
      !memory::IsPages(block.second().address)) {
    return false;
  }
  for (uint32_t i = 0; i < 8; i += 4) {
    memory::FreePages(guest::LoadU32(base, G::viewports(base).first_address() + i));
  }
  memory::FreePages(G::viewports(base).address);
  G::set_viewports(base, 0);
  return true;
}

void ViewportSetZoom(uint8_t* base, uint32_t viewport, double zoom) {
  ITC_NATIVE_RUNNING("sub_820AAE90 Viewport::SetZoom");
  const uint32_t size = guest::LoadU32(base, 0x82001EF0);
  const double half = Fmuls(double(float(double(int64_t(uint64_t(size))))), Lfs(base, kHalf));
  const double angle = double(float(c_runtime::Atan(half)));
  auto entry = [&](uint32_t i) { return Entry(base, viewport + Entry::kSize * i); };
  Stfs(base, entry(0).zoom_address(), zoom);
  Stfs(base, entry(1).zoom_address(), zoom);
  const double fixed = Fmuls(angle, Lfs(base, 0x82002808));
  for (uint32_t i = 2; i <= 7; i++) {
    Stfs(base, entry(i).zoom_address(), fixed);
  }
  for (uint32_t i = 8; i <= 17; i++) {
    Stfs(base, entry(i).zoom_address(), zoom);
  }
  const double inverse = Fdivs(Lfs(base, kOne), Lfs(base, Viewport(base, viewport).far_18_address()));
  Stfs(base, entry(18).zoom_address(), Fmuls(inverse, Lfs(base, 0x82002800)));
  Stfs(base, entry(19).zoom_address(), zoom);
}

void ViewportSetRoll(uint8_t* base, uint32_t viewport, double angle) {
  ITC_NATIVE_RUNNING("sub_820AA2D0 Viewport::SetRoll");
  const double pi = Lfs(base, 0x82002240);
  double x = Fadds(angle, pi);
  const double two_pi = Lfs(base, kTwoPi);
  while (x > two_pi) {
    x = Fsubs(x, two_pi);
  }
  const double zero = Lfs(base, 0x82000A34);
  while (x < zero) {
    x = Fadds(x, two_pi);
  }
  x = Fsubs(x, pi);
  auto entry = [&](uint32_t i) { return Entry(base, viewport + Entry::kSize * i); };
  Stfs(base, entry(0).roll_address(), x);
  Stfs(base, entry(1).roll_address(), Fneg(x));
  for (uint32_t i = 2; i <= 17; i++) {
    Stfs(base, entry(i).roll_address(), x);
  }
  Stfs(base, entry(19).roll_address(), x);
}

uint32_t ViewportUpdateEntries(uint8_t* base, uint32_t viewport) {
  ITC_NATIVE_RUNNING("sub_820AA550 Viewport::UpdateEntries");
  const uint32_t v = viewport;
  const Viewport vp(base, v);
  auto entry = [&](uint32_t i) { return v + Entry::kSize * i; };
  auto E = [&](uint32_t e) { return Entry(base, e); };
  // A word copied through a float register (lfs, stfs): its bits.
  auto copy = [&](uint32_t to, uint32_t from) {
    guest::StoreU32(base, to, Through(guest::LoadU32(base, from)));
  };
  auto copy4 = [&](uint32_t to, uint32_t from) {
    for (uint32_t i = 0; i < 16; i += 4) {
      copy(to + i, from + i);
    }
  };
  // Entry 0's eye, target and up kept (eight bytes at a time).
  for (uint32_t i = 0; i < 48; i += 8) {
    const uint32_t high = guest::LoadU32(base, E(v).eye_x_address() + i);
    const uint32_t low = guest::LoadU32(base, E(v).eye_y_address() + i);
    guest::StoreU32(base, vp.last_view_address() + i, high);
    guest::StoreU32(base, vp.last_view_address() + 4 + i, low);
  }
  copy4(E(v).eye_address(), vp.eye_address());
  copy4(E(v).target_address(), vp.target_address());
  copy4(E(v).up_address(), vp.up_address());
  // Entry 1: along the target's direction, y fixed, normalized, scaled and
  // put on the eye.
  const uint32_t e1 = entry(1), t1 = E(e1).target_address();
  const uint32_t from = vp.view_mode() == 1 ? vp.view1_target_address() : vp.target_address();
  Stfs(base, t1, Fsubs(Lfs(base, from), Lfs(base, vp.eye_x_address())));
  Stfs(base, t1 + 4, Fsubs(Lfs(base, from + 4), Lfs(base, vp.eye_y_address())));
  Stfs(base, t1 + 8, Fsubs(Lfs(base, from + 8), Lfs(base, vp.eye_z_address())));
  copy(E(e1).target_y_address(), 0x82002804);
  {
    FlushToZero flush;
    Stvx(base, t1, Normalize(Lvx(base, t1)));
  }
  const double scale = Lfs(base, 0x82001BE0);
  for (uint32_t i = 0; i < 12; i += 4) {
    Stfs(base, t1 + i, Fmuls(Lfs(base, t1 + i), scale));
  }
  for (uint32_t i = 0; i < 12; i += 4) {
    Stfs(base, t1 + i, Fadds(Lfs(base, t1 + i), Lfs(base, vp.eye_address() + i)));
  }
  copy4(E(e1).eye_address(), vp.eye_address());
  copy4(E(e1).up_address(), vp.up_address());
  // The rest: one, zero.
  const uint32_t mode = Mode(base, 1);
  const double one = Lfs(base, kOne);
  const double zero = Lfs(base, 0x82000A34);
  auto up = [&](uint32_t e, double x, double y, double z, double w) {
    Stfs(base, E(e).up_x_address(), x);
    Stfs(base, E(e).up_y_address(), y);
    Stfs(base, E(e).up_z_address(), z);
    Stfs(base, E(e).up_w_address(), w);
  };
  const uint32_t e2 = entry(2);
  if (mode == 0) {
    copy4(E(e2).eye_address(), vp.look_address());
    copy4(E(e2).target_address(), E(v).eye_address());
    up(e2, zero, one, zero, one);
  } else if (mode == 1) {
    copy4(E(e2).eye_address(), vp.look_address());
    Stfs(base, E(e2).target_x_address(), Fadds(Lfs(base, E(e2).eye_x_address()), one));
    copy(E(e2).target_y_address(), E(e2).eye_y_address());
    copy(E(e2).target_z_address(), E(e2).eye_z_address());
    up(e2, zero, one, zero, zero);
  }
  // An entry at `eye` looking one unit along an axis (0 x, 1 y, 2 z) one
  // way or the other.
  auto face = [&](uint32_t e, uint32_t eye, uint32_t axis, bool plus) {
    copy4(E(e).eye_address(), eye);
    for (uint32_t k = 0; k < 3; k++) {
      if (k == axis) {
        const double c = Lfs(base, E(e).eye_address() + 4 * k);
        Stfs(base, E(e).target_address() + 4 * k, plus ? Fadds(c, one) : Fsubs(c, one));
      } else {
        copy(E(e).target_address() + 4 * k, E(e).eye_address() + 4 * k);
      }
    }
  };
  face(entry(3), vp.look_address(), 0, false);
  up(entry(3), zero, one, zero, zero);
  face(entry(4), vp.look_address(), 1, true);
  up(entry(4), zero, zero, scale, zero);
  face(entry(5), vp.look_address(), 1, false);
  up(entry(5), zero, zero, one, zero);
  face(entry(6), vp.look_address(), 2, true);
  up(entry(6), zero, one, zero, zero);
  face(entry(7), vp.look_address(), 2, false);
  up(entry(7), zero, one, zero, zero);
  for (uint32_t i = 8; i <= 16; i++) {
    face(entry(i), E(v).eye_address(), 2, false);
    up(entry(i), zero, one, zero, one);
  }
  const uint32_t e17 = entry(17);
  copy4(E(e17).eye_address(), vp.eye_address());
  copy4(E(e17).target_address(), vp.target_address());
  copy4(E(e17).up_address(), vp.up_address());
  const uint32_t e18 = entry(18);
  const Entry top(base, e18);
  const double height = Fmuls(Lfs(base, vp.far_18_address()), Lfs(base, kHalf));
  copy(top.eye_x_address(), vp.look_x_address());
  Stfs(base, top.eye_y_address(), height);
  copy(top.eye_z_address(), vp.look_z_address());
  Stfs(base, top.eye_w_address(), one);
  copy(top.target_x_address(), vp.look_x_address());
  Stfs(base, top.target_y_address(), zero);
  copy(top.target_z_address(), vp.look_z_address());
  Stfs(base, top.target_w_address(), one);
  up(e18, zero, zero, one, one);
  face(entry(19), E(v).eye_address(), 2, false);
  up(entry(19), zero, one, zero, one);
  return entry(16);
}

uint32_t ViewportUpdate(uint8_t* base, uint32_t viewport) {
  ITC_NATIVE_RUNNING("sub_820AAE40 Viewport::Update");
  return ViewportUpdateEntries(base, viewport);
}

uint32_t ViewportSet(uint8_t* base, uint32_t viewport, uint32_t eye, uint32_t target, uint32_t up,
                     uint32_t fourth) {
  ITC_NATIVE_RUNNING("sub_820AAE48 Viewport::Set");
  uint32_t to = Viewport(base, viewport).eye_address();
  for (uint32_t from : {eye, target, up, fourth}) {
    for (uint32_t i = 0; i < 16; i += 8) {
      const uint32_t high = guest::LoadU32(base, from + i);
      const uint32_t low = guest::LoadU32(base, from + i + 4);
      guest::StoreU32(base, to + i, high);
      guest::StoreU32(base, to + i + 4, low);
    }
    to += 16;
  }
  return ViewportUpdateEntries(base, viewport);
}

uint32_t ViewportLookAt(uint8_t* base, uint32_t viewport, uint32_t eye, uint32_t target) {
  ITC_NATIVE_RUNNING("sub_820AAF68 Viewport::LookAt");
  auto copy16 = [&](uint32_t to, uint32_t from) {
    for (uint32_t i = 0; i < 16; i += 8) {
      const uint32_t high = guest::LoadU32(base, from + i);
      const uint32_t low = guest::LoadU32(base, from + i + 4);
      guest::StoreU32(base, to + i, high);
      guest::StoreU32(base, to + i + 4, low);
    }
  };
  const Viewport vp(base, viewport);
  copy16(vp.eye_address(), eye);
  copy16(vp.target_address(), target);
  const uint32_t zero = Bits(Lfs(base, 0x82000A34)), one = Bits(Lfs(base, kOne));
  guest::StoreU32(base, vp.up_x_address(), zero);
  guest::StoreU32(base, vp.up_y_address(), one);
  guest::StoreU32(base, vp.up_z_address(), zero);
  guest::StoreU32(base, vp.up_w_address(), one);
  copy16(vp.look_address(), target);
  return ViewportUpdateEntries(base, viewport);
}

std::optional<uint32_t> RecordViewport(uint8_t* base, uint32_t list, uint32_t pass) {
  ITC_NATIVE_RUNNING("sub_820A0978 CameraPass::RecordViewport");
  // What the original's command list calls would print about: another chunk
  // open (OpenChunk), or the list full on any of its four allocations.
  if (List(base, list).chunk_open() == 0 && ListG::open_chunks(base) != 0) {
    return std::nullopt;
  }
  const List l(base, list);
  if (l.used() + 4 >= l.capacity() || l.used() + 4 < l.used()) {
    // (each allocation compares the count so far; the last one is the
    // largest unless the count wraps, which is left to the original too)
    return std::nullopt;
  }
  command_list::OpenChunk(base, list);
  auto slot = [&] { return command_list::AllocateSlots(base, list, 1); };
  const uint32_t chunk = slot();
  guest::StoreU16(base, chunk + 2, 5);
  guest::StoreU32(base, chunk + 4, 0);
  uint32_t s = slot();
  guest::StoreU32(base, s + 8, 0);
  guest::StoreU32(base, s + 0, 30);
  guest::StoreU32(base, s + 4, 1);
  s = slot();
  const Cam p(base, pass);
  guest::StoreU16(base, s + 0, uint16_t(p.viewport_x()));
  guest::StoreU16(base, s + 2, uint16_t(p.viewport_y()));
  guest::StoreU16(base, s + 4, uint16_t(p.viewport_width()));
  const uint32_t height = p.viewport_height();
  Stfs(base, s + 8, Lfs(base, 0x82000A34));
  Stfs(base, s + 12, Lfs(base, kOne));
  guest::StoreU16(base, s + 6, uint16_t(height));
  s = slot();
  guest::StoreU32(base, s + 4, 0);
  guest::StoreU32(base, s + 0, 36);
  guest::StoreU32(base, s + 8, 0);
  command_list::CloseChunk(base, list);
  return chunk;
}

std::optional<uint32_t> RecordClear(uint8_t* base, uint32_t list, uint32_t flags, uint32_t color,
                                    uint32_t depth) {
  ITC_NATIVE_RUNNING("sub_8209FA38 RecordClear");
  // Another chunk open, or the list full on any allocation (1, 1, 18 and 1
  // slots: the last the largest unless the count wraps, left to the
  // original too).
  if (List(base, list).chunk_open() == 0 && ListG::open_chunks(base) != 0) {
    return std::nullopt;
  }
  const uint32_t used = List(base, list).used();
  if (used + 21 >= List(base, list).capacity() || used + 21 < used) {
    return std::nullopt;
  }
  command_list::OpenChunk(base, list);
  const uint32_t chunk = command_list::AllocateSlots(base, list, 1);
  guest::StoreU16(base, chunk + 2, 5);
  guest::StoreU32(base, chunk + 4, 0);
  uint32_t s = command_list::AllocateSlots(base, list, 1);
  guest::StoreU32(base, s + 8, 0);
  guest::StoreU32(base, s + 0, 26);
  guest::StoreU32(base, s + 4, 1);
  const ClearItem item(base, command_list::AllocateBytes(base, list, ClearItem::kSize, false));
  item.set_flags(flags);
  item.set_color(color);
  item.set_stencil(0);
  item.set_rects(0);
  item.set_rect_count(0);
  Stfs(base, item.depth_address(), double(float(double(int64_t(uint64_t(depth))))));
  s = command_list::AllocateSlots(base, list, 1);
  guest::StoreU32(base, s + 4, 0);
  guest::StoreU32(base, s + 0, 36);
  guest::StoreU32(base, s + 8, 0);
  command_list::CloseChunk(base, list);
  return chunk;
}

namespace {

// Whether `slots` more fit on the list with no message from its calls: no
// other chunk open (unless the list's own is), and the slots used then below
// the capacity (each allocation's check; the last is the largest unless the
// count wraps, which is left to the original too).
bool Room(uint8_t* base, uint32_t list, uint32_t slots) {
  if (List(base, list).chunk_open() == 0 && ListG::open_chunks(base) != 0) {
    return false;
  }
  const uint32_t used = List(base, list).used();
  return used + slots < List(base, list).capacity() && used + slots >= used;
}

uint32_t Slot(uint8_t* base, uint32_t list) {
  return command_list::AllocateSlots(base, list, 1);
}
// A chunk's header slot: +2 = 5, +4 = 0.
uint32_t Header(uint8_t* base, uint32_t list) {
  const uint32_t s = Slot(base, list);
  guest::StoreU16(base, s + 2, 5);
  guest::StoreU32(base, s + 4, 0);
  return s;
}
// A command slot: [id, arg1, arg2].
void Command(uint8_t* base, uint32_t list, uint32_t id, uint32_t arg1, uint32_t arg2) {
  const uint32_t s = Slot(base, list);
  guest::StoreU32(base, s + 8, arg2);
  guest::StoreU32(base, s + 0, id);
  guest::StoreU32(base, s + 4, arg1);
}
// Command 23 with the four render targets (`surface` each) and command 24
// with the depth surface (+580).
void Targets(uint8_t* base, uint32_t list, uint32_t pass, bool surfaces) {
  Command(base, list, 23, 4, 0);
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t s = Slot(base, list);
    guest::StoreU32(base, s + 0, i);
    guest::StoreU32(base, s + 4, surfaces ? Cam(base, pass).color_surface().address : 0);
  }
  Command(base, list, 24, 1, 0);
  const uint32_t s = Slot(base, list);
  guest::StoreU32(base, s + 0, Cam(base, pass).depth_surface().address);
}
// Command 26 and its 280-byte item: the flags, the color, the depth.
void Clear(uint8_t* base, uint32_t list, uint32_t flags, uint32_t color, double depth) {
  Command(base, list, 26, 1, 0);
  const ClearItem item(base, command_list::AllocateBytes(base, list, ClearItem::kSize, false));
  item.set_flags(flags);
  item.set_color(color);
  item.set_stencil(0);
  item.set_rects(0);
  item.set_rect_count(0);
  Stfs(base, item.depth_address(), depth);
}
// The second chunk: a header, command 7 (1, 0x01020001) with its 4-byte item
// (`value`), command 36.
uint32_t ConstantsChunk(uint8_t* base, uint32_t list, uint32_t value) {
  const uint32_t header = Header(base, list);
  Command(base, list, 7, 1, 0x01020001);
  const uint32_t s = command_list::AllocateBytes(base, list, 4, false);
  guest::StoreU32(base, s + 0, value);
  Command(base, list, 36, 0, 0);
  return header;
}

}  // namespace

std::optional<Submissions> RecordBegin(uint8_t* base, uint32_t list, uint32_t pass) {
  ITC_NATIVE_RUNNING("sub_820A0CC0 CameraPass::RecordBegin");
  const Cam p(base, pass);
  const uint32_t type = p.type();
  Submissions out;
  const double zero = Lfs(base, 0x82000A34);
  if (type == 3 || type == 4 || type == 19) {
    if (!Room(base, list, 28)) {
      return std::nullopt;
    }
    command_list::OpenChunk(base, list);
    const uint32_t header = Header(base, list);
    Targets(base, list, pass, true);
    const uint32_t depth = p.clear_depth();
    Clear(base, list, 49, p.clear_color(),
          double(float(double(int64_t(uint64_t(depth))))));
    Command(base, list, 36, 0, 0);
    command_list::CloseChunk(base, list);
    out.count = 1;
    out.chunk[0] = header;
    out.key[0] = type == 3 ? 49 : type == 4 ? 55 : 103;
    return out;
  }
  if (type < 10 || type > 20 || type == 19) {
    return out;
  }
  // Types 10 to 18 and 20: their render targets, a clear and the constants'
  // chunk. In threes: 10, 13, 16 (no surfaces, flags 48); 11, 14, 17 and 12,
  // 15, 18 (the surface, flags 49).
  const bool clears = type != 20 || Cam(base, Pass(10)).enabled() == 0;
  if (!Room(base, list, clears ? 32 : 13)) {
    return std::nullopt;
  }
  const uint32_t set = type == 20 ? 0 : (type - 10) % 3;
  command_list::OpenChunk(base, list);
  const uint32_t header = Header(base, list);
  Targets(base, list, pass, set != 0);
  if (clears) {
    Clear(base, list, set == 0 ? 48 : 49, 0, zero);
  }
  Command(base, list, 36, 0, 0);
  const uint32_t second = ConstantsChunk(base, list, 1);
  command_list::CloseChunk(base, list);
  out.count = 2;
  out.chunk[0] = header;
  out.chunk[1] = second;
  static constexpr uint32_t kFirst[3] = {16, 4, 13}, kLast[3] = {41, 29, 38};
  if (type == 20) {
    out.key[0] = 22;
    out.key[1] = 161;
  } else if (type >= 16) {
    out.key[0] = kLast[set];
    out.key[1] = 382;
  } else {
    out.key[0] = kFirst[set];
    out.key[1] = 161;
  }
  return out;
}

std::optional<Submissions> RecordEnd(uint8_t* base, uint32_t list, uint32_t pass,
                                     const std::function<uint32_t(uint32_t id)>& render_target) {
  ITC_NATIVE_RUNNING("sub_820A1C30 CameraPass::RecordEnd");
  const uint32_t type = Cam(base, pass).type();
  if (type < 3 || type > 20) {
    return Submissions{};
  }
  // The render target's id, the resolve's mode (+4) and face (+40), whether
  // the constants' chunk follows, and the keys.
  uint32_t id, mode, face = 0;
  bool constants = false;
  Submissions out;
  if (type <= 9) {
    id = type == 3 ? 0 : 1;
    mode = 768;
    face = type <= 4 ? 0 : type - 4;
    static constexpr uint32_t kKeys[] = {54, 60, 66, 72, 78, 84, 90};
    out.key[0] = kKeys[type - 3];
  } else if (type <= 18) {
    const uint32_t set = (type - 10) % 3;
    id = type - 8;
    mode = set == 0 ? 516 : 0;
    constants = true;
    static constexpr uint32_t kFirst[3] = {20, 12, 15}, kLast[3] = {45, 37, 40};
    out.key[0] = type >= 16 ? kLast[set] : kFirst[set];
    out.key[1] = 603;
  } else if (type == 19) {
    id = 11;
    mode = 768;
    out.key[0] = 105;
  } else {
    id = 12;
    mode = 4;
    constants = true;
    out.key[0] = 24;
    out.key[1] = 603;
  }
  if (!Room(base, list, constants ? 12 : 8)) {
    return std::nullopt;
  }
  command_list::OpenChunk(base, list);
  const uint32_t header = Header(base, list);
  Command(base, list, 25, 1, 0);
  const uint32_t r = command_list::AllocateResolve(base, list, false);
  guest::StoreU32(base, r + 0, 2);
  guest::StoreU32(base, r + 4, mode);
  const uint32_t texture = render_target(id);
  guest::StoreU32(base, r + 24, texture);
  guest::StoreU32(base, r + 36, 0);
  guest::StoreU32(base, r + 40, face);
  guest::StoreU32(base, r + 52, 0);
  Stfs(base, r + 48, Lfs(base, 0x82000A34));
  guest::StoreU8(base, r + 44, 0);
  guest::StoreU8(base, r + 45, 0);
  guest::StoreU8(base, r + 46, 0);
  guest::StoreU8(base, r + 47, 255);
  guest::StoreU32(base, r + 56, 0x18280186);
  guest::StoreU32(base, r + 60, 0);
  guest::StoreU32(base, r + 64, 0x1A220197);
  Command(base, list, 36, 0, 0);
  out.count = 1;
  out.chunk[0] = header;
  if (constants) {
    out.count = 2;
    out.chunk[1] = ConstantsChunk(base, list, 0);
  }
  command_list::CloseChunk(base, list);
  return out;
}

void RecordBegins(uint8_t* base, const std::function<void(uint32_t pass)>& record) {
  ITC_NATIVE_RUNNING("sub_8209F8A0 CameraPasses::RecordBegins");
  for (uint32_t i = 0; i < kPassCount; i++) {
    if (Cam(base, Pass(i)).enabled() != 0) {
      record(Pass(i));
    }
  }
}

void RecordEnds(uint8_t* base, const std::function<void(uint32_t pass)>& record) {
  ITC_NATIVE_RUNNING("sub_8209F900 CameraPasses::RecordEnds");
  for (uint32_t i = 0; i < kPassCount; i++) {
    if (Cam(base, Pass(i)).enabled() != 0) {
      record(Pass(i));
    }
  }
}

bool PassUpdate(uint8_t* base, uint32_t pass, uint32_t frame,
                const std::function<void(uint32_t key)>& record_viewport) {
  ITC_NATIVE_RUNNING("sub_820A3978 CameraPass::Update");
  const Cam p(base, pass);
  if (p.vtable().set_viewport_size() != 0x82593500) {
    return false;
  }
  const uint32_t type = p.type();
  if (type <= 2) {
    record_viewport(type == 2 ? 371 : 150);
  }
  SetViewportSize(base, pass, p.viewport_width(), p.viewport_height());
  if ((type >= 10 && type <= 18) || type == 20) {
    return true;
  }
  // The forward, target - eye, with w 1.0, into +464 (through the stack, as
  // the original copies it).
  const uint32_t one = Bits(Lfs(base, kOne));
  const double fx = Fsubs(Lfs(base, p.target_x_address()), Lfs(base, p.eye_x_address()));
  const double fy = Fsubs(Lfs(base, p.target_y_address()), Lfs(base, p.eye_y_address()));
  const double fz = Fsubs(Lfs(base, p.target_z_address()), Lfs(base, p.eye_z_address()));
  guest::StoreU32(base, p.forward_x_address(), Bits(fx));
  guest::StoreU32(base, p.forward_y_address(), Bits(fy));
  guest::StoreU32(base, p.forward_z_address(), Bits(fz));
  guest::StoreU32(base, p.forward_w_address(), one);
  simde__m128 forward;
  {
    FlushToZero flush;
    Stvx(base, p.forward_x_address(), Normalize(Lvx(base, p.forward_x_address())));
  }
  // The normalized forward as the stack copy reads it back (w normalized
  // too), then +476 = 1.0.
  forward = VectorOf(guest::LoadU32(base, p.forward_x_address()),
                     guest::LoadU32(base, p.forward_y_address()),
                     guest::LoadU32(base, p.forward_z_address()),
                     guest::LoadU32(base, p.forward_w_address()));
  guest::StoreU32(base, p.forward_w_address(), one);
  const simde__m128 up = Lvx(base, p.up_x_address());
  const uint32_t zero = Bits(Lfs(base, 0x82000A34));
  simde__m128 right, up2;
  {
    FlushToZero flush;
    right = NegMulSub(Shuffle<0x78>(up), Shuffle<0x9C>(forward),
                      Mul(Shuffle<0x9C>(up), Shuffle<0x78>(forward)));
    right = Normalize(right);
    forward = Normalize(forward);
    up2 = NegMulSub(Shuffle<0x78>(forward), Shuffle<0x9C>(right),
                    Mul(Shuffle<0x9C>(forward), Shuffle<0x78>(right)));
  }
  // -eye (each word negated through a float register).
  uint32_t eye[4];
  for (uint32_t i = 0; i < 4; i++) {
    eye[i] = Bits(Fneg(Lfs(base, p.eye_x_address() + 4 * i)));
  }
  const simde__m128 minus_eye = VectorOf(eye[0], eye[1], eye[2], eye[3]);
  simde__m128 dots[3];
  {
    FlushToZero flush;
    dots[0] = Dot3(minus_eye, right);
    dots[1] = Dot3(minus_eye, up2);
    dots[2] = Dot3(minus_eye, forward);
  }
  // The view at frame + 128: the right, up and forward as columns, -eye's
  // dots as the fourth row.
  const uint32_t view = frame + 128;
  const simde__m128 columns[3] = {right, up2, forward};
  for (uint32_t row = 0; row < 3; row++) {
    for (uint32_t column = 0; column < 3; column++) {
      guest::StoreU32(base, view + 16 * row + 4 * column, Through(WordOf(columns[column], int(row))));
    }
    guest::StoreU32(base, view + 16 * row + 12, zero);
  }
  for (uint32_t column = 0; column < 3; column++) {
    guest::StoreU32(base, view + 48 + 4 * column, Through(WordOf(dots[column], 0)));
  }
  guest::StoreU32(base, view + 60, one);
  // Copied to +16 and turned by the roll, as the original's calls (noted for
  // interpolation).
  const uint32_t view_out = p.view_address();
  matrix_math::Copy(base, view_out, view);
  interpolation::BlendState_OnCopy(base, view_out, view, 0x820A3D18);
  matrix_math::RotateZ(base, view_out, view_out, p.roll_address(), frame - 80);
  // +480: the up again, from the forward at +464 (w 1.0) and +432.
  {
    FlushToZero flush;
    const simde__m128 f = Lvx(base, p.forward_x_address());
    const simde__m128 u = Lvx(base, p.up_x_address());
    Stvx(base, p.up_again_address(),
         NegMulSub(Shuffle<0x78>(u), Shuffle<0x9C>(f), Mul(Shuffle<0x9C>(u), Shuffle<0x78>(f))));
    Stvx(base, p.up_again_address(), Normalize(Lvx(base, p.up_again_address())));
  }
  matrix_math::Multiply(base, p.view_projection_address(), view_out, p.projection_address());
  interpolation::BlendState_OnMultiply(base, p.view_projection_address(), view_out,
                                       p.projection_address(), 0x820A3D90);
  return true;
}

// --- Viewports::Init and Viewport::Init ---

void ViewportInit(uint8_t* base, uint32_t viewport, double tangent, uint32_t frame,
                  const ViewportCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820AAFF0 Viewport::Init");
  constexpr uint32_t kZero = 0x82000A34;
  const uint32_t video = calls.video_output(frame);
  const int32_t width = int32_t(VideoOutput(base, video).display_width());
  const double scaled = Fmuls(double(float(double(int64_t(width)))), double(float(tangent)));
  ViewportSetZoom(base, viewport, Fmuls(scaled, Lfs(base, kHalf)));
  const double zero = Lfs(base, kZero);
  const double one = Lfs(base, kOne);
  ViewportSetRoll(base, viewport, zero);
  Stfs(base, Cam(base, Pass(0)).near_address(), one);
  const Viewport vp(base, viewport);
  auto entry = [&](uint32_t i) { return Entry(base, viewport + Entry::kSize * i); };
  vp.set_view_mode(0);
  // The entries' far planes: entry 0 from pass 0, 1 to 16 from passes 3 to
  // 18, 17 the float at 0x82002814, 18 and 19 from passes 19 and 20.
  for (uint32_t i = 0; i < 20; i++) {
    const uint32_t from = i == 17 ? 0x82002814
                          : i == 0 ? Cam(base, Pass(0)).far_address()
                          : i <= 16 ? Cam(base, Pass(i + 2)).far_address()
                                    : Cam(base, Pass(i + 1)).far_address();
    Stfs(base, entry(i).far_address(), Lfs(base, from));
  }
  for (uint32_t i = 0; i < 20; i++) {
    entry(i).set_unknown_0(0);
  }
  // The three vectors, each made once.
  constexpr uint32_t kOrigin = G::origin_vector_address(), kSide = G::side_vector_address(),
                     kUp = G::up_vector_address();
  if ((G::vector_guard(base) & 1) == 0) {
    const uint32_t guard = G::vector_guard(base) | 1;
    Stfs(base, kOrigin + 0, zero);
    Stfs(base, kOrigin + 4, zero);
    Stfs(base, kOrigin + 8, zero);
    Stfs(base, kOrigin + 12, one);
    G::set_vector_guard(base, guard);
    c_runtime::AtexitFrom(base, 0x825DAE00, calls.atexit);
  }
  if ((G::vector_guard(base) & 2) == 0) {
    G::set_vector_guard(base, G::vector_guard(base) | 2);
    Stfs(base, kSide + 0, Lfs(base, 0x82001FD8));
    Stfs(base, kSide + 4, zero);
    Stfs(base, kSide + 8, zero);
    Stfs(base, kSide + 12, one);
    c_runtime::AtexitFrom(base, 0x825DADF8, calls.atexit);
  }
  if ((G::vector_guard(base) & 4) == 0) {
    const uint32_t guard = G::vector_guard(base) | 4;
    Stfs(base, kUp + 0, zero);
    Stfs(base, kUp + 4, one);
    Stfs(base, kUp + 8, zero);
    Stfs(base, kUp + 12, one);
    G::set_vector_guard(base, guard);
    c_runtime::AtexitFrom(base, 0x825DADF0, calls.atexit);
  }
  const uint32_t vectors[3] = {kOrigin, kSide, kUp};
  for (uint32_t v = 0; v < 3; v++) {
    for (uint32_t k = 0; k < 4; k++) {
      Stfs(base, vp.base_eye_address() + 16 * v + 4 * k, Lfs(base, vectors[v] + 4 * k));
    }
  }
  for (uint32_t v = 0; v < 7; v++) {
    Stfs(base, vp.unknown_1728_address() + 16 * v + 0, zero);
    Stfs(base, vp.unknown_1728_address() + 16 * v + 4, zero);
    Stfs(base, vp.unknown_1728_address() + 16 * v + 8, zero);
    Stfs(base, vp.unknown_1728_address() + 16 * v + 12, one);
  }
  const double far = Lfs(base, 0x82002810);
  Stfs(base, vp.zoom_address(), Lfs(base, entry(0).zoom_address()));
  Stfs(base, vp.near_address(), Lfs(base, Cam(base, Pass(0)).near_address()));
  Stfs(base, vp.far_18_address(), far);
  Stfs(base, entry(18).zoom_address(), Lfs(base, 0x8200280C));
  Stfs(base, entry(18).far_address(), far);
}

std::optional<uint32_t> ViewportsInit(uint8_t* base, uint32_t frame, const ViewportCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820AB358 Viewports::Init");
  // tan (sub_825A8A50) as its adapter gives it with nearest rounding set.
  using Fpscr = ::base::FloatMode;
  double tangent;
  if ((Fpscr::getcsr() & Fpscr::RoundMaskVal) != 0 ||
      !c_runtime::Tan(std::bit_cast<double>(uint64_t(guest::LoadU32(base, 0x82002818)) << 32 |
                                            guest::LoadU32(base, 0x8200281C)),
                      &tangent)) {
    return std::nullopt;
  }
  const schema::Viewports block(base, memory::AllocPages(schema::Viewports::kSize, 4096));
  G::set_viewports(base, block);
  c_runtime::XMemSet(base, block.address, 0, schema::Viewports::kSize);
  uint32_t viewport = 0;
  for (uint32_t i = 0; i < 2; i++) {
    viewport = memory::AllocPages(Viewport::kSize, 4096);
    guest::StoreU32(base, block.first_address() + 4 * i, viewport);
    c_runtime::XMemSet(base, viewport, 0, Viewport::kSize);
    ViewportInit(base, viewport, tangent, frame - kViewportInitFrame, calls);
  }
  block.set_split(0);
  return viewport;
}

}  // namespace cameras
