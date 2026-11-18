// The views of docs/functions/matrix-math.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/matrix-math.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// file; measure: Matrix, Matrix::Copy.r4, Matrix::Multiply.r4, Matrix::Multiply.r5, Matrix::Inverse.r4, Vec4::Transform.r4, Matrix::ProjectPoint.r4, sub_8209B038.r3
struct Matrix : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  float m00() const { return guest::LoadFloat(base, address + 0); }
  void set_m00(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t m00_address() const { return address + 0; }
  // m00 and m01 moved 8 bytes at a time (Matrix::Copy, the multiply's and inverse's first doubleword load)
  uint64_t m00_m01_bits() const { return guest::LoadU64(base, address + 0); }
  void set_m00_m01_bits(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t m00_m01_bits_address() const { return address + 0; }
  float m01() const { return guest::LoadFloat(base, address + 4); }
  void set_m01(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t m01_address() const { return address + 4; }
  float m02() const { return guest::LoadFloat(base, address + 8); }
  void set_m02(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t m02_address() const { return address + 8; }
  // m02 and m03 moved 8 bytes at a time (Matrix::Copy, the multiply's and inverse's first doubleword load)
  uint64_t m02_m03_bits() const { return guest::LoadU64(base, address + 8); }
  void set_m02_m03_bits(uint64_t v) const { guest::StoreU64(base, address + 8, v); }
  uint32_t m02_m03_bits_address() const { return address + 8; }
  // 1.0 after sub_8209B038 (SetWsToOne)
  float m03() const { return guest::LoadFloat(base, address + 12); }
  void set_m03(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t m03_address() const { return address + 12; }
  float m10() const { return guest::LoadFloat(base, address + 16); }
  void set_m10(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t m10_address() const { return address + 16; }
  // m10 and m11 moved 8 bytes at a time (Matrix::Copy, the multiply's and inverse's first doubleword load)
  uint64_t m10_m11_bits() const { return guest::LoadU64(base, address + 16); }
  void set_m10_m11_bits(uint64_t v) const { guest::StoreU64(base, address + 16, v); }
  uint32_t m10_m11_bits_address() const { return address + 16; }
  float m11() const { return guest::LoadFloat(base, address + 20); }
  void set_m11(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t m11_address() const { return address + 20; }
  float m12() const { return guest::LoadFloat(base, address + 24); }
  void set_m12(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t m12_address() const { return address + 24; }
  // m12 and m13 moved 8 bytes at a time (Matrix::Copy, the multiply's and inverse's first doubleword load)
  uint64_t m12_m13_bits() const { return guest::LoadU64(base, address + 24); }
  void set_m12_m13_bits(uint64_t v) const { guest::StoreU64(base, address + 24, v); }
  uint32_t m12_m13_bits_address() const { return address + 24; }
  // 1.0 after sub_8209B038
  float m13() const { return guest::LoadFloat(base, address + 28); }
  void set_m13(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t m13_address() const { return address + 28; }
  float m20() const { return guest::LoadFloat(base, address + 32); }
  void set_m20(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t m20_address() const { return address + 32; }
  // m20 and m21 moved 8 bytes at a time (Matrix::Copy, the multiply's and inverse's first doubleword load)
  uint64_t m20_m21_bits() const { return guest::LoadU64(base, address + 32); }
  void set_m20_m21_bits(uint64_t v) const { guest::StoreU64(base, address + 32, v); }
  uint32_t m20_m21_bits_address() const { return address + 32; }
  float m21() const { return guest::LoadFloat(base, address + 36); }
  void set_m21(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t m21_address() const { return address + 36; }
  float m22() const { return guest::LoadFloat(base, address + 40); }
  void set_m22(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t m22_address() const { return address + 40; }
  // m22 and m23 moved 8 bytes at a time (Matrix::Copy, the multiply's and inverse's first doubleword load)
  uint64_t m22_m23_bits() const { return guest::LoadU64(base, address + 40); }
  void set_m22_m23_bits(uint64_t v) const { guest::StoreU64(base, address + 40, v); }
  uint32_t m22_m23_bits_address() const { return address + 40; }
  // 1.0 after sub_8209B038
  float m23() const { return guest::LoadFloat(base, address + 44); }
  void set_m23(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t m23_address() const { return address + 44; }
  // the translation's x
  float m30() const { return guest::LoadFloat(base, address + 48); }
  void set_m30(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t m30_address() const { return address + 48; }
  // m30 and m31 moved 8 bytes at a time (Matrix::Copy, the multiply's and inverse's first doubleword load)
  uint64_t m30_m31_bits() const { return guest::LoadU64(base, address + 48); }
  void set_m30_m31_bits(uint64_t v) const { guest::StoreU64(base, address + 48, v); }
  uint32_t m30_m31_bits_address() const { return address + 48; }
  // the translation's y
  float m31() const { return guest::LoadFloat(base, address + 52); }
  void set_m31(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t m31_address() const { return address + 52; }
  // the translation's z
  float m32() const { return guest::LoadFloat(base, address + 56); }
  void set_m32(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t m32_address() const { return address + 56; }
  // m32 and m33 moved 8 bytes at a time (Matrix::Copy, the multiply's and inverse's first doubleword load)
  uint64_t m32_m33_bits() const { return guest::LoadU64(base, address + 56); }
  void set_m32_m33_bits(uint64_t v) const { guest::StoreU64(base, address + 56, v); }
  uint32_t m32_m33_bits_address() const { return address + 56; }
  // 1.0 after sub_8209B038
  float m33() const { return guest::LoadFloat(base, address + 60); }
  void set_m33(float v) const { guest::StoreFloat(base, address + 60, v); }
  uint32_t m33_address() const { return address + 60; }
};

// embedded; measure: Quaternion, Matrix::FromQuaternion.r4, Quaternion::Multiply.r4
struct Quaternion : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  float x() const { return guest::LoadFloat(base, address + 0); }
  void set_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t x_address() const { return address + 0; }
  // x and y copied 8 bytes at a time (RigidBody::SetOrientation)
  uint64_t xy_bits() const { return guest::LoadU64(base, address + 0); }
  void set_xy_bits(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t xy_bits_address() const { return address + 0; }
  float y() const { return guest::LoadFloat(base, address + 4); }
  void set_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t y_address() const { return address + 4; }
  float z() const { return guest::LoadFloat(base, address + 8); }
  void set_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t z_address() const { return address + 8; }
  // z and w copied 8 bytes at a time (RigidBody::SetOrientation)
  uint64_t zw_bits() const { return guest::LoadU64(base, address + 8); }
  void set_zw_bits(uint64_t v) const { guest::StoreU64(base, address + 8, v); }
  uint32_t zw_bits_address() const { return address + 8; }
  float w() const { return guest::LoadFloat(base, address + 12); }
  void set_w(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t w_address() const { return address + 12; }
  // a measure fault: Quaternion::ToMatrix's and ToMatrix3's stores into the matrix they make (a Matrix's m10)
  float fault_m10() const { return guest::LoadFloat(base, address + 16); }
  void set_fault_m10(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t fault_m10_address() const { return address + 16; }
  // a measure fault: Quaternion::ToMatrix's and ToMatrix3's stores into the matrix they make (a Matrix's m11)
  float fault_m11() const { return guest::LoadFloat(base, address + 20); }
  void set_fault_m11(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t fault_m11_address() const { return address + 20; }
  // a measure fault: Quaternion::ToMatrix's and ToMatrix3's stores into the matrix they make (a Matrix's m12)
  float fault_m12() const { return guest::LoadFloat(base, address + 24); }
  void set_fault_m12(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t fault_m12_address() const { return address + 24; }
  // a measure fault: Quaternion::ToMatrix's and ToMatrix3's stores into the matrix they make (a Matrix's m20)
  float fault_m20() const { return guest::LoadFloat(base, address + 32); }
  void set_fault_m20(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t fault_m20_address() const { return address + 32; }
  // a measure fault: Quaternion::ToMatrix's and ToMatrix3's stores into the matrix they make (a Matrix's m21)
  float fault_m21() const { return guest::LoadFloat(base, address + 36); }
  void set_fault_m21(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t fault_m21_address() const { return address + 36; }
  // a measure fault: Quaternion::ToMatrix's and ToMatrix3's stores into the matrix they make (a Matrix's m22)
  float fault_m22() const { return guest::LoadFloat(base, address + 40); }
  void set_fault_m22(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t fault_m22_address() const { return address + 40; }
  // a measure fault: Quaternion::ToMatrix's and ToMatrix3's stores into the matrix they make (a Matrix's m30)
  float fault_m30() const { return guest::LoadFloat(base, address + 48); }
  void set_fault_m30(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t fault_m30_address() const { return address + 48; }
  // a measure fault: Quaternion::ToMatrix's and ToMatrix3's stores into the matrix they make (a Matrix's m31)
  float fault_m31() const { return guest::LoadFloat(base, address + 52); }
  void set_fault_m31(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t fault_m31_address() const { return address + 52; }
  // a measure fault: Quaternion::ToMatrix's and ToMatrix3's stores into the matrix they make (a Matrix's m32)
  float fault_m32() const { return guest::LoadFloat(base, address + 56); }
  void set_fault_m32(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t fault_m32_address() const { return address + 56; }
  // a measure fault: Quaternion::ToMatrix's and ToMatrix3's stores into the matrix they make (a Matrix's m33)
  float fault_m33() const { return guest::LoadFloat(base, address + 60); }
  void set_fault_m33(float v) const { guest::StoreFloat(base, address + 60, v); }
  uint32_t fault_m33_address() const { return address + 60; }
};

// embedded; measure: Matrix::InverseRotate.r5, Quaternion::FromAxisAngle.r4
struct MatrixMathVector3 : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  float x() const { return guest::LoadFloat(base, address + 0); }
  void set_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t x_address() const { return address + 0; }
  float y() const { return guest::LoadFloat(base, address + 4); }
  void set_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t y_address() const { return address + 4; }
  float z() const { return guest::LoadFloat(base, address + 8); }
  void set_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t z_address() const { return address + 8; }
};

// embedded; measure: Matrix::RotateX.r5, Matrix::RotateZ.r5, SinCos.r4, SinCos.r5
struct MatrixMathFloat : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  float value() const { return guest::LoadFloat(base, address + 0); }
  void set_value(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// The image words docs/functions/matrix-math.md's code uses.
struct MatrixMathGlobals {
  // the turns' sine and cosine polynomials' coefficients: six 16-byte vectors (Matrix::RotateX, Y and Z)
  static constexpr uint32_t turn_polynomials_address() { return 0x820009C0u; }
  // 1.0
  static float one(uint8_t* base) { return guest::LoadFloat(base, 0x82000A30u); }
  static void set_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // 0.0 (probably)
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // 0.5 (probably): half angles, the quaternion from a matrix
  static float half(uint8_t* base) { return guest::LoadFloat(base, 0x82001BE4u); }
  static void set_half(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BE4u, v); }
  static constexpr uint32_t half_address() { return 0x82001BE4u; }
  // 1/(2 pi) (probably): the turns' angle brought into [-pi, pi]
  static float inverse_two_pi(uint8_t* base) { return guest::LoadFloat(base, 0x82001FA4u); }
  static void set_inverse_two_pi(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FA4u, v); }
  static constexpr uint32_t inverse_two_pi_address() { return 0x82001FA4u; }
  // 2 pi (probably)
  static float two_pi(uint8_t* base) { return guest::LoadFloat(base, 0x82001FA8u); }
  static void set_two_pi(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FA8u, v); }
  static constexpr uint32_t two_pi_address() { return 0x82001FA8u; }
  // 2.0 (probably): the quaternion's matrix
  static float two(uint8_t* base) { return guest::LoadFloat(base, 0x820021E8u); }
  static void set_two(uint8_t* base, float v) { guest::StoreFloat(base, 0x820021E8u, v); }
  static constexpr uint32_t two_address() { return 0x820021E8u; }
  // probably (0, 0, 0, 1): Matrix::FromQuaternionAndRow's fourth row
  static constexpr uint32_t fourth_row_address() { return 0x826382D0u; }
  // a zero matrix, built at boot by ZeroMatrix::StaticInit (sub_825D8F88)
  static constexpr uint32_t zero_matrix_address() { return 0x8266D000u; }
  // the identity, built at boot by IdentityMatrix::StaticInit (sub_825D9060)
  static constexpr uint32_t identity_matrix_address() { return 0x8266D040u; }
  // probably the identity matrix (Matrix::SetIdentity copies it)
  static constexpr uint32_t identity_address() { return 0x82953740u; }
};

}  // namespace schema
