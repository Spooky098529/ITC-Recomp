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
#include "schema/xaudio_objects.h"
#include "xaudio/ppc_float.h"

#include "base/float_mode.h"
#include "lifted/vector_helpers.h"
#include <simde/x86/sse4.1.h>

#include <bit>
#include <climits>
#include <cmath>
#include <cstring>

namespace xaudio {

using schema::ConverterState;

namespace {

using namespace ppc_float;

// stvewx (the lane its address selects).
void Stvewx(uint32_t address, Vec value) {
  const uint32_t ea = address & ~3u;
  alignas(16) uint32_t lanes[4];
  simde_mm_store_si128(reinterpret_cast<Vec*>(lanes), value);
  guest::StoreU32(ea, lanes[3 - ((ea & 0xF) >> 2)]);
}

Vec Zero() { return simde_mm_set1_epi32(0); }
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
Vec Vsr(Vec a, Vec b) { return vector_helpers::simde_mm_vsr(a, b); }
Vec Perm(Vec a, Vec b, Vec c) { return vector_helpers::simde_mm_perm_epi8_(a, b, c); }
// vupkhsh and vupklsh.
Vec High16(Vec v) { return simde_mm_cvtepi16_epi32(simde_mm_unpackhi_epi64(v, v)); }
Vec Low16(Vec v) { return simde_mm_cvtepi16_epi32(v); }

Vec Reciprocal(Vec v) { return I(simde_mm_div_ps(simde_mm_set1_ps(1), F(v))); }
Vec ToFloat(Vec v) { return I(simde_mm_cvtepi32_ps(Pin(v))); }
// vcfsx and vcfux with a scale (the scale's float bits).
Vec SignedToFloat(Vec v, uint32_t scale) {
  return I(simde_mm_mul_ps(simde_mm_cvtepi32_ps(Pin(v)),
                           simde_mm_castsi128_ps(simde_mm_set1_epi32(int(scale)))));
}
Vec UnsignedToFloat(Vec v, uint32_t scale) {
  return I(simde_mm_mul_ps(vector_helpers::simde_mm_cvtepu32_ps_(Pin(v)),
                           simde_mm_castsi128_ps(simde_mm_set1_epi32(int(scale)))));
}
// The dot product the translated code makes of vmsum4fp: a's lane 3 replaced
// by b's (the blend), then the four lanes' sum.
Vec Blend3(Vec a, Vec b) { return I(simde_mm_blend_ps(F(a), simde_mm_permute_ps(F(b), 228), 8)); }
Vec Dot(Vec a, Vec b) { return I(simde_mm_dp_ps(F(a), F(b), 0xEF)); }

constexpr uint32_t kSample = 0x38000000;    // 2^-15: a 16-bit sample to a float
constexpr uint32_t kFraction = 0x30000000;  // 2^-31: half a 32-bit fraction to a float

double LoadSingle(uint32_t address) {
  return double(guest::LoadFloat(address));
}
double LoadDouble(uint32_t address) {
  return std::bit_cast<double>((uint64_t(guest::LoadU32(address)) << 32) |
                               guest::LoadU32(address + 4));
}
// A float word stored from the float register it was loaded into (lfs, then
// stfs): its bits, as the translated code leaves them (a signaling NaN kept).
void CopyFloat(uint32_t destination, uint32_t source) {
  guest::StoreU32(destination, guest::LoadU32(source));
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

// A frame's slots that only these kernels read and write (the integer and
// float moves through memory, the volume words loaded into vector lanes), as
// host bytes in the guest's order: `at` is the byte offset from the first
// slot, a 16-byte-aligned stack address in the original (r1 is kept 16-byte
// aligned), so lvlx's lanes come out the same. Zeroed: the lanes lvlx loads
// from bytes the original never wrote are the ones vspltw leaves out.
struct Slots {
  alignas(16) uint8_t bytes[16] = {};

  void StoreU32(uint32_t at, uint32_t value) {
    value = std::byteswap(value);
    std::memcpy(bytes + at, &value, 4);
  }
  uint32_t LoadU32(uint32_t at) const {
    uint32_t value;
    std::memcpy(&value, bytes + at, 4);
    return std::byteswap(value);
  }
  void StoreU64(uint32_t at, uint64_t value) {
    StoreU32(at, uint32_t(value >> 32));
    StoreU32(at + 4, uint32_t(value));
  }
  uint64_t LoadU64(uint32_t at) const { return (uint64_t(LoadU32(at)) << 32) | LoadU32(at + 4); }
  void StoreFloat(uint32_t at, float value) { StoreU32(at, std::bit_cast<uint32_t>(value)); }
  // lvlx at the slot `at` (within the first 16 bytes).
  Vec Lvlx(uint32_t at) const {
    return simde_mm_shuffle_epi8(
        simde_mm_load_si128(reinterpret_cast<const Vec*>(bytes)),
        simde_mm_load_si128(reinterpret_cast<const Vec*>(&vector_helpers::VectorMaskL[at * 16])));
  }
};

// The end of each kernel: the position (+8, at most the end), the frames
// written (+28, at most the output's) from the input and output pointers.
void StorePositions(uint32_t state, uint64_t input, uint64_t output) {
  const ConverterState s(state);
  const uint32_t channels = s.channels();
  const uint32_t frames = Frames(uint32_t(input - s.input()), channels);
  const uint32_t end = s.input_end();
  s.set_position(frames < end ? frames : end);
  const uint32_t written = uint32_t(output - s.output()) >> 2;
  const uint32_t limit = s.output_frames();
  s.set_frames_written(written < limit ? written : limit);
}

}  // namespace


// --- The kernels for any ratio (entries 81 and 82, 6 and 7) ---

namespace {

// sub_8246E208: the vector path of sub_8247E188, eight mono frames a pass
// (r3 the input at the position, r5 the output at the frames written, r6
// the frames left; the original's 176-byte frame's slots are host Slots). The position
// is 32.32 fixed point (+48 x 2^32, the ratio's step +44 x 2^32: the double
// at 0x8204D240); the volume ramps from +36 to +40 over the frames left
// (+36 = +40 at once). Returns __ultod's r3 (the fraction's double, whose
// float x 2^-32 goes to +48).
uint64_t ResampleMonoVector(uint64_t r3, uint64_t r5, uint64_t r6, uint32_t state) {
  const ConverterState s(state);
  ITC_NATIVE_RUNNING("sub_8246E208 the mono resampler's vector path");
  Slots slots;  // the original's frame (176 bytes) +80 to +95
  Mode mode;
  mode.Scalar();
  uint64_t r11 = r3;
  const double f11 = double(s.ratio());
  double f10 = double(s.fraction());
  double f0 = double(s.volume());
  const uint64_t r10 = Ext(uint32_t(r6));
  double f13 = double(s.target_volume());
  slots.StoreU32(0, guest::LoadU32(s.volume_address()));
  f0 = double(float(f13 - f0));
  CopyFloat(s.volume_address(), s.target_volume_address());
  Vec v0 = Zero();
  const Vec v4 = simde_mm_set1_epi32(1);
  slots.StoreU64(8, r10);
  f13 = double(int64_t(slots.LoadU64(8)));
  Vec v13 = slots.Lvlx(0);
  Vec v10 = Splat0(v13);
  Vec v1 = v0, v31 = v0;
  double f12 = LoadDouble(0x8204D240);
  f10 = f10 * f12;
  f12 = f11 * f12;
  f13 = double(float(f13));
  Vec v11 = Lvx(0x8204D250);
  const int64_t fraction = ToInt64(f10);
  const int64_t step = ToInt64(f12);
  Vec v8 = Lvx(0x8204D260);
  f0 = double(float(f0 / f13));
  f13 = LoadSingle(0x82000C34);
  f13 = double(float(f0 * f13));
  slots.StoreFloat(0, float(f13));
  Vec v12 = slots.Lvlx(0);
  slots.StoreFloat(0, float(f0));
  const Vec v30 = Splat0(v12);
  Vec v9 = slots.Lvlx(0);
  slots.StoreU64(8, uint64_t(fraction));
  v12 = Splat0(v9);
  slots.StoreU64(0, uint64_t(step));
  Vec v7 = slots.Lvlx(4);
  Vec v6 = slots.Lvlx(12);
  v13 = Splat0(v7);
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
  v0 = AddI(v12, v12);
  const Vec v29 = AddI(v0, v0);
  v13 = Sld<12>(v13, v7);
  v0 = AddI(v13, v0);
  const uint64_t r8 = slots.LoadU64(0);
  r3 = slots.LoadU64(8);
  Vec v2 = AddI(v13, v9);
  Vec v3 = AddI(v0, v9);
  do {
    // Eight frames' samples and the ones after them, a halfword each.
    for (int k = 0; k < 8; k++) {
      const uint64_t r9 = uint64_t(uint32_t(r3 >> 32) << 1) + r11;
      r3 = r8 + r3;
      v0 = Sld<14>(v1, Lvlx(uint32_t(r9)));
      v13 = Sld<14>(v31, Lvlx(uint32_t(r9 + 2)));
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
    Stvx(uint32_t(r5), v0);
    v0 = Madd(v13, v9, v5);
    Stvx(uint32_t(r9), v0);
    r6 = r6 - 8;
    v11 = Add(v11, v30);
    v10 = Add(v10, v30);
    r5 = r5 + 32;
    v3 = AddI(v3, v29);
    v2 = AddI(v2, v29);
  } while (int32_t(r6) > 0);
  StorePositions(state, r11, r5);
  const double f1 = c_runtime::U64ToDouble(r3);
  mode.Scalar();
  f0 = f1 * LoadDouble(0x820022B0);
  f0 = double(float(f0));
  s.set_fraction(float(f0));
  return std::bit_cast<uint64_t>(f1);
}

// sub_8246E4C8: the vector path of sub_8247E260, as sub_8246E208 over two
// channels (the input's frames 4 bytes, the second channel's output 1,024
// bytes after the first's).
uint64_t ResampleStereoVector(uint64_t r3, uint64_t r5, uint64_t r6,
                              uint32_t state) {
  const ConverterState s(state);
  ITC_NATIVE_RUNNING("sub_8246E4C8 the stereo resampler's vector path");
  Slots slots;  // the original's frame (176 bytes) +80 to +95
  Mode mode;
  mode.Scalar();
  uint64_t r10 = r3;
  const double f11 = double(s.ratio());
  double f10 = double(s.fraction());
  double f0 = double(s.volume());
  const uint64_t r11 = Ext(uint32_t(r6));
  double f13 = double(s.target_volume());
  slots.StoreU32(0, guest::LoadU32(s.volume_address()));
  f0 = double(float(f13 - f0));
  CopyFloat(s.volume_address(), s.target_volume_address());
  Vec v0 = Zero();
  const Vec v2 = simde_mm_set1_epi32(1);
  slots.StoreU64(8, r11);
  f13 = double(int64_t(slots.LoadU64(8)));
  Vec v13 = slots.Lvlx(0);
  Vec v11 = Splat0(v13);
  Vec v28 = v0, v30 = v0;
  double f12 = LoadDouble(0x8204D240);
  f10 = f10 * f12;
  f12 = f11 * f12;
  Vec v27 = v0, v29 = v0;
  f13 = double(float(f13));
  Vec v10 = Lvx(0x8204D250);
  const int64_t fraction = ToInt64(f10);
  const int64_t step = ToInt64(f12);
  Vec v9 = Lvx(0x8204D260);
  f0 = double(float(f0 / f13));
  f13 = LoadSingle(0x82000C34);
  f13 = double(float(f0 * f13));
  slots.StoreFloat(0, float(f13));
  Vec v12 = slots.Lvlx(0);
  slots.StoreFloat(0, float(f0));
  const Vec v26 = Splat0(v12);
  Vec v8 = slots.Lvlx(0);
  slots.StoreU64(8, uint64_t(fraction));
  v12 = Splat0(v8);
  slots.StoreU64(0, uint64_t(step));
  Vec v7 = slots.Lvlx(4);
  Vec v6 = slots.Lvlx(12);
  v13 = Splat0(v7);
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
  v0 = AddI(v12, v12);
  const Vec v25 = AddI(v0, v0);
  v13 = Sld<12>(v13, v7);
  v0 = AddI(v13, v0);
  const uint64_t r8 = slots.LoadU64(0);
  Vec v31 = AddI(v13, v8);
  r3 = slots.LoadU64(8);
  Vec v1 = AddI(v0, v8);
  do {
    for (int k = 0; k < 8; k++) {
      const uint64_t at = uint64_t(uint32_t(r3 >> 32) << 2) + r10;
      r3 = r8 + r3;
      v12 = Sld<14>(v28, Lvlx(uint32_t(at)));
      v0 = Lvlx(uint32_t(at + 2));
      v13 = Lvlx(uint32_t(at + 6));
      v0 = Sld<14>(v30, v0);
      v11 = Lvlx(uint32_t(at + 4));
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
    Stvx(uint32_t(r5 + 1040), v13);
    v13 = High16(v12);
    Stvx(uint32_t(r5 + 1024), v7);
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
    Stvx(uint32_t(r5), v13);
    Stvx(uint32_t(r5 + 16), v0);
    r6 = r6 - 8;
    v10 = Add(v10, v26);
    v9 = Add(v9, v26);
    r5 = r5 + 32;
    v1 = AddI(v1, v25);
    v31 = AddI(v31, v25);
  } while (int32_t(r6) > 0);
  StorePositions(state, r10, r5);
  const double f1 = c_runtime::U64ToDouble(r3);
  mode.Scalar();
  f0 = f1 * LoadDouble(0x820022B0);
  f0 = double(float(f0));
  s.set_fraction(float(f0));
  return std::bit_cast<uint64_t>(f1);
}

// sub_8247E188 and sub_8247E260 (the same steps over their two paths): the
// vector path when the output (+20 + 4 x +28) is 16-byte aligned, the
// frames left (+24 - +28) a multiple of 8, the last input frame's index
// (+4 - +8 - 1) past the position's end (+44 x the frames + +48) and the
// fraction not below 0.0 (the float at 0x82000A34); else the scalar path.
template <bool stereo>
uint64_t Resample(uint32_t state, uint32_t sp) {
  const ConverterState s(state);
  Slots slots;  // the original's frame (112 bytes) +80 to +95
  Mode mode;
  mode.Scalar();
  const uint64_t written = s.frames_written();
  const double f13 = double(s.fraction());
  const uint64_t frames = s.output_frames();
  const uint64_t position = s.position();
  uint64_t r5 = uint64_t(uint32_t(written) << 2);
  const uint32_t channels = s.channels();
  const uint64_t r6 = frames - written;
  const uint64_t end = s.input_end();
  const int64_t offset = int64_t(int32_t(channels)) * int64_t(int32_t(uint32_t(position)));
  const uint64_t input = s.input();
  const uint64_t output = s.output();
  const uint64_t r4 = end - position;
  const uint64_t r3 = uint64_t(uint32_t(uint64_t(offset)) << 1) + input;
  r5 = r5 + output;
  bool vector = (((uint32_t(r6) & 7) | uint32_t(r5)) & 0xF) == 0;
  if (vector) {
    const uint64_t left = Ext(uint32_t(r6));
    const double ratio = double(s.ratio());
    const uint64_t last = Ext(uint32_t(r4 - 1));
    slots.StoreU64(0, left);
    slots.StoreU64(8, last);
    double f12 = double(int64_t(slots.LoadU64(0)));
    double f11 = double(int64_t(slots.LoadU64(8)));
    f12 = double(float(f12));
    f11 = double(float(f11));
    const double reach = double(float(std::fma(ratio, f12, f13)));
    vector = f11 > reach && !(f13 < LoadSingle(0x82000A34));
  }
  if (vector) {
    return stereo ? ResampleStereoVector(r3, r5, r6, state)
                  : ResampleMonoVector(r3, r5, r6, state);
  }
  return stereo ? ResampleStereoScalar(state, sp - 112) : ResampleMonoScalar(state, sp - 112);
}

}  // namespace

uint64_t ResampleMono(uint32_t state, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8247E188 the mono resampler");
  return Resample<false>(state, sp);
}

uint64_t ResampleStereo(uint32_t state, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8247E260 the stereo resampler");
  return Resample<true>(state, sp);
}

// sub_82473E48 (entry 6) and sub_82474188 (entry 7): a frame at a time,
// linear between the input frame at the position and the next (the last
// input frame kept at +52, and +56 for the second channel, for a position
// before the first), the volume ramp added a frame at a time; their words
// below `sp` in the original, host Slots here.
uint64_t ResampleMonoScalar(uint32_t state, uint32_t sp) {
  const ConverterState s(state);
  ITC_NATIVE_RUNNING("sub_82473E48 the mono resampler's scalar path");
  Slots slots;  // the words 144 to 129 bytes below sp (no frame of its own)
  Mode mode;
  mode.Scalar();
  const uint32_t input = s.input();
  const double f13_ratio = double(s.ratio());
  const uint32_t written = s.frames_written();
  uint64_t r7 = uint64_t(uint32_t(written << 2)) + s.output();
  const uint32_t position = s.position();
  const uint32_t channels = s.channels();
  const uint64_t frames = s.output_frames();
  const int64_t offset = int64_t(int32_t(channels)) * int64_t(int32_t(position));
  const uint64_t end = s.input_end();
  const double f12_fraction = double(s.fraction());
  uint64_t r8 = frames - written;
  const uint64_t r25 = end - position;
  const uint64_t r9 = uint64_t(uint32_t(uint64_t(offset)) << 1) + input;
  slots.StoreU32(0, uint32_t(r8));
  Vec v13 = slots.Lvlx(0);
  mode.Vector();
  v13 = ToFloat(v13);
  Vec v12 = Lvlx(s.target_volume_address());
  slots.StoreU64(8, 1ull << 32);
  mode.Scalar();
  double f0 = double(int64_t(slots.LoadU64(8)));
  Vec v0 = Lvlx(s.volume_address());
  mode.Vector();
  v12 = Sub(v12, v0);
  Vec v9 = Reciprocal(v13);
  mode.Scalar();
  const double f12 = f12_fraction * f0;
  const double f11 = f13_ratio * f0;
  f0 = LoadDouble(0x82002250) / f0;
  slots.StoreU64(0, uint64_t(ToInt64(f12)));
  mode.Vector();
  v12 = Mul(v12, v9);
  mode.Scalar();
  slots.StoreU64(8, uint64_t(ToInt64(f11)));
  const uint64_t r4 = slots.LoadU64(8);
  Vec v11 = slots.Lvlx(4);
  Vec v10 = slots.Lvlx(12);
  v13 = Splat0(v11);
  uint64_t r11 = slots.LoadU64(0);
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
      Vec v8 = Lvlx(uint32_t(r9));
      Vec v7 = Lvlx(s.last_sample_0_address());
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
      Stvewx(uint32_t(r7), v12);
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
      Vec v8 = High16(Lvlx(uint32_t(r6) + uint32_t(r9)));
      Vec v7 = High16(Lvlx(uint32_t(r10) + uint32_t(r9)));
      r10 = uint64_t(int64_t(r11) >> 32);
      v8 = SignedToFloat(v8, kSample);
      v9 = Sld<12>(v4, v9);
      v7 = SignedToFloat(v7, kSample);
      v9 = Mul(v12, v9);
      v12 = Splat0(v8);
      v12 = Blend3(v12, v7);
      v9 = Sld<8>(v9, v3);
      v12 = Dot(v12, v9);
      Stvewx(uint32_t(r7), v12);
      r7 = r7 + 4;
    } while (int64_t(r10) < r30);
  }
  if (!ended && int32_t(r8) != 0) {
    if (int64_t(r10) == r30) {
      // The last input frame kept for the next pass.
      const int64_t sample = int16_t(guest::LoadU16(uint32_t(uint32_t(r24) << 1) + uint32_t(r9)));
      r10 = r10 + 1;
      slots.StoreU64(8, uint64_t(sample));
      mode.Scalar();
      const double f13 = double(int64_t(slots.LoadU64(8)));
      const double f12s = double(float(f13));
      const double scaled = double(float(f12s * LoadSingle(0x8204D160)));
      s.set_last_sample_0(float(scaled));
    } else {
      const int64_t r8s = int64_t(int32_t(uint32_t(r25)));
      if (int64_t(r10) > r8s) {
        r11 = ((r10 - uint64_t(r8s)) << 32) + r11;
      }
    }
  }
  const uint64_t r6 = r10 << 32;
  Stvewx(s.volume_address(), v0);
  const uint64_t at = uint64_t(uint32_t(uint32_t(r10) << 1)) + r9;
  r11 = r11 - r6;
  slots.StoreU64(8, r11);
  mode.Scalar();
  const double f13 = double(int64_t(slots.LoadU64(8)));
  s.set_fraction(float(double(float(f13 * f0))));
  StorePositions(state, at, r7);
  return state;
}

uint64_t ResampleStereoScalar(uint32_t state, uint32_t sp) {
  const ConverterState s(state);
  ITC_NATIVE_RUNNING("sub_82474188 the stereo resampler's scalar path");
  Slots slots;  // the words 160 to 145 bytes below sp (no frame of its own)
  Mode mode;
  mode.Scalar();
  const uint32_t input = s.input();
  const double f13_ratio = double(s.ratio());
  const uint32_t written = s.frames_written();
  const uint32_t output = s.output();
  uint64_t r8 = uint64_t(uint32_t(written << 2)) + output;
  const uint32_t position = s.position();
  const uint32_t channels = s.channels();
  const uint32_t kept = s.last_sample_0_address();
  const uint64_t frames = s.output_frames();
  const int64_t offset = int64_t(int32_t(channels)) * int64_t(int32_t(position));
  const uint64_t end = s.input_end();
  const double f12_fraction = double(s.fraction());
  uint64_t r7 = frames - written;
  const uint64_t r25 = end - position;
  const uint64_t r9 = uint64_t(uint32_t(uint64_t(offset)) << 1) + input;
  slots.StoreU32(0, uint32_t(r7));
  Vec v13 = slots.Lvlx(0);
  mode.Vector();
  v13 = ToFloat(v13);
  Vec v12 = Lvlx(s.target_volume_address());
  slots.StoreU64(8, 1ull << 32);
  mode.Scalar();
  double f0 = double(int64_t(slots.LoadU64(8)));
  Vec v0 = Lvlx(s.volume_address());
  mode.Vector();
  v12 = Sub(v12, v0);
  Vec v9 = Reciprocal(v13);
  mode.Scalar();
  const double f12 = f12_fraction * f0;
  const double f11 = f13_ratio * f0;
  f0 = LoadDouble(0x82002250) / f0;
  slots.StoreU64(0, uint64_t(ToInt64(f12)));
  mode.Vector();
  v12 = Mul(v12, v9);
  mode.Scalar();
  slots.StoreU64(8, uint64_t(ToInt64(f11)));
  const uint64_t r4 = slots.LoadU64(8);
  Vec v11 = slots.Lvlx(4);
  Vec v10 = slots.Lvlx(12);
  v13 = Splat0(v11);
  uint64_t r11 = slots.LoadU64(0);
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
      v12 = Lvlx(uint32_t(r9 + 2));
      mode.Vector();
      const Vec v5 = Sub(v10, v0);
      Vec v7 = Lvlx(kept + 4);
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
      Stvewx(second, v12);
      v12 = High16(Lvlx(uint32_t(r9)));
      v11 = Lvlx(kept);
      v10 = SignedToFloat(v12, kSample);
      v12 = Splat0(v11);
      v11 = Sld<8>(v7, v6);
      v12 = Blend3(v12, v10);
      v12 = Dot(v12, v11);
      Stvewx(uint32_t(r8), v12);
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
      v10 = High16(Lvlx(uint32_t(at + 2)));
      Vec v4 = UnsignedToFloat(v12, kFraction);
      v12 = High16(Lvlx(uint32_t(at_next + 2)));
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
      Stvewx(second, v12);
      v10 = Lvlx(uint32_t(at_next));
      v12 = Lvlx(uint32_t(at));
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
      Stvewx(uint32_t(r8), v12);
      r8 = r8 + 4;
    } while (int64_t(r10) < r30);
  }
  if (!ended && int32_t(r7) != 0) {
    if (int64_t(r10) == r30) {
      // The last input frame kept for the next pass (its second channel
      // first).
      const uint32_t last = uint32_t(uint64_t(uint32_t(uint32_t(r24) << 2)) + r9);
      r10 = r10 + 1;
      int64_t sample = int16_t(guest::LoadU16(last + 2));
      slots.StoreU64(8, uint64_t(sample));
      mode.Scalar();
      double f13 = double(int64_t(slots.LoadU64(8)));
      double f12s = double(float(f13));
      f13 = LoadSingle(0x8204D160);
      f12s = double(float(f12s * f13));
      guest::StoreFloat(kept + 4, float(f12s));
      sample = int16_t(guest::LoadU16(last));
      slots.StoreU64(8, uint64_t(sample));
      f12s = double(int64_t(slots.LoadU64(8)));
      f12s = double(float(f12s));
      f13 = double(float(f12s * f13));
      guest::StoreFloat(kept, float(f13));
    } else {
      const int64_t r25s = int64_t(int32_t(uint32_t(r25)));
      if (int64_t(r10) > r25s) {
        r11 = ((r10 - uint64_t(r25s)) << 32) + r11;
      }
    }
  }
  const uint64_t r6 = r10 << 32;
  Stvewx(s.volume_address(), v0);
  const uint64_t at = uint64_t(uint32_t(uint32_t(r10) << 2)) + r9;
  r11 = r11 - r6;
  slots.StoreU64(8, r11);
  mode.Scalar();
  const double f13 = double(int64_t(slots.LoadU64(8)));
  s.set_fraction(float(double(float(f13 * f0))));
  StorePositions(state, at, r8);
  return state;
}

