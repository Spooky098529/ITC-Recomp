// The C runtime's math functions (src/hooks/math.cpp) against the
// recompiled ones: f1 compared bit for bit, over the special values (signed
// zeros, infinities, NaNs with their payloads and signs, denormals), each
// function's own edges, and random doubles of every size.

#include "harness.h"

#include "base/log.h"
#include "c_runtime/math.h"

#include <xmmintrin.h>

#include <bit>
#include <cmath>
#include <functional>
#include <limits>

namespace harness {

namespace {

double D(uint64_t bits) {
  return std::bit_cast<double>(bits);
}

// Values every function gets: the special ones and their neighbors.
std::vector<double> SpecialValues() {
  std::vector<double> v;
  const uint64_t bits[] = {
      0x0000000000000000, 0x8000000000000000,  // +-0
      0x7FF0000000000000, 0xFFF0000000000000,  // +-infinity
      0x7FF8000000000000, 0xFFF8000000000000,  // quiet NaNs
      0x7FF8000000012345, 0xFFFC000000000001,  // with payloads
      0x7FF0000000000001, 0xFFF4000000000000,  // signaling NaNs
      0x0000000000000001, 0x8000000000000001,  // the smallest denormals
      0x000FFFFFFFFFFFFF, 0x800FFFFFFFFFFFFF,  // the largest denormals
      0x0010000000000000, 0x8010000000000000,  // the smallest normals
      0x7FEFFFFFFFFFFFFF, 0xFFEFFFFFFFFFFFFF,  // the largest
  };
  for (uint64_t b : bits) {
    v.push_back(D(b));
  }
  return v;
}

// x and the doubles either side of it, both signs.
void AddAround(std::vector<double>& v, double x) {
  for (double s : {x, -x}) {
    v.push_back(s);
    v.push_back(std::nextafter(s, std::numeric_limits<double>::infinity()));
    v.push_back(std::nextafter(s, -std::numeric_limits<double>::infinity()));
  }
}

// Random doubles: uniform in [-range, range], random bit patterns, and
// random exponents (every size from denormal to huge).
void AddRandom(Context& h, std::vector<double>& v, double range, int count) {
  for (int i = 0; i < count; i++) {
    const double unit = double(h.random.Next() >> 11) / double(1ull << 53);
    v.push_back((unit * 2.0 - 1.0) * range);
  }
  for (int i = 0; i < count / 4; i++) {
    v.push_back(D(h.random.Next()));
  }
  for (int i = 0; i < count / 4; i++) {
    const uint64_t r = h.random.Next();
    const uint64_t exponent = (r >> 52) % 2047;  // never infinity or NaN
    v.push_back(D((r & 0x800FFFFFFFFFFFFFull) | (exponent << 52)));
  }
}

// One unary function over the values: f1 = x (and `args` for the rest),
// f1 and r3 compared.
void Unary(Context& h, const char* name, GuestFunction recompiled, GuestFunction native,
           const std::vector<double>& values,
           const std::function<void(PPCContext&)>& args = [](PPCContext&) {}) {
  Results results(name);
  for (double x : values) {
    std::vector<uint8_t> result[2];
    for (int v = 0; v < 2; v++) {
      PPCContext t = CallContext(h);
      t.f1.f64 = x;
      args(t);
      (v == 0 ? recompiled : native)(t, h.base);
      for (int i = 0; i < 8; i++) {
        result[v].push_back(uint8_t(t.f1.u64 >> (56 - 8 * i)));
      }
      for (int i = 0; i < 4; i++) {
        result[v].push_back(uint8_t(t.r3.u32 >> (24 - 8 * i)));
      }
    }
    results.Check(result[0], result[1],
                  fmt::format("x {:016X}", std::bit_cast<uint64_t>(x)));
  }
  results.Report();
}

// atan2 over pairs: f1 = y, f2 = x; f1 and r3 compared.
void Binary(Context& h, const char* name, GuestFunction recompiled, GuestFunction native,
            const std::vector<std::pair<double, double>>& pairs) {
  Results results(name);
  for (const auto& [y, x] : pairs) {
    std::vector<uint8_t> result[2];
    for (int v = 0; v < 2; v++) {
      PPCContext t = CallContext(h);
      t.f1.f64 = y;
      t.f2.f64 = x;
      (v == 0 ? recompiled : native)(t, h.base);
      for (int i = 0; i < 8; i++) {
        result[v].push_back(uint8_t(t.f1.u64 >> (56 - 8 * i)));
      }
      for (int i = 0; i < 4; i++) {
        result[v].push_back(uint8_t(t.r3.u32 >> (24 - 8 * i)));
      }
    }
    results.Check(result[0], result[1],
                  fmt::format("y {:016X} x {:016X}", std::bit_cast<uint64_t>(y),
                              std::bit_cast<uint64_t>(x)));
  }
  results.Report();
}

// A general case runner: `args(i, t)` sets case i's registers, `out(t, bytes)`
// appends what's compared; the scratch area is filled before each call.
template <typename Args, typename Out, typename What>
void Cases(Context& h, const char* name, GuestFunction recompiled, GuestFunction native,
           size_t count, Args args, Out out, What what) {
  Results results(name);
  for (size_t i = 0; i < count; i++) {
    std::vector<uint8_t> result[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, 16);
      PPCContext t = CallContext(h);
      args(i, t);
      (v == 0 ? recompiled : native)(t, h.base);
      out(t, result[v]);
    }
    results.Check(result[0], result[1], what(i));
  }
  results.Report();
}

void Append(std::vector<uint8_t>& bytes, uint64_t value) {
  for (int i = 0; i < 8; i++) {
    bytes.push_back(uint8_t(value >> (56 - 8 * i)));
  }
}

std::string Bits(double x) {
  return fmt::format("{:016X}", std::bit_cast<uint64_t>(x));
}

// The register steps of the functions whose adapters went once their
// callers were native (docs/loops/b7-loop.md, "Removing an adapter"): the
// cases run the plain C++ through these, against the recompiled versions.
void NativeAsinAcos(PPCContext& ctx, uint8_t*) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = c_runtime::AsinAcos(ctx.f1.f64, ctx.r4.s32 != 0);
}

