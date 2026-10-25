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
#include "hooks/models.h"
#include "hooks/scenery_lists.h"
#include "memory/c_runtime_heap.h"
#include "models/memory_blocks.h"
#include "models/model.h"
#include "memory/heap.h"
#include "scenery_lists/scenery_lists.h"

#include <fmt/format.h>

#include <algorithm>
#include <bit>
#include <cstdlib>
#include <functional>
#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_models);

namespace harness {

namespace {

namespace sl = scenery_lists;

// The globals the functions read: the race data's pointer, the depth vertex
// declarations.
struct Region {
  uint32_t address;
  uint32_t bytes;
};
// the command list the recorders use and the open chunks' count, the view,
// the camera's position, the projector origin, pixel c43 and the constants
// put back, the scene lights, the draws' sort keys, the vertex declarations'
// and shared shaders' tables, the stream kinds.
constexpr Region kRegions[] = {
    {0x826C5B20, 0x10}, {0x8296B8B8, 0x20}, {0x8296B910, 0x10},
    {0x8296BD20, 0x10}, {0x8265ECA0, 0xA0}, {0x8296B940, 0x3A0},
    {0x8296B8F0, 0x10}, {0x8296B7B0, 0x140}, {0x8265EC00, 0x90},
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

// Whether the next case runs with flush-to-zero on at its start (the
// originals set it off before their float work; set by a case's setup).
bool g_flush_on_entry = false;

// One case through both versions: the block filled at random and
// `setup(t)` writing the inputs and the registers (once: its inputs are
// written back for the second run); `floats` the block's float ranges.
void Case(Context& h, Results& results, uint32_t block, Random& r,
          const std::function<void(PPCContext&)>& setup, GuestFunction recompiled,
          GuestFunction native, const FloatRanges& floats, const std::string& what,
          bool race = false,
          const std::function<void(const PPCContext&, Outputs&)>& extra = {},
          const std::function<void()>& before = {}) {
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
    if (before) {
      before();
    }
    if (g_flush_on_entry) {
      t.fpscr.enableFlushModeUnconditional();
    } else {
      t.fpscr.disableFlushModeUnconditional();
    }
    (v == 0 ? recompiled : native)(t, h.base);
    AddBlock(out[v], h.base, block, floats);
    for (const Region& region : kRegions) {
      out[v].Bytes(Read(h.base, region.address, region.bytes));
    }
    if (extra) {
      extra(t, out[v]);
    }
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  for (size_t i = 0; i < saved.size(); i++) {
    Write(h.base, kRegions[i].address, saved[i].data(), kRegions[i].bytes);
  }
  start.fpscr.disableFlushModeUnconditional();
  g_flush_on_entry = false;
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

// --- The model draws ---

// A float's bits: now and then a NaN (either sign, quiet or signaling), an
// infinity, a zero or a denormal; else a value of a few hundred.
uint32_t RandomFloat(Random& r) {
  switch (r.Below(12)) {
    case 0: return 0x7FC00000u | (r.Below(2) << 31) | r.Below(0x400000);
    case 1: return 0x7F800001u | (r.Below(2) << 31) | r.Below(0x3FFFFF);
    case 2: return 0x7F800000u | (r.Below(2) << 31);
    case 3: return r.Below(2) << 31;
    case 4: return r.Below(0x800000) | (r.Below(2) << 31);
    default: {
      const float f = (float(r.Below(200000)) - 100000.0f) / 256.0f;
      return std::bit_cast<uint32_t>(f);
    }
  }
}

void RandomFloats(uint8_t* base, uint32_t address, uint32_t count, Random& r) {
  for (uint32_t i = 0; i < count; i++) {
    guest::StoreU32(base, address + 4 * i, RandomFloat(r));
  }
}

void StoreFloats(uint8_t* base, uint32_t address, std::initializer_list<float> values) {
  for (float f : values) {
    guest::StoreFloat(base, address, f);
    address += 4;
  }
}

float Spread(Random& r, float range) {
  return (float(r.Below(20001)) - 10000.0f) / 10000.0f * range;
}

void Visibility(Context& h, Random& r, uint32_t block) {
  Results results("Model::IsShapeVisible (sub_825951E0)");
  int visible[2] = {0, 0};  // both versions' results, counted
  for (int c = 0; c < 512; c++) {
    Case(
        h, results, block, r,
        [&](PPCContext& t) {
          g_flush_on_entry = r.Below(2) != 0;
          const uint32_t model = block + 0x100;
          const uint32_t shapes = block + 0x2000;
          guest::StoreU32(h.base, model + 40, shapes);
          const uint32_t shape = r.Below(4);
          const uint32_t corners = shapes + 544 * shape + 96;
          const uint32_t view_projection = block + 0x400 + 16 * r.Below(4);
          const uint32_t world = block + 0x500 + 4 * r.Below(4);
          if (c % 4 == 0) {
            // Anything (NaNs, infinities, zeros among them).
            RandomFloats(h.base, view_projection, 16, r);
            RandomFloats(h.base, world, 16, r);
            RandomFloats(h.base, corners, 32, r);
          } else {
            // An identity view-projection now and then scaled, a world
            // moved, and the corners in a cluster about a point (sometimes
            // behind the camera: w below 0 when the scale is negative).
            const float scale = c % 4 == 1 ? 1.0f : Spread(r, 2.0f);
            StoreFloats(h.base, view_projection, {scale, 0, 0, 0, 0, scale, 0, 0, 0, 0, scale, 0,
                                                  0, 0, 0, c % 8 == 3 ? -1.0f : 1.0f});
            StoreFloats(h.base, world, {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, Spread(r, 3),
                                        Spread(r, 3), Spread(r, 3), 1});
            const float x = Spread(r, 3), y = Spread(r, 3), z = Spread(r, 3);
            const float size = Spread(r, 0.5f);
            for (uint32_t i = 0; i < 8; i++) {
              StoreFloats(h.base, corners + 16 * i,
                          {x + (i & 1 ? size : -size), y + (i & 2 ? size : -size),
                           z + (i & 4 ? size : -size), c % 16 == 5 ? Spread(r, 1) : 1.0f});
            }
          }
          t.r3.u64 = model;
          t.r4.u64 = view_projection;
          t.r5.u64 = world;
          t.r6.u64 = shape;
        },
        __imp__sub_825951E0, hooks::SceneryListsIsShapeVisible, {}, fmt::format("case {}", c),
        false, [&](const PPCContext& t, Outputs& out) {
          // The product the original leaves in its frame.
          out.Floats(h.base, t.r1.u32 - 96, 16);
          visible[t.r3.u32 & 1]++;
        });
  }
  ITC_LOG_INFO("harness: Model::IsShapeVisible: results 0, 1: {}, {}", visible[0] / 2,
               visible[1] / 2);
  results.Report();
}

// The command list at [0x8296B918], in the block at +0x3000, its slots from
// +0x8000: the next chunk's start and the write pointer there, a chunk open
// on it or another list now and then, room for `slots` (sometimes just).
void RandomList(uint8_t* base, uint32_t block, Random& r, uint32_t slots) {
  const uint32_t list = block + 0x3000;
  guest::StoreU32(base, scenery_lists::kRecordList, list);
  const uint32_t start = block + 0x8000;
  guest::StoreU32(base, list + 32, start);
  guest::StoreU32(base, list + 44, start);
  const uint32_t capacity = 1000;
  guest::StoreU32(base, list + 8, capacity);
  // Never too few: the original then writes through 0 (a full list's
  // AllocateSlots returns 0); the plain C++ declines those for it.
  guest::StoreU32(base, list + 12, r.Below(8) == 0 ? capacity - slots - 1 : r.Below(64));
  const bool open = r.Below(3) == 0;
  guest::StoreU8(base, list + 52, open ? 1 : 0);
  guest::StoreU32(base, 0x8296BD28, open ? 1 : r.Below(8) == 0 ? 1 : 0);
}

void Recorders(Context& h, Random& r, uint32_t block) {
  const uint32_t slots = block + 0x8000;
  {
    Results results("Recorder::RecordObject (sub_825953D0)");
    for (int c = 0; c < 256; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            RandomList(h.base, block, r, 11);
            const uint32_t view_projection = block + 0x400 + 16 * r.Below(4);
            const uint32_t world = block + 0x500 + 16 * r.Below(4);
            RandomFloats(h.base, view_projection, 16, r);
            RandomFloats(h.base, world, 16, r);
            RandomBytes(h.base, 0x8265ECE0, 16, r);
            t.r3.u64 = block + 0x100;
            t.r4.u64 = view_projection;
            t.r5.u64 = world;
          },
          __imp__sub_825953D0, hooks::SceneryListsRecordObjectSteps,
          {{slots + 16 - block, 32}}, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("Recorder::RecordSceneLights (sub_82595490)");
    for (int c = 0; c < 256; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            g_flush_on_entry = r.Below(2) != 0;
            RandomList(h.base, block, r, 65);
            RandomFloats(h.base, 0x8265ECA0, 0x70 / 4, r);
            RandomFloats(h.base, 0x8296B940, 0x3A0 / 4, r);
            // Zero vectors now and then among the normalized ones.
            for (uint32_t v = 0; v < 11; v++) {
              if (r.Below(6) == 0) {
                const uint32_t at = v < 3 ? 0x8296B950 + 16 * v : 0x8296BBE0 + 16 * (v - 3);
                StoreFloats(h.base, at, {r.Below(2) ? -0.0f : 0.0f, 0, 0, Spread(r, 1)});
              }
            }
            t.r3.u64 = block + 0x100;
            t.r4.u64 = block + 0x500;
          },
          __imp__sub_82595490, hooks::SceneryListsRecordSceneLights,
          {{slots + 16 + 80 - block, 12}, {slots + 16 + 384 - block, 32},
           {slots + 16 + 640 - block, 92}},
          fmt::format("case {}", c));
    }
    results.Report();
  }
}

// A model at the block's +0x100 with four shapes from +0x2000 (544 bytes
// each), the shape `shape`'s material block at +0x6000 (up to six
// materials) and index buffer object at +0x5000, its corners about the
// origin (in view of an identity view-projection) or anywhere.
void RandomShape(uint8_t* base, uint32_t block, Random& r, uint32_t shape, bool in_view) {
  const uint32_t model = block + 0x100;
  guest::StoreU32(base, model + 28, r.Below(8) == 0 ? 0 : 1 + r.Below(4));
  guest::StoreU32(base, model + 40, block + 0x2000);
  const uint32_t at = block + 0x2000 + 544 * shape;
  for (uint32_t i = 0; i < 8; i++) {
    if (in_view) {
      StoreFloats(base, at + 96 + 16 * i,
                  {Spread(r, 0.9f), Spread(r, 0.9f), Spread(r, 0.9f), 1.0f});
    } else {
      RandomFloats(base, at + 96 + 16 * i, 4, r);
    }
  }
  guest::StoreU32(base, at + 260, r.Below(2) ? 11 : r.Below(16));
  guest::StoreU32(base, at + 428, r.Below(2) ? 1 : r.Below(4));
  guest::StoreU32(base, at + 408, block + 0x5000);
  guest::StoreU32(base, block + 0x5000 + 20, r.Below(3) == 0 ? 0 : uint32_t(r.Next()));
  const uint32_t materials = block + 0x6000;
  guest::StoreU32(base, at + 396, materials);
  const uint32_t first = r.Below(8) == 0 ? 0 : r.Below(4);
  const uint32_t second = r.Below(8) == 0 ? 0 : r.Below(3);
  guest::StoreU32(base, materials + 8, first);
  guest::StoreU32(base, materials + 12, second);
  for (uint32_t m = 0; m < first + second; m++) {
    const uint32_t material = materials + 16 + 512 * m;
    // +388 set now and then, +404 a few bits.
    guest::StoreU32(base, material + 388, r.Below(3) == 0 ? 1 + r.Below(4) : 0);
    guest::StoreU32(base, material + 404, 1u << r.Below(4));
  }
}

void DepthDraws(Context& h, Random& r, uint32_t block) {
  const uint32_t model = block + 0x100;
  {
    Results results("Model::RecordShapeDepth (sub_825960D0)");
    for (int c = 0; c < 256; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            RandomList(h.base, block, r, 70);
            const uint32_t shape = r.Below(4);
            RandomShape(h.base, block, r, shape, false);
            RandomBytes(h.base, 0x8296B8C0, 16, r);
            RandomBytes(h.base, 0x8265ED10, 16, r);
            t.r3.u64 = model;
            t.r4.u64 = shape;
            t.r5.u64 = r.Below(2);
            t.r6.u64 = r.Next();
            t.r7.u64 = r.Below(2) ? 0 : 1u << r.Below(4);
            t.r8.u64 = r.Next();
          },
          __imp__sub_825960D0, hooks::SceneryListsRecordShapeDepth, {},
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("Model::DrawShapeDepth (sub_82596D48)");
    // The render queue: a vtable whose slot 4 does nothing.
    const uint32_t queue = block + 0x700;
    int submitted = 0;
    for (int c = 0; c < 512; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            g_flush_on_entry = r.Below(2) != 0;
            RandomList(h.base, block, r, 80);
            const uint32_t shape = r.Below(4);
            const bool in_view = r.Below(4) != 0;
            RandomShape(h.base, block, r, shape, in_view);
            RandomBytes(h.base, 0x8296B8C0, 16, r);
            RandomBytes(h.base, 0x8265ECE0, 64, r);
            RandomBytes(h.base, 0x8296B8FC, 4, r);
            guest::StoreU32(h.base, scenery_lists::kRenderQueue, queue);
            guest::StoreU32(h.base, queue, queue + 0x20);
            guest::StoreU32(h.base, queue + 0x20 + 16, kEmptyFunction);
            const uint32_t view_projection = block + 0x400;
            const uint32_t world = block + 0x500;
            if (in_view) {
              StoreFloats(h.base, view_projection, {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1});
              StoreFloats(h.base, world, {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, Spread(r, 0.1f),
                                          Spread(r, 0.1f), Spread(r, 0.1f), 1});
            } else {
              RandomFloats(h.base, view_projection, 16, r);
              RandomFloats(h.base, world, 16, r);
            }
            t.r3.u64 = model;
            t.r4.u64 = view_projection;
            t.r5.u64 = world;
            t.r6.u64 = shape;
            t.r7.u64 = r.Below(2) ? r.Next() : uint64_t(r.Below(2));
            t.r8.u64 = r.Below(2) ? 0 : 1u << r.Below(4);
            t.r9.u64 = r.Next();
          },
          __imp__sub_82596D48, hooks::SceneryListsDrawShapeDepth,
          {{0x8000 + 16, 32}, {0x400, 16}, {0x500, 16}}, fmt::format("case {}", c), false,
          [&](const PPCContext& t, Outputs& out) {
            // The product IsShapeVisible leaves in its frame (r1 - 256).
            out.Floats(h.base, t.r1.u32 - 256, 16);
            // What went to the render queue (its slot 4 left them).
            if (t.r3.u32 == queue) {
              submitted++;
              out.Word(t.r4.u32);
              out.Word(t.r5.u32);
              out.Double(t.f1.u64);
            }
          });
    }
    ITC_LOG_INFO("harness: Model::DrawShapeDepth: {} of 512 cases handed to the render queue",
                 submitted / 2);
    results.Report();
  }
}

