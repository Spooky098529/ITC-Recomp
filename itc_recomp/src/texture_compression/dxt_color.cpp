// The DXT color block natively (docs/functions/texture-compression.md):
// DxtColor::PackR5G6B5 sub_824206E8, DxtColor::FitEndpoints sub_82420AB8 and
// DxtColor::EncodeBlock sub_82420F50, step for step as the translated code
// does them (steps.h).

#include "texture_compression/dxt_color.h"

#include "base/log.h"
#include "texture_compression/steps.h"

#include <rex/platform/fpscr.h>
#include <simde/x86/sse.h>

#include <atomic>
#include <bit>
#include <cstring>
#include <utility>

namespace texture_compression {

using namespace steps;

namespace {

// The game's constants (lfs from .rdata).
constexpr uint32_t kOne = 0x82000A30;   // 1.0
constexpr uint32_t kZero = 0x82000A34;  // 0.0
constexpr uint32_t kHalf = 0x82001BE4;  // probably 0.5
// PackR5G6B5's scales (probably 31 for red and blue, 63 for green).
constexpr uint32_t kScale5 = 0x8203B7A0;
constexpr uint32_t kScale6 = 0x8203B7A4;
// FitEndpoints': the low end's start (the color weights the encoder scales
// the pixels by, in .data: 0.2970, 1.0, 0.1008 in play, so the top of each
// scaled channel), the smallest squared extent (FLT_MIN in play), the
// smallest squared length (f15, 2^-12), the squared corrections to stop at
// (f16, 2^-16), the corrections' factor (f17, -1.0) and the weights' factor
// (f19, 0.125).
constexpr uint32_t kLowStart = 0x82649B10;
constexpr uint32_t kExtentMin = 0x82018FE0;
constexpr uint32_t kLengthMin = 0x8203EAAC;
constexpr uint32_t kStopMin = 0x8203EAA8;
constexpr uint32_t kCorrection = 0x82001BE0;
constexpr uint32_t kWeightScale = 0x82002A88;
// The weights of the low and high ends for each step.
constexpr uint32_t kLowWeights3 = 0x8203EADC;
constexpr uint32_t kHighWeights3 = 0x8203EAD0;
constexpr uint32_t kLowWeights4 = 0x8203EAC0;
constexpr uint32_t kHighWeights4 = 0x8203EAB0;

// The clamp to 0-1 PackR5G6B5 does (blt, then bgt: a NaN kept).
double Clamp(double v, double zero, double one) {
  if (v < zero) {
    return zero;
  }
  if (v > one) {
    return one;
  }
  return v;
}

uint32_t Rotl(uint32_t v, int n) {
  return (v << n) | (v >> (32 - n));
}

}  // namespace

uint16_t PackR5G6B5(uint8_t* base, const float color[3]) {
  ITC_NATIVE_RUNNING("sub_824206E8 DxtColor::PackR5G6B5");
  const double zero = Lfs(base, kZero);  // f13
  const double one = Lfs(base, kOne);    // f11
  const double r = Clamp(color[0], zero, one);  // f9
  const double g = Clamp(color[1], zero, one);  // f10
  const double b = Clamp(color[2], zero, one);  // f11
  const double scale5 = Lfs(base, kScale5);
  const double scale6 = Lfs(base, kScale6);
  const double half = Lfs(base, kHalf);
  const uint32_t ri = uint32_t(Fctiwz(Fmadds(r, scale5, half)));
  const uint32_t gi = uint32_t(Fctiwz(Fmadds(g, scale6, half)));
  const uint32_t bi = uint32_t(Fctiwz(Fmadds(b, scale5, half)));
  // rlwinm 6, or, rlwinm 5, or, clrlwi 16.
  uint32_t v = (Rotl(ri, 6) & 0xFFFFFFC0u) | gi;
  v = (Rotl(v, 5) & 0xFFFFFFE0u) | bi;
  return uint16_t(v & 0xFFFF);
}

namespace {

std::atomic<uint64_t> stack_reads{0};

// Logs once for the game and once for a harness, for each of the three
// places it reads (the words the original wrote at r1 - 272 to - 260, the
// never-written stack, the original's saved registers), that a step index
// fell outside the steps.
void NoteStackRead(int32_t index, uint32_t address, int place) {
  stack_reads.fetch_add(1, std::memory_order_relaxed);
  static std::atomic<bool> noted[2][3] = {};
  const bool harness = ::base::log_detail::in_harness;
  if (!noted[harness][place].exchange(true, std::memory_order_relaxed)) {
    static const char* const kPlaces[] = {"the words it wrote", "never-written stack",
                                          "the original's saved registers: not given"};
    ITC_LOG_INFO("DxtColor::FitEndpoints ({}): step index {} read from the stack at {:08X} ({})",
                 harness ? "harness" : "game", index, address, kPlaces[place]);
  }
}

}  // namespace

uint64_t FitColorEndpointsStackReads() {
  return stack_reads.load(std::memory_order_relaxed);
}

void FitColorEndpoints(uint8_t* base, const float pixels[16][3], uint32_t colors, uint32_t stack,
                       float low[3], float high[3]) {
  ITC_NATIVE_RUNNING("sub_82420AB8 DxtColor::FitEndpoints");
  const uint32_t low_weights = colors == 3 ? kLowWeights3 : kLowWeights4;     // r30
  const uint32_t high_weights = colors == 3 ? kHighWeights3 : kHighWeights4;  // r29
  const double zero = Lfs(base, kZero);  // f0

  // The bounds: lo (f24, f26, f25) from the floats at 0x82649B10, hi (f21,
  // f23, f22) from 0.0; each pixel tested against both.
  double lo[3] = {Lfs(base, kLowStart), Lfs(base, kLowStart + 4), Lfs(base, kLowStart + 8)};
  double hi[3] = {zero, zero, zero};
  for (int i = 0; i < 16; i++) {
    const double x = pixels[i][0];
    const double y = pixels[i][1];
    const double z = pixels[i][2];
    if (x < lo[0]) lo[0] = x;
    if (y < lo[1]) lo[1] = y;
    if (z < lo[2]) lo[2] = z;
    if (x > hi[0]) hi[0] = x;
    if (y > hi[1]) hi[1] = y;
    if (z > hi[2]) hi[2] = z;
  }

  auto finish = [&] {
    for (int c = 0; c < 3; c++) {
      low[c] = float(lo[c]);
      high[c] = float(hi[c]);
    }
  };

  const double ey = Fsubs(hi[1], lo[1]);  // f11
  const double ez = Fsubs(hi[2], lo[2]);  // f12
  const double ex = Fsubs(hi[0], lo[0]);  // f6
  const double extent = Fmadds(ex, ex, Fmadds(ez, ez, Fmuls(ey, ey)));  // f31
  if (extent < Lfs(base, kExtentMin)) {
    finish();
    return;
  }

  // Which of the box's four diagonals the pixels follow best: the sums of
  // the squares of their projections on each, from the box's center (kept at
  // r1 - 272 to - 260).
  const double half = Lfs(base, kHalf);  // f20
  double sums[4] = {zero, zero, zero, zero};  // f10, f9, f8, f7
  {
    const double inv = Fdivs(Lfs(base, kOne), extent);
    const double cx = Fmuls(Fadds(hi[0], lo[0]), half);  // f5
    const double cy = Fmuls(Fadds(hi[1], lo[1]), half);  // f4
    const double cz = Fmuls(Fadds(hi[2], lo[2]), half);  // f3
    const double ax = Fmuls(inv, ex);  // f6
    const double ay = Fmuls(inv, ey);  // f2
    const double az = Fmuls(inv, ez);  // f1
    for (int i = 0; i < 16; i++) {
      const double dy = Fsubs(pixels[i][1], cy);
      const double dz = Fsubs(pixels[i][2], cz);
      const double dx = Fsubs(pixels[i][0], cx);
      const double a = Fmuls(dy, ay);  // f12
      const double b = Fmuls(dz, az);  // f13
      const double c = Fmuls(dx, ax);  // f11
      const double ba = Fadds(b, a);
      const double ac = Fadds(a, c);
      const double ca = Fsubs(c, a);
      const double p0 = Fadds(ba, c);
      sums[0] = Fmadds(p0, p0, sums[0]);
      const double p1 = Fsubs(ac, b);
      sums[1] = Fmadds(p1, p1, sums[1]);
      const double p2 = Fadds(ca, b);
      const double p3 = Fsubs(ca, b);
      sums[2] = Fmadds(p2, p2, sums[2]);
      sums[3] = Fmadds(p3, p3, sums[3]);
    }
  }
  double best = sums[0];
  uint32_t diagonal = 0;
  for (uint32_t k = 1; k < 4; k++) {
    if (sums[k] > best) {
      best = sums[k];
      diagonal = k;
    }
  }
  if (diagonal & 2) {
    std::swap(lo[1], hi[1]);
  }
  if (diagonal & 1) {
    std::swap(lo[2], hi[2]);
  }
  const double length_min = Lfs(base, kLengthMin);  // f15
  if (extent < length_min) {
    finish();
    return;
  }

  // The refinement: each pass the steps' colors (r1 - 256 + 16 k), each
  // pixel's nearest step, and a correction of each end from the weighted
  // differences.
  const uint32_t last = colors - 1;                  // r7
  const double correction = Lfs(base, kCorrection);  // f17
  const double weight_scale = Lfs(base, kWeightScale);  // f19
  const double stop_min = Lfs(base, kStopMin);      // f16
  const double last_f = IntToSingle(int64_t(last));  // f18 (fcfid of the zero-extended word)
  float points[4][3] = {};
  // The words the original leaves at r1 - 272 to - 260 while it fits (the
  // first the pixel's step index, the second the last step's as a word, then
  // two of the diagonals' sums).
  uint32_t scratch[4] = {0, last, std::bit_cast<uint32_t>(float(sums[2])),
                         std::bit_cast<uint32_t>(float(sums[3]))};
  auto stack_word = [&](uint32_t address, int32_t index) -> uint32_t {
    const uint32_t steps_at = address - (stack - 256);
    if (steps_at < 16 * colors && (steps_at & 15) < 12 && (steps_at & 3) == 0) {
      return std::bit_cast<uint32_t>(points[steps_at >> 4][(steps_at & 15) >> 2]);
    }
    const uint32_t scratch_at = address - (stack - 272);
    if (scratch_at < 16 && (scratch_at & 3) == 0) {
      NoteStackRead(index, address, 0);
      return scratch[scratch_at >> 2];
    }
    // Below r1 - 272 (or past the steps' values): never written by the
    // original, so what's there; r1 - 192 to r1 held its saved registers.
    NoteStackRead(index, address, address - (stack - 192) < 192 ? 2 : 1);
    return guest::LoadU32(base, address);
  };

  for (uint32_t pass = 0; pass < 8; pass++) {
    for (uint32_t k = 0; k < colors; k++) {
      const double lw = Lfs(base, low_weights + 4 * k);
      const double hw = Lfs(base, high_weights + 4 * k);
      const double l0 = Fmuls(lw, lo[0]);
      const double l1 = Fmuls(lw, lo[1]);
      const double l2 = Fmuls(lw, lo[2]);
      points[k][0] = float(Fmadds(hw, hi[0], l0));
      points[k][1] = float(Fmadds(hw, hi[1], l1));
      points[k][2] = float(Fmadds(hw, hi[2], l2));
    }
    const double dy = Fsubs(hi[1], lo[1]);  // f10
    const double dz = Fsubs(hi[2], lo[2]);  // f12
    const double dx = Fsubs(hi[0], lo[0]);  // f9
    const double length = Fmadds(dx, dx, Fmadds(dz, dz, Fmuls(dy, dy)));
    if (length < length_min) {
      break;
    }
    const double scale = Fdivs(last_f, length);
    const double sx = Fmuls(scale, dx);  // f29
    const double sy = Fmuls(scale, dy);  // f28
    const double sz = Fmuls(scale, dz);  // f27
    double low_sq = zero;    // f11
    double high_sq = zero;   // f4
    double low_err[3] = {zero, zero, zero};   // f5, f3, f2
    double high_err[3] = {zero, zero, zero};  // f1, f31, f30
    for (int i = 0; i < 16; i++) {
      const double py = Fsubs(pixels[i][1], lo[1]);
      const double pz = Fsubs(pixels[i][2], lo[2]);
      const double px = Fsubs(pixels[i][0], lo[0]);
      const double t = Fmadds(px, sx, Fmadds(pz, sz, Fmuls(py, sy)));
      uint32_t index;  // r10
      if (!(t < last_f)) {
        index = last;
      } else {
        index = uint32_t(Fctiwz(Fadds(t, half)));
      }
      scratch[0] = index;
      const uint32_t word_at = index << 2;   // r9
      const uint32_t point_at = index << 4;  // r10
      const double lw = Lfs(base, word_at + low_weights);
      const double hw = Lfs(base, word_at + high_weights);
      double qx;
      double qy;
      double qz;
      if (index < colors) {
        qx = points[index][0];
        qy = points[index][1];
        qz = points[index][2];
      } else {
        const uint32_t at = point_at + (stack - 256);
        qx = std::bit_cast<float>(stack_word(at, int32_t(index)));
        qy = std::bit_cast<float>(stack_word(at + 4, int32_t(index)));
        qz = std::bit_cast<float>(stack_word(at + 8, int32_t(index)));
      }
      const double ddx = Fsubs(qx, pixels[i][0]);  // f8
      const double ddy = Fsubs(qy, pixels[i][1]);  // f7
      const double ddz = Fsubs(qz, pixels[i][2]);  // f6
      const double lws = Fmuls(lw, weight_scale);  // f13
      const double hws = Fmuls(hw, weight_scale);  // f12
      low_sq = Fmadds(lw, lws, low_sq);
      low_err[0] = Fmadds(lws, ddx, low_err[0]);
      low_err[1] = Fmadds(lws, ddy, low_err[1]);
      low_err[2] = Fmadds(lws, ddz, low_err[2]);
      high_sq = Fmadds(hw, hws, high_sq);
      high_err[0] = Fmadds(hws, ddx, high_err[0]);
      high_err[1] = Fmadds(hws, ddy, high_err[1]);
      high_err[2] = Fmadds(hws, ddz, high_err[2]);
    }
    if (low_sq > zero) {
      const double f = Fdivs(correction, low_sq);
      lo[0] = Fmadds(f, low_err[0], lo[0]);
      lo[1] = Fmadds(f, low_err[1], lo[1]);
      lo[2] = Fmadds(f, low_err[2], lo[2]);
    }
    if (high_sq > zero) {
      const double f = Fdivs(correction, high_sq);
      hi[0] = Fmadds(f, high_err[0], hi[0]);
      hi[1] = Fmadds(f, high_err[1], hi[1]);
      hi[2] = Fmadds(f, high_err[2], hi[2]);
    }
    if (Fmuls(low_err[0], low_err[0]) < stop_min && Fmuls(low_err[1], low_err[1]) < stop_min &&
        Fmuls(low_err[2], low_err[2]) < stop_min && Fmuls(high_err[0], high_err[0]) < stop_min &&
        Fmuls(high_err[1], high_err[1]) < stop_min && Fmuls(high_err[2], high_err[2]) < stop_min) {
      break;
    }
  }
  finish();
}

namespace {

using Fpscr = rex::platform::FPSCRPlatform;

// EncodeBlock's constants: the alpha reference (in .data, read twice as the
// original does), the color weights' inverses (0x82649B20, beside the
// weights at kLowStart), the 5- and 6-bit steps' sizes (probably 1/31 and
// 1/63), the palette's in-between steps for 4 colors (probably 1/3 and 2/3;
// 3 colors use kHalf), the error diffusion's weights (right, below left,
// below, below right: probably 7/16, 3/16, 5/16, 1/16, as the alpha half's)
// and the step to palette index tables (words).
constexpr uint32_t kAlphaReference = 0x8263B7E8;
constexpr uint32_t kInverseWeights = 0x82649B20;
constexpr uint32_t kStep5 = 0x8203B7A8;
constexpr uint32_t kStep6 = 0x8203B7AC;
constexpr uint32_t kThird = 0x82002A38;
constexpr uint32_t kTwoThirds = 0x82038450;
constexpr uint32_t kWeightRight = 0x8203EB04;
constexpr uint32_t kWeightBelowLeft = 0x8203B78C;
constexpr uint32_t kWeightBelow = 0x8203B790;
constexpr uint32_t kWeightBelowRight = 0x820093A0;
constexpr uint32_t kIndices3 = 0x8203EAF8;
constexpr uint32_t kIndices4 = 0x8203EAE8;
// Where the original keeps its palette in its frame (r1 + 176, 16 bytes an
// entry), for a table word past the palette.
constexpr uint32_t kPaletteInFrame = 176;

// The error diffusion over the 4 x 4 block (error buffer at r1 + 272 in the
// original): pixel i's error e, each channel to its right, below left,
// below and below right neighbors (fmadds e, weight, old).
struct Diffusion {
  double right;
  double below_left;
  double below;
  double below_right;
  float error[16][3];

