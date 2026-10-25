#include "vehicle_accessors/vectors.h"

#include "guest.h"

#include "base/log.h"

#include <rex/platform/fpscr.h>
#include <simde/x86/sse4.1.h>

#include <bit>
#include <cmath>

namespace vector_library {

namespace {

using Fpscr = rex::platform::FPSCRPlatform;

constexpr uint32_t kOne = 0x82000A30;   // the game's float 1.0
constexpr uint32_t kZero = 0x82000A34;  // the game's float 0.0

// lfs: a single from guest memory, widened.
double Lfs(uint8_t* base, uint32_t address) {
  return double(guest::LoadFloat(base, address));
}

// stfs of a result (always a single already).
void Stfs(uint8_t* base, uint32_t address, double value) {
  guest::StoreFloat(base, address, float(value));
}

// lfs then stfs of one word: its bits (the translated code's float to double
// and back folds away, signaling NaNs included).
void CopyWord(uint8_t* base, uint32_t from, uint32_t to) {
  guest::StoreU32(base, to, guest::LoadU32(base, from));
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
double Fmadds(double a, double b, double c) {
  return double(float(std::fma(a, b, c)));
}
double Fmsubs(double a, double b, double c) {
  return double(float(std::fma(a, b, -c)));
}
double Fsqrts(double a) {
  return double(float(std::sqrt(a)));
}

// Vec3::Add's and Vec3::Sub's steps (the same loads and stores; `subtract`
// picks fsubs); `b(offset)` loads b's component, at the original's point.
template <typename LoadB>
void AddOrSubSteps(uint8_t* base, uint32_t out, uint32_t a, LoadB b, bool subtract) {
  auto op = [subtract](double x, double y) { return subtract ? Fsubs(x, y) : Fadds(x, y); };
  {
    const double bx = b(0);
    const double ax = Lfs(base, a + 0);
    Stfs(base, out + 0, op(ax, bx));
  }
  {
    const double by = b(4);
    const double ay = Lfs(base, a + 4);
    Stfs(base, out + 4, op(ay, by));
  }
  const double az = Lfs(base, a + 8);
  const double bz = b(8);
  const uint32_t one = guest::LoadU32(base, kOne);
  const double z = op(az, bz);
  guest::StoreU32(base, out + 12, one);
  Stfs(base, out + 8, z);
}

void AddOrSub(uint8_t* base, uint32_t out, uint32_t a, uint32_t b, bool subtract) {
  AddOrSubSteps(base, out, a, [base, b](uint32_t offset) { return Lfs(base, b + offset); },
                subtract);
}

// The x, y, z of a - b or a + b with all loads first (the copies through a
// temporary), stored with w 1.0.
void AddOrSubCopy(uint8_t* base, uint32_t out, uint32_t a, uint32_t b, bool subtract) {
  auto op = [subtract](double x, double y) { return subtract ? Fsubs(x, y) : Fadds(x, y); };
  const double bx = Lfs(base, b + 0);
  const double ax = Lfs(base, a + 0);
  const double x = op(ax, bx);
  const double by = Lfs(base, b + 4);
  const double ay = Lfs(base, a + 4);
  const double y = op(ay, by);
  const double az = Lfs(base, a + 8);
  const double bz = Lfs(base, b + 8);
  const double z = op(az, bz);
  Stfs(base, out + 0, x);
  Stfs(base, out + 4, y);
  Stfs(base, out + 8, z);
  CopyWord(base, kOne, out + 12);
}

// Matrix::Rotate's and Matrix::TransformPoint's steps (`translate` adds the
// fourth row).
void Transform(uint8_t* base, uint32_t out, uint32_t m, uint32_t v, bool translate) {
  const double vz = Lfs(base, v + 8);
  const double m32 = Lfs(base, m + 32);
  const double vy = Lfs(base, v + 4);
  const double m20 = Lfs(base, m + 20);
  const double m24 = Lfs(base, m + 24);
  CopyWord(base, kZero, out + 12);
  const double t11 = Fmuls(m32, vz);
  const double t10 = Fmuls(m20, vy);
  const double m16 = Lfs(base, m + 16);
  const double t9 = Fmuls(m24, vy);
  const double vx = Lfs(base, v + 0);
  const double m4 = Lfs(base, m + 4);
  const double m8 = Lfs(base, m + 8);
  const double m0 = Lfs(base, m + 0);
  const double m36 = Lfs(base, m + 36);
  const double m40 = Lfs(base, m + 40);
  double m48 = 0, m52 = 0, m56 = 0;
  if (translate) {
    m48 = Lfs(base, m + 48);
    m52 = Lfs(base, m + 52);
    m56 = Lfs(base, m + 56);
  }
  const double x0 = Fmadds(m16, vy, t11);
  const double y0 = Fmadds(m4, vx, t10);
  const double z0 = Fmadds(m8, vx, t9);
  const double x = Fmadds(vx, m0, x0);
  const double y = Fmadds(m36, vz, y0);
  const double z = Fmadds(m40, vz, z0);
  if (translate) {
    Stfs(base, out + 0, Fadds(x, m48));
    Stfs(base, out + 4, Fadds(y, m52));
    Stfs(base, out + 8, Fadds(z, m56));
  } else {
    Stfs(base, out + 0, x);
    Stfs(base, out + 4, y);
    Stfs(base, out + 8, z);
  }
}

// A guest vector as the translated code holds it (word 0 in the top lane).
simde__m128 VectorAt(uint8_t* base, uint32_t address) {
  return simde_mm_castsi128_ps(simde_mm_set_epi32(
      int32_t(guest::LoadU32(base, address + 0)), int32_t(guest::LoadU32(base, address + 4)),
      int32_t(guest::LoadU32(base, address + 8)), int32_t(guest::LoadU32(base, address + 12))));
}

}  // namespace

float Sqrtf(double x) {
  ITC_NATIVE_RUNNING("sub_82293590 sqrtf");
  return float(Fsqrts(x));
}

void MatrixSetTranslation(uint8_t* base, uint32_t matrix, uint32_t vector) {
  ITC_NATIVE_RUNNING("sub_82293818 Matrix::SetTranslation");
  const uint32_t x = guest::LoadU32(base, vector + 0);
  const uint32_t y = guest::LoadU32(base, vector + 4);
  const uint32_t z = guest::LoadU32(base, vector + 8);
  const uint32_t one = guest::LoadU32(base, kOne);
  guest::StoreU32(base, matrix + 48, x);
  guest::StoreU32(base, matrix + 52, y);
  guest::StoreU32(base, matrix + 56, z);
  guest::StoreU32(base, matrix + 60, one);
}

void MatrixRotate(uint8_t* base, uint32_t out, uint32_t matrix, uint32_t vector) {
  ITC_NATIVE_RUNNING("sub_82293880 Matrix::Rotate");
  Transform(base, out, matrix, vector, false);
}

void MatrixTransformPoint(uint8_t* base, uint32_t out, uint32_t matrix, uint32_t vector) {
  ITC_NATIVE_RUNNING("sub_822938F0 Matrix::TransformPoint");
  Transform(base, out, matrix, vector, true);
}

void Vec3Add(uint8_t* base, uint32_t out, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_82294028 Vec3::Add");
  AddOrSub(base, out, a, b, false);
}

void Vec3Sub(uint8_t* base, uint32_t out, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_82294068 Vec3::Sub");
  AddOrSub(base, out, a, b, true);
}

void Vec4ScaleTo(uint8_t* base, uint32_t out, uint32_t v, double s) {
  ITC_NATIVE_RUNNING("sub_822940A8 Vec4::ScaleTo");
  for (uint32_t i = 0; i < 16; i += 4) {
    Stfs(base, out + i, Fmuls(Lfs(base, v + i), s));
  }
}

void Vec4Copy(uint8_t* base, uint32_t out, uint32_t v) {
  ITC_NATIVE_RUNNING("sub_822940E0 Vec4::Copy");
  for (uint32_t i = 0; i < 16; i += 4) {
    CopyWord(base, v + i, out + i);
  }
}

void Vec3CrossW1(uint8_t* base, uint32_t out, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_82294108 Vec3::CrossW1");
  {
    const double by = Lfs(base, b + 4);
    const double az = Lfs(base, a + 8);
    const double t = Fmuls(az, by);
    const double bz = Lfs(base, b + 8);
    const double ay = Lfs(base, a + 4);
    Stfs(base, out + 0, Fmsubs(bz, ay, t));
  }
  {
    const double ax = Lfs(base, a + 0);
    const double bz = Lfs(base, b + 8);
    const double t = Fmuls(bz, ax);
    const double bx = Lfs(base, b + 0);
    const double az = Lfs(base, a + 8);
    Stfs(base, out + 4, Fmsubs(az, bx, t));
  }
  const double ay = Lfs(base, a + 4);
  const double bx = Lfs(base, b + 0);
  const double t = Fmuls(bx, ay);
  const double by = Lfs(base, b + 4);
  const double ax = Lfs(base, a + 0);
  CopyWord(base, kOne, out + 12);
  Stfs(base, out + 8, Fmsubs(ax, by, t));
}

float Vec3Dot(uint8_t* base, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_82294170 Vec3::Dot");
  simde__m128 va = VectorAt(base, a);
  simde__m128 vb = VectorAt(base, b);
  const uint32_t csr = Fpscr::getcsr();
  Fpscr::setcsr(csr | uint32_t(Fpscr::FlushMask));
  // The barriers keep dpps between the two control word writes (the
  // compiler doesn't know they change it: ThinLTO had moved the dot product
  // after the second, so tiny vectors' denormal products weren't flushed).
  PinInMode(va, vb);
  simde__m128 dot = simde_mm_dp_ps(va, vb, 0xEF);
  PinInMode(dot);
  Fpscr::setcsr(csr);
  return simde_mm_cvtss_f32(dot);
}

float Vec3Normalize(uint8_t* base, uint32_t out, uint32_t v) {
  ITC_NATIVE_RUNNING("sub_822941B0 Vec3::Normalize");
  const double y = Lfs(base, v + 4);
  const double y2 = Fmuls(y, y);
  const double x = Lfs(base, v + 0);
  const double z = Lfs(base, v + 8);
  const uint32_t one_word = guest::LoadU32(base, kOne);
  const double one = double(std::bit_cast<float>(one_word));
  const double squared = Fmadds(z, z, Fmadds(x, x, y2));
  const double length = Fsqrts(squared);
  const double inverse = Fdivs(one, length);
  Stfs(base, out + 0, Fmuls(inverse, x));
  Stfs(base, out + 4, Fmuls(inverse, Lfs(base, v + 4)));
  const double scaled_z = Fmuls(inverse, Lfs(base, v + 8));
  guest::StoreU32(base, out + 12, one_word);
  Stfs(base, out + 8, scaled_z);
  return float(length);
}

float Vec3Direction(uint8_t* base, uint32_t out, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_82294200 Vec3::Direction");
  const double bx = Lfs(base, b + 0);
  const double ax = Lfs(base, a + 0);
  Stfs(base, out + 0, Fsubs(ax, bx));
  const double ay = Lfs(base, a + 4);
  const double by = Lfs(base, b + 4);
  const double y = Fsubs(ay, by);
  Stfs(base, out + 4, y);
  const double bz = Lfs(base, b + 8);
  const double az = Lfs(base, a + 8);
  const double z = Fsubs(az, bz);
  const double x = Lfs(base, out + 0);
  const uint32_t one_word = guest::LoadU32(base, kOne);
  guest::StoreU32(base, out + 12, one_word);
  Stfs(base, out + 8, z);
  const double squared = Fmadds(z, z, Fmadds(x, x, Fmuls(y, y)));
  const double length = Fsqrts(squared);
  const double inverse = Fdivs(double(std::bit_cast<float>(one_word)), length);
  Stfs(base, out + 0, Fmuls(inverse, x));
  Stfs(base, out + 4, Fmuls(inverse, y));
  Stfs(base, out + 8, Fmuls(inverse, z));
  return float(length);
}

float Vec3Length(uint8_t* base, uint32_t v) {
  ITC_NATIVE_RUNNING("sub_82294270 Vec3::Length");
  return Sqrtf(double(Vec3Dot(base, v, v)));
}

float Vec3NormalizeSelf(uint8_t* base, uint32_t v) {
  ITC_NATIVE_RUNNING("sub_82294298 Vec3::NormalizeSelf");
  return Vec3Normalize(base, v, v);
}

void Vec3Sum(uint8_t* base, uint32_t out, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_822942A0 Vec3::Sum");
  CopyWord(base, kZero, out + 12);
  Vec3Add(base, out, a, b);
}

void Vec3Difference(uint8_t* base, uint32_t out, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_822942E0 Vec3::Difference");
  CopyWord(base, kZero, out + 12);
  Vec3Sub(base, out, a, b);
}

void Vec3AddTo(uint8_t* base, uint32_t v, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_82294320 Vec3::AddTo");
  Vec3Add(base, v, v, b);
}

void Vec4ScaleBy(uint8_t* base, uint32_t v, double s) {
  ITC_NATIVE_RUNNING("sub_82294340 Vec4::ScaleBy");
  Vec4ScaleTo(base, v, v, s);
}

float Vec4DivideBy(uint8_t* base, uint32_t v, double s) {
  ITC_NATIVE_RUNNING("sub_82294348 Vec4::DivideBy");
  const double inverse = Fdivs(Lfs(base, kOne), s);
  Vec4ScaleTo(base, v, v, inverse);
  return float(inverse);
}

void Vec3SubCopy(uint8_t* base, uint32_t out, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_82294360 Vec3::SubCopy");
  AddOrSubCopy(base, out, a, b, true);
}

void Vec3AddCopy(uint8_t* base, uint32_t out, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_822943C8 Vec3::AddCopy");
  AddOrSubCopy(base, out, a, b, false);
}

void Vec4Scaled(uint8_t* base, uint32_t out, uint32_t v, double s) {
  ITC_NATIVE_RUNNING("sub_82294430 Vec4::Scaled");
  CopyWord(base, kZero, out + 12);
  Vec4ScaleTo(base, out, v, s);
}

void Vec4ScaledFrom(uint8_t* base, uint32_t out, uint32_t v, double s) {
  ITC_NATIVE_RUNNING("sub_82294470 Vec4::ScaledFrom");
  CopyWord(base, kZero, out + 12);
  Vec4ScaleTo(base, out, v, s);
}

void Vec3ScaledCopy(uint8_t* base, uint32_t out, uint32_t v, double s) {
  ITC_NATIVE_RUNNING("sub_822944B0 Vec3::ScaledCopy");
  const double x = Fmuls(Lfs(base, v + 0), s);
  const double y = Fmuls(Lfs(base, v + 4), s);
  const double z = Fmuls(Lfs(base, v + 8), s);
  Stfs(base, out + 0, x);
  Stfs(base, out + 4, y);
  Stfs(base, out + 8, z);
  CopyWord(base, kOne, out + 12);
}

void Vec3Cross(uint8_t* base, uint32_t out, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_82294518 Vec3::Cross");
  const double az = Lfs(base, a + 8);
  const double by = Lfs(base, b + 4);
  const double bz = Lfs(base, b + 8);
  const double t7 = Fmuls(by, az);
  const double ax = Lfs(base, a + 0);
  const double ay = Lfs(base, a + 4);
  const double t6 = Fmuls(ax, bz);
  const double bx = Lfs(base, b + 0);
  const double t5 = Fmuls(bx, ay);
  const uint32_t zero = guest::LoadU32(base, kZero);
  Stfs(base, out + 0, Fmsubs(bz, ay, t7));
  Stfs(base, out + 4, Fmsubs(bx, az, t6));
  Stfs(base, out + 8, Fmsubs(ax, by, t5));
  guest::StoreU32(base, out + 12, zero);
}

void Vec3Lerp(uint8_t* base, uint32_t out, uint32_t a, uint32_t b, double t) {
  ITC_NATIVE_RUNNING("sub_82294568 Vec3::Lerp");
  // (a - b) x t, in a temporary (the original's two on its stack).
  const double bx = Lfs(base, b + 0);
  const double ax = Lfs(base, a + 0);
  const double dx = Fmuls(Fsubs(ax, bx), t);
  const double by = Lfs(base, b + 4);
  const double ay = Lfs(base, a + 4);
  const double dy = Fmuls(Fsubs(ay, by), t);
  const double az = Lfs(base, a + 8);
  const double bz = Lfs(base, b + 8);
  const double dz = Fmuls(Fsubs(az, bz), t);
  // Then out = a - that (Vec3::Sub's steps, a read again after each store).
  CopyWord(base, kZero, out + 12);
  Stfs(base, out + 0, Fsubs(Lfs(base, a + 0), dx));
  Stfs(base, out + 4, Fsubs(Lfs(base, a + 4), dy));
  const double z = Fsubs(Lfs(base, a + 8), dz);
  CopyWord(base, kOne, out + 12);
  Stfs(base, out + 8, z);
}

float Vec3DotOf(uint8_t* base, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_82294608 Vec3::DotOf");
  return Vec3Dot(base, a, b);
}

Vector LoadVector(uint8_t* base, uint32_t address) {
  return {std::bit_cast<float>(guest::LoadU32(base, address + 0)),
          std::bit_cast<float>(guest::LoadU32(base, address + 4)),
          std::bit_cast<float>(guest::LoadU32(base, address + 8)),
          std::bit_cast<float>(guest::LoadU32(base, address + 12))};
}

void StoreVector(uint8_t* base, uint32_t address, const Vector& v) {
  guest::StoreU32(base, address + 0, std::bit_cast<uint32_t>(v.x));
  guest::StoreU32(base, address + 4, std::bit_cast<uint32_t>(v.y));
  guest::StoreU32(base, address + 8, std::bit_cast<uint32_t>(v.z));
  guest::StoreU32(base, address + 12, std::bit_cast<uint32_t>(v.w));
}

Vector Sub(const Vector& a, const Vector& b) {
  return {float(Fsubs(a.x, b.x)), float(Fsubs(a.y, b.y)), float(Fsubs(a.z, b.z)), 1.0f};
}

Vector Add(const Vector& a, const Vector& b) {
  return {float(Fadds(a.x, b.x)), float(Fadds(a.y, b.y)), float(Fadds(a.z, b.z)), 1.0f};
}

Vector Cross(const Vector& a, const Vector& b) {
  const double t7 = Fmuls(b.y, a.z);
  const double t6 = Fmuls(a.x, b.z);
  const double t5 = Fmuls(b.x, a.y);
  return {float(Fmsubs(b.z, a.y, t7)), float(Fmsubs(b.x, a.z, t6)),
          float(Fmsubs(a.x, b.y, t5)), 0.0f};
}

Vector Scale(const Vector& v, double s) {
  return {float(Fmuls(v.x, s)), float(Fmuls(v.y, s)), float(Fmuls(v.z, s)),
          float(Fmuls(v.w, s))};
}

float Dot(const Vector& a, const Vector& b) {
  auto lanes = [](const Vector& v) {
    return simde_mm_set_ps(v.x, v.y, v.z, v.w);
  };
  simde__m128 va = lanes(a);
  simde__m128 vb = lanes(b);
  const uint32_t csr = Fpscr::getcsr();
  Fpscr::setcsr(csr | uint32_t(Fpscr::FlushMask));
  PinInMode(va, vb);
  simde__m128 dot = simde_mm_dp_ps(va, vb, 0xEF);
  PinInMode(dot);
  Fpscr::setcsr(csr);
  return simde_mm_cvtss_f32(dot);
}

void AddValue(uint8_t* base, uint32_t out, uint32_t a, const Vector& b, bool subtract) {
  const float parts[3] = {b.x, b.y, b.z};
  AddOrSubSteps(base, out, a, [&parts](uint32_t offset) { return double(parts[offset / 4]); },
                subtract);
}

}  // namespace vector_library