// A finite float, a few units either way.
float Finite(Random& r) {
  return Spread(r, 8.0f);
}
void FiniteFloats(uint8_t* base, uint32_t address, uint32_t count, Random& r) {
  for (uint32_t i = 0; i < count; i++) {
    guest::StoreFloat(base, address + 4 * i, Finite(r));
  }
}

// The shapes' materials as SceneryModel::RecordShape reads them (finite
// floats: everything is compared exactly), the model's pair table, color
// table, animation set, vtable (slot 1 the empty function) and texture
// provider (slots 5 and 6 the empty function), and the tables the vertex
// declaration and shared shaders read.
void RandomMaterials(uint8_t* base, uint32_t block, Random& r, uint32_t shape) {
  const uint32_t model = block + 0x100;
  const uint32_t at = block + 0x2000 + 544 * shape;
  // The model.
  guest::StoreU32(base, model, block + 0x740);
  guest::StoreU32(base, block + 0x740 + 4, kEmptyFunction);
  guest::StoreU32(base, model + 24, block + 0x800);
  RandomBytes(base, block + 0x800, 0x100, r);
  guest::StoreU32(base, model + 320, block + 0xA00);
  FiniteFloats(base, block + 0xA00, 0x80 / 4, r);
  const bool provider = r.Below(6) != 0;
  guest::StoreU32(base, model + 48, provider ? block + 0x700 : 0);
  guest::StoreU32(base, block + 0x700, block + 0x780);
  guest::StoreU32(base, block + 0x780 + 20, kEmptyFunction);
  guest::StoreU32(base, block + 0x780 + 24, kEmptyFunction);
  guest::StoreU32(base, model + 432, uint32_t(r.Next()));
  // An animation set of four animations with up to four frames, or none.
  const uint32_t set = block + 0xC00;
  guest::StoreU32(base, model + 436, r.Below(2) ? set : 0);
  guest::StoreU32(base, set, 4);
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU32(base, set + 4 + 4 * i, 0x40 * i);
    const uint32_t animation = set + 20 + 0x40 * i;
    guest::StoreU32(base, animation, r.Below(5));
    guest::StoreU32(base, animation + 4, r.Below(5));
  }
  // The shape's streams and declaration types: 5, 9, 10 or 11 (now and
  // then another, which the original prints a message for), and the
  // second stream kind now and then 6.
  guest::StoreU32(base, at + 244, r.Below(18));
  static constexpr uint32_t kTypes[] = {5, 9, 10, 11};
  guest::StoreU32(base, at + 260, r.Below(16) == 0 ? 7 : kTypes[r.Below(4)]);
  guest::StoreU32(base, at + 276, r.Below(4) == 0 ? 6 : r.Below(18));
  for (uint32_t i = 0; i < 18; i++) {
    guest::StoreU32(base, 0x8265EC00 + 4 * i, r.Below(8));
    guest::StoreU32(base, 0x8265EC00 + 72 + 4 * i, r.Below(3));
  }
  RandomBytes(base, 0x8296B7B0, 0x140, r);
  guest::StoreU32(base, 0x8296B7B4, r.Below(8) == 0 ? 0 : block + 0x900);
  guest::StoreU32(base, 0x8296B7B8, r.Below(8) == 0 ? 0 : block + 0x980);
  // The materials (RandomShape wrote their counts).
  const uint32_t materials = block + 0x6000;
  const uint32_t count = guest::LoadU32(base, materials + 8) + guest::LoadU32(base, materials + 12);
  for (uint32_t m = 0; m < count; m++) {
    const uint32_t material = materials + 16 + 512 * m;
    FiniteFloats(base, material, 32, r);  // +0 to +127: the constants
    guest::StoreU32(base, material + 96, r.Below(16));
    guest::StoreU32(base, material + 104, uint32_t(r.Next()) & (provider ? 6u : 0u));
    guest::StoreU32(base, material + 108, r.Below(8));
    const uint32_t textures = r.Below(16) == 0 ? 5 : r.Below(5);
    guest::StoreU32(base, material + 128, textures);
    // Five, for the cases with five textures (which the plain C++ leaves to
    // the original).
    for (uint32_t j = 0; j < 5; j++) {
      guest::StoreU32(base, material + 144 + 4 * j, r.Below(2) ? 0 : uint32_t(r.Next()));
      guest::StoreU32(base, material + 176 + 4 * j, r.Below(4));
      guest::StoreU32(base, material + 208 + 4 * j, r.Below(3) == 0 ? 0xFFFFFFFFu : r.Below(8));
      guest::StoreFloat(base, material + 240 + 4 * j, Finite(r));
    }
    guest::StoreU32(base, material + 392, r.Below(2) ? 0 : 1 + r.Below(4));
    guest::StoreU32(base, material + 396, r.Below(16) == 0 ? 0 : 0xFFFFFFFFu);
  }
}

