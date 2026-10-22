// The race overlays' sprite functions (src/hooks/sprites.cpp) against the
// recompiled ones.

#include "harness.h"

#include "base/log.h"

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
}

}  // namespace harness
