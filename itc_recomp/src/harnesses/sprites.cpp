// The race overlays' sprite functions (src/hooks/sprites.cpp) against the
// recompiled ones.

#include "harness.h"

#include "base/log.h"

#include <algorithm>
#include <bit>

namespace harness {

namespace {

// A sprite list (0x10A00 bytes) is larger than the scratch area: these
// cases use guest stack further down (the overlays' own lists live on the
// same thread's stack, deeper than the first Input::Update), which these
// leaf functions don't otherwise touch.
constexpr uint32_t kListArea = 0x10000;     // below the scratch area
constexpr uint32_t kListAreaBytes = 0x8200;  // what a reset reaches, and more

// One case: the area filled with `fill`, the function run on the sprite at
// `offset` into it, the whole area and r3 compared.
void ResetCase(Context& h, Results& results, GuestFunction recompiled, GuestFunction native,
               uint32_t offset, uint8_t fill, const std::string& what) {
  const uint32_t area = (h.data - kListArea) & ~0xFu;
  std::vector<uint8_t> result[2];
  for (int v = 0; v < 2; v++) {
    Fill(h.base, area, kListAreaBytes, fill);
    PPCContext t = CallContext(h);
    t.r3.u64 = area + offset;
    (v == 0 ? recompiled : native)(t, h.base);
    result[v] = Read(h.base, area, kListAreaBytes);
    for (int i = 0; i < 4; i++) {
      result[v].push_back(uint8_t(t.r3.u32 >> (24 - 8 * i)));
    }
  }
  results.Check(result[0], result[1], what);
}

}  // namespace

void Sprites(Context& h) {
  // The resets: aligned (as the overlays' stack lists are) and not (the
  // matrix's vector stores then land on the aligned rows), over different
  // fills.
  struct Function {
    const char* name;
    GuestFunction recompiled, native;
  };
  const Function resets[] = {
      {"SpriteList::Reset (sub_821298C0)", __imp__sub_821298C0, sub_821298C0},
      {"Sprite::Reset (sub_8211EB60)", __imp__sub_8211EB60, sub_8211EB60},
  };
  for (const Function& f : resets) {
    Results results(f.name);
    const uint8_t fills[] = {0xCD, 0x00, 0xFF};
    for (uint8_t fill : fills) {
      for (uint32_t offset = 0x40; offset < 0x60; offset += 4) {
        ResetCase(h, results, f.recompiled, f.native, offset, fill,
                  fmt::format("offset {:X} fill {:02X}", offset, fill));
      }
    }
    results.Report();
  }

  // The layer for a camera pass index: the four the game uses, the next
  // ones, the largest, and random ones.
  {
    Results results("Overlay::LayerFor (sub_82126630)");
    std::vector<uint32_t> indices = {0, 1, 2, 3, 4, 5, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF};
    for (int i = 0; i < 50; i++) {
      indices.push_back(uint32_t(h.random.Next()));
    }
    for (uint32_t index : indices) {
      Both(h, results, [] {},
           [&](PPCContext& t) {
             t.r3.u64 = h.data;
             t.r4.u64 = index;
           },
           __imp__sub_82126630, sub_82126630, h.data, 16, fmt::format("index {:X}", index));
    }
    results.Report();
  }

  // Vec4::Equal: equal vectors, one component off (each in turn), -0 against
  // 0, NaNs, infinities and denormals.
  {
    Results results("Vec4::Equal (sub_82123300)");
    const uint32_t specials[] = {0x00000000, 0x80000000, 0x7FC00000, 0x7F800001, 0x7F800000,
                                 0xFF800000, 0x00000001, 0x3F800000, 0xBF800000, 0x007FFFFF};
    Random& r = h.random;
    for (int i = 0; i < 2000; i++) {
      uint32_t a[4], b[4];
      for (int k = 0; k < 4; k++) {
        a[k] = r.Below(4) == 0 ? specials[r.Below(10)] : uint32_t(r.Next());
        b[k] = a[k];
      }
      int kind = r.Below(4);
      if (kind == 1) {
        b[r.Below(4)] ^= 1u << r.Below(32);
      } else if (kind == 2) {
        b[r.Below(4)] = specials[r.Below(10)];
      } else if (kind == 3) {
        for (int k = 0; k < 4; k++) {
          b[k] = r.Below(2) ? specials[r.Below(10)] : a[k];
        }
      }
      Both(h, results,
           [&] {
             for (int k = 0; k < 4; k++) {
               guest::StoreU32(h.base, h.data + 4 * k, a[k]);
               guest::StoreU32(h.base, h.data + 16 + 4 * k, b[k]);
             }
           },
           [&](PPCContext& t) {
             t.r3.u64 = h.data;
             t.r4.u64 = h.data + 16;
           },
           __imp__sub_82123300, sub_82123300, h.data, 32,
           fmt::format("{:08X} {:08X} {:08X} {:08X} against {:08X} {:08X} {:08X} {:08X}", a[0], a[1],
                       a[2], a[3], b[0], b[1], b[2], b[3]));
    }
    results.Report();
  }

  // Trail::TurnSide: directions in the horizontal plane (as a trail's), the
  // same direction, opposite ones, parallel ones (a zero cross product, of
  // either sign), random words, and tiny components whose products are
  // denormal.
  {
    Results results("Trail::TurnSide (sub_82129940)");
    Random& r = h.random;
    auto uniform = [&](float low, float high) {
      return low + (high - low) * float(r.Below(1u << 24)) / float(1u << 24);
    };
    auto bits = [](float f) { return std::bit_cast<uint32_t>(f); };
    for (int i = 0; i < 3000; i++) {
      uint32_t a[4], b[4];
      const int kind = r.Below(6);
      for (int k = 0; k < 4; k++) {
        a[k] = bits(uniform(-1, 1));
        b[k] = bits(uniform(-1, 1));
      }
      a[1] = b[1] = bits(0.0f);
      if (kind == 1) {
        std::copy(a, a + 4, b);
      } else if (kind == 2) {  // parallel or opposite
        const float scale = r.Below(2) ? -uniform(0, 3) : uniform(0, 3);
        b[0] = bits(std::bit_cast<float>(a[0]) * scale);
        b[2] = bits(std::bit_cast<float>(a[2]) * scale);
      } else if (kind == 3) {
        for (int k = 0; k < 4; k++) {
          a[k] = uint32_t(r.Next());
          b[k] = uint32_t(r.Next());
        }
      } else if (kind == 4) {  // denormal products
        a[0] = bits(uniform(-1e-20f, 1e-20f));
        b[2] = bits(uniform(-1e-20f, 1e-20f));
      } else if (kind == 5) {
        a[r.Below(4)] = 0x80000000;  // -0 against 0
      }
      Both(h, results, [] {},
           [&](PPCContext& t) {
             t.r3.u64 = (uint64_t(a[0]) << 32) | a[1];
             t.r4.u64 = (uint64_t(a[2]) << 32) | a[3];
             t.r5.u64 = (uint64_t(b[0]) << 32) | b[1];
             t.r6.u64 = (uint64_t(b[2]) << 32) | b[3];
           },
           __imp__sub_82129940, sub_82129940, h.data, 0,
           fmt::format("{:08X} {:08X} {:08X} {:08X} to {:08X} {:08X} {:08X} {:08X}", a[0], a[1],
                       a[2], a[3], b[0], b[1], b[2], b[3]));
    }
    results.Report();
  }
}

}  // namespace harness
