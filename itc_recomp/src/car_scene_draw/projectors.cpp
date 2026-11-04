// The projectors' matrices (docs/functions/car-scene-draw.md): the look-at
// and perspective matrices, and the projectors built from them for the
// headlight's and the light's textures, step for step as the translated
// code (see car_scene_draw.h).

#include "car_scene_draw/car_scene_draw.h"

#include "car_scene_draw/ops.h"
#include "guest.h"

#include "base/log.h"
#include "c_runtime/math.h"
#include "interpolation/blend_state.h"
#include "matrix_math/matrix_math.h"

namespace car_scene_draw {

namespace {

using namespace ops;

constexpr uint32_t kHalfFov = 0x82001BE4;   // the field of view's factor (probably 0.5)
constexpr uint32_t kOrthoScale = 0x820021E8;  // BuildOrtho's (probably 2.0)
constexpr uint32_t kBias = 0x8265EED0;      // the bias matrix both projectors end with

constexpr uint32_t kPerspectiveFrame = 128;
constexpr uint32_t kBuildFrame = 256;
constexpr uint32_t kBuildOrthoFrame = 272;

// The multiplies' return addresses (frame interpolation notes them).
constexpr uint32_t kBuildMultiply1 = 0x82597694;
constexpr uint32_t kBuildMultiply2 = 0x825976A8;
constexpr uint32_t kOrthoMultiply1 = 0x825975DC;
constexpr uint32_t kOrthoMultiply2 = 0x825975F0;

// tan as the original's call gives it: nearest rounding (its _ctrlfp), or
// false where c_runtime::Tan can't stand in.
bool Tangent(double x, double* result) {
  return (Fpscr::getcsr() & Fpscr::RoundMaskVal) == 0 && c_runtime::Tan(x, result);
}

void Multiply(uint8_t* base, uint32_t out, uint32_t a, uint32_t b, uint32_t caller) {
  matrix_math::Multiply(base, out, a, b);
  interpolation::BlendState_OnMultiply(base, out, a, b, caller);
}

}  // namespace

void LookAt(uint8_t* base, uint32_t out, uint32_t eye, uint32_t target, uint32_t up,
            uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82597378 Matrix::LookAt");
  const uint32_t s = stack;
  // forward = target - eye (x, y, z; w 1.0) at s - 32.
  {
    const double ex = Lfs(base, eye + 0);
    const double tx = Lfs(base, target + 0);
    Stfs(base, s - 32, Fsubs(tx, ex));
  }
  {
    const double ty = Lfs(base, target + 4);
    const double ey = Lfs(base, eye + 4);
    Stfs(base, s - 28, Fsubs(ty, ey));
  }
  const double tz = Lfs(base, target + 8);
  const double ez = Lfs(base, eye + 8);
  const double one = Lfs(base, kOne);
  Stfs(base, s - 24, Fsubs(tz, ez));
  Stfs(base, s - 20, one);
  const simde__m128 vup = Lvx(base, up);
  const simde__m128 v5 = Permute<0x9C>(vup);
  const simde__m128 v4 = Permute<0x78>(vup);
  const simde__m128 v9 = Lvx(base, s - 32);
  simde__m128 v0, v7, v6;
  {
    // forward normalized (z axis), stored at s - 16.
    FlushToZero flush;
    v0 = Normalized(v9, Dot3(v9, v9));
    v7 = Permute<0x78>(v0);
    Stvx(base, s - 16, v0);
    v6 = Permute<0x9C>(v0);
  }
  Stfs(base, out + 8, Lfs(base, s - 16));
  simde__m128 v13;
  {
    FlushToZero flush;
    v13 = simde_mm_mul_ps(v5, v7);
  }
  Stfs(base, out + 24, Lfs(base, s - 12));
  simde__m128 v12;
  {
    // right = up x forward (x axis), stored at s - 32.
    FlushToZero flush;
    v13 = Nmsub(v4, v6, v13);
    v12 = Dot3(v13, v13);
    Stvx(base, s - 32, v13);
  }
  const double zero = Lfs(base, kZero);
  Stfs(base, out + 12, zero);
  Stfs(base, out + 28, zero);
  Stfs(base, out + 44, zero);
  simde__m128 v11;
  {
    FlushToZero flush;
    v13 = Normalized(v13, v12);
    v12 = Permute<0x78>(v13);
    Stvx(base, s - 32, v13);
    v11 = Permute<0x9C>(v13);
  }
  Stfs(base, out + 0, Lfs(base, s - 32));
  {
    FlushToZero flush;
    v12 = simde_mm_mul_ps(v6, v12);
  }
  {
    const double f11 = Lfs(base, s - 28);
    const double f12 = Lfs(base, s - 24);
    Stfs(base, out + 32, f12);
    Stfs(base, out + 16, f11);
  }
  {
    // up' = forward x right (y axis), stored at s - 32.
    FlushToZero flush;
    v12 = Nmsub(v7, v11, v12);
    Stvx(base, s - 32, v12);
  }
  Stfs(base, out + 4, Lfs(base, s - 32));
  Stfs(base, out + 36, Lfs(base, s - 24));
  {
    const double f11 = Lfs(base, s - 28);
    const double f12 = Lfs(base, s - 8);
    Stfs(base, out + 20, f11);
    Stfs(base, out + 40, f12);
  }
  // The last row: minus each axis dotted with the eye, w 1.0.
  {
    const simde__m128 veye = Lvx(base, eye);
    FlushToZero flush;
    Stvx(base, s - 16, Dot3(v13, veye));
  }
  Stfs(base, out + 48, Fneg(Lfs(base, s - 16)));
  {
    const simde__m128 veye = Lvx(base, eye);
    FlushToZero flush;
    Stvx(base, s - 16, Dot3(v12, veye));
  }
  Stfs(base, out + 52, Fneg(Lfs(base, s - 16)));
  simde__m128 dz;
  {
    const simde__m128 veye = Lvx(base, eye);
    FlushToZero flush;
    dz = Dot3(v0, veye);
  }
  Stfs(base, out + 60, one);
  Stvx(base, s - 16, dz);
  Stfs(base, out + 56, Fneg(Lfs(base, s - 16)));
}

bool PerspectiveDeclines(uint8_t* base, double fov) {
  double t;
  return !Tangent(Fmuls(fov, Lfs(base, kHalfFov)), &t);
}

bool Perspective(uint8_t* base, uint32_t out, double fov, double aspect, double near_plane,
                 double far_plane, uint32_t stack) {
  double t;
  if (!Tangent(Fmuls(fov, Lfs(base, kHalfFov)), &t)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_825972A8 Matrix::Perspective");
  Store(base, stack - kPerspectiveFrame, stack);  // the back chain, as stwu
  const double depth = Fsubs(far_plane, near_plane);
  double offset = Fmuls(near_plane, far_plane);
  const double zero = Lfs(base, kZero);
  Stfs(base, out + 4, zero);
  Stfs(base, out + 8, zero);
  Stfs(base, out + 12, zero);
  Stfs(base, out + 16, zero);
  Stfs(base, out + 24, zero);
  Stfs(base, out + 28, zero);
  offset = Fdivs(offset, depth);
  Stfs(base, out + 32, zero);
  const double scale = Fdivs(far_plane, depth);
  Stfs(base, out + 36, zero);
  Stfs(base, out + 48, zero);
  Stfs(base, out + 52, zero);
  Stfs(base, out + 60, zero);
  const double tangent = Frsp(t);
  const double one = Lfs(base, kOne);
  Stfs(base, out + 40, scale);
  Stfs(base, out + 44, one);
  Stfs(base, out + 56, Fneg(offset));
  const double y = Fdivs(one, tangent);
  Stfs(base, out + 20, y);
  Stfs(base, out + 0, Fdivs(y, aspect));
  return true;
}

bool ProjectorBuild(uint8_t* base, uint32_t out, uint32_t eye, uint32_t target, uint32_t up,
                    double fov, double aspect, double near_plane, double far_plane,
                    uint32_t stack) {
  if (PerspectiveDeclines(base, fov)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_82597610 Projector::Build");
  const uint32_t f = stack - kBuildFrame;
  Store(base, f, stack);  // the back chain, as stwu
  const double one = Lfs(base, kOne);
  for (uint32_t at : {156u, 172u, 188u, 204u, 92u, 108u, 124u, 140u}) {
    Stfs(base, f + at, one);
  }
  LookAt(base, f + 144, eye, target, up, f);
  Perspective(base, f + 80, fov, aspect, near_plane, far_plane, f);
  Multiply(base, out, f + 144, f + 80, kBuildMultiply1);
  Multiply(base, out, out, kBias, kBuildMultiply2);
  return true;
}

void ProjectorBuildOrtho(uint8_t* base, uint32_t out, uint32_t eye, uint32_t target,
                         uint32_t up, double width, double height, double near_plane,
                         double far_plane, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82597518 Projector::BuildOrtho");
  const uint32_t f = stack - kBuildOrthoFrame;
  Store(base, f, stack);  // the back chain, as stwu
  const double one = Lfs(base, kOne);
  for (uint32_t at : {156u, 172u, 188u, 204u}) {
    Stfs(base, f + at, one);
  }
  LookAt(base, f + 144, eye, target, up, f);
  // The orthographic matrix at +80.
  const double far_minus_near = Fsubs(far_plane, near_plane);
  const double near_minus_far = Fsubs(near_plane, far_plane);
  Stfs(base, f + 140, one);
  const double scale = Lfs(base, kOrthoScale);
  Stfs(base, f + 80, Fdivs(scale, width));
  Stfs(base, f + 100, Fdivs(scale, height));
  const double zero = Lfs(base, kZero);
  const double depth = Fdivs(one, far_minus_near);
  for (uint32_t at : {84u, 88u, 92u, 96u, 104u, 108u, 112u, 116u, 124u, 128u, 132u}) {
    Stfs(base, f + at, zero);
  }
  const double offset = Fdivs(near_plane, near_minus_far);
  Stfs(base, f + 120, depth);
  Stfs(base, f + 136, offset);
  Multiply(base, out, f + 144, f + 80, kOrthoMultiply1);
  Multiply(base, out, out, kBias, kOrthoMultiply2);
}

}  // namespace car_scene_draw