  void Add(uint32_t to, const double e[3], double weight) {
    for (int c = 0; c < 3; c++) {
      error[to][c] = float(Fmadds(e[c], weight, error[to][c]));
    }
  }
  void Spread(uint32_t i, const double e[3]) {
    const uint32_t column = i & 3;
    if (column != 3) {
      Add(i + 1, e, right);
    }
    if (i < 12) {
      if (column != 0) {
        Add(i + 3, e, below_left);
      }
      Add(i + 4, e, below);
      if (column != 3) {
        Add(i + 5, e, below_right);
      }
    }
  }
};

// The palette's in-between colors: lvlx/lvrx, vsubfp, vmaddfp (a multiply
// then an add) with the vector unit's flush-to-zero (inputs and results),
// as the translated code does them on the host. The fourth lane (the
// original's leftovers) never reaches the block.
void Lerp(const float a[3], const float b[3], float t, float out[3]) {
  const uint32_t csr = Fpscr::getcsr();
  Fpscr::setcsr(csr | uint32_t(Fpscr::FlushMask));
  const simde__m128 va = simde_mm_set_ps(0.0f, a[2], a[1], a[0]);
  const simde__m128 vb = simde_mm_set_ps(0.0f, b[2], b[1], b[0]);
  const simde__m128 d = simde_mm_sub_ps(vb, va);
  const simde__m128 r = simde_mm_add_ps(simde_mm_mul_ps(d, simde_mm_set1_ps(t)), va);
  float lanes[4];
  simde_mm_storeu_ps(lanes, r);
  Fpscr::setcsr(csr);
  out[0] = lanes[0];
  out[1] = lanes[1];
  out[2] = lanes[2];
}

void StoreU16(uint8_t* out, uint32_t v) {
  out[0] = uint8_t(v >> 8);
  out[1] = uint8_t(v);
}

}  // namespace

void EncodeColorBlock(uint8_t* base, const float pixels[16][4], bool one_bit_alpha, int32_t dither,
                      uint32_t stack, uint8_t block[8]) {
  ITC_NATIVE_RUNNING("sub_82420F50 DxtColor::EncodeBlock");
  const uint32_t frame = stack - kColorFrameBytes;  // the original's r1

  // DXT1's 1-bit alpha: the pixels below the reference (a NaN isn't).
  uint32_t colors = 4;
  if (one_bit_alpha) {
    const double reference = Lfs(base, kAlphaReference);
    uint32_t below = 0;
    for (int i = 0; i < 16; i++) {
      if (double(pixels[i][3]) < reference) {
        below++;
      }
    }
    if (below == 16) {  // sth 0 at +0, sth -1 at +2, stw -1 at +4
      StoreU16(block, 0);
      StoreU16(block + 2, 0xFFFF);
      for (int b = 4; b < 8; b++) block[b] = 0xFF;
      return;
    }
    colors = below != 0 ? 3 : 4;
  }

  const double w[3] = {Lfs(base, kLowStart), Lfs(base, kLowStart + 4),
                       Lfs(base, kLowStart + 8)};  // f27, f28, f29
  const double scale5 = Lfs(base, kScale5);        // f8
  const double scale6 = Lfs(base, kScale6);        // f7
  const double step5 = Lfs(base, kStep5);          // f31
  const double step6 = Lfs(base, kStep6);          // f30
  const double half = Lfs(base, kHalf);            // f23
  // Each weighted pixel's fourth float (stfs of 1.0, read back as the
  // diffused error's factor).
  const double weighted_alpha = double(float(Lfs(base, kOne)));
  Diffusion diffusion;
  diffusion.right = Lfs(base, kWeightRight);           // f22
  diffusion.below_left = Lfs(base, kWeightBelowLeft);  // f21
  diffusion.below = Lfs(base, kWeightBelow);           // f20
  diffusion.below_right = Lfs(base, kWeightBelowRight);  // f19

  // The pixels quantized to 5:6:5 (with the error diffused so far) and
  // weighted, into r1 + 528.
  if (dither != 0) {
    std::memset(diffusion.error, 0, sizeof(diffusion.error));
  }
  float weighted[16][3];
  for (uint32_t i = 0; i < 16; i++) {
    double r = pixels[i][0];  // f13
    double g = pixels[i][1];  // f12
    double b = pixels[i][2];  // f0
    if (dither != 0) {
      b = Fadds(diffusion.error[i][2], b);
      r = Fadds(diffusion.error[i][0], r);
      g = Fadds(g, diffusion.error[i][1]);
    }
    const double qr = Fmuls(IntToSingle(Fctiwz(Fmadds(r, scale5, half))), step5);  // f11
    const double qg = Fmuls(IntToSingle(Fctiwz(Fmadds(g, scale6, half))), step6);  // f10
    const double qb = Fmuls(IntToSingle(Fctiwz(Fmadds(b, scale5, half))), step5);  // f9
    if (dither != 0) {
      const double e[3] = {Fsubs(r, qr), Fsubs(g, qg), Fsubs(b, qb)};
      diffusion.Spread(i, e);
    }
    weighted[i][0] = float(Fmuls(w[0], qr));
    weighted[i][1] = float(Fmuls(w[1], qg));
    weighted[i][2] = float(Fmuls(w[2], qb));
  }

  // The ends (r1 + 96 and r1 + 240), unweighted and packed.
  float low[3];
  float high[3];
  FitColorEndpoints(base, weighted, colors, frame, low, high);
  const double inverse[3] = {Lfs(base, kInverseWeights), Lfs(base, kInverseWeights + 4),
                             Lfs(base, kInverseWeights + 8)};
  const float low_color[3] = {float(Fmuls(inverse[0], low[0])), float(Fmuls(low[1], inverse[1])),
                              float(Fmuls(low[2], inverse[2]))};
  const float high_color[3] = {float(Fmuls(high[0], inverse[0])),
                               float(Fmuls(high[1], inverse[1])),
                               float(Fmuls(high[2], inverse[2]))};
  const uint32_t c0 = PackR5G6B5(base, low_color);   // r7
  const uint32_t c1 = PackR5G6B5(base, high_color);  // r11
  if (colors == 4 && c0 == c1) {  // sth c1 at +2, sth c0 at +0, stw 0 at +4
    StoreU16(block, c0);
    StoreU16(block + 2, c1);
    for (int b = 4; b < 8; b++) block[b] = 0;
    return;
  }

  // The packed ends decoded and weighted again: the palette's ends.
  const float end0[3] = {
      float(Fmuls(w[0], Fmuls(IntToSingle((c0 >> 11) & 0x1F), step5))),
      float(Fmuls(Fmuls(IntToSingle((c0 >> 5) & 0x3F), step6), w[1])),
      float(Fmuls(Fmuls(IntToSingle(c0 & 0x1F), step5), w[2])),
  };
  const float end1[3] = {
      float(Fmuls(Fmuls(IntToSingle((c1 >> 11) & 0x1F), step5), w[0])),
      float(Fmuls(Fmuls(IntToSingle((c1 >> 5) & 0x3F), step6), w[1])),
      float(Fmuls(Fmuls(IntToSingle(c1 & 0x1F), step5), w[2])),
  };
  // DXT1's order: the first end above the second for 4 colors, not above it
  // for 3.
  float palette[4][3];
  const bool in_order = (colors == 3) == (c1 >= c0);
  const uint32_t first = in_order ? c0 : c1;
  const uint32_t second = in_order ? c1 : c0;
  std::memcpy(palette[0], in_order ? end0 : end1, sizeof(palette[0]));
  std::memcpy(palette[1], in_order ? end1 : end0, sizeof(palette[1]));
  StoreU16(block, first);
  StoreU16(block + 2, second);
  if (colors == 3) {
    Lerp(palette[0], palette[1], float(half), palette[2]);
  } else {
    Lerp(palette[0], palette[1], float(Lfs(base, kThird)), palette[2]);
    Lerp(palette[0], palette[1], float(Lfs(base, kTwoThirds)), palette[3]);
  }

  // Each pixel's step along the palette's line.
  const double p0[3] = {palette[0][0], palette[0][1], palette[0][2]};  // f27, f26, f25
  const double dr = Fsubs(palette[1][0], p0[0]);                        // f13
  const double dg = Fsubs(palette[1][1], p0[1]);                        // f12
  const double db = Fsubs(palette[1][2], p0[2]);                        // f11
  const double last = IntToSingle(int64_t(colors - 1));                 // f24
  const double zero = Lfs(base, kZero);                                 // f28
  const double scale = c0 == c1 ? zero : Fdivs(last, Fmadds(dr, dr, Fmadds(db, db, Fmuls(dg, dg))));
  const double sr = Fmuls(scale, dr);  // f31
  const double sg = Fmuls(scale, dg);  // f30
  const double sb = Fmuls(scale, db);  // f29
  const uint32_t indices = colors == 3 ? kIndices3 : kIndices4;  // r28
  const double reference = Lfs(base, kAlphaReference);  // f7, through r1 + 256
  if (dither != 0) {
    std::memset(diffusion.error, 0, sizeof(diffusion.error));
  }
  uint32_t bits = 0;  // r29: each index shifted in from the top
  for (uint32_t i = 0; i < 16; i++) {
    if (colors == 3 && double(pixels[i][3]) < reference) {
      bits = (bits >> 2) | 0xC0000000u;
      continue;
    }
    double r = Fmuls(pixels[i][0], w[0]);  // f13
    double g = Fmuls(pixels[i][1], w[1]);  // f12
    double b = Fmuls(pixels[i][2], w[2]);  // f11
    if (dither != 0) {
      r = Fadds(diffusion.error[i][0], r);
      b = Fadds(diffusion.error[i][2], b);
      g = Fadds(diffusion.error[i][1], g);
    }
    const double t = Fmadds(Fsubs(r, p0[0]), sr, Fmadds(Fsubs(b, p0[2]), sb, Fmuls(Fsubs(g, p0[1]), sg)));
    uint32_t index;  // r10
    if (!(t > zero)) {
      index = 0;
    } else if (!(t < last)) {
      index = 1;
    } else {
      index = guest::LoadU32(base, (uint32_t(Fctiwz(Fadds(t, half))) << 2) + indices);
    }
    bits = (bits >> 2) | ((index & 3) << 30);
    if (dither != 0) {
      float entry[3];
      if (index < colors) {
        std::memcpy(entry, palette[index], sizeof(entry));
      } else {
        // A table word past the palette: the original reads its frame there
        // (never seen; the frame isn't kept here, so guest memory).
        static std::atomic<bool> noted{false};
        if (!noted.exchange(true, std::memory_order_relaxed)) {
          ITC_LOG_WARN("DxtColor::EncodeBlock: palette index {} of {} colors", index, colors);
        }
        const uint32_t at = frame + kPaletteInFrame + (index << 4);
        for (uint32_t c = 0; c < 3; c++) entry[c] = guest::LoadFloat(base, at + 4 * c);
      }
      const double e[3] = {Fmuls(Fsubs(r, entry[0]), weighted_alpha),
                           Fmuls(Fsubs(g, entry[1]), weighted_alpha),
                           Fmuls(Fsubs(b, entry[2]), weighted_alpha)};
      diffusion.Spread(i, e);
    }
  }
  // rotlwi 16: the index word's halfwords swapped, stored big-endian.
  const uint32_t word = (bits << 16) | (bits >> 16);
  block[4] = uint8_t(word >> 24);
  block[5] = uint8_t(word >> 16);
  block[6] = uint8_t(word >> 8);
  block[7] = uint8_t(word);
}

}  // namespace texture_compression
