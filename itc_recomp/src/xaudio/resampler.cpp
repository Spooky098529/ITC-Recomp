// XAudio's sample rate converter as plain C++ (resampler.h;
// docs/functions/xaudio-objects.md). The kernels are translated step by step
// from the original (the registers' names kept where a value has no better
// one), as their float results must be the original's bit for bit: the same
// vector operations, in the same order and with the same operands, under the
// same flush-to-zero setting (Mode below).

#include "xaudio/resampler.h"

#include "base/log.h"
#include "c_runtime/math.h"
#include "guest.h"

#include <rex/platform/fpscr.h>
#include <rex/ppc/intrinsics.h>
#include <simde/x86/sse4.1.h>

#include <bit>
#include <climits>
#include <cmath>

namespace xaudio {

namespace {

using Fpscr = rex::platform::FPSCRPlatform;
using Vec = simde__m128i;

// The host's flush to zero (and denormals as zero) as the translated code
// sets it: on for the vector unit's float operations, off for the scalar
// unit's; the setting found on entry put back on return.
class Mode {
 public:
  Mode() : saved_(Fpscr::getcsr()) {}
  ~Mode() { Fpscr::setcsr(saved_); }
  Mode(const Mode&) = delete;
  Mode& operator=(const Mode&) = delete;
  void Vector() { Fpscr::setcsr(Fpscr::getcsr() | uint32_t(Fpscr::FlushMask)); }
  void Scalar() { Fpscr::setcsr(Fpscr::getcsr() & ~uint32_t(Fpscr::FlushMask)); }

