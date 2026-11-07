// The tuning screen (src/tuning_screen/, docs/functions/tuning-screen.md)
// against the recompiled functions. Each case fills a world of its own (a
// block from the allocation layer holding the screen, 37,000 bytes) with
// random words from the case's seed, plus the stack below the call's stack
// pointer and the caller's area above it; shapes the fields the function
// tests (the state, the category and item around their ranges, the picks,
// the table rows); runs the original (with --native_tuning_screen off, so
// what it calls of the screen's is the original too) or the register steps
// (hooks/tuning_screen.h); and compares the block, the stack below the
// original's register saves, the caller's area, r1 and all of r3, exactly
// (nothing here is a float). The screen runs only in the front end, which
// attract races don't reach: these cases are its check.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/tuning_screen.h"
#include "memory/heap.h"
#include "tuning_screen/tuning_screen.h"

#include <fmt/format.h>

#include <functional>
#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_tuning_screen);

namespace harness {

namespace {

namespace ts = tuning_screen;

constexpr uint32_t kScreenBytes = 0x9400;  // the screen (to +37888)
constexpr uint32_t kBlockBytes = 0xA000;
constexpr uint32_t kStackBytes = 1024;  // below the call's stack pointer
constexpr uint32_t kCallerBytes = 256;  // above it

struct Block {
  explicit Block(uint32_t bytes) : bytes(bytes), address(memory::Alloc(bytes, 0)) {}
  ~Block() { memory::Free(address); }
  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;
  uint32_t bytes;
  uint32_t address;
};

void WriteRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

struct World {
  World() : block(kBlockBytes) {}
  bool ok() const { return block.address != 0; }
  uint32_t screen() const { return block.address; }
  Block block;
};

// A small signed word in [lo, hi], now and then any word.
uint32_t Around(Random& r, int32_t lo, int32_t hi) {
  if (r.Below(16) == 0) {
    return uint32_t(r.Next());
  }
  return uint32_t(lo + int32_t(r.Below(uint32_t(hi - lo + 1))));
}

// The fields the screen's functions test: the state mostly 5, the category
// -1 to 7, the item 0 to 11, the picks small (now and then any byte), the
// table rows small.
void ShapeScreen(uint8_t* base, const World& w, Random& r) {
  const uint32_t s = w.screen();
  guest::StoreU32(base, s + ts::kState, r.Below(3) == 0 ? Around(r, 3, 58) : 5);
  guest::StoreU32(base, s + ts::kCategory, Around(r, -1, 7));
  guest::StoreU8(base, s + ts::kItem, uint8_t(r.Below(8) == 0 ? r.Next() : r.Below(12)));
  for (uint32_t k = ts::kLevel; k <= ts::kOption; k++) {
    guest::StoreU8(base, s + k, uint8_t(r.Below(8) == 0 ? r.Next() : r.Below(6)));
  }
  // The row stays small: the tables' index grows with it, and any word would
  // read far outside the world.
  guest::StoreU32(base, s + ts::kPartSet, r.Below(6));
  guest::StoreU32(base, s + 33540, Around(r, 0, 2));
  guest::StoreU32(base, s + 33548, r.Below(10));
  guest::StoreU32(base, s + 33564, r.Below(6));
  guest::StoreU32(base, s + 33572, r.Below(5));
  guest::StoreU32(base, s + 25196, r.Below(16));
  guest::StoreU32(base, s + 25204, r.Below(17));
  guest::StoreU32(base, s + 25220, r.Below(38));
}

// A small index for r4 (all 64 bits), now and then with upper bits.
uint64_t SmallIndex(Random& r, uint32_t hi) {
  uint64_t v = r.Below(hi + 1);
  if (r.Below(6) == 0) {
    v |= r.Next() << 32;
  }
  return v;
}

using SetupFn = std::function<void(PPCContext&, Random&, const World&)>;

// `saves` the original's register saves' bytes below the stack pointer, left
// out of the comparison.
void Case(Context& h, Results& results, const World& w, uint64_t seed, const SetupFn& setup,
          GuestFunction recompiled, GuestFunction native, uint32_t saves,
          const std::string& what) {
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Random r(seed);
    PPCContext t = CallContext(h);
    const uint32_t sp = t.r1.u32;
    WriteRandom(h.base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
    WriteRandom(h.base, w.block.address, w.block.bytes, r);
    ShapeScreen(h.base, w, r);
    t.r3.u64 = w.screen();
    setup(t, r, w);
    t.lr = 0x82000000 | (r.Below(0x10000) << 2);
    if (v == 0) {
      const bool flag = ITC_FLAG(native_tuning_screen);
      ITC_FLAG(native_tuning_screen) = false;
      recompiled(t, h.base);
      ITC_FLAG(native_tuning_screen) = flag;
    } else {
      native(t, h.base);
    }
    out[v].Bytes(Read(h.base, w.block.address, w.block.bytes));
    out[v].Bytes(Read(h.base, sp - kStackBytes, kStackBytes - saves));
    out[v].Bytes(Read(h.base, sp, kCallerBytes));
    out[v].Word(t.r1.u32);
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  results.Check(out[0], out[1], what);
}

void Group(Context& h, const World& w, const char* name, int cases, const SetupFn& setup,
           GuestFunction recompiled, GuestFunction native, uint32_t saves) {
  Results results(name);
  for (int c = 0; c < cases; c++) {
    Case(h, results, w, h.random.Next(), setup, recompiled, native, saves,
         fmt::format("case {}", c));
  }
  results.Report();
}

const SetupFn kScreenOnly = [](PPCContext&, Random&, const World&) {};

}  // namespace

void TuningScreen(Context& h) {
  World w;
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: tuning_screen: no memory for the world");
    return;
  }
  Group(h, w, "TuningScreen::CategoryItemCount", 24, kScreenOnly, __imp__sub_821CC6C0,
        hooks::TuningCategoryItemCount, 0);
  Group(
      h, w, "TuningScreen::IsItemAvailable", 40,
      [](PPCContext& t, Random& r, const World&) { t.r4.u64 = SmallIndex(r, 12); },
      __imp__sub_821CDB18, hooks::TuningIsItemAvailable, 0);
  Group(
      h, w, "TuningScreen::WheelNumber", 30,
      [](PPCContext& t, Random& r, const World&) {
        t.r4.u64 = r.Below(5) == 0 ? r.Next() : r.Below(18);
        t.r5.u64 = r.Below(3) == 0 ? r.Next() : r.Below(40);
      },
      __imp__sub_821CDED8, hooks::TuningWheelNumber, 0);
  Group(h, w, "TuningScreen::GetPrice", 40, kScreenOnly, __imp__sub_821CD160,
        hooks::TuningGetPrice, 0);
  Group(h, w, "TuningScreen::GetCurrentChoice", 40, kScreenOnly, __imp__sub_821CD868,
        hooks::TuningGetCurrentChoice, 0);
  Group(
      h, w, "TuningScreen::GetChoiceFlag", 40,
      [](PPCContext& t, Random& r, const World&) { t.r4.u64 = SmallIndex(r, 10); },
      __imp__sub_821CDC38, hooks::TuningGetChoiceFlag, 0);
}

}  // namespace harness
