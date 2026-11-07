// The tuning screen's pages (src/tuning_pages/,
// docs/functions/tuning-pages.md) against the recompiled functions. Each
// case fills a world of its own (a block from the allocation layer holding
// the screen, its decal images at +10048, and the save data the screen's
// +18904 points at) with random words from the case's seed, plus the stack
// below the call's stack pointer and the caller's area above it; shapes the
// fields the function tests (the state, the category and item around their
// ranges, the picks, the table rows, the garage list); runs the original
// (with --native_tuning_pages off) or the register steps
// (hooks/tuning_pages.h); and compares the blocks, the stack, the caller's
// area, r1 and all of r3 with harness::Outputs (nothing here is a float, so
// all exactly). The screen runs only in the front end, which attract races
// don't reach: these cases are its check.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/tuning_pages.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <functional>
#include <string>

ITC_FLAG_DECLARE(bool, native_tuning_pages);

namespace harness {

namespace {

constexpr uint32_t kScreenBytes = 0xA000;  // the screen (to +40960: the picks' tables with any byte)
constexpr uint32_t kSaveBytes = 0x1A000;   // the save data (to +0x19924)
constexpr uint32_t kStackBytes = 2048;     // below the call's stack pointer
constexpr uint32_t kCallerBytes = 256;     // above it

struct Block {
  explicit Block(uint32_t bytes) : bytes(bytes), address(memory::Alloc(bytes, 0)) {}
  ~Block() { memory::Free(address); }
  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;
  uint32_t bytes;
  uint32_t address;
};

struct World {
  World() : screen_block(kScreenBytes), save(kSaveBytes) {}
  bool ok() const { return screen_block.address != 0 && save.address != 0; }
  uint32_t screen() const { return screen_block.address; }
  Block screen_block;
  Block save;
  uint8_t* base = nullptr;
};

void WriteRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

// A small signed word in [lo, hi], now and then any word.
uint32_t Around(Random& r, int32_t lo, int32_t hi) {
  if (r.Below(16) == 0) {
    return uint32_t(r.Next());
  }
  return uint32_t(lo + int32_t(r.Below(uint32_t(hi - lo + 1))));
}

// The fields the pages' functions test: the state 0 to 14 mostly, the
// category -1 to 7, the item 0 to 11, the picks small (now and then any
// byte), the table rows and the wheel fields small (the word indices grow
// with them, and any word would read far outside the world), the garage
// list as the screen's harness lays it.
void ShapeScreen(uint8_t* base, const World& w, Random& r) {
  const uint32_t s = w.screen();
  guest::StoreU32(base, s + 9652, Around(r, 0, 14));
  guest::StoreU32(base, s + 9656, Around(r, -1, 7));
  guest::StoreU8(base, s + 9660, uint8_t(r.Below(8) == 0 ? r.Next() : r.Below(12)));
  for (uint32_t k = 9661; k <= 9665; k++) {
    guest::StoreU8(base, s + k, uint8_t(r.Below(8) == 0 ? r.Next() : r.Below(6)));
  }
  guest::StoreU32(base, s + 33900, r.Below(6));
  guest::StoreU32(base, s + 33540, Around(r, 0, 2));
  guest::StoreU32(base, s + 33548, r.Below(10));
  guest::StoreU32(base, s + 33564, r.Below(6));
  guest::StoreU32(base, s + 33572, r.Below(5));
  guest::StoreU32(base, s + 25204, r.Below(5) == 0 ? 17 + r.Below(4) : r.Below(17));
  guest::StoreU32(base, s + 25220, r.Below(38));
  guest::StoreU32(base, s + 25168, Around(r, 0, 1));
  guest::StoreU32(base, s + 25172, Around(r, 0, 1));
  guest::StoreU32(base, s + 30724, Around(r, -1, 3));
  // The decal images' states: 0 to 2, now and then any word.
  for (uint32_t i = 0; i < 200; i++) {
    guest::StoreU32(base, s + 10048 + 36 * i + 8, r.Below(3) == 0 ? Around(r, 0, 2) : 2);
  }
  // Category 5's item 6 (the vinyls) now and then.
  if (r.Below(3) == 0) {
    guest::StoreU32(base, s + 9656, 5);
    guest::StoreU8(base, s + 9660, 6);
  }
  // The save data's garage list: 51 nodes of 4 bytes from +6120 (the head
  // halfword at +204), each link and the head a node, each slot 0 to 3; the
  // walk [+0x19920] steps; the cars' ids (+224) 0 to 25, now and then any
  // word for InchListEnd (LoadPageLayouts' table read stays near).
  const uint32_t save = w.save.address, nodes = save + 6120;
  guest::StoreU32(base, s + 18904, save);
  for (uint32_t k = 0; k < 51; k++) {
    guest::StoreU16(base, nodes + 4 * k, uint16_t(r.Below(51)));
    guest::StoreU16(base, nodes + 4 * k + 2, uint16_t(r.Below(4)));
  }
  guest::StoreU16(base, nodes + 204, uint16_t(r.Below(51)));
  guest::StoreU32(base, save + 0x19920, r.Below(6));
  for (uint32_t k = 0; k < 4; k++) {
    guest::StoreU32(base, nodes + 1968 * k + 224, r.Below(26));
  }
}

using SetupFn = std::function<void(PPCContext&, Random&, const World&)>;

void Case(Context& h, Results& results, const World& w, uint64_t seed, const SetupFn& setup,
          GuestFunction recompiled, GuestFunction native, const std::string& what) {
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Random r(seed);
    PPCContext t = CallContext(h);
    const uint32_t sp = t.r1.u32;
    WriteRandom(h.base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
    WriteRandom(h.base, w.screen_block.address, w.screen_block.bytes, r);
    WriteRandom(h.base, w.save.address, w.save.bytes, r);
    ShapeScreen(h.base, w, r);
    t.r3.u64 = w.screen();
    setup(t, r, w);
    t.lr = 0x82000000 | (r.Below(0x10000) << 2);
    if (v == 0) {
      const bool flag = ITC_FLAG(native_tuning_pages);
      ITC_FLAG(native_tuning_pages) = false;
      recompiled(t, h.base);
      ITC_FLAG(native_tuning_pages) = flag;
    } else {
      native(t, h.base);
    }
    out[v].Bytes(Read(h.base, w.screen_block.address, w.screen_block.bytes));
    out[v].Bytes(Read(h.base, w.save.address, w.save.bytes));
    out[v].Bytes(Read(h.base, sp - kStackBytes, kStackBytes + kCallerBytes));
    out[v].Word(t.r1.u32);
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  results.Check(out[0], out[1], what);
}

void Group(Context& h, const World& w, const char* name, int cases, const SetupFn& setup,
           GuestFunction recompiled, GuestFunction native) {
  Results results(name);
  for (int c = 0; c < cases; c++) {
    Case(h, results, w, h.random.Next(), setup, recompiled, native, fmt::format("case {}", c));
  }
  results.Report();
}

const SetupFn kScreenOnly = [](PPCContext&, Random&, const World&) {};

}  // namespace

void TuningPages(Context& h) {
  World w;
  w.base = h.base;
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: tuning_pages: no memory for the world");
    return;
  }
  // The pages' counts and owned flags.
  Group(h, w, "TuningScreen::LevelCount", 30, kScreenOnly, __imp__sub_821CC728,
        hooks::TuningPagesLevelCount);
  Group(h, w, "TuningScreen::TypeCount", 30, kScreenOnly, __imp__sub_821CC7B8,
        hooks::TuningPagesTypeCount);
  Group(h, w, "TuningScreen::PageCursor", 30, kScreenOnly, __imp__sub_821CCD38,
        hooks::TuningPagesPageCursor);
  Group(h, w, "TuningScreen::PageItemCount", 30, kScreenOnly, __imp__sub_821CCDB0,
        hooks::TuningPagesPageItemCount);
  Group(h, w, "TuningScreen::IsPickOwned", 50, kScreenOnly, __imp__sub_821CCE30,
        hooks::TuningPagesIsPickOwned);
  Group(h, w, "TuningScreen::MarkOwned", 50, kScreenOnly, __imp__sub_821CD498,
        hooks::TuningPagesMarkOwned);
  // TestAndSetWheelFlag: wheel numbers -2 to 180, now and then any word
  // (with upper bits).
  Group(
      h, w, "TuningScreen::TestAndSetWheelFlag", 40,
      [](PPCContext& t, Random& r, const World&) {
        t.r4.u64 = r.Below(8) == 0 ? r.Next() : uint64_t(int64_t(Around(r, -2, 180)));
      },
      __imp__sub_821CDF98, hooks::TuningPagesTestAndSetWheelFlag);
  Group(h, w, "TuningScreen::SetWheelListEnd", 30, kScreenOnly, __imp__sub_821E7DB0,
        hooks::TuningPagesSetWheelListEnd);
  // InchListEnd: the selected car's id 0 to 25, now and then any word.
  Group(
      h, w, "TuningScreen::InchListEnd", 40,
      [](PPCContext&, Random& r, const World& w) {
        if (r.Below(6) == 0) {
          const uint32_t nodes = w.save.address + 6120;
          for (uint32_t k = 0; k < 4; k++) {
            guest::StoreU32(w.base, nodes + 1968 * k + 224, uint32_t(r.Next()));
          }
        }
      },
      __imp__sub_821EF288, hooks::TuningPagesInchListEnd);

