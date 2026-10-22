// The game's matrix, vector and quaternion math as plain C++
// (docs/functions/matrix-math.md: 4x4 matrices of 16-byte rows, used with
// row vectors; quaternions (x, y, z, w)). The matrices are in guest memory,
// shared with the recompiled code, so these work on guest memory, with the
// originals' loads and stores in their order where an output may be one of
// the inputs, and their arithmetic step for step so the results are the
// translated code's bit for bit:
// - the vector unit's work (vmsum4fp128's dot products, the inverse's
//   steps) as the translation does it, on SSE vectors with word 0 in the top
//   lane and flush-to-zero on (set and put back here; the adapters leave it
//   on afterwards, as the translated code does);
// - the scalar work as the translation's doubles: fmuls, fadds, fsubs and
//   fdivs rounded to single, fmadds, fmsubs and fnmsubs as std::fma rounded
//   to single, the operands in the PowerPC code's order; flush-to-zero off
//   (the adapters' job).
// Built with -ffp-model=strict and -ffp-contract=off (CMakeLists.txt).
// Adapters: src/hooks/matrix_math.cpp; Matrix::Multiply's and
// Matrix::Copy's in src/interpolation/blend_state.cpp, Vec4::Transform's in
// src/debug/frame_state_diff.cpp (they keep the interpolation's and the
// diagnostics' calls around the work).

#pragma once

#include <cstdint>

namespace matrix_math {

// Matrix::Multiply (sub_8209BD50): out = a * b, both read first (any
// alignment), out stored aligned (to its 16-byte boundary, as stvx).
void Multiply(uint8_t* base, uint32_t out, uint32_t a, uint32_t b);
// Matrix::Copy (sub_820A0AC0): the 16 words, copied five times over as the
// original does (it only matters when the two overlap).
void Copy(uint8_t* base, uint32_t dst, uint32_t src);
// Matrix::Inverse (sub_820A9988): out = the inverse of in (read first, any
// alignment), out stored aligned.
void Inverse(uint8_t* base, uint32_t out, uint32_t in);
// Matrix::RotateX, RotateY and RotateZ (sub_820A65F0, sub_820A67F8,
// sub_820A2D30): out = in * the turn about that axis by the float at
// `angle`, its sine and cosine from the game's polynomials (not std::sin).
// The turn is built at `turn` (64 bytes of guest memory, 16-byte aligned:
// the original's stack, r1 - 80), then multiplied with Multiply and noted
// for interpolation (interpolation::BlendState_OnMultiply) with the return
// address of the original's call.
void RotateX(uint8_t* base, uint32_t out, uint32_t in, uint32_t angle, uint32_t turn);
void RotateY(uint8_t* base, uint32_t out, uint32_t in, uint32_t angle, uint32_t turn);
void RotateZ(uint8_t* base, uint32_t out, uint32_t in, uint32_t angle, uint32_t turn);
// Vec4::Transform (sub_820AB560): out = v * m, all four components; m read
// first (any alignment), v and out aligned.
void Vec4Transform(uint8_t* base, uint32_t out, uint32_t m, uint32_t v);
// Vec3::SwapXZ (sub_820AB688): swaps the words at +0 and +8.
void Vec3SwapXZ(uint8_t* base, uint32_t v);
// sub_8209B038: the words at +12, +28, +44 and +60 = 1.0 (0x82000A30's).
void SetWsToOne(uint8_t* base, uint32_t m);

// Matrix::MultiplyUnaligned (sub_822B8F78): out = a * b, any word-aligned
// addresses, all read before out is written.
void MultiplyUnaligned(uint8_t* base, uint32_t out, uint32_t a, uint32_t b);
// Matrix::MultiplyReversed (sub_82293988): out = b * a (through
// Matrix::MultiplyTo, sub_82293598, merged: a tail call).
void MultiplyReversed(uint8_t* base, uint32_t out, uint32_t a, uint32_t b);
// Matrix::InverseRotate (sub_82293998): out = (v.x, v.y, v.z) times the
// transposed upper 3x3 of m, w 0.0 (Matrix::Transpose, sub_822936D8, and
// Matrix::TransposeUnaligned, sub_822B9158, merged: the transposed words
// held here instead of on the original's stack).
void InverseRotate(uint8_t* base, uint32_t out, uint32_t m, uint32_t v);
// Matrix::Set (sub_822935A0): the four rows copied with Vec4::Copy.
void Set(uint8_t* base, uint32_t out, uint32_t in);
// Matrix::SetIdentity (sub_822936E0): Set(out, 0x82953740).
void SetIdentity(uint8_t* base, uint32_t out);
// Matrix::FromQuaternion (sub_822936F0): the upper 3x3 of the quaternion's
// turn; the fourth row and column left as they were.
void FromQuaternion(uint8_t* base, uint32_t out, uint32_t q);
// Quaternion::LengthSquared (sub_82293CB0): x² + y² + z² + w².
float QuaternionLengthSquared(uint8_t* base, uint32_t q);
// Matrix::ProjectPoint (sub_82293600): (p, 1) * m divided by its w, out w
// 1.0; all read before out is written.
void ProjectPoint(uint8_t* base, uint32_t out, uint32_t m, uint32_t p);
// Quaternion::FromMatrix (sub_82293AD8): the quaternion of m's upper 3x3.
void QuaternionFromMatrix(uint8_t* base, uint32_t out, uint32_t m);
// Quaternion::Multiply (sub_82293D78): out = a b (Hamilton product).
void QuaternionMultiply(uint8_t* base, uint32_t out, uint32_t a, uint32_t b);
// Quaternion::NormalizeSelf (sub_82293F10): q / |q|.
void QuaternionNormalizeSelf(uint8_t* base, uint32_t q);
// SinCos (sub_820B0E10): sin and cos of the angle stored as floats at
// `sine` and `cosine` (the C runtime's, src/c_runtime/math.h); returns the
// cosine's double (the original leaves it in f1).
double SinCos(uint8_t* base, double angle, uint32_t sine, uint32_t cosine);

}  // namespace matrix_math