// --- Equal rates (entries 107 and 32) ---

uint64_t CopyStereoScalar(uint32_t state, uint32_t sp) {
  const ConverterState s(state);
  ITC_NATIVE_RUNNING("sub_82478E60 the stereo copy's scalar path");
  Slots slots;  // the words 16 to 1 bytes below sp (no frame of its own)
  Mode mode;
  const uint32_t position = s.position();
  mode.Scalar();
  double f0 = double(s.volume());
  const uint32_t channels = s.channels();
  const uint32_t written = s.frames_written();
  uint64_t r9 = uint64_t(s.input_end()) - position;
  const uint64_t r6 = uint64_t(s.output_frames()) - written;
  const int64_t offset = int64_t(int32_t(channels)) * int64_t(int32_t(position));
  uint64_t r11 = uint64_t(uint32_t(uint64_t(offset)) << 1) + s.input();
  uint64_t r8 = uint64_t(uint32_t(written << 2)) + s.output();
  if (!(int32_t(r9) < int32_t(r6))) {
    r9 = r6;
  }
  const uint64_t left = Ext(uint32_t(r6));
  double f13 = double(s.target_volume());
  f13 = double(float(f13 - f0));
  slots.StoreU64(0, left);
  double f12 = double(int64_t(slots.LoadU64(0)));
  f12 = double(float(f12));
  f12 = double(float(f13 / f12));
  f13 = LoadSingle(0x8204D160);
  do {
    int64_t sample = int16_t(guest::LoadU16(uint32_t(r11) + 2));
    r9 = r9 - 1;
    slots.StoreU64(0, uint64_t(sample));
    double f11 = double(int64_t(slots.LoadU64(0)));
    f11 = double(float(f11));
    f11 = double(float(f11 * f13));
    f11 = double(float(f11 * f0));
    guest::StoreFloat(uint32_t(r8) + 1024, float(f11));
    sample = int16_t(guest::LoadU16(uint32_t(r11)));
    r11 = r11 + 4;
    slots.StoreU64(8, uint64_t(sample));
    f11 = double(int64_t(slots.LoadU64(8)));
    f11 = double(float(f11));
    f11 = double(float(f11 * f13));
    f11 = double(float(f11 * f0));
    guest::StoreFloat(uint32_t(r8), float(f11));
    r8 = r8 + 4;
    f0 = double(float(f12 + f0));
  } while (int32_t(r9) != 0);
  StorePositions(state, r11, r8);
  s.set_volume(float(f0));
  return state;
}

