// The course set's lists of models and the model draws (src/scenery_lists/,
// docs/functions/scenery-lists.md) against the recompiled functions. Each
// case fills a block of guest memory with random bytes, writes its inputs
// there and points the globals the functions read at it, then runs the
// original and the register steps (hooks/scenery_lists.h) from the same
// inputs, and compares the block, the globals and r3. The globals are put
// back after each case. Floats are compared as floats only where the
// originals copy or compute them with float instructions; everything else
// exactly.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/scenery_lists.h"
#include "memory/heap.h"
#include "scenery_lists/scenery_lists.h"

#include <fmt/format.h>

#include <algorithm>
#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace harness {

namespace {

namespace sl = scenery_lists;

// The globals the functions read: the race data's pointer, the depth vertex
// declarations.
struct Region {
  uint32_t address;
  uint32_t bytes;
};
constexpr Region kRegions[] = {
    {0x826C5B20, 0x10},
    {0x8296B8B8, 0x20},
};

// The block: 64 KB for the lists, models and sets, then room for the race
// data's fields (up to +0xA3AF8) from +0x10000.
constexpr uint32_t kBlockBytes = 0x10000;
constexpr uint32_t kRaceBytes = 0xA4000;
constexpr uint32_t kRace = kBlockBytes;

// A float range [offset, offset + 4 x count) of the block.
using FloatRanges = std::vector<std::pair<uint32_t, uint32_t>>;

void AddBlock(Outputs& out, uint8_t* base, uint32_t block, FloatRanges floats) {
  std::sort(floats.begin(), floats.end());
  uint32_t at = 0;
  for (const auto& [offset, count] : floats) {
    if (offset > at) {
      out.Bytes(Read(base, block + at, offset - at));
    }
    out.Floats(base, block + offset, count);
    at = offset + 4 * count;
  }
  if (at < kBlockBytes) {
    out.Bytes(Read(base, block + at, kBlockBytes - at));
  }
}

void RandomBytes(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i < bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

// The race data's words the picks read, at random (small values mostly,
// so the switches' cases come up).
void RandomRace(uint8_t* base, uint32_t race, Random& r) {
  auto small = [&](uint32_t n) {
    return r.Below(10) == 0 ? uint32_t(r.Next()) : r.Below(n);
  };
  guest::StoreU32(base, race + 4, small(26));
  guest::StoreU32(base, race + 20, uint32_t(r.Next()) & (r.Below(2) ? 0xFFu : 0x3Cu));
  guest::StoreU32(base, race + 24, uint32_t(r.Next()) & (r.Below(2) ? 0xFFFu : 0xFFFFFFFFu));
  const uint32_t stages = race + sl::kStages;
  const uint32_t current = r.Below(10);
  guest::StoreU32(base, stages + 804, current);
  guest::StoreU32(base, stages + 80 * current, r.Below(4) == 0 ? small(8) : r.Below(2));
  guest::StoreU32(base, stages + 1220, uint32_t(r.Next()) & (r.Below(2) ? 0x60u : 0x9Fu));
  guest::StoreU32(base, race + sl::kRaceFlags, r.Below(4) == 0 ? 0x80u : uint32_t(r.Next()) & ~0x80u);
  const uint32_t kind = r.Below(3) == 0 ? small(9) : 0;
  guest::StoreU32(base, race + sl::kRaceKind, kind);
  guest::StoreU32(base, race + sl::kRaceMode, small(10));
}

// One case through both versions: the block filled at random and
// `setup(t)` writing the inputs and the registers (once: its inputs are
// written back for the second run); `floats` the block's float ranges.
void Case(Context& h, Results& results, uint32_t block, Random& r,
          const std::function<void(PPCContext&)>& setup, GuestFunction recompiled,
          GuestFunction native, const FloatRanges& floats, const std::string& what,
          bool race = false) {
  std::vector<std::vector<uint8_t>> saved;
  for (const Region& region : kRegions) {
    saved.push_back(Read(h.base, region.address, region.bytes));
  }
  RandomBytes(h.base, block, kBlockBytes, r);
  PPCContext start = CallContext(h);
  start.r1.u64 &= ~0xFull;
  setup(start);
  const std::vector<uint8_t> inputs = Read(h.base, block, kBlockBytes);
  std::vector<uint8_t> race_inputs;
  if (race) {
    race_inputs = Read(h.base, block + kRace, kRaceBytes);
  }
  std::vector<std::vector<uint8_t>> globals;
  for (const Region& region : kRegions) {
    globals.push_back(Read(h.base, region.address, region.bytes));
  }
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Write(h.base, block, inputs.data(), kBlockBytes);
    if (race) {
      Write(h.base, block + kRace, race_inputs.data(), kRaceBytes);
    }
    for (size_t i = 0; i < globals.size(); i++) {
      Write(h.base, kRegions[i].address, globals[i].data(), kRegions[i].bytes);
    }
    Fill(h.base, start.r1.u32 - 1024, 1024);  // the callee's frames
    PPCContext t = start;
    (v == 0 ? recompiled : native)(t, h.base);
    AddBlock(out[v], h.base, block, floats);
    for (const Region& region : kRegions) {
      out[v].Bytes(Read(h.base, region.address, region.bytes));
    }
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  for (size_t i = 0; i < saved.size(); i++) {
    Write(h.base, kRegions[i].address, saved[i].data(), kRegions[i].bytes);
  }
  results.Check(out[0], out[1], what);
}

// --- The lists ---

// The empty function the fake models' vtable slot 0 points at (the scenery
// model's slot 1, which does nothing).
constexpr uint32_t kEmptyFunction = 0x82106540;

void Lists(Context& h, Random& r, uint32_t block) {
  Results results("ModelList::Release (sub_82147FB8)");
  for (int c = 0; c < 64; c++) {
    Case(
        h, results, block, r,
        [&](PPCContext& t) {
          // A List64 of up to 8 models (0 to -1 for none), linked in a
          // random order, its unused nodes random; models 448 bytes of zeros
          // (Model::Release then frees nothing) with a vtable whose slot 0
          // does nothing. No null models: Model::Release would write at 0.
          const uint32_t list = block;
          const int32_t count = c < 2 ? -c : int32_t(1 + r.Below(8));
          guest::StoreU32(h.base, list, uint32_t(count));
          const uint32_t nodes = list + 4;
          const uint32_t vtable = block + 0x400;
          guest::StoreU32(h.base, vtable, kEmptyFunction);
          int16_t order[8];
          for (int i = 0; i < 8; i++) {
            order[i] = int16_t(i * 3 + 1);
          }
          for (int i = 7; i > 0; i--) {
            std::swap(order[i], order[r.Below(uint32_t(i + 1))]);
          }
          guest::StoreU16(h.base, nodes + 260, uint16_t(order[0]));
          for (int i = 0; i < std::max(count, 0); i++) {
            const uint32_t node = nodes + 4 * uint32_t(order[i]);
            guest::StoreU16(h.base, node, uint16_t(i + 1 < 8 ? order[i + 1] : -1));
            const int16_t slot = int16_t(r.Below(64));
            guest::StoreU16(h.base, node + 2, uint16_t(slot));
            const uint32_t model = block + 0x1000 + 0x200 * uint32_t(i);
            guest::StoreU32(h.base, nodes + 268 + 4 * uint32_t(slot), model);
            Fill(h.base, model, 448, 0);
            guest::StoreU32(h.base, model, vtable);
          }
          t.r3.u64 = list;
        },
        __imp__sub_82147FB8, hooks::SceneryListsModelListRelease, {},
        fmt::format("case {}", c));
  }
  results.Report();
}

// --- The draw state, the animations ---

void DrawState(Context& h, Random& r, uint32_t block) {
  {
    Results results("SceneryModel::ResetDrawState (sub_82146AA8)");
    for (int c = 0; c < 16; c++) {
      Case(
          h, results, block, r, [&](PPCContext& t) { t.r3.u64 = block + 16 * r.Below(64); },
          __imp__sub_82146AA8, hooks::SceneryListsResetDrawState, {}, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("AnimationSet::CurrentFrame (sub_82146B80)");
    for (int c = 0; c < 256; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            // A set of up to 16 animations, offsets small, frames and
            // current frames small (negative now and then).
            const uint32_t set = block + 0x100;
            const uint32_t count = r.Below(16);
            guest::StoreU32(h.base, set, count);
            const uint32_t index = r.Below(count + 1);
            const uint32_t offset = 16 * r.Below(64);
            guest::StoreU32(h.base, set + 4 * (index + 1), offset);
            const uint32_t animation = set + 4 * (count + 1) + offset;
            auto small = [&] {
              return r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(12);
            };
            guest::StoreU32(h.base, animation, small());
            guest::StoreU32(h.base, animation + 4, small());
            t.r3.u64 = set;
            t.r4.u64 = index;
          },
          __imp__sub_82146B80, hooks::SceneryListsAnimationCurrentFrame, {},
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("PassDescription::SetBillboardPass (sub_82148650)");
    for (int c = 0; c < 16; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            t.r3.u64 = block + 4 * r.Below(256);
            t.r4.u64 = r.Next();
            t.r5.u64 = r.Next();
            t.r6.u64 = r.Next();
          },
          __imp__sub_82148650, hooks::SceneryListsSetBillboardPass, {},
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("VertexDeclarations::ForDepth (sub_82589970)");
    for (int c = 0; c < 32; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            RandomBytes(h.base, 0x8296B8B8, 0x20, r);
            t.r3.u64 = r.Below(4) == 0 ? r.Next() : uint64_t(r.Below(2) ? 11 : r.Below(16));
          },
          __imp__sub_82589970, hooks::SceneryListsDepthVertexDeclaration, {},
          fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- The race's picks ---

void Picks(Context& h, Random& r, uint32_t block) {
  struct Pick {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
    uint32_t entries;
  };
  const Pick picks[] = {
      {"ModelList::IsBEntryOn (sub_82147850)", __imp__sub_82147850,
       hooks::SceneryListsIsBEntryOn, 14},
      {"ModelList::IsCEntryOn (sub_82147958)", __imp__sub_82147958,
       hooks::SceneryListsIsCEntryOn, 27},
      {"Race::CourseLayout (sub_8228F408)", __imp__sub_8228F408,
       hooks::SceneryListsCourseLayout, 1},
      {"PointList::IsEntryOn (sub_82147B18)", __imp__sub_82147B18,
       hooks::SceneryListsPointListIsEntryOn, 22},
  };
  for (const Pick& pick : picks) {
    Results results(pick.name);
    for (int c = 0; c < 1024; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            const uint32_t race = block + kRace;
            RandomRace(h.base, race, r);
            // The race data, or none now and then (IsBEntryOn and IsCEntryOn
            // read it without a check: always there for them).
            const bool none = pick.entries != 14 && pick.entries != 27 && r.Below(16) == 0;
            guest::StoreU32(h.base, sl::kRaceData, none ? 0 : race);
            t.r3.u64 = block;
            t.r4.u64 = r.Below(10) == 0 ? r.Next() : uint64_t(r.Below(pick.entries));
          },
          pick.recompiled, pick.native, {}, fmt::format("case {}", c), true);
    }
    results.Report();
  }
}

// --- The texture provider ---

void Provider(Context& h, Random& r, uint32_t block) {
  // A provider at +0x200: its vector's first (+8), end (+12) and storage end
  // (+16) in the block's +0x1000 to +0x1100; for Set, no storage now and
  // then (ClearAll would clear from 0 up).
  auto setup = [&](PPCContext& t, bool set) {
    const uint32_t provider = block + 0x200;
    const uint32_t storage = block + 0x1000;
    const uint32_t capacity = r.Below(64);
    const uint32_t size = r.Below(capacity + 1);
    guest::StoreU32(h.base, provider + 8, set && r.Below(16) == 0 ? 0 : storage);
    guest::StoreU32(h.base, provider + 12, storage + 4 * size);
    guest::StoreU32(h.base, provider + 16, storage + 4 * capacity);
    t.r3.u64 = provider;
    if (set) {
      // Mostly a slot below the size (the path seen in play).
      t.r4.u64 = r.Below(8) == 0 ? r.Below(80) : r.Below(size + 1);
      t.r5.u64 = r.Next();
    }
  };
  {
    Results results("TextureProvider::ClearAll (sub_82594328)");
    for (int c = 0; c < 64; c++) {
      Case(
          h, results, block, r, [&](PPCContext& t) { setup(t, false); }, __imp__sub_82594328,
          hooks::SceneryListsTextureProviderClearAll, {}, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // Only the cases the plain C++ takes: the others run the original on
    // both sides (its reserve and resize allocate).
    Results results("TextureProvider::Set (sub_82594718)");
    for (int c = 0; c < 256; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            do {
              RandomBytes(h.base, block + 0x200, 32, r);
              setup(t, true);
            } while (guest::LoadU32(h.base, block + 0x208) == 0 ||
                     t.r4.u32 >= (guest::LoadU32(h.base, block + 0x20C) -
                                  guest::LoadU32(h.base, block + 0x208)) / 4);
          },
          __imp__sub_82594718, hooks::SceneryListsTextureProviderSet, {},
          fmt::format("case {}", c));
    }
    results.Report();
  }
}

}  // namespace

void SceneryLists(Context& h) {
  Random r(0x7363656E657279ull);  // "scenery"
  const uint32_t allocation = memory::Alloc(kBlockBytes + kRaceBytes + 16, 0);
  const uint32_t block = (allocation + 15) & ~15u;
  Lists(h, r, block);
  DrawState(h, r, block);
  Picks(h, r, block);
  Provider(h, r, block);
  memory::Free(allocation);
}

}  // namespace harness