void SceneryShapes(Context& h, Random& r, uint32_t block) {
  const uint32_t model = block + 0x100;
  const uint32_t queue = block + 0x7C0;  // the provider is at +0x700
  {
    Results results("SceneryModel::RecordShape (sub_82146BD0)");
    for (int c = 0; c < 512; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            g_flush_on_entry = r.Below(2) != 0;
            RandomList(h.base, block, r, 400);
            const uint32_t shape = r.Below(4);
            RandomShape(h.base, block, r, shape, false);
            RandomMaterials(h.base, block, r, shape);
            FiniteFloats(h.base, 0x8265ED20, 8, r);
            FiniteFloats(h.base, block + 0x600, 8, r);
            t.r3.u64 = model;
            t.r4.u64 = shape;
            t.r5.u64 = r.Below(2);
            t.r6.u64 = r.Below(4) == 0 ? 0 : 1;
            t.r7.u64 = r.Below(2) ? 0 : block + 0x600;
            t.r8.u64 = r.Below(2) ? 0 : block + 0x610;
            t.r9.u64 = r.Below(3);
          },
          __imp__sub_82146BD0, hooks::SceneryListsRecordShape, {}, fmt::format("case {}", c),
          false, [&](const PPCContext& t, Outputs& out) {
            // The original's stack temporaries (r1 - 304 + 80 to + 128).
            out.Bytes(Read(h.base, t.r1.u32 - 304 + 80, 48));
          });
    }
    results.Report();
  }
  {
    Results results("SceneryModel::DrawShape (sub_82147D88)");
    int queued = 0;
    for (int c = 0; c < 512; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            g_flush_on_entry = r.Below(2) != 0;
            RandomList(h.base, block, r, 600);
            const uint32_t shape = r.Below(4);
            const bool in_view = r.Below(4) != 0;
            RandomShape(h.base, block, r, shape, in_view);
            RandomMaterials(h.base, block, r, shape);
            FiniteFloats(h.base, 0x8265ECA0, 0xA0 / 4, r);
            FiniteFloats(h.base, 0x8296B940, 0x3A0 / 4, r);
            FiniteFloats(h.base, block + 0x600, 8, r);
            RandomBytes(h.base, 0x8296B8F0, 16, r);
            RandomBytes(h.base, 0x8296B900, 16, r);
            guest::StoreU32(h.base, scenery_lists::kRenderQueue, queue);
            guest::StoreU32(h.base, queue, queue + 0x20);
            guest::StoreU32(h.base, queue + 0x20 + 16, kEmptyFunction);
            const uint32_t view_projection = block + 0x400;
            const uint32_t world = block + 0x500;
            StoreFloats(h.base, view_projection, {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1});
            StoreFloats(h.base, world, {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, Spread(r, 0.1f),
                                        Spread(r, 0.1f), in_view ? 0.0f : 9.0f, 1});
            t.r3.u64 = model;
            t.r4.u64 = view_projection;
            t.r5.u64 = world;
            t.r6.u64 = shape;
            t.r7.u64 = r.Below(2);
            t.r8.u64 = r.Below(2) ? 0 : block + 0x600;
            t.r9.u64 = r.Below(2) ? 0 : block + 0x610;
            t.r10.u64 = r.Below(3);
          },
          __imp__sub_82147D88, hooks::SceneryListsDrawShape, {}, fmt::format("case {}", c),
          false, [&](const PPCContext& t, Outputs& out) {
            // IsShapeVisible's product (r1 - 208 - 96) and RecordShape's
            // stack temporaries (r1 - 208 - 304 + 80 to + 128).
            out.Bytes(Read(h.base, t.r1.u32 - 304, 64));
            out.Bytes(Read(h.base, t.r1.u32 - 512 + 80, 48));
            if (t.r3.u32 == queue) {
              queued++;
              out.Word(t.r4.u32);
              out.Word(t.r5.u32);
              out.Double(t.f1.u64);
            }
          });
    }
    ITC_LOG_INFO("harness: SceneryModel::DrawShape: {} of 512 cases ended handing an entry to "
                 "the render queue",
                 queued / 2);
    results.Report();
  }
}

