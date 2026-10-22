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

  // memset: every alignment of the destination, counts from none to a few
  // hundred bytes (the word loop's 16-byte steps and the rest), values with
  // bits above the low byte; the scratch area around it and r3 compared.
  {
    Results results("memset (sub_825A71D0)");
    std::vector<uint32_t> counts;
    for (uint32_t c = 0; c <= 40; c++) {
      counts.push_back(c);
    }
    for (uint32_t c : {63u, 64u, 65u, 255u, 256u, 257u, 1000u, 2048u, 3000u}) {
      counts.push_back(c);
    }
    for (int i = 0; i < 40; i++) {
      counts.push_back(h.random.Below(3500));
    }
    const uint64_t values[] = {0, 0xFF, 0x5A, 0x12345678ABull, 0xFFFFFFFFFFFFFF00ull};
    for (uint32_t count : counts) {
      for (uint32_t offset = 0; offset < 8; offset++) {
        const uint64_t value = values[(count + offset) % 5];
        Both(h, results, [] {},
             [&](PPCContext& t) {
               t.r3.u64 = h.data + 64 + offset;
               t.r4.u64 = value;
               t.r5.u64 = count;
             },
             __imp__sub_825A71D0, sub_825A71D0, h.data, kScratchBytes,
             fmt::format("count {} offset {} value {:X}", count, offset, value));
      }
    }
    results.Report();
  }
}

}  // namespace harness