uint64_t CopyStereo(uint32_t state, uint32_t sp) {
  const ConverterState s(state);
  ITC_NATIVE_RUNNING("sub_8247F558 the stereo copy");
  Slots slots;  // the original's frame (160 bytes) +80 to +95
  const uint32_t position = s.position();
  const uint64_t end = s.input_end();
  const uint32_t channels = s.channels();
  const uint32_t written = s.frames_written();
  uint64_t r9 = end - position;
  const uint64_t frames = s.output_frames();
  const int64_t offset = int64_t(int32_t(channels)) * int64_t(int32_t(position));
  const uint64_t input = s.input();
  const uint64_t output = s.output();
  uint64_t r7 = frames - written;
  uint64_t r11 = uint64_t(uint32_t(uint64_t(offset)) << 1) + input;
  uint64_t r8 = uint64_t(uint32_t(written << 2)) + output;
  if (!(int32_t(r9) < int32_t(r7))) {
    r9 = r7;
  }
  if ((((uint32_t(r9) & 7) | uint32_t(r8) | uint32_t(r11)) & 0xF) != 0) {
    return CopyStereoScalar(state, sp - 160);
  }
  Mode mode;
  const uint64_t left = Ext(uint32_t(r7));
  Vec v0 = Zero();
  r7 = Ext(uint32_t(r9) << 1);
  mode.Scalar();
  double f0 = double(s.volume());
  double f13 = double(s.target_volume());
  f13 = double(float(f13 - f0));
  slots.StoreU64(0, left);
  double f12 = double(int64_t(slots.LoadU64(0)));
  slots.StoreU32(0, guest::LoadU32(s.volume_address()));
  slots.StoreU64(8, r7);
  f12 = double(float(f12));
  f13 = double(float(f13 / f12));
  double f11 = double(int64_t(slots.LoadU64(8)));
  f11 = double(float(f11));
  f0 = double(float(std::fma(f11, f13, f0)));
  s.set_volume(float(f0));
  f0 = LoadSingle(0x820021F8);
  v0 = slots.Lvlx(0);
  f0 = double(float(f13 * f0));
  slots.StoreFloat(0, float(f0));
  Vec v13 = slots.Lvlx(0);
  v0 = Splat0(v0);
  slots.StoreFloat(0, float(f13));
  const Vec v8 = Splat0(v13);
  Vec v12 = slots.Lvlx(0);
  v13 = Splat0(v12);
  Vec v11 = Lvx(0x8204D250);
  mode.Vector();
  const Vec v9 = Add(v8, v8);
  v12 = Mul(v13, v11);
  Vec v10 = Lvx(0x8204D260);
  v13 = Mul(v13, v10);
  v10 = Add(v0, v12);
  v0 = Add(v0, v13);
  if (int32_t(r9) > 0) {
    uint32_t passes = ((uint32_t(r9) - 1) >> 3) + 1;
    const Vec order = Lvx(0x8204D1D0);
    do {
      v13 = Lvx(uint32_t(r11));
      v13 = Perm(v13, v13, order);
      mode.Vector();
      v11 = Add(v0, v8);
      const Vec v4 = v0, v3 = v0;
      v10 = Add(v10, v9);
      v12 = Lvx(uint32_t(r11 + 16));
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
      Stvx(uint32_t(r8), v6);
      const uint64_t at = r8;
      r8 = r8 + 32;
      Stvx(uint32_t(at + 1024), v13);
      Stvx(uint32_t(at + 16), v5);
      Stvx(uint32_t(at + 1040), v12);
    } while (passes != 0);
  }
  // The position and the frames written from the words loaded first.
  const uint32_t used = Frames(uint32_t(r11 - input), channels);
  s.set_position(used < uint32_t(end) ? used : uint32_t(end));
  const uint32_t made = uint32_t(r8 - output) >> 2;
  s.set_frames_written(made < uint32_t(frames) ? made : uint32_t(frames));
  return state;
}