// Model::RecordShape reads the environment textures' ids (+124, +120) as
// words and skips them when either is -1: now and then one is. The model's
// material animations (+364, read by sub_825A1148 for a material whose +396
// isn't -1, which the plain C++ leaves to the original): none, as in play.
void RandomEnvironmentIds(uint8_t* base, uint32_t block, Random& r) {
  guest::StoreU32(base, block + 0x100 + 364, 0);
  const uint32_t materials = block + 0x6000;
  const uint32_t count = guest::LoadU32(base, materials + 8) + guest::LoadU32(base, materials + 12);
  for (uint32_t m = 0; m < count && m < 8; m++) {
    const uint32_t material = materials + 16 + 512 * m;
    if (r.Below(4) == 0) {
      guest::StoreU32(base, material + (r.Below(2) ? 120 : 124), 0xFFFFFFFFu);
    }
  }
}

void ModelShapes(Context& h, Random& r, uint32_t block) {
  const uint32_t model = block + 0x100;
  const uint32_t queue = block + 0x7C0;  // the provider is at +0x700
  {
    Results results("Model::RecordShape (sub_82595550)");
    uint32_t stack = 0;
    for (int c = 0; c < 512; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            g_flush_on_entry = r.Below(2) != 0;
            stack = t.r1.u32;
            RandomList(h.base, block, r, 400);
            const uint32_t shape = r.Below(4);
            RandomShape(h.base, block, r, shape, false);
            RandomMaterials(h.base, block, r, shape);
            RandomEnvironmentIds(h.base, block, r);
            FiniteFloats(h.base, 0x8265ED20, 8, r);
            FiniteFloats(h.base, block + 0x600, 8, r);
            t.r3.u64 = model;
            t.r4.u64 = shape;
            t.r5.u64 = r.Below(2);
            t.r6.u64 = r.Below(4) == 0 ? 0 : 1;
            t.r7.u64 = r.Below(2) ? 0 : block + 0x600;
            t.r8.u64 = r.Below(2) ? 0 : block + 0x610;
          },
          __imp__sub_82595550, hooks::SceneryListsModelRecordShape, {},
          fmt::format("case {}", c), false,
          [&](const PPCContext& t, Outputs& out) {
            // The stack temporaries (r1 - 304 + 80 to + 128) and the
            // arguments saved in the caller's frame (r1 + 28 to + 64).
            out.Bytes(Read(h.base, t.r1.u32 - 304 + 80, 48));
            out.Bytes(Read(h.base, t.r1.u32 + 28, 36));
          },
          [&] { Fill(h.base, stack, 64); });
    }
    results.Report();
  }
  {
    Results results("Model::DrawShape (sub_82596B18)");
    int queued = 0;
    // The recompiled Model::DrawShape calls the recompiled Model::RecordShape
    // (--native_models off for it).
    const bool models_flag = ITC_FLAG(native_models);
    int version = 0;
    bool with_materials = false;
    for (int c = 0; c < 512; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            g_flush_on_entry = r.Below(2) != 0;
            RandomList(h.base, block, r, 600);
            const uint32_t shape = r.Below(4);
            const bool in_view = r.Below(4) != 0;
            RandomShape(h.base, block, r, shape, in_view);
            RandomMaterials(h.base, block, r, shape);
            RandomEnvironmentIds(h.base, block, r);
            with_materials = guest::LoadU32(h.base, block + 0x6000 + 8) != 0 ||
                             guest::LoadU32(h.base, block + 0x6000 + 12) != 0;
            FiniteFloats(h.base, 0x8265ECA0, 0xA0 / 4, r);
            FiniteFloats(h.base, 0x8296B940, 0x3A0 / 4, r);
            FiniteFloats(h.base, block + 0x600, 8, r);
            RandomBytes(h.base, 0x8296B8F0, 16, r);
            RandomBytes(h.base, 0x8296B900, 16, r);
            guest::StoreU32(h.base, scenery_lists::kRenderQueue, queue);
            guest::StoreU32(h.base, queue, queue + 0x20);
            guest::StoreU32(h.base, queue + 0x20 + 16, kEmptyFunction);
            const uint32_t view_projection = block + 0x400;
            const uint32_t world = block + 0x500;
            StoreFloats(h.base, view_projection, {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1});
            StoreFloats(h.base, world, {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, Spread(r, 0.1f),
                                        Spread(r, 0.1f), in_view ? 0.0f : 9.0f, 1});
            t.r3.u64 = model;
            t.r4.u64 = view_projection;
            t.r5.u64 = world;
            t.r6.u64 = shape;
            t.r7.u64 = r.Below(4) == 0 ? r.Next() : r.Below(2);
            t.r8.u64 = r.Below(2) ? 0 : block + 0x600;
            t.r9.u64 = r.Below(2) ? 0 : block + 0x610;
          },
          __imp__sub_82596B18, hooks::SceneryListsModelDrawShape, {}, fmt::format("case {}", c),
          false, [&](const PPCContext& t, Outputs& out) {
            ITC_FLAG(native_models) = models_flag;
            // IsShapeVisible's product (r1 - 208 - 96) when no RecordShape
            // ran after it (the recompiled one saves its registers over it),
            // RecordShape's stack temporaries (r1 - 208 - 304 + 80 to + 128)
            // and the arguments it saves in the draw's frame (r1 - 208 + 28
            // to + 64).
            if (t.r3.u32 == model || t.r3.u32 == 0 || !with_materials) {
              out.Bytes(Read(h.base, t.r1.u32 - 304, 64));
            }
            out.Bytes(Read(h.base, t.r1.u32 - 512 + 80, 48));
            out.Bytes(Read(h.base, t.r1.u32 - 208 + 28, 36));
            if (t.r3.u32 == queue) {
              queued++;
              out.Word(t.r4.u32);
              out.Word(t.r5.u32);
              out.Double(t.f1.u64);
            }
          },
          [&] { ITC_FLAG(native_models) = version++ % 2 == 1; });
    }
    ITC_LOG_INFO("harness: Model::DrawShape: {} of 512 cases ended handing an entry to the "
                 "render queue",
                 queued / 2);
    ITC_FLAG(native_models) = models_flag;
    results.Report();
  }
}

