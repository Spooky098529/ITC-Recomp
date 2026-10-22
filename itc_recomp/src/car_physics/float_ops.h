// The PowerPC float steps the car physics' plain C++ is written in
// (src/car_physics/), as the translated code does them: lfs widens a single
// to a double; fmuls, fadds, fsubs and fdivs round the double's result to
// single; fmadds, fmsubs and fnmsubs are std::fma on the doubles rounded to
// single; an lfs then stfs of one word keeps its bits. Only for files built
// with -ffp-model=strict and -ffp-contract=off (CMakeLists.txt), with
// flush-to-zero off (the adapters' job).

#pragma once

#include "guest.h"

#include <bit>
#include <cmath>
#include <cstdint>

namespace car_physics::ops {

constexpr uint32_t kOne = 0x82000A30;         // the game's float 1.0
constexpr uint32_t kZero = 0x82000A34;        // the game's float 0.0
constexpr uint32_t kZeroVector = 0x82953790;  // the game's zero vector

inline double Lfs(uint8_t* base, uint32_t address) {
  return double(guest::LoadFloat(base, address));
}
inline void Stfs(uint8_t* base, uint32_t address, double value) {
  guest::StoreFloat(base, address, float(value));
}
// An lfs then stfs of one word: its bits.
inline void CopyWord(uint8_t* base, uint32_t from, uint32_t to) {
  guest::StoreU32(base, to, guest::LoadU32(base, from));
}
inline double Fmuls(double a, double b) {
  return double(float(a * b));
}
inline double Fadds(double a, double b) {
  return double(float(a + b));
}
inline double Fsubs(double a, double b) {
  return double(float(a - b));
}
inline double Fdivs(double a, double b) {
  return double(float(a / b));
}
inline double Fmadds(double a, double b, double c) {
  return double(float(std::fma(a, b, c)));
}
inline double Fmsubs(double a, double b, double c) {
  return double(float(std::fma(a, b, -c)));
}
inline double Fnmsubs(double a, double b, double c) {
  return double(float(-std::fma(a, b, -c)));
}
inline double Fnmadds(double a, double b, double c) {
  return double(float(-std::fma(a, b, c)));
}
// fneg and fabs: the sign bit only.
inline double Fneg(double a) {
  return std::bit_cast<double>(std::bit_cast<uint64_t>(a) ^ 0x8000000000000000ull);
}
inline double Fabs(double a) {
  return std::bit_cast<double>(std::bit_cast<uint64_t>(a) & ~0x8000000000000000ull);
}

}  // namespace car_physics::ops
