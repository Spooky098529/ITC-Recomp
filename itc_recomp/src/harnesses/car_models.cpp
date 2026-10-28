// The car models (src/car_models/, docs/functions/car-models.md) against the
// recompiled functions. Each case fills a block of guest memory with random
// bytes, writes its inputs, runs the original and the register steps
// (hooks/car_models.h) from the same inputs, and compares the block and r3.
// None of these functions loads or stores a float, so everything is
// compared exactly.

#include "harness.h"
#include "car_model_inputs.h"
#include "pending_frees.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/car_models.h"
#include "hooks/models.h"
#include "models/memory_blocks.h"
#include "scene_loaders/objects.h"
#include "scene_loaders/pools.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <functional>
#include <mutex>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_scene_loaders);
ITC_FLAG_DECLARE(bool, native_car_models);

namespace harness {

namespace {

constexpr uint32_t kBlockBytes = 0x4000;
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

// CarModel::Release: the car model's inputs as for its reset
// (car_model_inputs.h), with the three slots' buffers (pages each side
// allocates, shared or not) and the set its sheets are put off into (+7256:
// records at random states, as DeferRelease's cases). The recompiled side
// with --native_car_models and --native_scene_loaders off. Compared as the
// resets are (harnesses/scene_loaders.cpp), the pages by whether they're
// still alive.
void Releases(Context& h, uint32_t block) {
  namespace cmi = car_model_inputs;
  namespace sl = scene_loaders;
  if (!hooks::ModelsHostLock()) {
    ITC_LOG_WARN("harness: car_models: --native_models was off at start-up: CarModel::Release's "
                 "cases skipped");
    return;
  }
  std::lock_guard<std::recursive_mutex> lock(models::MemoryBlocksMutex());
  const uint32_t scratch = block + 0x100;
  const uint32_t car = block + 0x1000;
  const uint32_t loads = car + sl::kCarModelLoads;
  const uint32_t set = block + 0x3000;
  const uint32_t records = set + 0x100;
  const uint32_t saved_set = guest::LoadU32(h.base, sl::kFirstSet);
  Results results("CarModel::Release (sub_820D4508, on the game's list)");
  for (int c = 0; c < 48; c++) {
    const uint64_t seed = h.random.Next();
    const cmi::Plan plan = cmi::MakePlan(h.random);
    const uint32_t first_set = uint32_t(h.random.Next());
    const uint32_t a = h.random.Below(3), b = h.random.Below(3);
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random bytes(seed);
      RandomBytes(h.base, block, kBlockBytes, bytes);
      cmi::Held held;
      cmi::Write(h.base, loads, scratch, plan, true, held);
      Put(h.base, car + sl::kCarModelSet, set);
      Put(h.base, set + 40, a);
      Put(h.base, set + 12, b);
      Put(h.base, set + 52, records);
      for (uint32_t i = 0; i < 4 * (a + b); i++) {
        Put(h.base, records + 20 * i + 16, bytes.Below(4) == 0 ? 0 : 2);
      }
      Put(h.base, sl::kFirstSet, first_set);
      const pending_frees::ListState was = pending_frees::SaveList(h.base);
      PPCContext t = CallContext(h);
      t.r1.u64 &= ~0xFull;
      t.r3.u64 = car;
      ITC_FLAG(native_scene_loaders) = v == 1;
      ITC_FLAG(native_car_models) = v == 1;
      (v == 0 ? __imp__sub_820D4508 : hooks::CarModelsCarModelRelease)(t, h.base);
      ITC_FLAG(native_scene_loaders) = true;
      ITC_FLAG(native_car_models) = true;
      pending_frees::TakeAddedNodes(h.base, was, out[v]);
      cmi::Take(h.base, loads, held, out[v]);
      out[v].Bytes(Read(h.base, block, kBlockBytes));
      out[v].Word(guest::LoadU32(h.base, sl::kFirstSet));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
  Put(h.base, sl::kFirstSet, saved_set);
}

}  // namespace

void CarModels(Context& h) {
  const uint32_t allocation = memory::Alloc(kBlockBytes + 16, 0);
  const uint32_t block = (allocation + 15) & ~15u;
  Slots(h, block);
  DeferReleases(h, block);
  Releases(h, block);
  memory::Free(allocation);
}

}  // namespace harness