// PointList::Draw: a list at the block's +0xE00 whose two nodes both hold
// the model at +0x100 (shape 0), the entry's points at +0x1100 (most below
// the lowest y, so a few are drawn), the camera pass at +0xD00 (an identity
// view-projection), and the command list's slots in a buffer of their own
// (`slots`, compared up to the slots used).
void PointLists(Context& h, Random& r, uint32_t block, uint32_t slots) {
  constexpr uint32_t kSlotsBytes = 0x40000;
  const uint32_t model = block + 0x100;
  const uint32_t list = block + 0xE00;
  const uint32_t queue = block + 0x7C0;
  const uint32_t pass = block + 0xD00;
  Results results("PointList::Draw (sub_82148058)");
  int drawn = 0;
  for (int c = 0; c < 128; c++) {
    Case(
        h, results, block, r,
        [&](PPCContext& t) {
          g_flush_on_entry = r.Below(2) != 0;
          RandomList(h.base, block, r, 100);
          // The slots in their own buffer, room for all.
          const uint32_t record = guest::LoadU32(h.base, scenery_lists::kRecordList);
          guest::StoreU32(h.base, record + 32, slots);
          guest::StoreU32(h.base, record + 44, slots);
          guest::StoreU32(h.base, record + 8, kSlotsBytes / 16);
          RandomShape(h.base, block, r, 0, true);
          RandomMaterials(h.base, block, r, 0);
          FiniteFloats(h.base, 0x8265ECA0, 0xA0 / 4, r);
          FiniteFloats(h.base, 0x8296B940, 0x3A0 / 4, r);
          RandomBytes(h.base, 0x8296B8F0, 16, r);
          RandomBytes(h.base, 0x8296B900, 16, r);
          guest::StoreU32(h.base, scenery_lists::kRenderQueue, queue);
          guest::StoreU32(h.base, queue, queue + 0x20);
          guest::StoreU32(h.base, queue + 0x20 + 16, kEmptyFunction);
          StoreFloats(h.base, pass + 144, {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1});
          // The list: head node 3, then node 9, both holding the model.
          guest::StoreU16(h.base, list + 4 + 260, 3);
          guest::StoreU16(h.base, list + 4 + 12, 9);
          guest::StoreU16(h.base, list + 4 + 14, 5);
          guest::StoreU16(h.base, list + 4 + 36, 0xFFFF);
          guest::StoreU16(h.base, list + 4 + 38, 7);
          guest::StoreU32(h.base, list + 272 + 4 * 5, model);
          guest::StoreU32(h.base, list + 272 + 4 * 7, model);
          guest::StoreU32(h.base, list + 608, r.Below(2) ? block + 0xC00 : 0);
          guest::StoreU32(h.base, list + 612, r.Below(6) ? block + 0x700 : 0);
          guest::StoreU32(h.base, list + 616, r.Below(5) - 1);
          const uint32_t entry = r.Below(20);
          const uint32_t points = block + 0x1100;
          guest::StoreU32(h.base, list + 528 + 4 * entry, points);
          for (uint32_t i = 0; i < 16; i++) {
            StoreFloats(h.base, points + 16 * i,
                        {Spread(r, 0.3f), r.Below(5) == 0 ? Spread(r, 0.3f) : -2000.0f,
                         Spread(r, 0.3f), Spread(r, 4.0f)});
          }
          // The material flags that call the provider only with one.
          if (guest::LoadU32(h.base, list + 612) == 0) {
            const uint32_t materials = block + 0x6000;
            for (uint32_t m = 0; m < 6; m++) {
              guest::StoreU32(h.base, materials + 16 + 512 * m + 104, 0);
            }
          }
          t.r3.u64 = list;
          t.r4.u64 = entry;
          t.r5.u64 = pass;
        },
        __imp__sub_82148058, hooks::SceneryListsPointListDraw, {}, fmt::format("case {}", c),
        false,
        [&](const PPCContext& t, Outputs& out) {
          const uint32_t record = guest::LoadU32(h.base, scenery_lists::kRecordList);
          const uint32_t end = guest::LoadU32(h.base, record + 44);
          out.Word(end - slots);
          out.Bytes(Read(h.base, slots, std::min(end - slots, kSlotsBytes)));
          // The frame's temporaries (r1 - 464 + 80 to + 256).
          out.Bytes(Read(h.base, t.r1.u32 - 464 + 80, 176));
          drawn += t.r3.u32 == queue;
        },
        [&] { Fill(h.base, slots, kSlotsBytes, 0xCD); });
  }
  ITC_LOG_INFO("harness: PointList::Draw: {} of 128 cases ended with a draw handed to the queue",
               drawn / 2);
  results.Report();
}

