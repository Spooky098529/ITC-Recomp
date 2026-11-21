#include "vehicle_accessors/vectors.h"

#include "guest.h"

#include "base/log.h"
#include "schema/vehicle_accessors.h"

#include "base/float_mode.h"
#include <simde/x86/sse4.1.h>

#include <bit>
#include <cmath>

namespace vector_library {

namespace {

using Fpscr = ::base::FloatMode;

using G = schema::VehicleAccessorsGlobals;  // one (1.0), zero (0.0)
using V = schema::VectorLibVector;
using M = schema::VectorLibMatrix;

// lfs then stfs of one word copies its bits (the translated code's float to
// double and back folds away, signaling NaNs included): the *_bits fields.

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
// picks fsubs); `b(i)` loads b's component i (x, y, z), at the original's
// point.
template <typename LoadB>
void AddOrSubSteps(uint8_t* base, uint32_t out, uint32_t a, LoadB b, bool subtract) {
  auto op = [subtract](double x, double y) { return subtract ? Fsubs(x, y) : Fadds(x, y); };
  const V va(base, a);
  const V vo(base, out);
  {
    const double bx = b(0);
    const double ax = double(va.x());
    vo.set_x(float(op(ax, bx)));
  }
  {
    const double by = b(1);
    const double ay = double(va.y());
    vo.set_y(float(op(ay, by)));
  }
  const double az = double(va.z());
  const double bz = b(2);
  const uint32_t one = G::one_bits(base);
  const double z = op(az, bz);
  vo.set_w_bits(one);
  vo.set_z(float(z));
}

void AddOrSub(uint8_t* base, uint32_t out, uint32_t a, uint32_t b, bool subtract) {
  const V vb(base, b);
  AddOrSubSteps(
      base, out, a,
      [vb](uint32_t i) { return double(i == 0 ? vb.x() : i == 1 ? vb.y() : vb.z()); },
      subtract);
}

// The x, y, z of a - b or a + b with all loads first (the copies through a
// temporary), stored with w 1.0.
void AddOrSubCopy(uint8_t* base, uint32_t out, uint32_t a, uint32_t b, bool subtract) {
  auto op = [subtract](double x, double y) { return subtract ? Fsubs(x, y) : Fadds(x, y); };
  const V va(base, a);
  const V vb(base, b);
  const V vo(base, out);
  const double bx = double(vb.x());
  const double ax = double(va.x());
  const double x = op(ax, bx);
  const double by = double(vb.y());
  const double ay = double(va.y());
  const double y = op(ay, by);
  const double az = double(va.z());
  const double bz = double(vb.z());
  const double z = op(az, bz);
  vo.set_x(float(x));
  vo.set_y(float(y));
  vo.set_z(float(z));
  vo.set_w_bits(G::one_bits(base));
}

// Matrix::Rotate's and Matrix::TransformPoint's steps (`translate` adds the
// fourth row).
// Locals are named by the matrix's fields (m<row><column>, rows of 16 bytes).
void Transform(uint8_t* base, uint32_t out, uint32_t m, uint32_t v, bool translate) {
  const M mat(base, m);
  const V vv(base, v);
  const V vo(base, out);
  const double vz = double(vv.z());
  const double m20 = double(mat.m20());
  const double vy = double(vv.y());
  const double m11 = double(mat.m11());
  const double m12 = double(mat.m12());
  vo.set_w_bits(G::zero_bits(base));
  const double t11 = Fmuls(m20, vz);
  const double t10 = Fmuls(m11, vy);
  const double m10 = double(mat.m10());
  const double t9 = Fmuls(m12, vy);
  const double vx = double(vv.x());
  const double m01 = double(mat.m01());
  const double m02 = double(mat.m02());
  const double m00 = double(mat.m00());
  const double m21 = double(mat.m21());
  const double m22 = double(mat.m22());
  double m30 = 0, m31 = 0, m32 = 0;
  if (translate) {
    m30 = double(mat.m30());
    m31 = double(mat.m31());
    m32 = double(mat.m32());
  }
  const double x0 = Fmadds(m10, vy, t11);
  const double y0 = Fmadds(m01, vx, t10);
  const double z0 = Fmadds(m02, vx, t9);
  const double x = Fmadds(vx, m00, x0);
  const double y = Fmadds(m21, vz, y0);
  const double z = Fmadds(m22, vz, z0);
  if (translate) {
    vo.set_x(float(Fadds(x, m30)));
    vo.set_y(float(Fadds(y, m31)));
    vo.set_z(float(Fadds(z, m32)));
  } else {
    vo.set_x(float(x));
    vo.set_y(float(y));
    vo.set_z(float(z));
  }
}

// A guest vector as the translated code holds it (word 0 in the top lane).
simde__m128 VectorAt(uint8_t* base, uint32_t address) {
  const V v(base, address);
  return simde_mm_castsi128_ps(simde_mm_set_epi32(int32_t(v.x_bits()), int32_t(v.y_bits()),
                                                  int32_t(v.z_bits()), int32_t(v.w_bits())));
}

}  // namespace

float Sqrtf(double x) {
  ITC_NATIVE_RUNNING("sub_82293590 sqrtf");
  return float(Fsqrts(x));
}

void MatrixSetTranslation(uint8_t* base, uint32_t matrix, uint32_t vector) {
  ITC_NATIVE_RUNNING("sub_82293818 Matrix::SetTranslation");
  const V v(base, vector);
  const M m(base, matrix);
  const uint32_t x = v.x_bits();
  const uint32_t y = v.y_bits();
  const uint32_t z = v.z_bits();
  const uint32_t one = G::one_bits(base);
  m.set_m30_bits(x);
  m.set_m31_bits(y);
  m.set_m32_bits(z);
  m.set_m33_bits(one);
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
  const V vv(base, v);
  const V vo(base, out);
  vo.set_x(float(Fmuls(double(vv.x()), s)));
  vo.set_y(float(Fmuls(double(vv.y()), s)));
  vo.set_z(float(Fmuls(double(vv.z()), s)));
  vo.set_w(float(Fmuls(double(vv.w()), s)));
}

void Vec4Copy(uint8_t* base, uint32_t out, uint32_t v) {
  ITC_NATIVE_RUNNING("sub_822940E0 Vec4::Copy");
  const V vv(base, v);
  const V vo(base, out);
  vo.set_x_bits(vv.x_bits());
  vo.set_y_bits(vv.y_bits());
  vo.set_z_bits(vv.z_bits());
  vo.set_w_bits(vv.w_bits());
}

void Vec3CrossW1(uint8_t* base, uint32_t out, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_82294108 Vec3::CrossW1");
  const V va(base, a);
  const V vb(base, b);
  const V vo(base, out);
  {
    const double by = double(vb.y());
    const double az = double(va.z());
    const double t = Fmuls(az, by);
    const double bz = double(vb.z());
    const double ay = double(va.y());
    vo.set_x(float(Fmsubs(bz, ay, t)));
  }
  {
    const double ax = double(va.x());
    const double bz = double(vb.z());
    const double t = Fmuls(bz, ax);
    const double bx = double(vb.x());
    const double az = double(va.z());
    vo.set_y(float(Fmsubs(az, bx, t)));
  }
  const double ay = double(va.y());
  const double bx = double(vb.x());
  const double t = Fmuls(bx, ay);
  const double by = double(vb.y());
  const double ax = double(va.x());
  vo.set_w_bits(G::one_bits(base));
  vo.set_z(float(Fmsubs(ax, by, t)));
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
  const V vv(base, v);
  const V vo(base, out);
  const double y = double(vv.y());
  const double y2 = Fmuls(y, y);
  const double x = double(vv.x());
  const double z = double(vv.z());
  const uint32_t one_word = G::one_bits(base);
  const double one = double(std::bit_cast<float>(one_word));
  const double squared = Fmadds(z, z, Fmadds(x, x, y2));
  const double length = Fsqrts(squared);
  const double inverse = Fdivs(one, length);
  vo.set_x(float(Fmuls(inverse, x)));
  vo.set_y(float(Fmuls(inverse, double(vv.y()))));
  const double scaled_z = Fmuls(inverse, double(vv.z()));
  vo.set_w_bits(one_word);
  vo.set_z(float(scaled_z));
  return float(length);
}

float Vec3Direction(uint8_t* base, uint32_t out, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_82294200 Vec3::Direction");
  const V va(base, a);
  const V vb(base, b);
  const V vo(base, out);
  const double bx = double(vb.x());
  const double ax = double(va.x());
  vo.set_x(float(Fsubs(ax, bx)));
  const double ay = double(va.y());
  const double by = double(vb.y());
  const double y = Fsubs(ay, by);
  vo.set_y(float(y));
  const double bz = double(vb.z());
  const double az = double(va.z());
  const double z = Fsubs(az, bz);
  const double x = double(vo.x());
  const uint32_t one_word = G::one_bits(base);
  vo.set_w_bits(one_word);
  vo.set_z(float(z));
  const double squared = Fmadds(z, z, Fmadds(x, x, Fmuls(y, y)));
  const double length = Fsqrts(squared);
  const double inverse = Fdivs(double(std::bit_cast<float>(one_word)), length);
  vo.set_x(float(Fmuls(inverse, x)));
  vo.set_y(float(Fmuls(inverse, y)));
  vo.set_z(float(Fmuls(inverse, z)));
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
  V(base, out).set_w_bits(G::zero_bits(base));
  Vec3Add(base, out, a, b);
}

void Vec3Difference(uint8_t* base, uint32_t out, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_822942E0 Vec3::Difference");
  V(base, out).set_w_bits(G::zero_bits(base));
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
  const double inverse = Fdivs(double(G::one(base)), s);
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
  V(base, out).set_w_bits(G::zero_bits(base));
  Vec4ScaleTo(base, out, v, s);
}

void Vec4ScaledFrom(uint8_t* base, uint32_t out, uint32_t v, double s) {
  ITC_NATIVE_RUNNING("sub_82294470 Vec4::ScaledFrom");
  V(base, out).set_w_bits(G::zero_bits(base));
  Vec4ScaleTo(base, out, v, s);
}

void Vec3ScaledCopy(uint8_t* base, uint32_t out, uint32_t v, double s) {
  ITC_NATIVE_RUNNING("sub_822944B0 Vec3::ScaledCopy");
  const V vv(base, v);
  const V vo(base, out);
  const double x = Fmuls(double(vv.x()), s);
  const double y = Fmuls(double(vv.y()), s);
  const double z = Fmuls(double(vv.z()), s);
  vo.set_x(float(x));
  vo.set_y(float(y));
  vo.set_z(float(z));
  vo.set_w_bits(G::one_bits(base));
}

void Vec3Cross(uint8_t* base, uint32_t out, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_82294518 Vec3::Cross");
  const V va(base, a);
  const V vb(base, b);
  const V vo(base, out);
  const double az = double(va.z());
  const double by = double(vb.y());
  const double bz = double(vb.z());
  const double t7 = Fmuls(by, az);
  const double ax = double(va.x());
  const double ay = double(va.y());
  const double t6 = Fmuls(ax, bz);
  const double bx = double(vb.x());
  const double t5 = Fmuls(bx, ay);
  const uint32_t zero = G::zero_bits(base);
  vo.set_x(float(Fmsubs(bz, ay, t7)));
  vo.set_y(float(Fmsubs(bx, az, t6)));
  vo.set_z(float(Fmsubs(ax, by, t5)));
  vo.set_w_bits(zero);
}

void Vec3Lerp(uint8_t* base, uint32_t out, uint32_t a, uint32_t b, double t) {
  ITC_NATIVE_RUNNING("sub_82294568 Vec3::Lerp");
  // (a - b) x t, in a temporary (the original's two on its stack).
  const V va(base, a);
  const V vb(base, b);
  const V vo(base, out);
  const double bx = double(vb.x());
  const double ax = double(va.x());
  const double dx = Fmuls(Fsubs(ax, bx), t);
  const double by = double(vb.y());
  const double ay = double(va.y());
  const double dy = Fmuls(Fsubs(ay, by), t);
  const double az = double(va.z());
  const double bz = double(vb.z());
  const double dz = Fmuls(Fsubs(az, bz), t);
  // Then out = a - that (Vec3::Sub's steps, a read again after each store).
  vo.set_w_bits(G::zero_bits(base));
  vo.set_x(float(Fsubs(double(va.x()), dx)));
  vo.set_y(float(Fsubs(double(va.y()), dy)));
  const double z = Fsubs(double(va.z()), dz);
  vo.set_w_bits(G::one_bits(base));
  vo.set_z(float(z));
}

float Vec3DotOf(uint8_t* base, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_82294608 Vec3::DotOf");
  return Vec3Dot(base, a, b);
}

Vector LoadVector(uint8_t* base, uint32_t address) {
  const V v(base, address);
  return {std::bit_cast<float>(v.x_bits()), std::bit_cast<float>(v.y_bits()),
          std::bit_cast<float>(v.z_bits()), std::bit_cast<float>(v.w_bits())};
}

void StoreVector(uint8_t* base, uint32_t address, const Vector& v) {
  const V to(base, address);
  to.set_x_bits(std::bit_cast<uint32_t>(v.x));
  to.set_y_bits(std::bit_cast<uint32_t>(v.y));
  to.set_z_bits(std::bit_cast<uint32_t>(v.z));
  to.set_w_bits(std::bit_cast<uint32_t>(v.w));
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
  AddOrSubSteps(base, out, a, [&parts](uint32_t i) { return double(parts[i]); }, subtract);
}

}  // namespace vector_library