 private:
  uint32_t saved_;
};

// lvx, lvlx and stvx (aligned down, the words byte-swapped, the lanes in the
// translated code's order) and stvewx (the lane its address selects).
Vec Lvx(uint8_t* base, uint32_t address) {
  return simde_mm_shuffle_epi8(
      simde_mm_load_si128(reinterpret_cast<const Vec*>(REX_RAW_ADDR(address & ~0xFu))),
      simde_mm_load_si128(reinterpret_cast<const Vec*>(rex::ppc::VectorMaskL)));
}
Vec Lvlx(uint8_t* base, uint32_t address) {
  return simde_mm_shuffle_epi8(
      simde_mm_load_si128(reinterpret_cast<const Vec*>(REX_RAW_ADDR(address & ~0xFu))),
      simde_mm_load_si128(
          reinterpret_cast<const Vec*>(&rex::ppc::VectorMaskL[(address & 0xF) * 16])));
}
void Stvx(uint8_t* base, uint32_t address, Vec value) {
  simde_mm_store_si128(
      reinterpret_cast<Vec*>(REX_RAW_ADDR(address & ~0xFu)),
      simde_mm_shuffle_epi8(value,
                            simde_mm_load_si128(reinterpret_cast<const Vec*>(rex::ppc::VectorMaskL))));
}
void Stvewx(uint8_t* base, uint32_t address, Vec value) {
  const uint32_t ea = address & ~3u;
  alignas(16) uint32_t lanes[4];
  simde_mm_store_si128(reinterpret_cast<Vec*>(lanes), value);
  guest::StoreU32(base, ea, lanes[3 - ((ea & 0xF) >> 2)]);
}

// Each value passed through an empty asm so nothing moves across the
// control word's changes.
Vec Pin(Vec value) {
  asm volatile("" : "+x"(value));
  return value;
}
simde__m128 F(Vec v) { return simde_mm_castsi128_ps(Pin(v)); }
Vec I(simde__m128 v) { return Pin(simde_mm_castps_si128(v)); }

Vec Zero() { return simde_mm_set1_epi32(0); }
Vec Splat0(Vec v) { return simde_mm_shuffle_epi32(v, 0xFF); }  // vspltw 0
Vec AddI(Vec a, Vec b) { return simde_mm_add_epi32(a, b); }
template <int bytes>
Vec Sld(Vec a, Vec b) {  // vsldoi, as the translated code does it
  return simde_mm_alignr_epi8(a, b, bytes);
}
// vsrw by the low five bits of each of b's words.
Vec Srw(Vec a, Vec b) {
  alignas(16) uint32_t x[4], s[4];
  simde_mm_store_si128(reinterpret_cast<Vec*>(x), a);
  simde_mm_store_si128(reinterpret_cast<Vec*>(s), b);
  for (int k = 0; k < 4; k++) {
    x[k] = x[k] >> (s[k] & 0x1F);
  }
  return simde_mm_load_si128(reinterpret_cast<const Vec*>(x));
}
Vec Vsr(Vec a, Vec b) { return rex::ppc::simde_mm_vsr(a, b); }
Vec Perm(Vec a, Vec b, Vec c) { return rex::ppc::simde_mm_perm_epi8_(a, b, c); }
// vupkhsh and vupklsh.
Vec High16(Vec v) { return simde_mm_cvtepi16_epi32(simde_mm_unpackhi_epi64(v, v)); }
Vec Low16(Vec v) { return simde_mm_cvtepi16_epi32(v); }

Vec Add(Vec a, Vec b) { return I(simde_mm_add_ps(F(a), F(b))); }
Vec Sub(Vec a, Vec b) { return I(simde_mm_sub_ps(F(a), F(b))); }
Vec Mul(Vec a, Vec b) { return I(simde_mm_mul_ps(F(a), F(b))); }
Vec Madd(Vec a, Vec b, Vec c) { return Add(Mul(a, b), c); }  // vmaddfp: a x b, then + c
Vec Reciprocal(Vec v) { return I(simde_mm_div_ps(simde_mm_set1_ps(1), F(v))); }
Vec ToFloat(Vec v) { return I(simde_mm_cvtepi32_ps(Pin(v))); }
// vcfsx and vcfux with a scale (the scale's float bits).
Vec SignedToFloat(Vec v, uint32_t scale) {
  return I(simde_mm_mul_ps(simde_mm_cvtepi32_ps(Pin(v)),
                           simde_mm_castsi128_ps(simde_mm_set1_epi32(int(scale)))));
}
Vec UnsignedToFloat(Vec v, uint32_t scale) {
  return I(simde_mm_mul_ps(rex::ppc::simde_mm_cvtepu32_ps_(Pin(v)),
                           simde_mm_castsi128_ps(simde_mm_set1_epi32(int(scale)))));
}
// The dot product the translated code makes of vmsum4fp: a's lane 3 replaced
// by b's (the blend), then the four lanes' sum.
Vec Blend3(Vec a, Vec b) { return I(simde_mm_blend_ps(F(a), simde_mm_permute_ps(F(b), 228), 8)); }
Vec Dot(Vec a, Vec b) { return I(simde_mm_dp_ps(F(a), F(b), 0xEF)); }

constexpr uint32_t kSample = 0x38000000;    // 2^-15: a 16-bit sample to a float
constexpr uint32_t kFraction = 0x30000000;  // 2^-31: half a 32-bit fraction to a float

double LoadSingle(uint8_t* base, uint32_t address) {
  return double(guest::LoadFloat(base, address));
}
double LoadDouble(uint8_t* base, uint32_t address) {
  return std::bit_cast<double>((uint64_t(guest::LoadU32(base, address)) << 32) |
                               guest::LoadU32(base, address + 4));
}
uint64_t LoadU64(uint8_t* base, uint32_t address) {
  return (uint64_t(guest::LoadU32(base, address)) << 32) | guest::LoadU32(base, address + 4);
}
void StoreU64(uint8_t* base, uint32_t address, uint64_t value) {
  guest::StoreU32(base, address, uint32_t(value >> 32));
  guest::StoreU32(base, address + 4, uint32_t(value));
}
// A float word stored from the float register it was loaded into (lfs, then
// stfs): its bits, as the translated code leaves them (a signaling NaN kept).
void CopyFloat(uint8_t* base, uint32_t destination, uint32_t source) {
  guest::StoreU32(base, destination, guest::LoadU32(base, source));
}
// fctidz as the translated code does it.
int64_t ToInt64(double x) {
  return std::isnan(x)               ? int64_t(0x8000000000000000ull)
         : x > double(LLONG_MAX)     ? LLONG_MAX
                                     : simde_mm_cvttsd_si64(simde_mm_load_sd(&x));
}
uint64_t Ext(uint32_t value) { return uint64_t(int64_t(int32_t(value))); }  // extsw
// A frame count by a byte count (the channels x 2): divwu, 0 for 0 (the
// original traps there; these kernels are picked for one or two channels).
uint32_t Frames(uint32_t bytes, uint32_t channels) {
  const uint32_t divisor = channels << 1;
  return divisor != 0 ? bytes / divisor : 0;
}

// The end of each kernel: the position (+8, at most the end), the frames
// written (+28, at most the output's) from the input and output pointers.
void StorePositions(uint8_t* base, uint32_t state, uint64_t input, uint64_t output) {
  const uint32_t channels = guest::LoadU8(base, state + 13);
  const uint32_t frames = Frames(uint32_t(input - guest::LoadU32(base, state + 0)), channels);
  const uint32_t end = guest::LoadU32(base, state + 4);
  guest::StoreU32(base, state + 8, frames < end ? frames : end);
  const uint32_t written = uint32_t(output - guest::LoadU32(base, state + 20)) >> 2;
  const uint32_t limit = guest::LoadU32(base, state + 24);
  guest::StoreU32(base, state + 28, written < limit ? written : limit);
}

}  // namespace


// --- The kernels for any ratio (entries 81 and 82, 6 and 7) ---

namespace {

// sub_8246E208: the vector path of sub_8247E188, eight mono frames a pass
// (r3 the input at the position, r5 the output at the frames written, r6
// the frames left; the original's 176-byte frame below `sp`). The position
// is 32.32 fixed point (+48 x 2^32, the ratio's step +44 x 2^32: the double
// at 0x8204D240); the volume ramps from +36 to +40 over the frames left
// (+36 = +40 at once). Returns __ultod's r3 (the fraction's double, whose
// float x 2^-32 goes to +48).
uint64_t ResampleMonoVector(uint8_t* base, uint64_t r3, uint64_t r5, uint64_t r6, uint32_t state,
                            uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8246E208 the mono resampler's vector path");
  const uint32_t frame = sp - 176;
  Mode mode;
  mode.Scalar();
  uint64_t r11 = r3;
  const double f11 = LoadSingle(base, state + 44);
  double f10 = LoadSingle(base, state + 48);
  double f0 = LoadSingle(base, state + 36);
  const uint64_t r10 = Ext(uint32_t(r6));
  double f13 = LoadSingle(base, state + 40);
  CopyFloat(base, frame + 80, state + 36);
  f0 = double(float(f13 - f0));
  CopyFloat(base, state + 36, state + 40);
  Vec v0 = Zero();
  const Vec v4 = simde_mm_set1_epi32(1);
  StoreU64(base, frame + 88, r10);
  f13 = double(int64_t(LoadU64(base, frame + 88)));
  Vec v13 = Lvlx(base, frame + 80);
  Vec v10 = Splat0(v13);
  Vec v1 = v0, v31 = v0;
  double f12 = LoadDouble(base, 0x8204D240);
  f10 = f10 * f12;
  f12 = f11 * f12;
  f13 = double(float(f13));
  Vec v11 = Lvx(base, 0x8204D250);
  const int64_t fraction = ToInt64(f10);
  const int64_t step = ToInt64(f12);
  Vec v8 = Lvx(base, 0x8204D260);
  f0 = double(float(f0 / f13));
  f13 = LoadSingle(base, 0x82000C34);
  f13 = double(float(f0 * f13));
  guest::StoreFloat(base, frame + 80, float(f13));
  Vec v12 = Lvlx(base, frame + 80);
  guest::StoreFloat(base, frame + 80, float(f0));
  const Vec v30 = Splat0(v12);
  Vec v9 = Lvlx(base, frame + 80);
  StoreU64(base, frame + 88, uint64_t(fraction));
  v12 = Splat0(v9);
  StoreU64(base, frame + 80, uint64_t(step));
  Vec v7 = Lvlx(base, frame + 84);
  Vec v6 = Lvlx(base, frame + 92);
  v13 = Splat0(v7);
  Stvx(base, frame + 96, v0);
  mode.Vector();
  v11 = Mul(v12, v11);
  v8 = Mul(v12, v8);
  v9 = Splat0(v6);
  v12 = AddI(v13, v13);
  v7 = AddI(v13, v12);
  v11 = Add(v10, v11);
  v10 = Add(v10, v8);
  v8 = Sld<12>(v0, v13);
  v13 = Sld<12>(v8, v12);
  Stvx(base, frame + 96, v0);
  v0 = AddI(v12, v12);
  const Vec v29 = AddI(v0, v0);
  Stvx(base, frame + 96, v8);
  Stvx(base, frame + 96, v13);
  v13 = Sld<12>(v13, v7);
  v0 = AddI(v13, v0);
  Stvx(base, frame + 96, v13);
  const uint64_t r8 = LoadU64(base, frame + 80);
  r3 = LoadU64(base, frame + 88);
  Vec v2 = AddI(v13, v9);
  Vec v3 = AddI(v0, v9);
  Stvx(base, frame + 96, v0);
  do {
    // Eight frames' samples and the ones after them, a halfword each.
    for (int k = 0; k < 8; k++) {
      const uint64_t r9 = uint64_t(uint32_t(r3 >> 32) << 1) + r11;
      r3 = r8 + r3;
      v0 = Sld<14>(v1, Lvlx(base, uint32_t(r9)));
      v13 = Sld<14>(v31, Lvlx(base, uint32_t(r9 + 2)));
      v1 = v0;
      v31 = v13;
    }
    v9 = High16(v0);
    v12 = Srw(v2, v4);
    v7 = High16(v13);
    Vec v5 = Srw(v3, v4);
    v8 = Low16(v0);
    const uint64_t whole = r3 >> 32;
    v6 = Low16(v13);
    const uint64_t r9 = r5 + 16;
    mode.Vector();
    v0 = SignedToFloat(v9, kSample);
    v9 = SignedToFloat(v7, kSample);
    r3 = r3 & 0xFFFFFFFF;
    v12 = UnsignedToFloat(v12, kFraction);
    r11 = uint64_t(uint32_t(whole) << 1) + r11;
    v13 = SignedToFloat(v8, kSample);
    v8 = SignedToFloat(v6, kSample);
    v7 = UnsignedToFloat(v5, kFraction);
    v6 = Mul(v0, v10);
    v0 = Sub(v9, v0);
    v12 = Mul(v12, v10);
    v5 = Mul(v13, v11);
    v13 = Sub(v8, v13);
    v9 = Mul(v7, v11);
    v0 = Madd(v0, v12, v6);
    Stvx(base, uint32_t(r5), v0);
    v0 = Madd(v13, v9, v5);
    Stvx(base, uint32_t(r9), v0);
    r6 = r6 - 8;
    v11 = Add(v11, v30);
    v10 = Add(v10, v30);
    r5 = r5 + 32;
    v3 = AddI(v3, v29);
    v2 = AddI(v2, v29);
  } while (int32_t(r6) > 0);
  StorePositions(base, state, r11, r5);
  const double f1 = c_runtime::U64ToDouble(r3);
  mode.Scalar();
  f0 = f1 * LoadDouble(base, 0x820022B0);
  f0 = double(float(f0));
  guest::StoreFloat(base, state + 48, float(f0));
  return std::bit_cast<uint64_t>(f1);
}

// sub_8246E4C8: the vector path of sub_8247E260, as sub_8246E208 over two
// channels (the input's frames 4 bytes, the second channel's output 1,024
// bytes after the first's).
uint64_t ResampleStereoVector(uint8_t* base, uint64_t r3, uint64_t r5, uint64_t r6,
                              uint32_t state, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8246E4C8 the stereo resampler's vector path");
  const uint32_t frame = sp - 176;
  Mode mode;
  mode.Scalar();
  uint64_t r10 = r3;
  const double f11 = LoadSingle(base, state + 44);
  double f10 = LoadSingle(base, state + 48);
  double f0 = LoadSingle(base, state + 36);
  const uint64_t r11 = Ext(uint32_t(r6));
  double f13 = LoadSingle(base, state + 40);
  CopyFloat(base, frame + 80, state + 36);
  f0 = double(float(f13 - f0));
  CopyFloat(base, state + 36, state + 40);
  Vec v0 = Zero();
  const Vec v2 = simde_mm_set1_epi32(1);
  StoreU64(base, frame + 88, r11);
  f13 = double(int64_t(LoadU64(base, frame + 88)));
  Vec v13 = Lvlx(base, frame + 80);
  Vec v11 = Splat0(v13);
  Vec v28 = v0, v30 = v0;
  double f12 = LoadDouble(base, 0x8204D240);
  f10 = f10 * f12;
  f12 = f11 * f12;
  Vec v27 = v0, v29 = v0;
  f13 = double(float(f13));
  Vec v10 = Lvx(base, 0x8204D250);
  const int64_t fraction = ToInt64(f10);
  const int64_t step = ToInt64(f12);
  Vec v9 = Lvx(base, 0x8204D260);
  f0 = double(float(f0 / f13));
  f13 = LoadSingle(base, 0x82000C34);
  f13 = double(float(f0 * f13));
  guest::StoreFloat(base, frame + 80, float(f13));
  Vec v12 = Lvlx(base, frame + 80);
  guest::StoreFloat(base, frame + 80, float(f0));
  const Vec v26 = Splat0(v12);
  Vec v8 = Lvlx(base, frame + 80);
  StoreU64(base, frame + 88, uint64_t(fraction));
  v12 = Splat0(v8);
  StoreU64(base, frame + 80, uint64_t(step));
  Vec v7 = Lvlx(base, frame + 84);
  Vec v6 = Lvlx(base, frame + 92);
  v13 = Splat0(v7);
  Stvx(base, frame + 96, v0);
  mode.Vector();
  v10 = Mul(v12, v10);
  v9 = Mul(v12, v9);
  v8 = Splat0(v6);
  v12 = AddI(v13, v13);
  v7 = AddI(v13, v12);
  v10 = Add(v11, v10);
  v9 = Add(v11, v9);
  v11 = Sld<12>(v0, v13);
  v13 = Sld<12>(v11, v12);
  Stvx(base, frame + 96, v0);
  v0 = AddI(v12, v12);
  const Vec v25 = AddI(v0, v0);
  Stvx(base, frame + 96, v11);
  Stvx(base, frame + 96, v13);
  v13 = Sld<12>(v13, v7);
  Stvx(base, frame + 96, v13);
  v0 = AddI(v13, v0);
  const uint64_t r8 = LoadU64(base, frame + 80);
  Vec v31 = AddI(v13, v8);
  r3 = LoadU64(base, frame + 88);
  Vec v1 = AddI(v0, v8);
  Stvx(base, frame + 96, v0);
  do {
    for (int k = 0; k < 8; k++) {
      const uint64_t at = uint64_t(uint32_t(r3 >> 32) << 2) + r10;
      r3 = r8 + r3;
      v12 = Sld<14>(v28, Lvlx(base, uint32_t(at)));
      v0 = Lvlx(base, uint32_t(at + 2));
      v13 = Lvlx(base, uint32_t(at + 6));
      v0 = Sld<14>(v30, v0);
      v11 = Lvlx(base, uint32_t(at + 4));
      v13 = Sld<14>(v29, v13);
      v11 = Sld<14>(v27, v11);
      v28 = v12;
      v30 = v0;
      v29 = v13;
      v27 = v11;
    }
    v7 = High16(v0);
    Vec v3 = Srw(v1, v2);
    v6 = High16(v13);
    v8 = Srw(v31, v2);
    Vec v5 = Low16(v0);
    const uint64_t whole = r3 >> 32;
    Vec v4 = Low16(v13);
    mode.Vector();
    v0 = SignedToFloat(v7, kSample);
    v7 = SignedToFloat(v6, kSample);
    r3 = r3 & 0xFFFFFFFF;
    v13 = SignedToFloat(v5, kSample);
    r10 = uint64_t(uint32_t(whole) << 2) + r10;
    v6 = SignedToFloat(v4, kSample);
    v5 = UnsignedToFloat(v3, kFraction);
    v8 = UnsignedToFloat(v8, kFraction);
    v4 = Mul(v0, v9);
    v7 = Sub(v7, v0);
    v3 = Mul(v13, v10);
    v13 = Sub(v6, v13);
    v0 = Mul(v5, v10);
    v8 = Mul(v8, v9);
    v13 = Madd(v13, v0, v3);
    v7 = Madd(v7, v8, v4);
    Stvx(base, uint32_t(r5 + 1040), v13);
    v13 = High16(v12);
    Stvx(base, uint32_t(r5 + 1024), v7);
    v7 = High16(v11);
    v12 = Low16(v12);
    v11 = Low16(v11);
    v13 = SignedToFloat(v13, kSample);
    v7 = SignedToFloat(v7, kSample);
    v12 = SignedToFloat(v12, kSample);
    v11 = SignedToFloat(v11, kSample);
    v6 = Mul(v13, v9);
    v13 = Sub(v7, v13);
    v5 = Mul(v12, v10);
    v12 = Sub(v11, v12);
    v13 = Madd(v13, v8, v6);
    v0 = Madd(v12, v0, v5);
    Stvx(base, uint32_t(r5), v13);
    Stvx(base, uint32_t(r5 + 16), v0);
    r6 = r6 - 8;
    v10 = Add(v10, v26);
    v9 = Add(v9, v26);
    r5 = r5 + 32;
    v1 = AddI(v1, v25);
    v31 = AddI(v31, v25);
  } while (int32_t(r6) > 0);
  StorePositions(base, state, r10, r5);
  const double f1 = c_runtime::U64ToDouble(r3);
  mode.Scalar();
  f0 = f1 * LoadDouble(base, 0x820022B0);
  f0 = double(float(f0));
  guest::StoreFloat(base, state + 48, float(f0));
  return std::bit_cast<uint64_t>(f1);
}

// sub_8247E188 and sub_8247E260 (the same steps over their two paths): the
// vector path when the output (+20 + 4 x +28) is 16-byte aligned, the
// frames left (+24 - +28) a multiple of 8, the last input frame's index
// (+4 - +8 - 1) past the position's end (+44 x the frames + +48) and the
// fraction not below 0.0 (the float at 0x82000A34); else the scalar path.
template <bool stereo>
uint64_t Resample(uint8_t* base, uint32_t state, uint32_t sp) {
  const uint32_t frame = sp - 112;
  Mode mode;
  mode.Scalar();
  const uint64_t written = guest::LoadU32(base, state + 28);
  const double f13 = LoadSingle(base, state + 48);
  const uint64_t frames = guest::LoadU32(base, state + 24);
  const uint64_t position = guest::LoadU32(base, state + 8);
  uint64_t r5 = uint64_t(uint32_t(written) << 2);
  const uint32_t channels = guest::LoadU8(base, state + 13);
  const uint64_t r6 = frames - written;
  const uint64_t end = guest::LoadU32(base, state + 4);
  const int64_t offset = int64_t(int32_t(channels)) * int64_t(int32_t(uint32_t(position)));
  const uint64_t input = guest::LoadU32(base, state + 0);
  const uint64_t output = guest::LoadU32(base, state + 20);
  const uint64_t r4 = end - position;
  const uint64_t r3 = uint64_t(uint32_t(uint64_t(offset)) << 1) + input;
  r5 = r5 + output;
  bool vector = (((uint32_t(r6) & 7) | uint32_t(r5)) & 0xF) == 0;
  if (vector) {
    const uint64_t left = Ext(uint32_t(r6));
    const double ratio = LoadSingle(base, state + 44);
    const uint64_t last = Ext(uint32_t(r4 - 1));
    StoreU64(base, frame + 80, left);
    StoreU64(base, frame + 88, last);
    double f12 = double(int64_t(LoadU64(base, frame + 80)));
    double f11 = double(int64_t(LoadU64(base, frame + 88)));
    f12 = double(float(f12));
    f11 = double(float(f11));
    const double reach = double(float(std::fma(ratio, f12, f13)));
    vector = f11 > reach && !(f13 < LoadSingle(base, 0x82000A34));
  }
  if (vector) {
    return stereo ? ResampleStereoVector(base, r3, r5, r6, state, frame)
                  : ResampleMonoVector(base, r3, r5, r6, state, frame);
  }
  return stereo ? ResampleStereoScalar(base, state, frame) : ResampleMonoScalar(base, state, frame);
}

}  // namespace

uint64_t ResampleMono(uint8_t* base, uint32_t state, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8247E188 the mono resampler");
  return Resample<false>(base, state, sp);
}

uint64_t ResampleStereo(uint8_t* base, uint32_t state, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8247E260 the stereo resampler");
  return Resample<true>(base, state, sp);
}

// sub_82473E48 (entry 6) and sub_82474188 (entry 7): a frame at a time,
// linear between the input frame at the position and the next (the last
// input frame kept at +52, and +56 for the second channel, for a position
// before the first), the volume ramp added a frame at a time; their words
// below `sp` as the original leaves them.
uint64_t ResampleMonoScalar(uint8_t* base, uint32_t state, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82473E48 the mono resampler's scalar path");
  Mode mode;
  mode.Scalar();
  const uint32_t input = guest::LoadU32(base, state + 0);
  const double f13_ratio = LoadSingle(base, state + 44);
  const uint32_t written = guest::LoadU32(base, state + 28);
  uint64_t r7 = uint64_t(uint32_t(written << 2)) + guest::LoadU32(base, state + 20);
  const uint32_t position = guest::LoadU32(base, state + 8);
  const uint32_t channels = guest::LoadU8(base, state + 13);
  const uint64_t frames = guest::LoadU32(base, state + 24);
  const int64_t offset = int64_t(int32_t(channels)) * int64_t(int32_t(position));
  const uint64_t end = guest::LoadU32(base, state + 4);
  const double f12_fraction = LoadSingle(base, state + 48);
  uint64_t r8 = frames - written;
  const uint64_t r25 = end - position;
  const uint64_t r9 = uint64_t(uint32_t(uint64_t(offset)) << 1) + input;
  guest::StoreU32(base, sp - 144, uint32_t(r8));
  Vec v13 = Lvlx(base, sp - 144);
  mode.Vector();
  v13 = ToFloat(v13);
  Vec v12 = Lvlx(base, state + 40);
  StoreU64(base, sp - 136, 1ull << 32);
  mode.Scalar();
  double f0 = double(int64_t(LoadU64(base, sp - 136)));
  Vec v0 = Lvlx(base, state + 36);
  mode.Vector();
  v12 = Sub(v12, v0);
  Vec v9 = Reciprocal(v13);
  mode.Scalar();
  const double f12 = f12_fraction * f0;
  const double f11 = f13_ratio * f0;
  f0 = LoadDouble(base, 0x82002250) / f0;
  StoreU64(base, sp - 144, uint64_t(ToInt64(f12)));
  mode.Vector();
  v12 = Mul(v12, v9);
  mode.Scalar();
  StoreU64(base, sp - 136, uint64_t(ToInt64(f11)));
  const uint64_t r4 = LoadU64(base, sp - 136);
  Vec v11 = Lvlx(base, sp - 140);
  Vec v10 = Lvlx(base, sp - 132);
  v13 = Splat0(v11);
  Stvx(base, sp - 128, v0);
  uint64_t r11 = LoadU64(base, sp - 144);
  v0 = Splat0(v0);
  v10 = Splat0(v10);
  v11 = Splat0(v12);
  const Vec ones = simde_mm_set1_epi8(char(1));
  // Before the first input frame: between the kept one (+52) and it.
  if (int64_t(r11) < 0) {
    do {
      if (int32_t(r8) <= 0) {
        break;
      }
      Vec v6 = v13;
      Vec v5 = v0, v4 = v0, v3 = v0;
      Vec v8 = Lvlx(base, uint32_t(r9));
      Vec v7 = Lvlx(base, state + 52);
      v6 = Vsr(v6, ones);
      v12 = Splat0(v7);
      mode.Vector();
      v5 = Sub(Zero(), v5);
      v9 = High16(v8);
      r11 = r4 + r11;
      v0 = Add(v0, v11);
      v13 = AddI(v13, v10);
      r8 = r8 - 1;
      v8 = UnsignedToFloat(v6, kFraction);
      v7 = SignedToFloat(v9, kSample);
      v9 = Sld<12>(v4, v5);
      v9 = Mul(v8, v9);
      v12 = Blend3(v12, v7);
      v9 = Sld<8>(v9, v3);
      v12 = Dot(v12, v9);
      Stvewx(base, uint32_t(r7), v12);
      r7 = r7 + 4;
    } while (int64_t(r11) < 0);
  }
  const uint64_t r24 = r25 - 1;
  const int64_t r30 = int64_t(int32_t(uint32_t(r24)));
  uint64_t r10 = uint64_t((int64_t(r11) >> 32) - (int64_t(r11) >> 63));
  bool ended = false;  // out of output frames inside the loop
  if (int64_t(r10) < r30) {
    do {
      if (int32_t(r8) == 0) {
        ended = true;
        break;
      }
      const Vec v6 = v13;
      const Vec v5 = v0, v4 = v0, v3 = v0;
      r10 = Ext(uint32_t(r10));
      r11 = r4 + r11;
      const uint64_t r6 = uint32_t(uint32_t(r10) << 1);
      v12 = Vsr(v6, ones);
      mode.Vector();
      v9 = Sub(Zero(), v5);
      r10 = r10 + 1;
      v0 = Add(v0, v11);
      v13 = AddI(v13, v10);
      r10 = uint32_t(uint32_t(r10) << 1);
      v12 = UnsignedToFloat(v12, kFraction);
      r8 = r8 - 1;
      Vec v8 = High16(Lvlx(base, uint32_t(r6) + uint32_t(r9)));
      Vec v7 = High16(Lvlx(base, uint32_t(r10) + uint32_t(r9)));
      r10 = uint64_t(int64_t(r11) >> 32);
      v8 = SignedToFloat(v8, kSample);
      v9 = Sld<12>(v4, v9);
      v7 = SignedToFloat(v7, kSample);
      v9 = Mul(v12, v9);
      v12 = Splat0(v8);
      v12 = Blend3(v12, v7);
      v9 = Sld<8>(v9, v3);
      v12 = Dot(v12, v9);
      Stvewx(base, uint32_t(r7), v12);
      r7 = r7 + 4;
    } while (int64_t(r10) < r30);
  }
  if (!ended && int32_t(r8) != 0) {
    if (int64_t(r10) == r30) {
      // The last input frame kept for the next pass.
      const int64_t sample = int16_t(guest::LoadU16(base, uint32_t(uint32_t(r24) << 1) + uint32_t(r9)));
      r10 = r10 + 1;
      StoreU64(base, sp - 136, uint64_t(sample));
      mode.Scalar();
      const double f13 = double(int64_t(LoadU64(base, sp - 136)));
      const double f12s = double(float(f13));
      const double scaled = double(float(f12s * LoadSingle(base, 0x8204D160)));
      guest::StoreFloat(base, state + 52, float(scaled));
    } else {
      const int64_t r8s = int64_t(int32_t(uint32_t(r25)));
      if (int64_t(r10) > r8s) {
        r11 = ((r10 - uint64_t(r8s)) << 32) + r11;
      }
    }
  }
  const uint64_t r6 = r10 << 32;
  Stvewx(base, state + 36, v0);
  const uint64_t at = uint64_t(uint32_t(uint32_t(r10) << 1)) + r9;
  r11 = r11 - r6;
  StoreU64(base, sp - 136, r11);
  mode.Scalar();
  const double f13 = double(int64_t(LoadU64(base, sp - 136)));
  guest::StoreFloat(base, state + 48, float(double(float(f13 * f0))));
  StorePositions(base, state, at, r7);
  return state;
}

uint64_t ResampleStereoScalar(uint8_t* base, uint32_t state, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82474188 the stereo resampler's scalar path");
  Mode mode;
  mode.Scalar();
  const uint32_t input = guest::LoadU32(base, state + 0);
  const double f13_ratio = LoadSingle(base, state + 44);
  const uint32_t written = guest::LoadU32(base, state + 28);
  const uint32_t output = guest::LoadU32(base, state + 20);
  uint64_t r8 = uint64_t(uint32_t(written << 2)) + output;
  const uint32_t position = guest::LoadU32(base, state + 8);
  const uint32_t channels = guest::LoadU8(base, state + 13);
  const uint32_t kept = state + 52;
  const uint64_t frames = guest::LoadU32(base, state + 24);
  const int64_t offset = int64_t(int32_t(channels)) * int64_t(int32_t(position));
  const uint64_t end = guest::LoadU32(base, state + 4);
  const double f12_fraction = LoadSingle(base, state + 48);
  uint64_t r7 = frames - written;
  const uint64_t r25 = end - position;
  const uint64_t r9 = uint64_t(uint32_t(uint64_t(offset)) << 1) + input;
  guest::StoreU32(base, sp - 160, uint32_t(r7));
  Vec v13 = Lvlx(base, sp - 160);
  mode.Vector();
  v13 = ToFloat(v13);
  Vec v12 = Lvlx(base, state + 40);
  StoreU64(base, sp - 152, 1ull << 32);
  mode.Scalar();
  double f0 = double(int64_t(LoadU64(base, sp - 152)));
  Vec v0 = Lvlx(base, state + 36);
  mode.Vector();
  v12 = Sub(v12, v0);
  Vec v9 = Reciprocal(v13);
  mode.Scalar();
  const double f12 = f12_fraction * f0;
  const double f11 = f13_ratio * f0;
  f0 = LoadDouble(base, 0x82002250) / f0;
  StoreU64(base, sp - 160, uint64_t(ToInt64(f12)));
  mode.Vector();
  v12 = Mul(v12, v9);
  mode.Scalar();
  StoreU64(base, sp - 152, uint64_t(ToInt64(f11)));
  const uint64_t r4 = LoadU64(base, sp - 152);
  Vec v11 = Lvlx(base, sp - 156);
  Vec v10 = Lvlx(base, sp - 148);
  v13 = Splat0(v11);
  Stvx(base, sp - 144, v0);
  uint64_t r11 = LoadU64(base, sp - 160);
  v0 = Splat0(v0);
  const Vec v8 = Splat0(v10);
  v9 = Splat0(v12);
  const Vec ones = simde_mm_set1_epi8(char(1));
  if (int64_t(r11) < 0) {
    do {
      if (int32_t(r7) <= 0) {
        break;
      }
      v10 = Zero();
      const uint32_t second = uint32_t(r8) + 1024;
      r11 = r4 + r11;
      r7 = r7 - 1;
      v12 = Lvlx(base, uint32_t(r9 + 2));
      mode.Vector();
      const Vec v5 = Sub(v10, v0);
      Vec v7 = Lvlx(base, kept + 4);
      Vec v6 = High16(v12);
      v12 = Splat0(v7);
      v7 = Vsr(v13, ones);
      v11 = Vsr(v13, ones);
      v10 = Sub(v10, v0);
      v13 = AddI(v13, v8);
      v6 = SignedToFloat(v6, kSample);
      v7 = UnsignedToFloat(v7, kFraction);
      const Vec v4 = UnsignedToFloat(v11, kFraction);
      v11 = Sld<12>(v0, v5);
      v10 = Sld<12>(v0, v10);
      v12 = Blend3(v12, v6);
      v11 = Mul(v7, v11);
      v6 = v10;
      v7 = Mul(v4, v6);
      v6 = v0;
      v11 = Sld<8>(v11, v0);
      v0 = Add(v0, v9);
      v12 = Dot(v12, v11);
      Stvewx(base, second, v12);
      v12 = High16(Lvlx(base, uint32_t(r9)));
      v11 = Lvlx(base, kept);
      v10 = SignedToFloat(v12, kSample);
      v12 = Splat0(v11);
      v11 = Sld<8>(v7, v6);
      v12 = Blend3(v12, v10);
      v12 = Dot(v12, v11);
      Stvewx(base, uint32_t(r8), v12);
      r8 = r8 + 4;
    } while (int64_t(r11) < 0);
  }
  const uint64_t r24 = r25 - 1;
  const int64_t r30 = int64_t(int32_t(uint32_t(r24)));
  uint64_t r10 = uint64_t((int64_t(r11) >> 32) - (int64_t(r11) >> 63));
  bool ended = false;
  if (int64_t(r10) < r30) {
    do {
      if (int32_t(r7) == 0) {
        ended = true;
        break;
      }
      Vec v11z = Zero();
      r10 = Ext(uint32_t(r10));
      const uint32_t second = uint32_t(r8) + 1024;
      const uint64_t next = r10 + 1;
      Vec v7 = Vsr(v13, ones);
      const uint64_t at = uint64_t(uint32_t(uint32_t(r10) << 2)) + r9;
      Vec v6 = Vsr(v13, ones);
      mode.Vector();
      Vec v5 = Sub(v11z, v0);
      const uint64_t at_next = uint64_t(uint32_t(uint32_t(next) << 2)) + r9;
      v13 = AddI(v13, v8);
      v12 = v7;
      r11 = r4 + r11;
      v6 = UnsignedToFloat(v6, kFraction);
      r7 = r7 - 1;
      v10 = High16(Lvlx(base, uint32_t(at + 2)));
      Vec v4 = UnsignedToFloat(v12, kFraction);
      v12 = High16(Lvlx(base, uint32_t(at_next + 2)));
      v10 = SignedToFloat(v10, kSample);
      v7 = Sld<12>(v0, v5);
      v5 = SignedToFloat(v12, kSample);
      v7 = Mul(v4, v7);
      v4 = v0;
      v12 = Splat0(v10);
      v12 = Blend3(v12, v5);
      v5 = v0;
      v10 = Sld<8>(v7, v0);
      v7 = v0;
      v0 = Add(v0, v9);
      v12 = Dot(v12, v10);
      v7 = Sub(v11z, v7);
      Stvewx(base, second, v12);
      v10 = Lvlx(base, uint32_t(at_next));
      v12 = Lvlx(base, uint32_t(at));
      v11z = High16(v10);
      v12 = High16(v12);
      r10 = uint64_t(int64_t(r11) >> 32);
      v10 = SignedToFloat(v11z, kSample);
      v11 = Sld<12>(v5, v7);
      v12 = SignedToFloat(v12, kSample);
      v11 = Mul(v6, v11);
      v12 = Splat0(v12);
      v11 = Sld<8>(v11, v4);
      v12 = Blend3(v12, v10);
      v12 = Dot(v12, v11);
      Stvewx(base, uint32_t(r8), v12);
      r8 = r8 + 4;
    } while (int64_t(r10) < r30);
  }
  if (!ended && int32_t(r7) != 0) {
    if (int64_t(r10) == r30) {
      // The last input frame kept for the next pass (its second channel
      // first).
      const uint32_t last = uint32_t(uint64_t(uint32_t(uint32_t(r24) << 2)) + r9);
      r10 = r10 + 1;
      int64_t sample = int16_t(guest::LoadU16(base, last + 2));
      StoreU64(base, sp - 152, uint64_t(sample));
      mode.Scalar();
      double f13 = double(int64_t(LoadU64(base, sp - 152)));
      double f12s = double(float(f13));
      f13 = LoadSingle(base, 0x8204D160);
      f12s = double(float(f12s * f13));
      guest::StoreFloat(base, kept + 4, float(f12s));
      sample = int16_t(guest::LoadU16(base, last));
      StoreU64(base, sp - 152, uint64_t(sample));
      f12s = double(int64_t(LoadU64(base, sp - 152)));
      f12s = double(float(f12s));
      f13 = double(float(f12s * f13));
      guest::StoreFloat(base, kept, float(f13));
    } else {
      const int64_t r25s = int64_t(int32_t(uint32_t(r25)));
      if (int64_t(r10) > r25s) {
        r11 = ((r10 - uint64_t(r25s)) << 32) + r11;
      }
    }
  }
  const uint64_t r6 = r10 << 32;
  Stvewx(base, state + 36, v0);
  const uint64_t at = uint64_t(uint32_t(uint32_t(r10) << 2)) + r9;
  r11 = r11 - r6;
  StoreU64(base, sp - 152, r11);
  mode.Scalar();
  const double f13 = double(int64_t(LoadU64(base, sp - 152)));
  guest::StoreFloat(base, state + 48, float(double(float(f13 * f0))));
  StorePositions(base, state, at, r8);
  return state;
}

// --- Equal rates (entries 107 and 32) ---

uint64_t CopyStereoScalar(uint8_t* base, uint32_t state, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82478E60 the stereo copy's scalar path");
  Mode mode;
  const uint32_t position = guest::LoadU32(base, state + 8);
  mode.Scalar();
  double f0 = LoadSingle(base, state + 36);
  const uint32_t channels = guest::LoadU8(base, state + 13);
  const uint32_t written = guest::LoadU32(base, state + 28);
  uint64_t r9 = uint64_t(guest::LoadU32(base, state + 4)) - position;
  const uint64_t r6 = uint64_t(guest::LoadU32(base, state + 24)) - written;
  const int64_t offset = int64_t(int32_t(channels)) * int64_t(int32_t(position));
  uint64_t r11 = uint64_t(uint32_t(uint64_t(offset)) << 1) + guest::LoadU32(base, state + 0);
  uint64_t r8 = uint64_t(uint32_t(written << 2)) + guest::LoadU32(base, state + 20);
  if (!(int32_t(r9) < int32_t(r6))) {
    r9 = r6;
  }
  const uint64_t left = Ext(uint32_t(r6));
  double f13 = LoadSingle(base, state + 40);
  f13 = double(float(f13 - f0));
  StoreU64(base, sp - 16, left);
  double f12 = double(int64_t(LoadU64(base, sp - 16)));
  f12 = double(float(f12));
  f12 = double(float(f13 / f12));
  f13 = LoadSingle(base, 0x8204D160);
  do {
    int64_t sample = int16_t(guest::LoadU16(base, uint32_t(r11) + 2));
    r9 = r9 - 1;
    StoreU64(base, sp - 16, uint64_t(sample));
    double f11 = double(int64_t(LoadU64(base, sp - 16)));
    f11 = double(float(f11));
    f11 = double(float(f11 * f13));
    f11 = double(float(f11 * f0));
    guest::StoreFloat(base, uint32_t(r8) + 1024, float(f11));
    sample = int16_t(guest::LoadU16(base, uint32_t(r11)));
    r11 = r11 + 4;
    StoreU64(base, sp - 8, uint64_t(sample));
    f11 = double(int64_t(LoadU64(base, sp - 8)));
    f11 = double(float(f11));
    f11 = double(float(f11 * f13));
    f11 = double(float(f11 * f0));
    guest::StoreFloat(base, uint32_t(r8), float(f11));
    r8 = r8 + 4;
    f0 = double(float(f12 + f0));
  } while (int32_t(r9) != 0);
  StorePositions(base, state, r11, r8);
  guest::StoreFloat(base, state + 36, float(f0));
  return state;
}

uint64_t CopyStereo(uint8_t* base, uint32_t state, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8247F558 the stereo copy");
  const uint32_t frame = sp - 160;
  const uint32_t position = guest::LoadU32(base, state + 8);
  const uint64_t end = guest::LoadU32(base, state + 4);
  const uint32_t channels = guest::LoadU8(base, state + 13);
  const uint32_t written = guest::LoadU32(base, state + 28);
  uint64_t r9 = end - position;
  const uint64_t frames = guest::LoadU32(base, state + 24);
  const int64_t offset = int64_t(int32_t(channels)) * int64_t(int32_t(position));
  const uint64_t input = guest::LoadU32(base, state + 0);
  const uint64_t output = guest::LoadU32(base, state + 20);
  uint64_t r7 = frames - written;
  uint64_t r11 = uint64_t(uint32_t(uint64_t(offset)) << 1) + input;
  uint64_t r8 = uint64_t(uint32_t(written << 2)) + output;
  if (!(int32_t(r9) < int32_t(r7))) {
    r9 = r7;
  }
  if ((((uint32_t(r9) & 7) | uint32_t(r8) | uint32_t(r11)) & 0xF) != 0) {
    return CopyStereoScalar(base, state, frame);
  }
  Mode mode;
  const uint64_t left = Ext(uint32_t(r7));
  Vec v0 = Zero();
  r7 = Ext(uint32_t(r9) << 1);
  mode.Scalar();
  double f0 = LoadSingle(base, state + 36);
  double f13 = LoadSingle(base, state + 40);
  f13 = double(float(f13 - f0));
  StoreU64(base, frame + 80, left);
  double f12 = double(int64_t(LoadU64(base, frame + 80)));
  CopyFloat(base, frame + 80, state + 36);
  StoreU64(base, frame + 88, r7);
  Stvx(base, frame + 96, v0);
  f12 = double(float(f12));
  f13 = double(float(f13 / f12));
  double f11 = double(int64_t(LoadU64(base, frame + 88)));
  Stvx(base, frame + 96, v0);
  f11 = double(float(f11));
  f0 = double(float(std::fma(f11, f13, f0)));
  guest::StoreFloat(base, state + 36, float(f0));
  f0 = LoadSingle(base, 0x820021F8);
  v0 = Lvlx(base, frame + 80);
  f0 = double(float(f13 * f0));
  guest::StoreFloat(base, frame + 80, float(f0));
  Vec v13 = Lvlx(base, frame + 80);
  v0 = Splat0(v0);
  guest::StoreFloat(base, frame + 80, float(f13));
  const Vec v8 = Splat0(v13);
  Vec v12 = Lvlx(base, frame + 80);
  v13 = Splat0(v12);
  Vec v11 = Lvx(base, 0x8204D250);
  mode.Vector();
  const Vec v9 = Add(v8, v8);
  v12 = Mul(v13, v11);
  Vec v10 = Lvx(base, 0x8204D260);
  v13 = Mul(v13, v10);
  v10 = Add(v0, v12);
  v0 = Add(v0, v13);
  if (int32_t(r9) > 0) {
    uint32_t passes = ((uint32_t(r9) - 1) >> 3) + 1;
    const Vec order = Lvx(base, 0x8204D1D0);
    do {
      v13 = Lvx(base, uint32_t(r11));
      v13 = Perm(v13, v13, order);
      mode.Vector();
      v11 = Add(v0, v8);
      const Vec v4 = v0, v3 = v0;
      v10 = Add(v10, v9);
      v12 = Lvx(base, uint32_t(r11 + 16));
      Vec v6 = High16(v13);
      v12 = Perm(v12, v12, order);
      passes--;
      v13 = Low16(v13);
      r11 = r11 + 32;
      v0 = Add(v0, v9);
      v6 = SignedToFloat(v6, kSample);
      Vec v5 = High16(v12);
      v12 = Low16(v12);
      v13 = SignedToFloat(v13, kSample);
      v5 = SignedToFloat(v5, kSample);
      v12 = SignedToFloat(v12, kSample);
      v6 = Mul(v6, v4);
      v13 = Mul(v13, v3);
      v5 = Mul(v5, v11);
      v12 = Mul(v12, v11);
      Stvx(base, uint32_t(r8), v6);
      const uint64_t at = r8;
      r8 = r8 + 32;
      Stvx(base, uint32_t(at + 1024), v13);
      Stvx(base, uint32_t(at + 16), v5);
      Stvx(base, uint32_t(at + 1040), v12);
    } while (passes != 0);
  }
  // The position and the frames written from the words loaded first.
  const uint32_t used = Frames(uint32_t(r11 - input), channels);
  guest::StoreU32(base, state + 8, used < uint32_t(end) ? used : uint32_t(end));
  const uint32_t made = uint32_t(r8 - output) >> 2;
  guest::StoreU32(base, state + 28, made < uint32_t(frames) ? made : uint32_t(frames));
  return state;
}

uint64_t ResamplerProcess(uint8_t* base, uint32_t state, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8246F140 Resampler::Process");
  const uint32_t end = guest::LoadU32(base, state + 4);
  const uint32_t start = guest::LoadU32(base, state + 8);
  if (!(start < end)) {
    return 0;
  }
  if (!(guest::LoadU32(base, state + 28) < guest::LoadU32(base, state + 24))) {
    return 0;
  }
  const uint32_t changed = guest::LoadU32(base, state + 80);
  if (changed != 0) {
    if ((changed & 3) != 0) {
      Mode mode;
      const uint32_t out_rate = guest::LoadU32(base, state + 32);
      const uint32_t in_rate = guest::LoadU32(base, state + 16);
      const uint32_t flags = guest::LoadU32(base, state + 84);
      StoreU64(base, frame + 80, in_rate);
      StoreU64(base, frame + 88, out_rate);
      mode.Scalar();
      double f0 = double(int64_t(LoadU64(base, frame + 80)));
      double f13 = double(int64_t(LoadU64(base, frame + 88)));
      f0 = double(float(f0));
      f13 = double(float(f13));
      f0 = double(float(f0 / f13));
      guest::StoreFloat(base, state + 44, float(f0));
      // The case: 3 any ratio, 4 equal rates, 5 two to one (with flags bit
      // 0); 3 less with bit 7 (the scalar set).
      const uint64_t scalar = (flags & 0x80) != 0 ? 0xFFFFFFFFFFFFFFFDull : 0;
      uint64_t kind = scalar + 3;
      if ((flags & 1) != 0) {
        if (in_rate == out_rate) {
          kind = scalar + 4;
        } else if (uint32_t(in_rate << 1) == out_rate) {
          kind = scalar + 5;
        }
      }
      const uint64_t channels = guest::LoadU8(base, state + 13);
      const uint64_t format = guest::LoadU8(base, state + 12);
      uint64_t code = 0;
      switch (uint32_t(channels - 1)) {
        case 0:
        case 1:
        case 3:
        case 5:
          code = (uint32_t(channels) >> 1) + 1;
          break;
        default:
          break;
      }
      uint64_t index = kind * 5 + format;
      index = index * 5 + code;
      const uint32_t entry = uint32_t(index << 2) + kResamplerKernels;
      guest::StoreU32(base, state + 76, guest::LoadU32(base, entry));
    }
    guest::StoreU32(base, state + 80, 0);
  }
  const uint32_t kernel = guest::LoadU32(base, state + 76);
  switch (kernel) {
    case 0x8247E188:
      ResampleMono(base, state, frame);
      break;
    case 0x8247E260:
      ResampleStereo(base, state, frame);
      break;
    case 0x8247F558:
      CopyStereo(base, state, frame);
      break;
    case 0x82473E48:
      ResampleMonoScalar(base, state, frame);
      break;
    case 0x82474188:
      ResampleStereoScalar(base, state, frame);
      break;
    case 0x82478E60:
      CopyStereoScalar(base, state, frame);
      break;
    default:
      Regs regs;
    regs.r[regs.count++] = state;
    regs.sp = frame;
    call(kernel, 0x8246F2DC, regs);
      break;
  }
  return uint64_t(guest::LoadU32(base, state + 8)) - uint64_t(start);
}

}  // namespace xaudio