  // The vinyl editor's set-up.
  Group(h, w, "TuningScreen::SetUpVinylBusyIcon", 10, kScreenOnly, __imp__sub_821FE560,
        hooks::TuningPagesSetUpVinylBusyIcon);
  // AllLoaded: r3 the decal images; none loading a third of the time.
  Group(
      h, w, "DecalImages::AllLoaded", 30,
      [](PPCContext& t, Random& r, const World& w) {
        t.r3.u64 = w.screen() + 10048;
        if (r.Below(3) == 0) {
          for (uint32_t i = 0; i < 200; i++) {
            const uint32_t state = w.screen() + 10048 + 36 * i + 8;
            if (guest::LoadU32(w.base, state) == 1) {
              guest::StoreU32(w.base, state, 2);
            }
          }
        }
      },
      __imp__sub_821FE5D8, hooks::TuningPagesAllLoaded);
  Group(h, w, "TuningScreen::KeepListTextures", 20, kScreenOnly, __imp__sub_821FE7D8,
        hooks::TuningPagesKeepListTextures);
  Group(h, w, "TuningScreen::SetEditorLimits", 30, kScreenOnly, __imp__sub_821FE420,
        hooks::TuningPagesSetEditorLimits);
  Group(h, w, "TuningScreen::LoadPageLayouts", 20, kScreenOnly, __imp__sub_822045F8,
        hooks::TuningPagesLoadPageLayouts);
}

}  // namespace harness
