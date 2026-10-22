// The game's vector library as plain C++ (docs/functions/vehicle-accessors.md:
// 3D vectors kept in 16 bytes, x, y, z at +0, +4, +8 and a w at +12; 4x4
// matrices of 16-byte rows). The vectors are in guest memory, shared with
// the recompiled physics, so these work on guest memory, with the
// original's loads and stores in its order (an output may be one of the
// inputs) and its arithmetic step for step: fmuls, fadds and fsubs as the
// doubles' result rounded to single, fmadds and fmsubs as std::fma on the
// doubles rounded to single (as the translated code), the w each leaves as
// it was. Flush-to-zero off (the callers' job), except in the dot product,
// which turns it on as the translated vmsum3fp128. Adapters:
// src/hooks/vehicle_accessors.cpp.

#pragma once

#include <cstdint>

namespace vector_library {

// sqrtf (sub_82293590): fsqrts, the double's square root rounded to single.
float Sqrtf(double x);

// Matrix::SetTranslation (sub_82293818): the fourth row (+48) = the
// vector's x, y, z (their bits) and 1.0.
void MatrixSetTranslation(uint8_t* base, uint32_t matrix, uint32_t vector);
// Matrix::Rotate (sub_82293880): out = x row 0 + y row 1 + z row 2, w 0.0.
void MatrixRotate(uint8_t* base, uint32_t out, uint32_t matrix, uint32_t vector);
// Matrix::TransformPoint (sub_822938F0): Rotate plus the fourth row, w 0.0.
void MatrixTransformPoint(uint8_t* base, uint32_t out, uint32_t matrix, uint32_t vector);

// Vec3::Add (sub_82294028) and Vec3::Sub (sub_82294068): out = a + b, a -
// b, w 1.0.
void Vec3Add(uint8_t* base, uint32_t out, uint32_t a, uint32_t b);
void Vec3Sub(uint8_t* base, uint32_t out, uint32_t a, uint32_t b);
// Vec4::ScaleTo (sub_822940A8): out = v x s, all four.
void Vec4ScaleTo(uint8_t* base, uint32_t out, uint32_t v, double s);
// Vec4::Copy (sub_822940E0): the four words' bits.
void Vec4Copy(uint8_t* base, uint32_t out, uint32_t v);
// Vec3::CrossW1 (sub_82294108): a x b, w 1.0.
void Vec3CrossW1(uint8_t* base, uint32_t out, uint32_t a, uint32_t b);
// Vec3::Dot (sub_82294170): a.x b.x + a.y b.y + a.z b.z as vmsum3fp128
// (dpps, flush-to-zero on).
float Vec3Dot(uint8_t* base, uint32_t a, uint32_t b);
// Vec3::Normalize (sub_822941B0): out = v / its length, w 1.0; the length.
float Vec3Normalize(uint8_t* base, uint32_t out, uint32_t v);
// Vec3::Direction (sub_82294200): out = (a - b) normalized, w 1.0; the
// length.
float Vec3Direction(uint8_t* base, uint32_t out, uint32_t a, uint32_t b);
// Vec3::Length (sub_82294270): sqrtf(Dot(v, v)).
float Vec3Length(uint8_t* base, uint32_t v);
// Vec3::NormalizeSelf (sub_82294298): Normalize(v, v).
float Vec3NormalizeSelf(uint8_t* base, uint32_t v);
// Vec3::Sum (sub_822942A0) and Vec3::Difference (sub_822942E0): out's w =
// 0.0, then Add or Sub (w 1.0).
void Vec3Sum(uint8_t* base, uint32_t out, uint32_t a, uint32_t b);
void Vec3Difference(uint8_t* base, uint32_t out, uint32_t a, uint32_t b);
// Vec3::AddTo (sub_82294320): Add(v, v, b).
void Vec3AddTo(uint8_t* base, uint32_t v, uint32_t b);
// Vec4::ScaleBy (sub_82294340): ScaleTo(v, v, s).
void Vec4ScaleBy(uint8_t* base, uint32_t v, double s);
// Vec4::DivideBy (sub_82294348): ScaleTo(v, v, 1.0 / s), the reciprocal
// (fdivs) returned (the original leaves it in f1).
float Vec4DivideBy(uint8_t* base, uint32_t v, double s);
// sub_82294360 and sub_822943C8: a - b and a + b through a temporary (all
// loads before out is written), w 1.0.
void Vec3SubCopy(uint8_t* base, uint32_t out, uint32_t a, uint32_t b);
void Vec3AddCopy(uint8_t* base, uint32_t out, uint32_t a, uint32_t b);
// Vec4::Scaled (sub_82294430): out's w = 0.0, then ScaleTo(out, v, s).
void Vec4Scaled(uint8_t* base, uint32_t out, uint32_t v, double s);
// sub_82294470: as Vec4::Scaled, with the vector in r5 (r4 isn't read).
void Vec4ScaledFrom(uint8_t* base, uint32_t out, uint32_t v, double s);
// sub_822944B0: v x s through a temporary, w 1.0.
void Vec3ScaledCopy(uint8_t* base, uint32_t out, uint32_t v, double s);
// Vec3::Cross (sub_82294518): a x b, w 0.0.
void Vec3Cross(uint8_t* base, uint32_t out, uint32_t a, uint32_t b);
// Vec3::Lerp (sub_82294568): out = a - (a - b) x t, w 1.0.
void Vec3Lerp(uint8_t* base, uint32_t out, uint32_t a, uint32_t b, double t);
// Vec3::DotOf (sub_82294608): Dot(a, b).
float Vec3DotOf(uint8_t* base, uint32_t a, uint32_t b);

}  // namespace vector_library
