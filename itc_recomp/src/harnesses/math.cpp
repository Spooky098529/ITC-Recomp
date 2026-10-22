// The C runtime's math functions (src/hooks/math.cpp) against the
// recompiled ones: f1 compared bit for bit, over the special values (signed
// zeros, infinities, NaNs with their payloads and signs, denormals), each
// function's own edges, and random doubles of every size.

#include "harness.h"

#include "base/log.h"

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
    Unary(h, "asin and acos's kernel, acos (sub_825A7FB0)", __imp__sub_825A7FB0, sub_825A7FB0,
          values, [](PPCContext& t) { t.r4.u64 = 1; });
    Unary(h, "asin and acos's kernel, asin (sub_825A7FB0)", __imp__sub_825A7FB0, sub_825A7FB0,
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
}

}  // namespace harness