// --- ModelList::Load and LoadPoints ---

// A word inside one of a side's blocks (or up to 16 bytes past its end) made
// "region r, offset o", as the blocks sit at different addresses on each
// side.
struct HeapRegion {
  uint32_t start, bytes;
};
uint32_t Normalized(uint32_t word, const std::vector<HeapRegion>& regions) {
  for (size_t r = 0; r < regions.size(); r++) {
    if (word >= regions[r].start && word - regions[r].start < regions[r].bytes + 16) {
      return 0xB0000000u | (uint32_t(r) << 20) | (word - regions[r].start);
    }
  }
  return word;
}
void AddNormalized(Outputs& out, uint8_t* base, const std::vector<HeapRegion>& regions,
                   uint32_t start, uint32_t bytes) {
  std::vector<uint8_t> data = Read(base, start, bytes);
  for (uint32_t at = 0; at + 4 <= bytes; at += 4) {
    const uint32_t word = Normalized(guest::LoadU32(base, start + at), regions);
    for (int k = 0; k < 4; k++) data[at + k] = uint8_t(word >> (24 - 8 * k));
  }
  out.Bytes(data);
}

void RequireModel(bool done) {
  if (!done) {
    ITC_LOG_ERROR("harness: scenery_lists: a model's teardown declined");
    std::abort();
  }
}