uint64_t ResamplerProcess(uint32_t state, uint32_t frame, const GuestCall& call) {
  const ConverterState s(state);
  ITC_NATIVE_RUNNING("sub_8246F140 Resampler::Process");
  const uint32_t end = s.input_end();
  const uint32_t start = s.position();
  if (!(start < end)) {
    return 0;
  }
  if (!(s.frames_written() < s.output_frames())) {
    return 0;
  }
  const uint32_t changed = s.changed();
  if (changed != 0) {
    if ((changed & 3) != 0) {
      Slots slots;  // the caller's frame +80 to +95 (Framed's 128 bytes)
      Mode mode;
      const uint32_t out_rate = s.output_rate();
      const uint32_t in_rate = s.input_rate();
      const uint32_t flags = s.flags();
      slots.StoreU64(0, in_rate);
      slots.StoreU64(8, out_rate);
      mode.Scalar();
      double f0 = double(int64_t(slots.LoadU64(0)));
      double f13 = double(int64_t(slots.LoadU64(8)));
      f0 = double(float(f0));
      f13 = double(float(f13));
      f0 = double(float(f0 / f13));
      s.set_ratio(float(f0));
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
      const uint64_t channels = s.channels();
      const uint64_t format = s.format();
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
      const uint32_t entry = uint32_t(index << 2) + schema::XaudioObjectsGlobals::resampler_kernels_address();
      s.set_kernel(guest::LoadU32(entry));
    }
    s.set_changed(0);
  }
  const uint32_t kernel = s.kernel();
  switch (kernel) {
    case 0x8247E188:
      ResampleMono(state, frame);
      break;
    case 0x8247E260:
      ResampleStereo(state, frame);
      break;
    case 0x8247F558:
      CopyStereo(state, frame);
      break;
    case 0x82473E48:
      ResampleMonoScalar(state, frame);
      break;
    case 0x82474188:
      ResampleStereoScalar(state, frame);
      break;
    case 0x82478E60:
      CopyStereoScalar(state, frame);
      break;
    default:
      Regs regs;
    regs.r[regs.count++] = state;
    regs.sp = frame;
    call(kernel, 0x8246F2DC, regs);
      break;
  }
  return uint64_t(s.position()) - uint64_t(start);
}

}  // namespace xaudio