void NativeSetExp(PPCContext& ctx, uint8_t*) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = c_runtime::SetExp(ctx.f1.f64, ctx.r4.s32);
}

void NativeGetExp(PPCContext& ctx, uint8_t*) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.s64 = c_runtime::GetExp(ctx.f1.f64);
}

void NativeDecomp(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  int32_t exponent;
  ctx.f1.f64 = c_runtime::Decomp(ctx.f1.f64, &exponent);
  guest::StoreU32(base, ctx.r4.u32, uint32_t(exponent));
}

void NativeIntType(PPCContext& ctx, uint8_t*) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.s64 = c_runtime::IntType(ctx.f1.f64);
}

void NativeRint(PPCContext& ctx, uint8_t*) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = c_runtime::Rint(ctx.f1.f64);
}

}  // namespace

void Math(Context& h) {
  // asin and acos: their domain [-1, 1], the 0.5 where the kernel switches
  // to the square root, and outside it.
  {
    std::vector<double> values = SpecialValues();
    for (double x : {0.5, 1.0, 0.25, 1e-8, 1e-300, 0.75, 0.999999, 2.0, 1e10}) {
      AddAround(values, x);
    }
    AddRandom(h, values, 1.0, 2000);
    AddRandom(h, values, 1.5, 200);
    Unary(h, "acos (sub_825A8170)", __imp__sub_825A8170, sub_825A8170, values);
    Unary(h, "asin and acos's kernel, acos (sub_825A7FB0)", __imp__sub_825A7FB0, NativeAsinAcos,
          values, [](PPCContext& t) { t.r4.u64 = 1; });
    Unary(h, "asin and acos's kernel, asin (sub_825A7FB0)", __imp__sub_825A7FB0, NativeAsinAcos,
          values, [](PPCContext& t) { t.r4.u64 = 0; });
    Unary(h, "asin (sub_825A80A8)", __imp__sub_825A80A8, sub_825A80A8, values);
  }

  // sin and cos: multiples of pi/2 (where k changes and r is near 0), the
  // largest argument (2.2e8) and beyond, and angles of every size.
  {
    std::vector<double> values = SpecialValues();
    const double half_pi = 1.5707963267948966;
    for (int i = 1; i <= 12; i++) {
      AddAround(values, half_pi * i);
    }
    for (double x : {220000000.0, 1e9, 1e-8, 1e-300, 0.5, 1.0, 100.0, 1e6, 1e15, 1e19}) {
      AddAround(values, x);
    }
    AddRandom(h, values, 8.0, 2000);
    AddRandom(h, values, 1000.0, 500);
    AddRandom(h, values, 3e8, 300);
    Unary(h, "sin (sub_825A88A0)", __imp__sub_825A88A0, sub_825A88A0, values);
    Unary(h, "cos (sub_825A8978)", __imp__sub_825A8978, sub_825A8978, values);
  }

  // atan and atan2: 1.0 (where 1/|x| starts), 2 - sqrt(3) (where the
  // reduction starts), the largest reduced argument, every size; atan2 over
  // every pair of the special values (the zeros' signs pick the quadrant),
  // equal and near-equal magnitudes, and random pairs.
  {
    std::vector<double> values = SpecialValues();
    for (double x : {1.0, 0.2679491924311227, 3.7320508075688772, 4.49423283715579e+307, 1e-8,
                     0.5, 2.0, 1e10}) {
      AddAround(values, x);
    }
    AddRandom(h, values, 1.0, 1000);
    AddRandom(h, values, 10.0, 1000);
    AddRandom(h, values, 1e6, 200);
    Unary(h, "atan (sub_825A8688)", __imp__sub_825A8688, sub_825A8688, values);

    std::vector<std::pair<double, double>> pairs;
    const std::vector<double> specials = SpecialValues();
    std::vector<double> edges = specials;
    for (double x : {1.0, -1.0, 0.5, -3.0, 1e-8}) {
      edges.push_back(x);
    }
    for (double y : edges) {
      for (double x : edges) {
        pairs.push_back({y, x});
      }
    }
    std::vector<double> random;
    AddRandom(h, random, 100.0, 2000);
    for (size_t i = 0; i + 1 < random.size(); i += 2) {
      pairs.push_back({random[i], random[i + 1]});
      pairs.push_back({random[i], random[i]});
      pairs.push_back({random[i], -std::nextafter(random[i], 0.0)});
    }
    Binary(h, "atan2 (sub_825A8770)", __imp__sub_825A8770, sub_825A8770, pairs);
  }
  // pow and what it calls: copysign, _set_exp, _get_exp, _decomp, the
  // integer test and log, each against its recompiled version. pow's
  // recompiled version calls the overrides of copysign and log (the native
  // ones), so their own cases are their check; f1 only for pow (its
  // recompiled version leaves the integer test's result in r3).
  {
    // Values: the special ones, integers and halves, the edges each
    // function tests, and random doubles of every size.
    std::vector<double> values = SpecialValues();
    for (double x : {1.0, 2.0, 3.0, 0.5, 1.5, 0.7071067811865476, 4503599627370496.0,
                     9007199254740992.0, 9223372036854775808.0, 1e18, 1e20, 1e300, 1e-300,
                     127.0, 128.0, 129.0, 2.2, 1.0 / 2.2, 10.0}) {
      AddAround(values, x);
    }
    AddRandom(h, values, 2.0, 400);
    AddRandom(h, values, 1e6, 200);
    for (int i = -40; i <= 40; i++) {
      values.push_back(i * 0.5);
    }

    Cases(h, "log (sub_825A9C00)", __imp__sub_825A9C00, sub_825A9C00, values.size(),
          [&](size_t i, PPCContext& t) { t.f1.f64 = values[i]; },
          [](PPCContext& t, std::vector<uint8_t>& b) { Append(b, t.f1.u64); },
          [&](size_t i) { return "x " + Bits(values[i]); });
    Cases(h, "pow's integer test (sub_825A8CE0)", __imp__sub_825A8CE0, NativeIntType,
          values.size(), [&](size_t i, PPCContext& t) { t.f1.f64 = values[i]; },
          [](PPCContext& t, std::vector<uint8_t>& b) { Append(b, t.r3.u64); },
          [&](size_t i) { return "y " + Bits(values[i]); });
    Cases(h, "_get_exp (sub_825AFEA8)", __imp__sub_825AFEA8, NativeGetExp, values.size(),
          [&](size_t i, PPCContext& t) { t.f1.f64 = values[i]; },
          [](PPCContext& t, std::vector<uint8_t>& b) { Append(b, t.r3.u64); },
          [&](size_t i) { return "x " + Bits(values[i]); });
    Cases(h, "_decomp (sub_825AFF80)", __imp__sub_825AFF80, NativeDecomp, values.size(),
          [&](size_t i, PPCContext& t) {
            t.f1.f64 = values[i];
            t.r4.u64 = h.data;
          },
          [&](PPCContext& t, std::vector<uint8_t>& b) {
            Append(b, t.f1.u64);
            const auto out = Read(h.base, h.data, 8);
            b.insert(b.end(), out.begin(), out.end());
          },
          [&](size_t i) { return "x " + Bits(values[i]); });

    // _set_exp: exponents over the whole range and beyond (into the sign).
    std::vector<std::pair<double, int64_t>> exps;
    for (double x : values) {
      for (int64_t e : {-1100, -1074, -1022, -1021, 0, 1, 1024, 1025, 1026, 3000}) {
        exps.push_back({x, e});
      }
    }
    for (int i = 0; i < 200; i++) {
      exps.push_back({values[h.random.Below(uint32_t(values.size()))], int64_t(h.random.Next())});
    }
    Cases(h, "_set_exp (sub_825AFE80)", __imp__sub_825AFE80, NativeSetExp, exps.size(),
          [&](size_t i, PPCContext& t) {
            t.f1.f64 = exps[i].first;
            t.r4.s64 = exps[i].second;
          },
          [](PPCContext& t, std::vector<uint8_t>& b) { Append(b, t.f1.u64); },
          [&](size_t i) { return fmt::format("x {} e {}", Bits(exps[i].first), exps[i].second); });

    // copysign and pow over pairs: every pair of the edge values, then
    // random ones (bases near 1, integer exponents, results near overflow
    // and underflow).
    std::vector<double> edges = SpecialValues();
    for (double x : {1.0, -1.0, 2.0, -2.0, 0.5, -0.5, 3.0, -3.0, 10.0, -10.0, 2.2, 1.0 / 2.2,
                     0.9999999, 1.0000001, 127.0, 128.0, 129.0, -127.0, 1e20, 1.0000000000000002e20,
                     -1e20, 1e21, 9223372036854775808.0, 9007199254740991.0, 1e-300, 1e300}) {
      edges.push_back(x);
    }
    std::vector<std::pair<double, double>> pairs;
    for (double x : edges) {
      for (double y : edges) {
        pairs.push_back({x, y});
      }
    }
    auto unit = [&] { return double(h.random.Next() >> 11) / double(1ull << 53); };
    for (int i = 0; i < 1500; i++) {
      pairs.push_back({unit() * 10.0, (unit() * 2.0 - 1.0) * 10.0});
      pairs.push_back({0.9 + unit() * 0.2, (unit() * 2.0 - 1.0) * 100.0});
      pairs.push_back({(unit() * 2.0 - 1.0) * 100.0, double(int(h.random.Below(260)) - 130)});
      pairs.push_back({unit() * 4.0, unit() * 3.0});
    }
    for (int i = 0; i < 300; i++) {
      pairs.push_back({1.0 + unit() * 9.0, 300.0 + unit() * 30.0});
      pairs.push_back({1.0 + unit() * 9.0, -300.0 - unit() * 30.0});
      pairs.push_back({2.0, 1020.0 + unit() * 8.0});
      pairs.push_back({2.0, -1080.0 + unit() * 20.0});
      pairs.push_back({D(h.random.Next()), D(h.random.Next())});
    }
    Cases(h, "copysign (sub_825AB1C8)", __imp__sub_825AB1C8, sub_825AB1C8, pairs.size(),
          [&](size_t i, PPCContext& t) {
            t.f1.f64 = pairs[i].first;
            t.f2.f64 = pairs[i].second;
          },
          [](PPCContext& t, std::vector<uint8_t>& b) { Append(b, t.f1.u64); },
          [&](size_t i) { return Bits(pairs[i].first) + " " + Bits(pairs[i].second); });
    Cases(h, "pow (sub_825A8ED0)", __imp__sub_825A8ED0, sub_825A8ED0, pairs.size(),
          [&](size_t i, PPCContext& t) {
            t.f1.f64 = pairs[i].first;
            t.f2.f64 = pairs[i].second;
          },
          [](PPCContext& t, std::vector<uint8_t>& b) { Append(b, t.f1.u64); },
          [&](size_t i) { return "x " + Bits(pairs[i].first) + " y " + Bits(pairs[i].second); });
  }

  // floor and ceil: integers and their neighbors, halves, 2^52, 2^63 and
  // the 1e18 bound, every size.
  {
    std::vector<double> values = SpecialValues();
    for (double x : {0.5, 1.0, 1.5, 2.0, 2.5, 1e-8, 0.9999999999999999, 4503599627370496.0,
                     9007199254740992.0, 1e18, 9.2233720368547758e18, 1e19, 1e300}) {
      AddAround(values, x);
    }
    for (int i = -50; i <= 50; i++) {
      AddAround(values, i * 0.25);
    }
    AddRandom(h, values, 10.0, 1000);
    AddRandom(h, values, 1e20, 300);
    Unary(h, "floor (sub_825A8CA0)", __imp__sub_825A8CA0, sub_825A8CA0, values);
    Unary(h, "ceil (sub_825AA0B8)", __imp__sub_825AA0B8, sub_825AA0B8, values);
  }

  // log10, __ultod and _fpclass.
  {
    std::vector<double> values = SpecialValues();
    for (double x : {1.0, 10.0, 100.0, 0.1, 0.5, 2.0, 1e-300, 1e300}) {
      AddAround(values, x);
    }
    AddRandom(h, values, 1000.0, 500);
    Unary(h, "log10 (sub_825A9D80)", __imp__sub_825A9D80, sub_825A9D80, values);
    Cases(h, "_fpclass (sub_825AB268)", __imp__sub_825AB268, sub_825AB268, values.size(),
          [&](size_t i, PPCContext& t) { t.f1.f64 = values[i]; },
          [](PPCContext& t, std::vector<uint8_t>& b) { Append(b, t.r3.u32); },
          [&](size_t i) { return "x " + Bits(values[i]); });

    // Integers: zero, powers of two and their neighbors, every bit length,
    // values with more bits than a mantissa (where truncation shows).
    std::vector<uint64_t> integers = {0, 1, 2, 3, ~0ull, 1ull << 63, (1ull << 63) - 1,
                                      (1ull << 53) + 1, (1ull << 54) + 3, 0x8000000000000401ull};
    for (int b = 0; b < 64; b++) {
      integers.push_back(1ull << b);
      integers.push_back((1ull << b) + 1);
      integers.push_back((1ull << b) - 1);
    }
    for (int i = 0; i < 2000; i++) {
      const uint64_t r = h.random.Next();
      integers.push_back(r);
      integers.push_back(r >> h.random.Below(64));
    }
    Cases(h, "__ultod (sub_825A9DB0)", __imp__sub_825A9DB0, sub_825A9DB0, integers.size(),
          [&](size_t i, PPCContext& t) { t.r3.u64 = integers[i]; },
          [](PPCContext& t, std::vector<uint8_t>& b) {
            Append(b, t.f1.u64);
            Append(b, t.r3.u64);
          },
          [&](size_t i) { return fmt::format("r3 {:016X}", integers[i]); });
  }

  // rint, in each of the four rounding modes (MXCSR set before each call
  // and put back after): zeros, halves (where the modes differ), 2^52 and
  // its neighbors, every size. _ctrlfp: new bits and masks (tan's and
  // _clearfp's among them) from each rounding mode; r3, the MXCSR after
  // (rounding, flush-to-zero, flags) and the context's copy of it compared.
  // _abstract_sw over every word up to 0xFFF and random registers; abs over
  // sign-extended and zero-extended negatives, INT_MIN and random registers.
  {
    const uint32_t saved = _mm_getcsr();
    auto set_mode = [&](PPCContext& t, uint32_t mode) {
      _mm_setcsr((saved & ~0x603Fu) | (mode << 13));
      t.fpscr.csr = _mm_getcsr();
    };
    std::vector<double> values = SpecialValues();
    for (double x : {0.5, 1.0, 1.5, 2.5, 3.5, 0.49999999999999994, 4503599627370496.0,
                     2251799813685248.5, 9007199254740992.0, 1e-300, 1e300}) {
      AddAround(values, x);
    }
    for (int i = -40; i <= 40; i++) {
      AddAround(values, i * 0.25);
    }
    AddRandom(h, values, 10.0, 500);
    AddRandom(h, values, 1e16, 300);
    for (uint32_t mode = 0; mode < 4; mode++) {
      Cases(h, fmt::format("rint, rounding mode {} (sub_825B0B30)", mode).c_str(),
            __imp__sub_825B0B30, NativeRint, values.size(),
            [&](size_t i, PPCContext& t) {
              t.f1.f64 = values[i];
              set_mode(t, mode);
            },
            [&](PPCContext& t, std::vector<uint8_t>& b) {
              Append(b, t.f1.u64);
              _mm_setcsr(saved);
            },
            [&](size_t i) { return "x " + Bits(values[i]); });
    }

    std::vector<std::pair<uint64_t, uint64_t>> words;
    const uint64_t masks[] = {0xC007FEFF, 0xFFF80700, 0, 0xFFFFFFFF, 3, 0xF8, ~0ull};
    for (uint64_t mask : masks) {
      for (uint64_t bits : {0ull, 1ull, 2ull, 3ull, 0xF8ull, 0xFFull, 0xFFFFFFFFull, ~0ull}) {
        words.push_back({bits, mask});
      }
    }
    for (int i = 0; i < 100; i++) {
      words.push_back({h.random.Next(), h.random.Next()});
      words.push_back({h.random.Next() & 0xFF, masks[i % 7]});
    }
    for (uint32_t mode = 0; mode < 4; mode++) {
      Cases(h, fmt::format("_ctrlfp, rounding mode {} (sub_825B0B80)", mode).c_str(),
            __imp__sub_825B0B80, sub_825B0B80, words.size(),
            [&](size_t i, PPCContext& t) {
              t.r3.u64 = words[i].first;
              t.r4.u64 = words[i].second;
              set_mode(t, mode);
            },
            [&](PPCContext& t, std::vector<uint8_t>& b) {
              Append(b, t.r3.u64);
              Append(b, _mm_getcsr());
              Append(b, t.fpscr.csr);
              _mm_setcsr(saved);
            },
            [&](size_t i) {
              return fmt::format("r3 {:X} r4 {:X}", words[i].first, words[i].second);
            });
    }

    std::vector<uint64_t> registers;
    for (uint64_t w = 0; w <= 0xFFF; w++) {
      registers.push_back(w);
    }
    for (int i = 0; i < 200; i++) {
      registers.push_back(h.random.Next());
    }
    Cases(h, "_abstract_sw (sub_825AB380)", __imp__sub_825AB380, sub_825AB380, registers.size(),
          [&](size_t i, PPCContext& t) { t.r3.u64 = registers[i]; },
          [](PPCContext& t, std::vector<uint8_t>& b) { Append(b, t.r3.u64); },
          [&](size_t i) { return fmt::format("r3 {:X}", registers[i]); });

    std::vector<uint64_t> ints = {0, 1, 2, ~0ull, 0xFFFFFFFFull, 0x80000000ull,
                                  0xFFFFFFFF80000000ull, 0x7FFFFFFFull, 0xFFFFFFFF80000001ull,
                                  0x123456789ABCDEF0ull, 0x00000001FFFFFFFEull};
    for (int i = 0; i < 300; i++) {
      const uint64_t r = h.random.Next();
      ints.push_back(r);
      ints.push_back(uint64_t(int64_t(int32_t(uint32_t(r)))));
      ints.push_back(uint32_t(r));
    }
    Cases(h, "abs (sub_825AC870)", __imp__sub_825AC870, sub_825AC870, ints.size(),
          [&](size_t i, PPCContext& t) { t.r3.u64 = ints[i]; },
          [](PPCContext& t, std::vector<uint8_t>& b) { Append(b, t.r3.u64); },
          [&](size_t i) { return fmt::format("r3 {:X}", ints[i]); });
    _mm_setcsr(saved);
  }

  // tan, ldexp and _clearfp, which set the rounding mode through _ctrlfp and
  // put it back: from each of the four modes (MXCSR set before each call and
  // put back after), f1, r3, the MXCSR after and the context's copy of it
  // compared. Inputs where the original would go to its _except paths
  // (never seen in play; the native adapters run the original there) are
  // left out: they reach the C runtime's exception handling.
  {
    const uint32_t saved = _mm_getcsr();
    auto set_mode = [&](PPCContext& t, uint32_t mode) {
      _mm_setcsr((saved & ~0x603Fu) | (mode << 13));
      t.fpscr.csr = _mm_getcsr();
    };
    auto out = [&](PPCContext& t, std::vector<uint8_t>& b) {
      Append(b, t.f1.u64);
      Append(b, t.r3.u64);
      Append(b, t.r4.u64);
      Append(b, _mm_getcsr());
      Append(b, t.fpscr.csr);
      _mm_setcsr(saved);
    };

    // tan: multiples of pi/4 (where k changes, and the poles), the bound
    // below which r is used as it is, the largest argument (2.98e8), every
    // size.
    std::vector<double> candidates = SpecialValues();
    const double quarter_pi = 0.7853981633974483;
    for (int i = 1; i <= 16; i++) {
      AddAround(candidates, quarter_pi * i);
    }
    for (double x : {1.0536712127723509e-08, 298156826.0, 2.9815682e8, 1e-8, 1e-6, 0.5, 1.0,
                     100.0, 1e6, 1e8}) {
      AddAround(candidates, x);
    }
    AddRandom(h, candidates, 2.0, 2000);
    AddRandom(h, candidates, 1000.0, 500);
    AddRandom(h, candidates, 3e8, 300);
    std::vector<double> values;
    for (double x : candidates) {
      double result;
      if (c_runtime::Tan(x, &result)) {
        values.push_back(x);
      }
    }
    for (uint32_t mode = 0; mode < 4; mode++) {
      Cases(h, fmt::format("tan, rounding mode {} (sub_825A8A50)", mode).c_str(),
            __imp__sub_825A8A50, sub_825A8A50, values.size(),
            [&](size_t i, PPCContext& t) {
              t.f1.f64 = values[i];
              set_mode(t, mode);
            },
            out, [&](size_t i) { return "x " + Bits(values[i]); });
    }

    // ldexp: every pair of edge values and exponents (the result's range,
    // -1021 to 1024, and the int's ends), then random ones.
    std::vector<double> xs = SpecialValues();
    for (double x : {1.0, -1.0, 0.5, 3.0, 1e-300, 1e300, 1.5e-310, 0.75}) {
      xs.push_back(x);
    }
    AddRandom(h, xs, 100.0, 100);
    std::vector<std::pair<double, int64_t>> pairs;
    const int64_t ns[] = {0, 1, -1, 10, -10, 1000, -1000, 1023, 1024, 1025, -1021, -1022,
                          -1074, -1075, 2046, -2046, 2147483647, -2147483647 - 1,
                          0x100000001ll, -0x100000000ll};
    for (double x : xs) {
      for (int64_t n : ns) {
        pairs.push_back({x, n});
      }
    }
    for (int i = 0; i < 2000; i++) {
      const double x = xs[h.random.Below(uint32_t(xs.size()))];
      pairs.push_back({x, int64_t(h.random.Below(4200)) - 2100});
      pairs.push_back({x, int64_t(h.random.Next())});
    }
    std::vector<std::pair<double, int64_t>> in_range;
    for (const auto& [x, n] : pairs) {
      double result;
      if (c_runtime::Ldexp(x, int32_t(n), &result)) {
        in_range.push_back({x, n});
      }
    }
    for (uint32_t mode = 0; mode < 4; mode++) {
      Cases(h, fmt::format("ldexp, rounding mode {} (sub_825AAD20)", mode).c_str(),
            __imp__sub_825AAD20, sub_825AAD20, in_range.size(),
            [&](size_t i, PPCContext& t) {
              t.f1.f64 = in_range[i].first;
              t.r4.s64 = in_range[i].second;
              set_mode(t, mode);
            },
            out,
            [&](size_t i) {
              return fmt::format("x {} n {}", Bits(in_range[i].first), in_range[i].second);
            });
    }

    // _clearfp: random registers in, from each mode.
    for (uint32_t mode = 0; mode < 4; mode++) {
      Cases(h, fmt::format("_clearfp, rounding mode {} (sub_825AB568)", mode).c_str(),
            __imp__sub_825AB568, sub_825AB568, 64,
            [&](size_t i, PPCContext& t) {
              t.r3.u64 = i * 0x9E3779B97F4A7C15ull;
              t.r4.u64 = i;
              set_mode(t, mode);
            },
            [&](PPCContext& t, std::vector<uint8_t>& b) {
              Append(b, t.r3.u64);
              Append(b, _mm_getcsr());
              Append(b, t.fpscr.csr);
              _mm_setcsr(saved);
            },
            [&](size_t i) { return fmt::format("case {}", i); });
    }
    _mm_setcsr(saved);
  }
}

}  // namespace harness