// ModelList::Load and LoadPoints: a container of 0 to 4 part files (no
// shapes, shader pairs or skeleton; the wrong version now and then; a count
// not above 0 now and then) and a list of random bytes. Compared: r3, the
// list's 624 bytes, the frame's model word and each model's first 432 bytes
// (the scenery model's last 16 the original leaves as the heap gave them)
// with its blocks (the header copy, the vertex data, the texture provider's
// slots), the pointers into those made offsets; then all freed.
void ModelListLoads(Context& h, Random& r) {
  Results results("ModelList::Load (sub_82148428) and LoadPoints (sub_82148528)");
  if (!hooks::ModelsNativeLifetime()) {
    ITC_LOG_INFO("harness: ModelList::Load: skipped (Model::Load isn't native in this run)");
    return;
  }
  constexpr uint32_t kFileBytes = 0x5000;
  constexpr uint32_t kHeader = 0x4000;
  constexpr uint32_t kVertices = 0x800;
  constexpr uint32_t kListBytes = 624;
  constexpr uint32_t kMaxModels = 4;
  const uint32_t allocation = memory::Alloc(0x100 + kMaxModels * kFileBytes + kListBytes + 32, 0);
  const uint32_t container = (allocation + 15) & ~15u;
  const uint32_t list = container + 0x100 + kMaxModels * kFileBytes;
  for (int c = 0; c < 40; c++) {
    const bool points = c % 2 == 1;
    const uint32_t models = c < 10 ? uint32_t(c / 2) % (kMaxModels + 1) : r.Below(kMaxModels + 1);
    const uint32_t count = c % 13 == 12 ? 0x80000000u | r.Below(4) : models;
    std::vector<uint32_t> words((0x100 + kMaxModels * kFileBytes) / 4, 0);
    auto put = [&](uint32_t at, uint32_t value) { words[at / 4] = value; };
    put(0, __builtin_bswap32(count));
    std::vector<bool> wrong(kMaxModels);
    for (uint32_t i = 0; i < kMaxModels; i++) {
      const uint32_t file = 0x100 + i * kFileBytes;
      put(4 + 4 * i, __builtin_bswap32(file));
      wrong[i] = r.Below(5) == 0;
      put(file + 0, wrong[i] ? 0x00140000 : 0x00150000);
      put(file + 4, 7);
      put(file + 8, 9);
      put(file + 16, 3);
      put(file + 20, 0x80);
      put(file + 24, 0x90);
      put(file + 28, 0x100);
      put(file + 32, 0xA0);
      put(file + 44, 0x2000);
      put(file + 64, kHeader);
      put(file + 68, kVertices);
      put(file + 72, kHeader + kVertices);
      for (uint32_t k = kHeader; k < kHeader + kVertices; k += 4) {
        put(file + k, uint32_t(r.Next()) & 0x0FFFFFFFu);
      }
    }
    std::vector<uint32_t> list_words(kListBytes / 4);
    for (auto& w : list_words) w = uint32_t(r.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      for (uint32_t k = 0; k < words.size(); k++) guest::StoreU32(h.base, container + 4 * k, words[k]);
      for (uint32_t k = 0; k < list_words.size(); k++) {
        guest::StoreU32(h.base, list + 4 * k, list_words[k]);
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = list;
      t.r4.u64 = container;
      if (points) {
        (v == 0 ? __imp__sub_82148528 : hooks::SceneryListsModelListLoadPoints)(t, h.base);
      } else {
        (v == 0 ? __imp__sub_82148428 : hooks::SceneryListsModelListLoad)(t, h.base);
      }
      const uint32_t frame = t.r1.u32 - (points ? 192 + 160 : 160);
      // The models, by the list's order (its data slots).
      const uint32_t made = int32_t(count) > 0 ? count : 0;
      std::vector<uint32_t> list_models;
      std::vector<HeapRegion> regions;
      for (uint32_t i = 0; i < made; i++) {
        const uint32_t model = guest::LoadU32(h.base, list + 4 + 268 + 4 * i);
        list_models.push_back(model);
        regions.push_back({model, scenery_lists::kSceneryModelBytes});
      }
      for (uint32_t model : list_models) {
        auto add = [&](uint32_t start, uint32_t bytes) {
          if (start != 0) regions.push_back({start, bytes});
        };
        add(guest::LoadU32(h.base, model + 16), kHeader);
        add(guest::LoadU32(h.base, model + 20), kVertices);
        add(guest::LoadU32(h.base, model + 88), 256);
      }
      out[v].Word(t.r3.u32);
      AddNormalized(out[v], h.base, regions, list, kListBytes);
      out[v].Word(Normalized(guest::LoadU32(h.base, frame + 80), regions));
      for (const HeapRegion& region : regions) {
        const bool is_model = std::find(list_models.begin(), list_models.end(), region.start) !=
                              list_models.end();
        AddNormalized(out[v], h.base, regions, region.start, is_model ? 432 : region.bytes);
      }
      // Freed: each model's blocks, then the model.
      for (uint32_t model : list_models) {
        const uint32_t header = guest::LoadU32(h.base, model + 16);
        const uint32_t vertices = guest::LoadU32(h.base, model + 20);
        if (header != 0) memory::FreePages(header - models::kBlockHeaderBytes);
        if (vertices != 0) memory::FreePhysical(vertices - models::kBlockHeaderBytes);
        for (uint32_t offset : {16u, 20u, 24u, 420u}) guest::StoreU32(h.base, model + offset, 0);
        RequireModel(models::ModelDestruct(h.base, model).has_value());
        RequireModel(memory::crt::OperatorDelete(model));
      }
    }
    std::string wrongs;
    for (uint32_t i = 0; i < models; i++) wrongs += wrong[i] ? "w" : "-";
    results.Check(out[0], out[1],
                  fmt::format("case {} ({}, count {:#x}, versions {})", c,
                              points ? "LoadPoints" : "Load", count, wrongs));
  }
  memory::Free(allocation);
  results.Report();
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
  Visibility(h, r, block);
  Recorders(h, r, block);
  DepthDraws(h, r, block);
  SceneryShapes(h, r, block);
  ModelShapes(h, r, block);
  const uint32_t slots = memory::Alloc(0x40000 + 16, 0);
  PointLists(h, r, block, (slots + 15) & ~15u);
  ModelListLoads(h, r);
  memory::Free(slots);
  memory::Free(allocation);
}

}  // namespace harness
