// The light glows' billboard functions (src/hooks/billboards.cpp and the
// vertex builder's adapter in src/interpolation/blend_state.cpp) against the
// recompiled ones.

#include "harness.h"

#include "base/log.h"

namespace harness {

void Billboards(Context& h) {
  uint8_t* base = h.base;
  const uint32_t data = h.data;
  Random& r = h.random;

  // The material's set-up: over 0xCD and random fills.
  {
    Results results("BillboardMaterial::Init (sub_8209F5A0)");
    for (int i = 0; i < 20; i++) {
      uint32_t fill[16];
      for (uint32_t& word : fill) {
        word = uint32_t(r.Next());
      }
      Both(h, results,
           [&] {
             if (i % 2) {
               for (uint32_t k = 0; k < 16; k++) {
                 guest::StoreU32(base, data + 4 * k, fill[k]);
               }
             }
           },
           [&](PPCContext& t) { t.r3.u64 = data; }, __imp__sub_8209F5A0, sub_8209F5A0, data, 64,
           fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("BillboardMaterial::SetLights (sub_8209F5C8)");
    for (int i = 0; i < 40; i++) {
      uint32_t count = i < 10 ? 0 : (r.Below(3) ? r.Below(64) : uint32_t(r.Next()));
      uint32_t lights = uint32_t(r.Next());
      Both(h, results, [] {},
           [&](PPCContext& t) {
             t.r3.u64 = data;
             t.r4.u64 = count;
             t.r5.u64 = lights;
           },
           __imp__sub_8209F5C8, sub_8209F5C8, data, 64,
           fmt::format("count {} lights {:08X}", count, lights));
    }
    results.Report();
  }
}

}  // namespace harness
