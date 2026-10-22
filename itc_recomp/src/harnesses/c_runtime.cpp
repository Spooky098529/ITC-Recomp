// The C runtime's helpers (src/hooks/c_runtime.cpp) against the recompiled
// ones.

#include "harness.h"

#include "base/log.h"

namespace harness {

namespace {

// Appends a register's 64 bits to a result.
void Append(std::vector<uint8_t>& out, uint64_t value) {
  for (int i = 0; i < 8; i++) {
    out.push_back(uint8_t(value >> (56 - 8 * i)));
  }
}

}  // namespace

void CRuntime(Context& h) {
  // The stack probe: frame sizes below, at and above whole pages (the
  // callers' are 4 KB to 64 KB). It only reads, so the scratch area and the
  // registers the callers use after it (r1, r12) and r3 must be the same.
  {
    Results results("__chkstk (sub_825A9934)");
    std::vector<uint32_t> sizes = {0,     1,     16,    4095,  4096,  4097,  8191,
                                   8192,  8193,  12288, 16384, 20000, 32768, 65536};
    for (int i = 0; i < 20; i++) {
      sizes.push_back(h.random.Below(65536));
    }
    for (uint32_t size : sizes) {
      std::vector<uint8_t> result[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        PPCContext t = CallContext(h);
        t.r12.s64 = -int64_t(size);
        t.r3.u64 = 0x12345678;
        (v == 0 ? __imp__sub_825A9934 : sub_825A9934)(t, h.base);
        result[v] = Read(h.base, h.data, kScratchBytes);
        Append(result[v], t.r1.u64);
        Append(result[v], t.r12.u64);
        Append(result[v], t.r3.u64);
      }
      results.Check(result[0], result[1], fmt::format("size {}", size));
    }
    results.Report();
  }
}

}  // namespace harness
