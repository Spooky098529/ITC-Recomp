// The gamma ramps (src/hooks/gamma.cpp, src/display/gamma.cpp) against the
// recompiled ones. The whole functions only meet the display's gamma type
// the kernel answers (2, BT.709), so their helpers are compared too, which
// covers the other types: the sRGB and BT.709 curves (sub_822B07B0,
// sub_822B0860) and pow (sub_825A8ED0) on the exponents the curves use and
// random ones.

#include "harness.h"

#include "display/gamma.h"

#include <bit>
#include <cmath>
#include <cstring>

namespace harness {
namespace {

constexpr uint32_t kRampBytes = 1536;  // three channels of 512 bytes

// The ramp functions return nothing: r3 is whatever they last left there.
void RecompiledRamp(PPCContext& ctx, uint8_t* base) {
  __imp__sub_822B0910(ctx, base);
  ctx.r3.u64 = 0;
}
void NativeRamp(PPCContext& ctx, uint8_t* base) {
  sub_822B0910(ctx, base);
  ctx.r3.u64 = 0;
}
void RecompiledPwl(PPCContext& ctx, uint8_t* base) {
  __imp__sub_822B0AB0(ctx, base);
  ctx.r3.u64 = 0;
}
void NativePwl(PPCContext& ctx, uint8_t* base) {
  sub_822B0AB0(ctx, base);
  ctx.r3.u64 = 0;
}

std::vector<uint8_t> FloatBytes(float f) {
  uint32_t bits;
  std::memcpy(&bits, &f, 4);
  return {uint8_t(bits >> 24), uint8_t(bits >> 16), uint8_t(bits >> 8), uint8_t(bits)};
}

float RandomFloat(Random& random, float low, float high) {
  return low + (high - low) * float(random.Below(1u << 24)) / float(1u << 24);
}

// A ramp's 768 16-bit words: random, the identity (i * 257, the game's
// usual), or random pairs for the piecewise-linear ramp.
std::vector<uint16_t> RampWords(Random& random, int kind) {
  std::vector<uint16_t> words(768);
  for (uint32_t i = 0; i < 768; i++) {
    if (kind == 0) {
      words[i] = uint16_t(random.Below(65536));
    } else if (kind == 1) {
      words[i] = uint16_t((i % 256) * 257);
    } else {
      // (base, delta) pairs climbing like the game's, deltas sometimes
      // overshooting 1023 (the clamp).
      uint32_t entry = (i / 2) % 128;
      words[i] = (i & 1) ? uint16_t(random.Below(4) == 0 ? random.Below(65536) : 512)
                         : uint16_t(entry * 512);
    }
  }
  return words;
}

void Ramps(Context& h, Results& results, GuestFunction recompiled, GuestFunction native,
           const char* name) {
  const uint32_t in = h.data, out = h.data + kRampBytes;
  for (int kind = 0; kind < 3; kind++) {
    for (int n = 0; n < (kind == 1 ? 1 : 8); n++) {
      std::vector<uint16_t> words = RampWords(h.random, kind);
      auto setup = [&] {
        for (uint32_t i = 0; i < 768; i++) {
          guest::StoreU16(h.base, in + 2 * i, words[i]);
        }
      };
      for (uint32_t inverse = 0; inverse < 2; inverse++) {
        for (int in_place = 0; in_place < 2; in_place++) {
          const uint32_t to = in_place ? in : out;
          Both(h, results, setup,
               [&](PPCContext& t) {
                 t.r3.u64 = to;
                 t.r4.u64 = in;
                 t.r5.u64 = inverse;
               },
               recompiled, native, to, kRampBytes,
               fmt::format("{} ramp {} #{}, inverse {}{}", name,
                           kind == 0 ? "random" : kind == 1 ? "identity" : "pairs", n, inverse,
                           in_place ? ", in place" : ""));
        }
      }
    }
  }
}

// A curve helper (f1 = x, r4 = to linear; returns f1) against ours.
void Curve(Context& h, Results& results, GuestFunction recompiled,
           float (*native)(float, bool), float x, bool to_linear) {
  PPCContext t = CallContext(h);
  t.f1.f64 = double(x);
  t.r4.u64 = to_linear;
  recompiled(t, h.base);
  results.Check(FloatBytes(float(t.f1.f64)), FloatBytes(native(x, to_linear)),
                fmt::format("x {} ({}), to linear {}", x, std::bit_cast<uint32_t>(x), to_linear));
}

void Curves(Context& h, Results& results, GuestFunction recompiled,
            float (*native)(float, bool)) {
  for (int to_linear = 0; to_linear < 2; to_linear++) {
    for (int i = 0; i < 1024; i++) {
      Curve(h, results, recompiled, native, float(i) * 0x1.00401p-10f, to_linear);
    }
    for (int n = 0; n < 2000; n++) {
      Curve(h, results, recompiled, native, RandomFloat(h.random, -0.1f, 1.2f), to_linear);
    }
    for (float x : {0.0f, -0.0f, 0.0031308f, 0.04045f, 0.018f, 0.0812f, 1.0f}) {
      Curve(h, results, recompiled, native, x, to_linear);
      Curve(h, results, recompiled, native, std::nextafter(x, 2.0f), to_linear);
      Curve(h, results, recompiled, native, std::nextafter(x, -1.0f), to_linear);
    }
  }
}

// pow (f1, f2; returns f1), compared as its callers here use it: rounded to
// float.
void Pow(Context& h, Results& results, float x, double y) {
  PPCContext t = CallContext(h);
  t.f1.f64 = double(x);
  t.f2.f64 = y;
  sub_825A8ED0(t, h.base);
  results.Check(FloatBytes(float(t.f1.f64)), FloatBytes(float(display::Pow(x, y))),
                fmt::format("{} ^ {}", x, y));
}

}  // namespace

void Gamma(Context& h) {
  {
    Results results("the 256-entry gamma ramp (sub_822B0910)");
    Ramps(h, results, RecompiledRamp, NativeRamp, "256-entry");
    results.Report();
  }
  {
    Results results("the piecewise-linear gamma ramp (sub_822B0AB0)");
    Ramps(h, results, RecompiledPwl, NativePwl, "piecewise-linear");
    results.Report();
  }
  {
    Results results("sRGB's curve (sub_822B07B0)");
    Curves(h, results, sub_822B07B0, display::Srgb);
    results.Report();
  }
  {
    Results results("BT.709's curve (sub_822B0860)");
    Curves(h, results, sub_822B0860, display::Bt709);
    results.Report();
  }
  {
    Results results("pow rounded to float (sub_825A8ED0)");
    // The curves' exponents (the power type's: a power and its inverse).
    const double exponents[] = {0x1.aaaaaap-2, 0x1.333334p+1, 0x1.ccccccp-2, 0x1.1c71c8p+1,
                                double(float(2.22222233)), double(1.0f / float(2.22222233))};
    for (double y : exponents) {
      for (int i = 0; i < 1024; i++) {
        Pow(h, results, float(i) * 0x1.00401p-10f, y);
      }
    }
    for (int n = 0; n < 4000; n++) {
      float power = RandomFloat(h.random, 0.3f, 4.0f);
      Pow(h, results, RandomFloat(h.random, 0.0f, 1.2f),
          n & 1 ? double(power) : double(1.0f / power));
    }
    results.Report();
  }
}

}  // namespace harness
