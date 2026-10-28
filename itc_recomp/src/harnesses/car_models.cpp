// The car models (src/car_models/, docs/functions/car-models.md) against the
// recompiled functions. Each case fills a block of guest memory with random
// bytes, writes its inputs, runs the original and the register steps
// (hooks/car_models.h) from the same inputs, and compares the block and r3.
// None of these functions loads or stores a float, so everything is
// compared exactly.

#include "harness.h"

#include "hooks/car_models.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <functional>
#include <string>
#include <vector>

namespace harness {

namespace {

constexpr uint32_t kBlockBytes = 0x1000;
constexpr uint32_t kMarker = 0x50414745;  // a word that held the case's pages

void Put(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

void RandomBytes(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i < bytes; i += 4) {
    Put(base, address + i, uint32_t(r.Next()));
  }
}

// One case through both versions: the block filled from `seed`, `setup(v,
// t)` writing the inputs and registers (from its own generator seeded the
// same for both), the function run, the block and r3 kept.
void Case(Context& h, Results& results, uint32_t block, uint64_t seed,
          const std::function<void(int, PPCContext&, Random&)>& setup,
          const std::function<void(int, Outputs&)>& take, GuestFunction recompiled,
          GuestFunction native, const std::string& what) {
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Random r(seed);
    RandomBytes(h.base, block, kBlockBytes, r);
    PPCContext t = CallContext(h);
    setup(v, t, r);
    (v == 0 ? recompiled : native)(t, h.base);
    if (take) {
      take(v, out[v]);
    }
    out[v].Bytes(Read(h.base, block, kBlockBytes));
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  results.Check(out[0], out[1], what);
}

void Slots(Context& h, uint32_t block) {
  {
    Results results("LoadSlot::Reset (sub_820D0730)");
    for (int c = 0; c < 16; c++) {
      Case(
          h, results, block, h.random.Next(),
          [&](int, PPCContext& t, Random& r) { t.r3.u64 = block + 4 * r.Below(64); }, nullptr,
          __imp__sub_820D0730, hooks::CarModelsLoadSlotReset, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // The buffer: none, or pages the allocation layer gives each side (the
    // word left compared as a marker while it's still the case's pages, and
    // whether they're still pages); shared or not.
    Results results("LoadSlot::ReleaseBuffer (sub_820D0758)");
    for (int c = 0; c < 32; c++) {
      uint32_t pages[2] = {0, 0};
      uint32_t slot = 0;
      Case(
          h, results, block, h.random.Next(),
          [&](int v, PPCContext& t, Random& r) {
            slot = block + 4 * r.Below(64);
            const bool shared = c % 4 >= 2 && r.Below(4) != 0;
            const bool buffer = c % 2 == 1 || r.Below(4) == 0;
            Put(h.base, slot + 12, shared ? (r.Below(2) ? 1 : uint32_t(r.Next()) | 1) : 0);
            pages[v] = buffer ? memory::AllocPages(1 + r.Below(0x8000), 4096) : 0;
            Put(h.base, slot + 16, pages[v]);
            t.r3.u64 = slot;
          },
          [&](int v, Outputs& out) {
            const uint32_t word = guest::LoadU32(h.base, slot + 16);
            if (pages[v] != 0 && word == pages[v]) {
              Put(h.base, slot + 16, kMarker);
            }
            const bool still = pages[v] != 0 && memory::IsPages(pages[v]);
            out.Word(still);
            if (still) {
              memory::FreePages(pages[v]);
            }
          },
          __imp__sub_820D0758, hooks::CarModelsLoadSlotReleaseBuffer, fmt::format("case {}", c));
    }
    results.Report();
  }
}

void DeferReleases(Context& h, uint32_t block) {
  Results results("DeferRelease (sub_820C3F50)");
  for (int c = 0; c < 64; c++) {
    Case(
        h, results, block, h.random.Next(),
        [&](int, PPCContext& t, Random& r) {
          const uint32_t set = block + 16 * r.Below(4);
          const uint32_t records = block + 0x100;
          // Counts: small mostly (up to 4 x 6 records), now and then ones
          // whose 4 x sum is 0 or negative as a signed word.
          uint32_t a = r.Below(4), b = r.Below(4);
          if (c % 8 == 7) {
            const uint32_t odd[] = {0x40000000u, 0x30000000u, 0xFFFFFFFFu, 0x80000000u};
            a = odd[r.Below(4)];
            b = r.Below(2);
          }
          Put(h.base, set + 40, a);
          Put(h.base, set + 12, b);
          Put(h.base, set + 52, records);
          // The records' states: in use mostly, now and then free.
          const uint32_t count = 4 * ((a + b) & 0x3FFFFFFFu);
          const uint32_t shown = count < 48 ? count : 48;
          for (uint32_t i = 0; i < shown; i++) {
            Put(h.base, records + 20 * i + 16, r.Below(5) == 0 ? 0 : 1 + r.Below(3));
          }
          t.r3.u64 = set;
          t.r4.u64 = r.Next();
          t.r5.u64 = r.Next();
          t.r6.u64 = r.Next();
          t.r7.u64 = r.Next();
        },
        nullptr, __imp__sub_820C3F50, hooks::CarModelsDeferRelease, fmt::format("case {}", c));
  }
  results.Report();
}

}  // namespace

void CarModels(Context& h) {
  const uint32_t allocation = memory::Alloc(kBlockBytes + 16, 0);
  const uint32_t block = (allocation + 15) & ~15u;
  Slots(h, block);
  DeferReleases(h, block);
  memory::Free(allocation);
}

}  // namespace harness
