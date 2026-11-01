// The scene's loading objects (src/scene_loaders/,
// docs/functions/scene-loaders.md) against the recompiled functions. Each
// case fills a block of guest memory with random bytes, writes its inputs
// there, runs the original and the register steps (hooks/scene_loaders.h)
// from the same inputs, and compares the block, the globals the functions
// read or write and r3. The globals are put back after each case. None of
// these functions loads or stores a float, so everything is compared
// exactly.

#include "harness.h"
#include "car_model_inputs.h"
#include "pending_frees.h"

#include "base/flags.h"
#include "base/log.h"
#include "car_models/load_slots.h"
#include "car_models/vinyl.h"
#include "graphics/resources.h"
#include "hooks/models.h"
#include "hooks/scene_loaders.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "models/texture_provider.h"
#include "scene_loaders/file_loaded.h"
#include "scene_loaders/loads.h"
#include "scene_loaders/model_point.h"
#include "model_animation/model_animation.h"
#include "scene_loaders/objects.h"
#include "models/memory_blocks.h"
#include "models/model.h"
#include "models/part_model.h"
#include "models/xpr.h"
#include "scene_loaders/pools.h"

#include <fmt/format.h>

#include <algorithm>
#include <bit>
#include <functional>
#include <mutex>
#include <unordered_set>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_scene_loaders);
ITC_FLAG_DECLARE(bool, native_car_models);
ITC_FLAG_DECLARE(bool, native_models);
ITC_FLAG_DECLARE(bool, native_accessories);

namespace harness {

namespace {

namespace sl = scene_loaders;

// The globals: the first pool set, the accessories' set.
struct Region {
  uint32_t address;
  uint32_t bytes;
};
constexpr Region kRegions[] = {
    {sl::kFirstSet, 4},
    {0x826C5510, 4},
};

// Big enough for the car parts (106,004 bytes) and what their part models
// point to.
constexpr uint32_t kBlockBytes = 0x20000;
constexpr uint32_t kSmallBytes = 0x2000;

void RandomBytes(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i < bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

// One case through both versions: `bytes` of the block filled at random and
// `setup(t)` writing the inputs and the registers (once: its inputs are
// written back for the second run).
void Case(Context& h, Results& results, uint32_t block, uint32_t bytes, Random& r,
          const std::function<void(PPCContext&)>& setup, GuestFunction recompiled,
          GuestFunction native, const std::string& what) {
  std::vector<std::vector<uint8_t>> saved;
  for (const Region& region : kRegions) {
    saved.push_back(Read(h.base, region.address, region.bytes));
  }
  RandomBytes(h.base, block, bytes, r);
  PPCContext start = CallContext(h);
  start.r1.u64 &= ~0xFull;
  setup(start);
  const std::vector<uint8_t> inputs = Read(h.base, block, bytes);
  std::vector<std::vector<uint8_t>> globals;
  for (const Region& region : kRegions) {
    globals.push_back(Read(h.base, region.address, region.bytes));
  }
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Write(h.base, block, inputs.data(), bytes);
    for (size_t i = 0; i < globals.size(); i++) {
      Write(h.base, kRegions[i].address, globals[i].data(), kRegions[i].bytes);
    }
    PPCContext t = start;
    (v == 0 ? recompiled : native)(t, h.base);
    out[v].Bytes(Read(h.base, block, bytes));
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

void Put(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// A slot state: mostly arrived or empty, now and then loading or another
// value.
uint32_t RandomState(Random& r, uint32_t loading_one_in) {
  if (r.Below(loading_one_in) == 0) {
    return 2;
  }
  const uint32_t states[] = {0, 1, 3, 3, 3, uint32_t(r.Next())};
  return states[r.Below(6)];
}

// A model set's word: 0 or 1 mostly, else 2 or random.
uint32_t RandomModelSet(Random& r) {
  const uint32_t sets[] = {0, 0, 1, 1, 2, uint32_t(r.Next())};
  return sets[r.Below(6)];
}

// --- The pool set ---

void Pools(Context& h, Random& r, uint32_t block) {
  struct Alloc {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
    bool make;  // MakeA (else sub_820D0960: r3 = where, r4 = the size)
  };
  const Alloc allocs[] = {
      {"ScenePools::MakeA (sub_820C3C40)", __imp__sub_820C3C40, hooks::SceneLoadersMakeA, true},
      {"sub_820D0960 (VirtualAlloc)", __imp__sub_820D0960, hooks::SceneLoadersAllocPages, false},
  };
  const uint32_t sizes[] = {0, 1, 80, 4095, 4096, 4097, 0x10000, 0x23456};
  for (const Alloc& alloc : allocs) {
    Results results(alloc.name);
    for (int c = 0; c < 16; c++) {
      const uint32_t size = alloc.make ? sl::kFirstSetBytes
                                       : (c < 8 ? sizes[c] : 1 + r.Below(0x40000));
      const uint32_t saved = guest::LoadU32(h.base, sl::kFirstSet);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        const uint32_t at = block + 4 * r.Below(16);
        PPCContext t = CallContext(h);
        t.r3.u64 = alloc.make ? r.Next() : at;
        t.r4.u64 = size;
        (v == 0 ? alloc.recompiled : alloc.native)(t, h.base);
        const uint32_t pages = t.r3.u32;
        const uint32_t stored = guest::LoadU32(h.base, alloc.make ? sl::kFirstSet : at);
        out[v].Word(stored == pages);
        out[v].Word(pages != 0);
        out[v].Word(pages != 0 && memory::IsPages(pages));
        out[v].Word(pages % 4096);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        bool cleared = true;
        for (uint32_t i = 0; pages != 0 && i < size; i++) {
          cleared = cleared && guest::LoadU8(h.base, pages + i) == 0;
        }
        out[v].Word(cleared);
        if (pages != 0 && memory::IsPages(pages)) {
          memory::FreePages(pages);
        }
        guest::StoreU32(h.base, sl::kFirstSet, saved);
      }
      results.Check(out[0], out[1], fmt::format("case {} (size {:#x})", c, size));
    }
    results.Report();
  }
}

// --- The pool node ---

// A list at `list` of `count` nodes (16 bytes each from `nodes`, linked by
// +8 and +12, each +4 the list), first and last at +0 and +4, the count at
// +8.
void MakeList(uint8_t* base, uint32_t list, uint32_t nodes, uint32_t count) {
  Put(base, list + 0, count == 0 ? 0 : nodes);
  Put(base, list + 4, count == 0 ? 0 : nodes + 16 * (count - 1));
  Put(base, list + 8, count);
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t node = nodes + 16 * i;
    Put(base, node + 4, list);
    Put(base, node + 8, i + 1 < count ? node + 16 : 0);
    Put(base, node + 12, i > 0 ? node - 16 : 0);
  }
}

void PoolNodes(Context& h, Random& r, uint32_t block) {
  {
    Results results("PoolNode::PoolNode (sub_82594050)");
    for (int c = 0; c < 48; c++) {
      Case(
          h, results, block, kSmallBytes, r,
          [&](PPCContext& t) {
            const uint32_t list = block + 0x100;
            const uint32_t count = c < 8 ? 0 : r.Below(5);
            MakeList(h.base, list, block + 0x200, count);
            // A count that disagrees with the links now and then.
            if (r.Below(8) == 0) {
              Put(h.base, list + 8, uint32_t(r.Next()));
            }
            t.r3.u64 = block + 0x800 + 16 * r.Below(64);
            t.r4.u64 = list;
          },
          __imp__sub_82594050, hooks::SceneLoadersPoolNodeConstruct, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("PoolNode::~PoolNode (sub_825940B0)");
    for (int c = 0; c < 64; c++) {
      Case(
          h, results, block, kSmallBytes, r,
          [&](PPCContext& t) {
            const uint32_t list = block + 0x100;
            const uint32_t nodes = block + 0x200;
            const uint32_t count = 1 + r.Below(5);
            MakeList(h.base, list, nodes, count);
            const uint32_t node = nodes + 16 * r.Below(count);
            if (c < 6) {
              Put(h.base, node + 4, 0);  // not in a list
            }
            t.r3.u64 = node;
          },
          __imp__sub_825940B0, hooks::SceneLoadersPoolNodeDestruct, fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- The queries ---

void Queries(Context& h, Random& r, uint32_t block) {
  {
    Results results("CarModel::IsLoaded (sub_820D3C28)");
    for (int c = 0; c < 64; c++) {
      Case(
          h, results, block, kSmallBytes, r,
          [&](PPCContext& t) {
            for (uint32_t offset : {3972u, 3996u, 3748u}) {
              Put(h.base, block + offset, RandomState(r, 4));
            }
            t.r3.u64 = block;
          },
          __imp__sub_820D3C28, hooks::SceneLoadersCarModelIsLoaded, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("Pool4Object::IsLoaded (sub_820D6A08)");
    for (int c = 0; c < 24; c++) {
      Case(
          h, results, block, kSmallBytes, r,
          [&](PPCContext& t) {
            Put(h.base, block + 100, RandomState(r, 3));
            t.r3.u64 = block;
          },
          __imp__sub_820D6A08, hooks::SceneLoadersPool4IsLoaded, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("Pool4Object::SetRecord (sub_820D6A60)");
    for (int c = 0; c < 24; c++) {
      Case(
          h, results, block, kSmallBytes, r,
          [&](PPCContext& t) {
            t.r3.u64 = block + 4 * r.Below(64);
            // Apart, or (now and then) overlapping the destination.
            t.r4.u64 = c % 6 == 5 ? t.r3.u32 + 28 + 4 * r.Below(8) - 16
                                  : block + 0x400 + r.Below(256);
          },
          __imp__sub_820D6A60, hooks::SceneLoadersPool4SetRecord, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("CarParts::SlotOf (sub_820D6D48)");
    for (int c = 0; c < 64; c++) {
      Case(
          h, results, block, 64, r,
          [&](PPCContext& t) {
            t.r3.u64 = r.Next();
            t.r4.u64 = c < 20 ? uint32_t(c) : (r.Below(4) == 0 ? r.Next() : r.Below(18));
            t.r5.u64 = r.Next();
            t.r6.u64 = r.Next();
          },
          __imp__sub_820D6D48, hooks::SceneLoadersPartsSlotOf, fmt::format("case {}", c));
    }
    results.Report();
  }
  // The car parts: the set at block + 0x1A000, its +8 the model set.
  const uint32_t set = block + 0x1A000;
  auto parts_setup = [&](uint32_t loading_one_in) {
    Put(h.base, block + sl::kPartsSet, set);
    Put(h.base, set + 8, RandomModelSet(r));
    for (uint32_t i = 0; i < sl::kPartsCount; i++) {
      Put(h.base, block + sl::kPartsStates + sl::kSlotBytes * i, RandomState(r, loading_one_in));
    }
  };
  {
    Results results("CarParts::Part (sub_820D6E10)");
    for (int c = 0; c < 96; c++) {
      Case(
          h, results, block, kBlockBytes, r,
          [&](PPCContext& t) {
            parts_setup(6);
            const uint32_t model_set = guest::LoadU32(h.base, set + 8);
            t.r3.u64 = block;
            // Kinds whose slots stay inside the parts: 0-16 in set a, below
            // 216 in set b (any in the others).
            t.r4.u64 = model_set == 1 ? r.Below(216) : r.Below(17);
            if (model_set == 1 && r.Below(3) == 0) {
              const uint32_t kinds[] = {7, 9, 14};
              t.r4.u64 = kinds[r.Below(3)];
            }
            t.r5.u64 = r.Below(5);
            t.r6.u64 = r.Below(4);
          },
          __imp__sub_820D6E10, hooks::SceneLoadersPartsPart, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("CarParts::IsLoaded (sub_820D7258)");
    for (int c = 0; c < 32; c++) {
      Case(
          h, results, block, kBlockBytes, r,
          [&](PPCContext& t) {
            // From none loading to several.
            parts_setup(c < 8 ? 0xFFFFFFFF : 20 + r.Below(400));
            t.r3.u64 = block;
          },
          __imp__sub_820D7258, hooks::SceneLoadersPartsIsLoaded, fmt::format("case {}", c));
    }
    results.Report();
  }
  // A part model at `model` with up to three shapes (544 bytes each from
  // `shapes`), each one's material block at `materials` + 64 x shape with
  // counts of 0 to 3.
  auto part_model = [&](uint32_t model, uint32_t shapes, uint32_t materials) {
    const int32_t counts[] = {0, 1, 2, 3, -1};
    Put(h.base, model + 412, uint32_t(counts[r.Below(5)]));
    Put(h.base, model + 40, shapes);
    for (uint32_t s = 0; s < 3; s++) {
      Put(h.base, shapes + 544 * s + 396, materials + 64 * s);
      Put(h.base, materials + 64 * s + 8, uint32_t(counts[r.Below(5)]));
      Put(h.base, materials + 64 * s + 12, uint32_t(counts[r.Below(5)]));
    }
  };
  {
    Results results("sub_820D2270");
    for (int c = 0; c < 48; c++) {
      Case(
          h, results, block, kSmallBytes, r,
          [&](PPCContext& t) {
            part_model(block, block + 0x400, block + 0x1000);
            t.r3.u64 = c < 4 ? 0 : block;
          },
          __imp__sub_820D2270, hooks::SceneLoadersPartWalkMaterials, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("CarParts::WalkMaterials (sub_820D6EB8)");
    for (int c = 0; c < 24; c++) {
      Case(
          h, results, block, kBlockBytes, r,
          [&](PPCContext& t) {
            parts_setup(6);
            for (uint32_t i = 0; i < sl::kPartsCount; i++) {
              part_model(block + sl::kPartsModels + sl::kPartModelBytes * i, block + 0x1A100,
                         block + 0x1AA00 + 0x100 * r.Below(4));
            }
            t.r3.u64 = block;
          },
          __imp__sub_820D6EB8, hooks::SceneLoadersPartsWalkMaterials, fmt::format("case {}", c));
    }
    results.Report();
  }
  // The paint walks: as WalkMaterials' cases (the cockpit's six part models
  // at +688 + 464i, their states at +556 + 24i), the colors in r4 to r9
  // (hue, saturation, value) for SetPaint's. Both sides reach the same
  // ApplyPalette and SetPaint (their adapters' plain C++), so the floats
  // they write are compared exactly.
  auto colors = [&](PPCContext& t) {
    PPCRegister* regs[6] = {&t.r4, &t.r5, &t.r6, &t.r7, &t.r8, &t.r9};
    for (int k = 0; k < 3; k++) {
      const float hue = float(r.Below(720)) - 180.0f;
      const float saturation = float(r.Below(1001)) / 1000.0f;
      const float value = float(r.Below(1001)) / 1000.0f;
      regs[2 * k]->u64 = (uint64_t(std::bit_cast<uint32_t>(hue)) << 32) |
                         std::bit_cast<uint32_t>(saturation);
      regs[2 * k + 1]->u64 =
          (uint64_t(std::bit_cast<uint32_t>(value)) << 32) | uint32_t(r.Next());
    }
  };
  struct Walk {
    const char* name;
    GuestFunction recompiled, native;
    bool cockpit, paint;
    uint32_t states = 556, models_at = 688;  // the six part models' (not the car parts)
  };
  const Walk walks[] = {
      {"sub_820D6F20 (CarParts' ApplyPalette walk)", __imp__sub_820D6F20,
       hooks::SceneLoadersPartsApplyPalette, false, false},
      {"sub_820D7388 (CarParts' SetPaint walk)", __imp__sub_820D7388,
       hooks::SceneLoadersPartsSetPaint, false, true},
      {"sub_820D5FC8 (the cockpit's ApplyPalette walk)", __imp__sub_820D5FC8,
       hooks::SceneLoadersCockpitApplyPalette, true, false},
      {"sub_820D6328 (the cockpit's SetPaint walk)", __imp__sub_820D6328,
       hooks::SceneLoadersCockpitSetPaint, true, true},
      {"sub_820D7690 (the interior's ApplyPalette walk)", __imp__sub_820D7690,
       hooks::SceneLoadersInteriorApplyPalette, true, false, 180, 304},
      {"sub_820D7840 (the interior's SetPaint walk)", __imp__sub_820D7840,
       hooks::SceneLoadersInteriorSetPaint, true, true, 180, 304},
      {"sub_820D7BF8 (the traffic car's ApplyPalette walk)", __imp__sub_820D7BF8,
       hooks::SceneLoadersTrafficApplyPalette, true, false, 4124, 4256},
  };
  for (const Walk& walk : walks) {
    Results results(walk.name);
    for (int c = 0; c < 24; c++) {
      Case(
          h, results, block, kBlockBytes, r,
          [&](PPCContext& t) {
            if (walk.cockpit) {
              for (uint32_t i = 0; i < 6; i++) {
                Put(h.base, block + walk.states + sl::kSlotBytes * i, RandomState(r, 6));
                part_model(block + walk.models_at + sl::kPartModelBytes * i, block + 0x1A100,
                           block + 0x1AA00 + 0x100 * r.Below(4));
              }
            } else {
              parts_setup(6);
              for (uint32_t i = 0; i < sl::kPartsCount; i++) {
                part_model(block + sl::kPartsModels + sl::kPartModelBytes * i, block + 0x1A100,
                           block + 0x1AA00 + 0x100 * r.Below(4));
              }
            }
            if (walk.paint) {
              colors(t);
            }
            t.r3.u64 = block;
          },
          walk.recompiled, walk.native, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // The car model at the block: +3712 the lookup (0-5), +7232 + 4i the six
    // words (-1 or an index), the first body XPR at +4000 (its records at
    // +0x2000, 16 bytes each, their +4 an offset; the count 0-8).
    Results results("CarModel::SelectedTexture (sub_820D4588)");
    for (int c = 0; c < 48; c++) {
      Case(
          h, results, block, kSmallBytes * 2, r,
          [&](PPCContext& t) {
            Put(h.base, block + 3712, r.Below(6));
            for (uint32_t i = 0; i < 6; i++) {
              Put(h.base, block + 7232 + 4 * i,
                  r.Below(3) == 0 ? 0xFFFFFFFFu : r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(10));
            }
            Put(h.base, block + 4000 + 16, block + 0x2000);
            Put(h.base, block + 4000 + 20, r.Below(9));
            t.r3.u64 = block;
          },
          __imp__sub_820D4588, hooks::SceneLoadersCarModelSelectedTexture,
          fmt::format("case {}", c));
    }
    results.Report();
  }
  // The accessories' set at block + 0x1800, its entries at block + 0x100 x
  // (1-3) (their +1096 0 or not).
  auto accessories = [&](uint32_t accessories_set) {
    for (uint32_t i = 0; i < 3; i++) {
      const uint32_t entry = block + 0x100 * (1 + i);
      Put(h.base, accessories_set + 4 + 4 * i, r.Below(3) == 0 ? 0 : entry);
      Put(h.base, entry + 1096, r.Below(4) == 0 ? 0 : uint32_t(r.Next()) | 1);
    }
  };
  {
    Results results("Accessories::AllLoaded (sub_820F07D8)");
    for (int c = 0; c < 48; c++) {
      Case(
          h, results, block, kSmallBytes, r,
          [&](PPCContext& t) {
            accessories(block + 0x1800);
            t.r3.u64 = block + 0x1800;
          },
          __imp__sub_820F07D8, hooks::SceneLoadersAccessoriesAllLoaded,
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("Interior::IsLoaded (sub_820D7790)");
    for (int c = 0; c < 64; c++) {
      Case(
          h, results, block, kBlockBytes, r,
          [&](PPCContext& t) {
            const uint32_t interior = block + 0x2000;
            Put(h.base, interior + 3092, set);
            Put(h.base, set + 8, RandomModelSet(r));
            for (uint32_t i = 0; i < 6; i++) {
              Put(h.base, interior + 180 + 24 * i, RandomState(r, 8));
            }
            Put(h.base, 0x826C5510, block + 0x1800);
            accessories(block + 0x1800);
            t.r3.u64 = interior;
          },
          __imp__sub_820D7790, hooks::SceneLoadersInteriorIsLoaded, fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- Names ---

// A name of letters (either case), digits, `_` and now and then a `*` or a
// byte with its top bit set.
std::string RandomName(Random& r, uint32_t length) {
  const char chars[] = "abcdefgABCDEFG019_*\x80\xE9";
  std::string name;
  for (uint32_t i = 0; i < length; i++) {
    name.push_back(chars[r.Below(sizeof(chars) - 1)]);
  }
  return name;
}

void PutString(uint8_t* base, uint32_t address, const std::string& s) {
  Write(base, address, s.c_str(), uint32_t(s.size() + 1));
}

// `name` with some letters' case flipped, some characters made `*`, or one
// changed.
std::string Variant(Random& r, std::string name) {
  for (char& ch : name) {
    switch (r.Below(8)) {
      case 0:
        if (ch >= 'a' && ch <= 'z') {
          ch = char(ch - 32);
        } else if (ch >= 'A' && ch <= 'Z') {
          ch = char(ch + 32);
        }
        break;
      case 1: ch = '*'; break;
      default: break;
    }
  }
  if (!name.empty() && r.Below(4) == 0) {
    name[r.Below(uint32_t(name.size()))] = char('a' + r.Below(26));
  }
  return name;
}

void Names(Context& h, Random& r, uint32_t block) {
  {
    Results results("NameMatches (sub_820CE800)");
    for (int c = 0; c < 128; c++) {
      Case(
          h, results, block, kSmallBytes, r,
          [&](PPCContext& t) {
            const std::string a = RandomName(r, c < 4 ? 0 : r.Below(12));
            std::string b = r.Below(4) == 0 ? RandomName(r, r.Below(12)) : Variant(r, a);
            if (c < 2) {
              b = c == 0 ? "" : "x";
            }
            PutString(h.base, block + 0x100, a);
            PutString(h.base, block + 0x200, b);
            t.r3.u64 = block + 0x100;
            t.r4.u64 = block + 0x200;
          },
          __imp__sub_820CE800, hooks::SceneLoadersNameMatches, fmt::format("case {}", c));
    }
    results.Report();
  }
  // An XPR object at `xpr`: up to six records (16 bytes from block + 0x100,
  // each +12 a name at block + 0x200 + 32i), or none (+16 = 0).
  auto xpr = [&](uint32_t object, const std::string& wanted) {
    const uint32_t count = r.Below(7);
    const uint32_t records = block + 0x100;
    Put(h.base, object + 16, r.Below(6) == 0 ? 0 : records);
    Put(h.base, object + 20, count);
    for (uint32_t i = 0; i < 6; i++) {
      const uint32_t name = block + 0x200 + 32 * i;
      Put(h.base, records + 16 * i + 12, name);
      PutString(h.base, name, r.Below(3) == 0 ? Variant(r, wanted) : RandomName(r, r.Below(10)));
    }
  };
  {
    Results results("XPR::Count (sub_82596F20)");
    for (int c = 0; c < 16; c++) {
      Case(
          h, results, block, kSmallBytes, r,
          [&](PPCContext& t) {
            xpr(block, "x");
            t.r3.u64 = block;
          },
          __imp__sub_82596F20, hooks::SceneLoadersXprCount, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("XPR::RecordName (sub_82596F40)");
    for (int c = 0; c < 32; c++) {
      Case(
          h, results, block, kSmallBytes, r,
          [&](PPCContext& t) {
            xpr(block, "x");
            Put(h.base, block + 16, block + 0x100);
            t.r3.u64 = block;
            t.r4.u64 = r.Below(8) == 0 ? r.Next() : r.Below(8);
          },
          __imp__sub_82596F40, hooks::SceneLoadersXprRecordName, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("XPR::FindByName (sub_820D09B8)");
    for (int c = 0; c < 96; c++) {
      Case(
          h, results, block, kSmallBytes, r,
          [&](PPCContext& t) {
            const std::string wanted = RandomName(r, r.Below(10));
            xpr(block, wanted);
            PutString(h.base, block + 0x400, wanted);
            t.r3.u64 = block;
            t.r4.u64 = block + 0x400;
          },
          __imp__sub_820D09B8, hooks::SceneLoadersXprFindByName, fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- The embedded loads ---

// After a case: a provider's vector (at provider + 4: +4 first, +8 last, +12
// end) as whether its storage is a heap block, its size and capacity and
// whether its slots are all 0; then its words cleared and the storage freed.
void TakeProvider(uint8_t* base, uint32_t provider, Outputs& out) {
  const uint32_t vector = provider + models::kTextureProviderVector;
  const uint32_t first = guest::LoadU32(base, vector + models::kVectorFirst);
  const uint32_t last = guest::LoadU32(base, vector + models::kVectorLast);
  const uint32_t end = guest::LoadU32(base, vector + models::kVectorEnd);
  const bool heap = first != 0 && memory::IsHeapBlock(first);
  out.Word(heap);
  out.Word(last - first);
  out.Word(end - first);
  bool zeros = true;
  for (uint32_t p = first; heap && p != last && last - first <= 4096; p += 4) {
    zeros = zeros && guest::LoadU32(base, p) == 0;
  }
  out.Word(zeros);
  for (uint32_t offset : {models::kVectorFirst, models::kVectorLast, models::kVectorEnd}) {
    Put(base, vector + offset, 0);
  }
  if (heap) {
    memory::crt::OperatorDelete(first);
  }
}

void Loads(Context& h, Random& r, uint32_t block) {
  const uint32_t loads = block + 0x100;
  {
    Results results("CarModelLoads::CarModelLoads (sub_820D3DE0)");
    for (int c = 0; c < 16; c++) {
      RandomBytes(h.base, block, kSmallBytes, r);
      const std::vector<uint8_t> inputs = Read(h.base, block, kSmallBytes);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Write(h.base, block, inputs.data(), kSmallBytes);
        PPCContext t = CallContext(h);
        t.r1.u64 &= ~0xFull;
        t.r3.u64 = loads;
        (v == 0 ? __imp__sub_820D3DE0 : hooks::SceneLoadersCarModelLoadsConstruct)(t, h.base);
        for (uint32_t offset : sl::kCarModelLoadsProviders) {
          TakeProvider(h.base, loads + offset, out[v]);
        }
        out[v].Bytes(Read(h.base, block, kSmallBytes));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // Each side makes the loads afresh (the plain C++ constructor, from the
    // same random bytes), puts random words in the providers' slots, then
    // destroys them: the storage is freed, so only the words left and r3.
    Results results("CarModelLoads::~CarModelLoads (sub_820D3E40)");
    for (int c = 0; c < 16; c++) {
      RandomBytes(h.base, block, kSmallBytes, r);
      const std::vector<uint8_t> inputs = Read(h.base, block, kSmallBytes);
      const uint64_t seed = r.Next();
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Write(h.base, block, inputs.data(), kSmallBytes);
        if (!sl::CarModelLoadsConstruct(h.base, loads)) {
          ITC_LOG_WARN("harness: scene_loaders: CarModelLoads::CarModelLoads refused");
          return;
        }
        Random slots(seed);
        uint32_t storage[2];
        for (int i = 0; i < 2; i++) {
          const uint32_t vector = loads + sl::kCarModelLoadsProviders[i] +
                                  models::kTextureProviderVector;
          storage[i] = guest::LoadU32(h.base, vector + models::kVectorFirst);
          for (uint32_t s = 0; s < models::kTextureProviderSlots; s++) {
            Put(h.base, storage[i] + 4 * s, slots.Below(3) == 0 ? 0 : uint32_t(slots.Next()));
          }
        }
        PPCContext t = CallContext(h);
        t.r1.u64 &= ~0xFull;
        t.r3.u64 = loads;
        (v == 0 ? __imp__sub_820D3E40 : hooks::SceneLoadersCarModelLoadsDestruct)(t, h.base);
        for (int i = 0; i < 2; i++) {
          out[v].Word(memory::IsHeapBlock(storage[i]));
        }
        out[v].Bytes(Read(h.base, block, kSmallBytes));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- The resets ---

// The car model's reset and its loads': each case's plan drawn once
// (car_model_inputs.h), both sides from the same random bytes, the
// recompiled side with --native_scene_loaders and --native_car_models off
// (its callees here the originals too). The pending frees' nodes, what each
// side allocated, the block (with the car model and the scratch its members
// point into), the first set's word and r3 compared. The matrices and
// positions are the xex's 0.0 and 1.0 stored back as floats: compared
// exactly (no NaN can come of them).
void Resets(Context& h, Random& r, uint32_t block) {
  namespace cmi = car_model_inputs;
  if (!hooks::ModelsHostLock()) {
    ITC_LOG_WARN("harness: scene_loaders: --native_models was off at start-up: the resets' "
                 "cases skipped");
    return;
  }
  std::lock_guard<std::recursive_mutex> lock(models::MemoryBlocksMutex());
  constexpr uint32_t kBytes = 0x4000;
  const uint32_t scratch = block + 0x100;
  const uint32_t car = block + 0x1000;
  const uint32_t loads = car + sl::kCarModelLoads;
  struct Reset {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
    uint32_t r3;
  };
  const Reset resets[] = {
      {"CarModelLoads::Reset (sub_820D3B58, on the game's list)", __imp__sub_820D3B58,
       hooks::SceneLoadersCarModelLoadsReset, loads},
      {"CarModel::Reset (sub_820D3E98, on the game's list)", __imp__sub_820D3E98,
       hooks::SceneLoadersCarModelReset, car},
  };
  const uint32_t saved_set = guest::LoadU32(h.base, sl::kFirstSet);
  for (const Reset& reset : resets) {
    Results results(reset.name);
    for (int c = 0; c < 32; c++) {
      const uint64_t seed = r.Next();
      const cmi::Plan plan = cmi::MakePlan(r);
      const uint32_t set = uint32_t(r.Next());
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random bytes(seed);
        RandomBytes(h.base, block, kBytes, bytes);
        cmi::Held held;
        cmi::Write(h.base, loads, scratch, plan, false, held);
        Put(h.base, sl::kFirstSet, set);
        const pending_frees::ListState was = pending_frees::SaveList(h.base);
        PPCContext t = CallContext(h);
        t.r1.u64 &= ~0xFull;
        t.r3.u64 = reset.r3;
        ITC_FLAG(native_scene_loaders) = v == 1;
        ITC_FLAG(native_car_models) = v == 1;
        (v == 0 ? reset.recompiled : reset.native)(t, h.base);
        ITC_FLAG(native_scene_loaders) = true;
        ITC_FLAG(native_car_models) = true;
        pending_frees::TakeAddedNodes(h.base, was, out[v]);
        cmi::Take(h.base, loads, held, out[v]);
        out[v].Bytes(Read(h.base, block, kBytes));
        out[v].Word(guest::LoadU32(h.base, sl::kFirstSet));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  Put(h.base, sl::kFirstSet, saved_set);
}

// --- The car parts and pool 4's object ---
//
// Each side builds its objects with the plain C++ constructors where the
// function under test needs them made (from the same random bytes), puts
// blocks for the releases to put off in a few models (headers in the
// block, at most eight nodes, all kept by TakeAddedNodes), runs, and is
// compared by the pending frees' nodes, the providers' storage (by its
// contents, then freed), the pages and storage each side allocated (by
// whether they're alive; words still pointing at them marked) and the
// block. The recompiled side runs with --native_scene_loaders,
// --native_car_models and --native_models off (its callees the originals).

constexpr uint32_t kMarked = 0x4D41524B;  // 'MARK': a side's own allocation

// Words in [from, from + bytes) equal to one of `held` marked.
void Mark(uint8_t* base, uint32_t from, uint32_t bytes, const std::unordered_set<uint32_t>& held) {
  for (uint32_t i = 0; i < bytes; i += 4) {
    if (held.count(guest::LoadU32(base, from + i)) != 0) {
      Put(base, from + i, kMarked);
    }
  }
}

// Runs one side of a lifetime case: the flags off for the recompiled side.
void RunSide(int v, GuestFunction recompiled, GuestFunction native, PPCContext& t, uint8_t* base) {
  ITC_FLAG(native_scene_loaders) = v == 1;
  ITC_FLAG(native_car_models) = v == 1;
  ITC_FLAG(native_models) = v == 1;
  (v == 0 ? recompiled : native)(t, base);
  ITC_FLAG(native_scene_loaders) = true;
  ITC_FLAG(native_car_models) = true;
  ITC_FLAG(native_models) = true;
}

// Blocks for Model::Release to put off in model `model`: +16, +20 and the
// XPR's header and data, each none, untagged or tagged (headers at
// `headers`, 16 bytes each, four for the model).
void PutBlocks(uint8_t* base, uint32_t model, uint32_t headers, Random& r) {
  const uint32_t at[] = {model + 16, model + 20, model + models::kModelXpr + models::kXprHeader,
                         model + models::kModelXpr + models::kXprData};
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t words[3] = {uint32_t(r.Next()), uint32_t(r.Next()), uint32_t(r.Next())};
    Put(base, at[i], pending_frees::WriteBlock(base, r.Below(3), headers + 16 * i, words));
  }
}

void Parts(Context& h, Random& r, uint32_t block) {
  if (!hooks::ModelsHostLock()) {
    ITC_LOG_WARN("harness: scene_loaders: --native_models was off at start-up: the car parts' "
                 "cases skipped");
    return;
  }
  std::lock_guard<std::recursive_mutex> lock(models::MemoryBlocksMutex());
  constexpr uint32_t kBytes = 0x1C000;
  const uint32_t parts = block + 0x400;
  const uint32_t loads = parts + sl::kCarPartsLoads;
  const uint32_t list = block + 0x1A400;
  const uint32_t headers = block + 0x1B000;
  const uint32_t saved_set = guest::LoadU32(h.base, sl::kFirstSet);
  auto model_of = [&](uint32_t i) { return parts + sl::kPartsModels + sl::kPartModelBytes * i; };
  enum Kind { kLoadsConstruct, kConstruct, kRelease, kLoadsDestruct };
  struct Function {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
    Kind kind;
  };
  const Function functions[] = {
      {"CarPartsLoads::CarPartsLoads (sub_820D6F88)", __imp__sub_820D6F88,
       hooks::SceneLoadersCarPartsLoadsConstruct, kLoadsConstruct},
      {"CarParts::CarParts (sub_820D7428, on the game's list)", __imp__sub_820D7428,
       hooks::SceneLoadersCarPartsConstruct, kConstruct},
      {"CarParts::Release (sub_820D6D00, on the game's list)", __imp__sub_820D6D00,
       hooks::SceneLoadersCarPartsRelease, kRelease},
      {"CarPartsLoads::~CarPartsLoads (sub_820D6FE8, on the game's list)", __imp__sub_820D6FE8,
       hooks::SceneLoadersCarPartsLoadsDestruct, kLoadsDestruct},
  };
  for (const Function& f : functions) {
    Results results(f.name);
    for (int c = 0; c < 6; c++) {
      const uint64_t seed = r.Next();
      const uint32_t set = uint32_t(r.Next());
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random in(seed);
        RandomBytes(h.base, block, kBytes, in);
        Put(h.base, sl::kFirstSet, set);  // before the constructors read it
        std::unordered_set<uint32_t> held;
        std::vector<uint32_t> storages;
        if (f.kind == kConstruct) {
          MakeList(h.base, list, list + 0x100, in.Below(4));
        }
        if (f.kind == kRelease || f.kind == kLoadsDestruct) {
          if (f.kind == kRelease) {
            MakeList(h.base, list, list + 0x100, 0);  // the pool node's list
          }
          const bool made = f.kind == kRelease
                                ? sl::CarPartsConstruct(h.base, parts, list)
                                : sl::CarPartsLoadsConstruct(h.base, loads);
          if (!made) {
            ITC_LOG_WARN("harness: scene_loaders: the car parts' constructor refused");
            return;
          }
          for (uint32_t i = 0; i < sl::kPartsCount; i++) {
            storages.push_back(guest::LoadU32(
                h.base, model_of(i) + models::kModelTextures + models::kTextureProviderVector +
                            models::kVectorFirst));
          }
          // Two models with blocks to put off (at most eight nodes).
          for (uint32_t k = 0; k < 2; k++) {
            PutBlocks(h.base, model_of(in.Below(sl::kPartsCount)), headers + 64 * k, in);
          }
        }
        if (f.kind == kRelease) {
          for (uint32_t i = 0; i < sl::kPartsCount; i++) {
            const uint32_t slot = loads + sl::kSlotBytes * i;
            Put(h.base, slot + 12, in.Below(4) == 0 ? 1 : 0);
            uint32_t pages = 0;
            if (in.Below(16) == 0) {
              pages = memory::AllocPages(1 + in.Below(0x4000), 4096);
              held.insert(pages);
            }
            Put(h.base, slot + 16, pages);
          }
        }
        const pending_frees::ListState was = pending_frees::SaveList(h.base);
        PPCContext t = CallContext(h);
        t.r1.u64 &= ~0xFull;
        t.r3.u64 = f.kind == kLoadsConstruct || f.kind == kLoadsDestruct ? loads : parts;
        t.r4.u64 = list;
        RunSide(v, f.recompiled, f.native, t, h.base);
        pending_frees::TakeAddedNodes(h.base, was, out[v]);
        for (const uint32_t pages : held) {
          const bool alive = memory::IsPages(pages);
          out[v].Word(alive);
          if (alive) {
            memory::FreePages(pages);
          }
        }
        if (f.kind == kLoadsDestruct) {
          for (const uint32_t storage : storages) {
            const bool alive = storage != 0 && memory::IsHeapBlock(storage);
            out[v].Word(alive);
            if (alive) {
              memory::crt::OperatorDelete(storage);
            }
            held.insert(storage);
          }
        } else {
          for (uint32_t i = 0; i < sl::kPartsCount; i++) {
            TakeProvider(h.base, model_of(i) + models::kModelTextures, out[v]);
          }
        }
        held.erase(0);
        Mark(h.base, block, kBytes, held);
        out[v].Bytes(Read(h.base, block, kBytes));
        out[v].Word(guest::LoadU32(h.base, sl::kFirstSet));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // Pool4Object::Reset on a part model made with the plain C++.
    Results results("Pool4Object::Reset (sub_820D6948, on the game's list)");
    const uint32_t object = block + 0x400;
    for (int c = 0; c < 16; c++) {
      const uint64_t seed = r.Next();
      const uint32_t set = uint32_t(r.Next());
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random in(seed);
        RandomBytes(h.base, block, kSmallBytes, in);
        Put(h.base, sl::kFirstSet, set);
        if (!models::PartModelConstruct(h.base, object + 112)) {
          ITC_LOG_WARN("harness: scene_loaders: the part model's constructor refused");
          return;
        }
        if (in.Below(2) == 0) {
          PutBlocks(h.base, object + 112, block + 0x1000, in);
        }
        Put(h.base, sl::kFirstSet, set);
        const pending_frees::ListState was = pending_frees::SaveList(h.base);
        PPCContext t = CallContext(h);
        t.r1.u64 &= ~0xFull;
        t.r3.u64 = object;
        RunSide(v, __imp__sub_820D6948, hooks::SceneLoadersPool4Reset, t, h.base);
        pending_frees::TakeAddedNodes(h.base, was, out[v]);
        TakeProvider(h.base, object + 112 + models::kModelTextures, out[v]);
        out[v].Bytes(Read(h.base, block, kSmallBytes));
        out[v].Word(guest::LoadU32(h.base, sl::kFirstSet));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  Put(h.base, sl::kFirstSet, saved_set);
}

// --- The cockpit body and the interior ---
//
// As the car parts' cases: the cockpit (or the interior) at the block's
// +0x400, its list at +0x2000, the blocks' headers at +0x3000.
void Cockpits(Context& h, Random& r, uint32_t block) {
  if (!hooks::ModelsHostLock()) {
    ITC_LOG_WARN("harness: scene_loaders: --native_models was off at start-up: the cockpit's "
                 "cases skipped");
    return;
  }
  std::lock_guard<std::recursive_mutex> lock(models::MemoryBlocksMutex());
  constexpr uint32_t kBytes = 0x4000;
  const uint32_t object = block + 0x400;
  const uint32_t loads = object + sl::kCockpitLoads;
  const uint32_t list = block + 0x2000;
  const uint32_t headers = block + 0x3000;
  const uint32_t saved_set = guest::LoadU32(h.base, sl::kFirstSet);
  enum Kind { kLoadsConstruct, kConstruct, kRelease, kLoadsDestruct, kInterior };
  struct Function {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
    Kind kind;
  };
  const Function functions[] = {
      {"CockpitLoads::CockpitLoads (sub_820D6010)", __imp__sub_820D6010,
       hooks::SceneLoadersCockpitLoadsConstruct, kLoadsConstruct},
      {"Cockpit::Cockpit (sub_820D63A0, on the game's list)", __imp__sub_820D63A0,
       hooks::SceneLoadersCockpitConstruct, kConstruct},
      {"Cockpit::Release (sub_820D6238, on the game's list)", __imp__sub_820D6238,
       hooks::SceneLoadersCockpitRelease, kRelease},
      {"CockpitLoads::~CockpitLoads (sub_820D6090, on the game's list)", __imp__sub_820D6090,
       hooks::SceneLoadersCockpitLoadsDestruct, kLoadsDestruct},
      {"Interior::Interior (sub_820D78B8, on the game's list)", __imp__sub_820D78B8,
       hooks::SceneLoadersInteriorConstruct, kInterior},
  };
  // The texture providers the objects hold: the loads' two and each part
  // model's (the cockpit's at the loads' +512, the interior's at its +304).
  auto providers = [&](Kind kind) {
    std::vector<uint32_t> list_of;
    if (kind == kInterior) {
      for (uint32_t i = 0; i < 6; i++) {
        list_of.push_back(object + 304 + sl::kPartModelBytes * i + models::kModelTextures);
      }
      return list_of;
    }
    for (uint32_t offset : sl::kCarModelLoadsProviders) {
      list_of.push_back(loads + offset);
    }
    for (uint32_t i = 0; i < 6; i++) {
      list_of.push_back(loads + sl::kCockpitLoadsParts + sl::kPartModelBytes * i +
                        models::kModelTextures);
    }
    return list_of;
  };
  for (const Function& f : functions) {
    Results results(f.name);
    for (int c = 0; c < 24; c++) {
      const uint64_t seed = r.Next();
      const uint32_t set = uint32_t(r.Next());
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random in(seed);
        RandomBytes(h.base, block, kBytes, in);
        Put(h.base, sl::kFirstSet, set);
        std::unordered_set<uint32_t> held;
        std::vector<uint32_t> storages;
        MakeList(h.base, list, list + 0x100, f.kind == kRelease ? 0 : in.Below(4));
        if (f.kind == kRelease || f.kind == kLoadsDestruct) {
          const bool made = f.kind == kRelease ? sl::CockpitConstruct(h.base, object, list)
                                               : sl::CockpitLoadsConstruct(h.base, loads);
          if (!made) {
            ITC_LOG_WARN("harness: scene_loaders: the cockpit's constructor refused");
            return;
          }
          for (const uint32_t provider : providers(f.kind)) {
            storages.push_back(guest::LoadU32(
                h.base, provider + models::kTextureProviderVector + models::kVectorFirst));
          }
          // The XPRs' blocks, and one part model's (at most eight nodes).
          for (uint32_t i = 0; i < 2; i++) {
            const uint32_t xpr = loads + sl::kCarModelLoadsXprs[i];
            for (uint32_t b = 0; b < 2; b++) {
              const uint32_t words[3] = {uint32_t(in.Next()), uint32_t(in.Next()),
                                         uint32_t(in.Next())};
              Put(h.base, xpr + 8 * b,
                  pending_frees::WriteBlock(h.base, in.Below(3), headers + 32 * i + 16 * b,
                                            words));
            }
          }
          if (f.kind == kLoadsDestruct && in.Below(2) == 0) {
            PutBlocks(h.base, loads + sl::kCockpitLoadsParts + sl::kPartModelBytes * in.Below(6),
                      headers + 0x100, in);
          }
        }
        if (f.kind == kRelease) {
          static constexpr uint32_t kSlots[] = {176, 400, 424, 536, 560, 584, 608, 632, 656};
          for (const uint32_t offset : kSlots) {
            Put(h.base, object + offset + 12, in.Below(4) == 0 ? 1 : 0);
            uint32_t pages = 0;
            if (in.Below(3) == 0) {
              pages = memory::AllocPages(1 + in.Below(0x4000), 4096);
              held.insert(pages);
            }
            Put(h.base, object + offset + 16, pages);
          }
          uint32_t values = 0;
          if (in.Below(2) == 0) {
            values = memory::Alloc(48, 0);
            held.insert(values);
          }
          Put(h.base, object + 208 + 176, values);
        }
        const pending_frees::ListState was = pending_frees::SaveList(h.base);
        PPCContext t = CallContext(h);
        t.r1.u64 &= ~0xFull;
        t.r3.u64 = f.kind == kLoadsConstruct || f.kind == kLoadsDestruct ? loads : object;
        t.r4.u64 = list;
        RunSide(v, f.recompiled, f.native, t, h.base);
        pending_frees::TakeAddedNodes(h.base, was, out[v]);
        for (const uint32_t address : held) {
          const bool alive = memory::IsPages(address) || memory::IsHeapBlock(address);
          out[v].Word(alive);
          if (memory::IsPages(address)) {
            memory::FreePages(address);
          } else if (alive) {
            memory::Free(address);
          }
        }
        if (f.kind == kLoadsDestruct) {
          for (const uint32_t storage : storages) {
            const bool alive = storage != 0 && memory::IsHeapBlock(storage);
            out[v].Word(alive);
            if (alive) {
              memory::crt::OperatorDelete(storage);
            }
            held.insert(storage);
          }
        } else {
          for (const uint32_t provider : providers(f.kind)) {
            TakeProvider(h.base, provider, out[v]);
          }
        }
        held.erase(0);
        Mark(h.base, block, kBytes, held);
        out[v].Bytes(Read(h.base, block, kBytes));
        out[v].Word(guest::LoadU32(h.base, sl::kFirstSet));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  Put(h.base, sl::kFirstSet, saved_set);
}

// --- The interior's release and destructors, ScenePools::ReleaseA ---
//
// As the cockpit's release cases: the interior built (Interior::Interior's
// plain C++) at the block's +0x400 on a list, the first set at +0x3800 (its
// +8 the model set: a, b, another), its six slots' buffers (pages or none,
// shared now and then) and a model's blocks to put off; the accessories'
// set (0x826C5510, put back after) at +0x3900 with entries at +0x3A00 (0
// or an entry whose count stays above 0; now and then two slots on one
// entry, which the native side hands to the original), the interior's +140
// the index (0-3). ReleaseA: pool 3 (the interior, alone on the set's list
// at +92), or a pool of 7 or more whose list's first node is a fake at
// +0x3C00.
void InteriorReleases(Context& h, Random& r, uint32_t block) {
  if (!hooks::ModelsHostLock()) {
    ITC_LOG_WARN("harness: scene_loaders: --native_models was off at start-up: the interior's "
                 "release cases skipped");
    return;
  }
  std::lock_guard<std::recursive_mutex> lock(models::MemoryBlocksMutex());
  constexpr uint32_t kBytes = 0x4000;
  constexpr uint32_t kAccessoriesSet = 0x826C5510;
  const uint32_t object = block + 0x400;
  const uint32_t headers = block + 0x3000;
  const uint32_t set = block + 0x3800;
  const uint32_t accessories = block + 0x3900;
  const uint32_t entries = block + 0x3A00;
  const uint32_t fake = block + 0x3C00;
  const uint32_t saved_set = guest::LoadU32(h.base, sl::kFirstSet);
  const uint32_t saved_accessories = guest::LoadU32(h.base, kAccessoriesSet);
  enum Kind { kRelease, kDestruct, kDeleting, kReleaseA };
  struct Function {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
    Kind kind;
  };
  const Function functions[] = {
      {"Interior::Release (sub_820D75F8, on the game's list)", __imp__sub_820D75F8,
       hooks::SceneLoadersInteriorRelease, kRelease},
      {"Interior::~Interior (sub_820D76D8, on the game's list)", __imp__sub_820D76D8,
       hooks::SceneLoadersInteriorDestruct, kDestruct},
      {"Interior's deleting destructor (sub_820D7960, on the game's list)", __imp__sub_820D7960,
       hooks::SceneLoadersInteriorDeletingDestruct, kDeleting},
      {"ScenePools::ReleaseA (sub_820C4488, the interior or no release)", __imp__sub_820C4488,
       hooks::SceneLoadersReleaseA, kReleaseA},
  };
  auto providers = [&]() {
    std::vector<uint32_t> list_of;
    for (uint32_t i = 0; i < 6; i++) {
      list_of.push_back(object + 304 + sl::kPartModelBytes * i + models::kModelTextures);
    }
    return list_of;
  };
  for (const Function& f : functions) {
    Results results(f.name);
    for (int c = 0; c < 32; c++) {
      const uint64_t seed = r.Next();
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random in(seed);
        RandomBytes(h.base, block, kBytes, in);
        Put(h.base, sl::kFirstSet, set);
        Put(h.base, set + 8, RandomModelSet(in) % 3 == 0 ? 0 : in.Below(3));
        const uint32_t pool = f.kind == kReleaseA && in.Below(4) == 0 ? 7 + in.Below(4) : 3;
        const uint32_t list = f.kind == kReleaseA ? set + 56 + 12 * (pool == 3 ? 3 : 0)
                                                  : block + 0x2000;
        MakeList(h.base, list, list + 0x100, 0);
        std::unordered_set<uint32_t> held;
        std::vector<uint32_t> storages;
        if (!sl::InteriorConstruct(h.base, object, list)) {
          ITC_LOG_WARN("harness: scene_loaders: the interior's constructor refused");
          return;
        }
        for (const uint32_t provider : providers()) {
          storages.push_back(guest::LoadU32(
              h.base, provider + models::kTextureProviderVector + models::kVectorFirst));
        }
        if (in.Below(2) == 0) {
          PutBlocks(h.base, object + 304 + sl::kPartModelBytes * in.Below(6), headers, in);
        }
        for (uint32_t i = 0; i < 6; i++) {
          const uint32_t slot = object + 160 + sl::kSlotBytes * i;
          Put(h.base, slot + 12, in.Below(4) == 0 ? 1 : 0);
          uint32_t pages = 0;
          if (in.Below(3) == 0) {
            pages = memory::AllocPages(1 + in.Below(0x4000), 4096);
            held.insert(pages);
          }
          Put(h.base, slot + 16, pages);
        }
        // The accessories' set: entries 0 or distinct (now and then shared),
        // their counts 4 to 6 (none reaches 0).
        Put(h.base, kAccessoriesSet, accessories);
        for (uint32_t i = 0; i < 3; i++) {
          uint32_t entry = in.Below(3) == 0 ? 0 : entries + 0x40 * i;
          if (i > 0 && in.Below(8) == 0) {
            entry = entries;
          }
          Put(h.base, accessories + 4 + 4 * i, entry);
          Put(h.base, entries + 0x40 * i, 4 + in.Below(3));
        }
        Put(h.base, object + 140, in.Below(4));
        // ReleaseA's other pools: the list's first node a fake.
        const uint32_t index = 0;
        if (f.kind == kReleaseA) {
          if (pool != 3) {
            Put(h.base, set + 56 + 12 * pool, fake);
          }
        }
        const pending_frees::ListState was = pending_frees::SaveList(h.base);
        PPCContext t = CallContext(h);
        t.r1.u64 &= ~0xFull;
        if (f.kind == kReleaseA) {
          t.r3.u64 = set;
          t.r4.u64 = pool;
          t.r5.u64 = index;
        } else {
          t.r3.u64 = object;
          t.r4.u64 = f.kind == kDeleting ? 0 : list;
        }
        ITC_FLAG(native_accessories) = true;
        RunSide(v, f.recompiled, f.native, t, h.base);
        pending_frees::TakeAddedNodes(h.base, was, out[v]);
        for (const uint32_t address : held) {
          const bool alive = memory::IsPages(address) || memory::IsHeapBlock(address);
          out[v].Word(alive);
          if (memory::IsPages(address)) {
            memory::FreePages(address);
          } else if (alive) {
            memory::Free(address);
          }
        }
        if (f.kind == kDestruct || f.kind == kDeleting) {
          for (const uint32_t storage : storages) {
            const bool alive = storage != 0 && memory::IsHeapBlock(storage);
            out[v].Word(alive);
            if (alive) {
              memory::crt::OperatorDelete(storage);
            }
            held.insert(storage);
          }
        } else {
          for (const uint32_t provider : providers()) {
            TakeProvider(h.base, provider, out[v]);
          }
        }
        held.erase(0);
        Mark(h.base, block, kBytes, held);
        out[v].Bytes(Read(h.base, block, kBytes));
        out[v].Word(guest::LoadU32(h.base, sl::kFirstSet));
        out[v].Word(guest::LoadU32(h.base, kAccessoriesSet));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  Put(h.base, sl::kFirstSet, saved_set);
  Put(h.base, kAccessoriesSet, saved_accessories);
}

// --- The wheels and the traffic body ---
//
// As the cockpit's cases, table-driven: each function's object (the wheels
// or the traffic body at the block's +0x400), its loads, their providers,
// XPRs and part models; constructors run on random bytes, resets and
// destructors on loads the plain C++ constructor made (with blocks for the
// releases to put off: the XPRs' and, now and then, one part model's).
void WheelsAndTraffic(Context& h, Random& r, uint32_t block) {
  if (!hooks::ModelsHostLock()) {
    ITC_LOG_WARN("harness: scene_loaders: --native_models was off at start-up: the wheels' "
                 "cases skipped");
    return;
  }
  std::lock_guard<std::recursive_mutex> lock(models::MemoryBlocksMutex());
  constexpr uint32_t kBytes = 0x3000;
  const uint32_t object = block + 0x400;
  const uint32_t headers = block + 0x2800;
  const uint32_t saved_set = guest::LoadU32(h.base, sl::kFirstSet);
  enum Kind { kConstruct, kReset, kDestruct };
  struct Layout {
    uint32_t loads;  // in the object
    std::vector<uint32_t> providers, xprs, models;  // in the loads
    bool (*make)(uint8_t*, uint32_t);
    uint32_t values;  // the skeleton's node values in the loads (0: none)
  };
  const Layout wheels{sl::kWheelsLoads,
                      {sl::kWheelsProviders, sl::kWheelsProviders + 20},
                      {sl::kWheelsXprs, sl::kWheelsXprs + 24},
                      {sl::kWheelsTires, sl::kWheelsTires + 464, sl::kWheelsRims,
                       sl::kWheelsRims + 464, sl::kWheelsCalipers, sl::kWheelsCalipers + 464},
                      sl::WheelsLoadsConstruct,
                      0};
  Layout traffic{sl::kTrafficLoads,
                 {sl::kTrafficMembers.providers[0], sl::kTrafficMembers.providers[1]},
                 {sl::kTrafficMembers.xprs[0], sl::kTrafficMembers.xprs[1]},
                 {},
                 sl::TrafficLoadsConstruct,
                 sl::kTrafficMembers.skeleton + 176};
  for (uint32_t i = 0; i < 6; i++) {
    traffic.models.push_back(sl::kTrafficLoadsParts + sl::kPartModelBytes * i);
  }
  struct Function {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
    Kind kind;
    const Layout* layout;
    bool on_object;  // r3 the object (else its loads)
  };
  const Function functions[] = {
      {"WheelsLoads::WheelsLoads (sub_820D8F60)", __imp__sub_820D8F60,
       hooks::SceneLoadersWheelsLoadsConstruct, kConstruct, &wheels, false},
      {"WheelsLoads::Reset (sub_820D8AA8, on the game's list)", __imp__sub_820D8AA8,
       hooks::SceneLoadersWheelsLoadsReset, kReset, &wheels, false},
      {"Wheels::Reset (sub_820D8B58, on the game's list)", __imp__sub_820D8B58,
       hooks::SceneLoadersWheelsReset, kReset, &wheels, true},
      {"WheelsLoads::~WheelsLoads (sub_820D9010, on the game's list)", __imp__sub_820D9010,
       hooks::SceneLoadersWheelsLoadsDestruct, kDestruct, &wheels, false},
      {"TrafficLoads::TrafficLoads (sub_820D7C40)", __imp__sub_820D7C40,
       hooks::SceneLoadersTrafficLoadsConstruct, kConstruct, &traffic, false},
      {"TrafficCar::Reset (sub_820D7AB0, on the game's list)", __imp__sub_820D7AB0,
       hooks::SceneLoadersTrafficReset, kReset, &traffic, true},
      {"TrafficLoads::~TrafficLoads (sub_820D7CC0, on the game's list)", __imp__sub_820D7CC0,
       hooks::SceneLoadersTrafficLoadsDestruct, kDestruct, &traffic, false},
  };
  for (const Function& f : functions) {
    const Layout& l = *f.layout;
    const uint32_t loads = object + l.loads;
    const uint32_t bytes = f.layout == &traffic ? 0x6000 + kBytes : kBytes;
    std::vector<uint32_t> providers;
    for (const uint32_t offset : l.providers) {
      providers.push_back(loads + offset);
    }
    for (const uint32_t offset : l.models) {
      providers.push_back(loads + offset + models::kModelTextures);
    }
    Results results(f.name);
    for (int c = 0; c < 24; c++) {
      const uint64_t seed = r.Next();
      const uint32_t set = uint32_t(r.Next());
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random in(seed);
        RandomBytes(h.base, block, bytes, in);
        Put(h.base, sl::kFirstSet, set);
        std::unordered_set<uint32_t> held;
        std::vector<uint32_t> storages;
        const uint32_t blocks_at = f.layout == &traffic ? block + 0x6000 + 0x2800 : headers;
        if (f.kind != kConstruct) {
          if (!l.make(h.base, loads)) {
            ITC_LOG_WARN("harness: scene_loaders: a loads' constructor refused");
            return;
          }
          for (const uint32_t provider : providers) {
            storages.push_back(guest::LoadU32(
                h.base, provider + models::kTextureProviderVector + models::kVectorFirst));
          }
          for (uint32_t i = 0; i < 2; i++) {
            for (uint32_t b = 0; b < 2; b++) {
              const uint32_t words[3] = {uint32_t(in.Next()), uint32_t(in.Next()),
                                         uint32_t(in.Next())};
              Put(h.base, loads + l.xprs[i] + 8 * b,
                  pending_frees::WriteBlock(h.base, in.Below(3), blocks_at + 32 * i + 16 * b,
                                            words));
            }
          }
          if (in.Below(2) == 0) {
            PutBlocks(h.base, loads + l.models[in.Below(6)], blocks_at + 0x100, in);
          }
          if (l.values != 0) {
            uint32_t values = 0;
            if (in.Below(2) == 0) {
              values = memory::Alloc(48, 0);
              held.insert(values);
            }
            Put(h.base, loads + l.values, values);
          }
        }
        const pending_frees::ListState was = pending_frees::SaveList(h.base);
        PPCContext t = CallContext(h);
        t.r1.u64 &= ~0xFull;
        t.r3.u64 = f.on_object ? object : loads;
        RunSide(v, f.recompiled, f.native, t, h.base);
        pending_frees::TakeAddedNodes(h.base, was, out[v]);
        for (const uint32_t address : held) {
          const bool alive = memory::IsHeapBlock(address);
          out[v].Word(alive);
          if (alive) {
            memory::Free(address);
          }
        }
        if (f.kind == kDestruct) {
          for (const uint32_t storage : storages) {
            const bool alive = storage != 0 && memory::IsHeapBlock(storage);
            out[v].Word(alive);
            if (alive) {
              memory::crt::OperatorDelete(storage);
            }
            held.insert(storage);
          }
        } else {
          for (const uint32_t provider : providers) {
            TakeProvider(h.base, provider, out[v]);
          }
        }
        held.erase(0);
        Mark(h.base, block, bytes, held);
        out[v].Bytes(Read(h.base, block, bytes));
        out[v].Word(guest::LoadU32(h.base, sl::kFirstSet));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  Put(h.base, sl::kFirstSet, saved_set);
}

// --- The objects' lifetimes ---
//
// Table-driven, as the cockpit's cases: each class's object at the block's
// +0x400, then its list, the blocks' headers and a pool set for the put-off
// releases. The constructors run on random bytes (r4 a list of 0 to 3
// nodes); the releases and destructors on an object the plain C++
// constructor made on an empty list, with blocks for the releases to put
// off (the XPRs' and, now and then, one part model's: at most eight nodes),
// the slots' buffers (pages each side allocates, shared or not), the
// skeleton's node values and, where the release puts textures off, the set
// it reads (records at random states, as the car model's release's cases).
// The deleting destructors with flags 0 or 2 (bit 0, operator delete, never
// seen in play, is left to the original).
struct ObjectLayout {
  uint32_t bytes;
  bool (*make)(uint8_t*, uint32_t object, uint32_t list);
  std::vector<uint32_t> providers;  // in the object
  std::vector<uint32_t> xprs;       // in the object
  std::vector<uint32_t> models;     // part models in the object
  std::vector<uint32_t> slots;      // the slots the release frees
  uint32_t values = 0;              // the skeleton's node values (0: none)
  uint32_t defer_set = 0;           // the word holding the set DeferRelease reads (0: none)
  uint32_t pages_one_in = 3;        // a slot's buffer one time in
};

// The loads with a skeleton, two XPRs and two providers (at `m`, in loads at
// `loads` in the object).
void AddMembers(ObjectLayout& l, uint32_t loads, const sl::MembersLayout& m) {
  for (uint32_t i = 0; i < 2; i++) {
    l.providers.push_back(loads + m.providers[i]);
    l.xprs.push_back(loads + m.xprs[i]);
  }
  l.values = loads + m.skeleton + 176;
}

void Lifetimes(Context& h, Random& r, uint32_t block) {
  if (!hooks::ModelsHostLock()) {
    ITC_LOG_WARN("harness: scene_loaders: --native_models was off at start-up: the lifetimes' "
                 "cases skipped");
    return;
  }
  std::lock_guard<std::recursive_mutex> lock(models::MemoryBlocksMutex());
  const uint32_t object = block + 0x400;
  const uint32_t saved_set = guest::LoadU32(h.base, sl::kFirstSet);
  ObjectLayout pool4{592, sl::Pool4Construct, {}, {}, {112}, {80}};
  ObjectLayout wheels{3280, sl::WheelsConstruct};
  for (uint32_t i = 0; i < 2; i++) {
    wheels.providers.push_back(sl::kWheelsLoads + sl::kWheelsProviders + 20 * i);
    wheels.xprs.push_back(sl::kWheelsLoads + sl::kWheelsXprs + 24 * i);
  }
  for (uint32_t offset : {sl::kWheelsTires, sl::kWheelsTires + 464, sl::kWheelsRims,
                          sl::kWheelsRims + 464, sl::kWheelsCalipers, sl::kWheelsCalipers + 464}) {
    wheels.models.push_back(sl::kWheelsLoads + offset);
  }
  wheels.slots = {192, 1168, 2144, 2280, 216, 1192, 2168, 2304};
  ObjectLayout car{sl::kCarModelBytes, sl::CarModelConstruct};
  AddMembers(car, sl::kCarModelLoads, sl::kCarModelMembers);
  car.slots = {3728, 3952, 3976};
  car.defer_set = sl::kCarModelSet;
  ObjectLayout parts{106016, nullptr};
  parts.make = [](uint8_t* base, uint32_t at, uint32_t list) {
    return sl::CarPartsConstruct(base, at, list);
  };
  for (uint32_t i = 0; i < sl::kPartsCount; i++) {
    parts.models.push_back(sl::kPartsModels + sl::kPartModelBytes * i);
    parts.slots.push_back(sl::kCarPartsLoads + sl::kSlotBytes * i);
  }
  parts.pages_one_in = 16;
  ObjectLayout cockpit{3520, sl::CockpitConstruct};
  AddMembers(cockpit, sl::kCockpitLoads, sl::kCarModelMembers);
  for (uint32_t i = 0; i < 6; i++) {
    cockpit.models.push_back(sl::kCockpitLoads + sl::kCockpitLoadsParts + sl::kPartModelBytes * i);
  }
  cockpit.slots = {176, 400, 424, 536, 560, 584, 608, 632, 656};
  ObjectLayout traffic{7408, sl::TrafficConstruct};
  AddMembers(traffic, sl::kTrafficLoads, sl::kTrafficMembers);
  for (uint32_t i = 0; i < 6; i++) {
    traffic.models.push_back(sl::kTrafficLoads + sl::kTrafficLoadsParts + sl::kPartModelBytes * i);
  }
  traffic.slots = {3744};
  traffic.defer_set = 7392;
  enum Kind { kConstruct, kRelease, kDestruct };
  struct Function {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
    Kind kind;
    const ObjectLayout* layout;
    bool deleting;
  };
  const Function functions[] = {
      {"Pool4Object::Pool4Object (sub_820D6A70, on the game's list)", __imp__sub_820D6A70,
       hooks::SceneLoadersPool4Construct, kConstruct, &pool4, false},
      {"Pool4Object::Release (sub_820D6A20, on the game's list)", __imp__sub_820D6A20,
       hooks::SceneLoadersPool4Release, kRelease, &pool4, false},
      {"Pool4Object::~Pool4Object (sub_820D6AD0, on the game's list)", __imp__sub_820D6AD0,
       hooks::SceneLoadersPool4Destruct, kDestruct, &pool4, false},
      {"Pool4Object's deleting destructor (sub_820D6BC0, on the game's list)", __imp__sub_820D6BC0,
       hooks::SceneLoadersPool4DeletingDestruct, kDestruct, &pool4, true},
      {"Wheels::Wheels (sub_820D9570, on the game's list)", __imp__sub_820D9570,
       hooks::SceneLoadersWheelsConstruct, kConstruct, &wheels, false},
      {"Wheels::Release (sub_820D8E68, on the game's list)", __imp__sub_820D8E68,
       hooks::SceneLoadersWheelsRelease, kRelease, &wheels, false},
      {"Wheels::~Wheels (sub_820D90B8, on the game's list)", __imp__sub_820D90B8,
       hooks::SceneLoadersWheelsDestruct, kDestruct, &wheels, false},
      {"Wheels' deleting destructor (sub_820D95E8, on the game's list)", __imp__sub_820D95E8,
       hooks::SceneLoadersWheelsDeletingDestruct, kDestruct, &wheels, true},
      {"CarModel::CarModel (sub_820D5890, on the game's list)", __imp__sub_820D5890,
       hooks::SceneLoadersCarModelConstruct, kConstruct, &car, false},
      {"CarModel::~CarModel (sub_820D4750, on the game's list)", __imp__sub_820D4750,
       hooks::SceneLoadersCarModelDestruct, kDestruct, &car, false},
      {"CarModel's deleting destructor (sub_820D5988, on the game's list)", __imp__sub_820D5988,
       hooks::SceneLoadersCarModelDeletingDestruct, kDestruct, &car, true},
      {"CarParts::~CarParts (sub_820D7038, on the game's list)", __imp__sub_820D7038,
       hooks::SceneLoadersCarPartsDestruct, kDestruct, &parts, false},
      {"CarParts' deleting destructor (sub_820D74C0, on the game's list)", __imp__sub_820D74C0,
       hooks::SceneLoadersCarPartsDeletingDestruct, kDestruct, &parts, true},
      {"Cockpit::~Cockpit (sub_820D64D0, on the game's list)", __imp__sub_820D64D0,
       hooks::SceneLoadersCockpitDestruct, kDestruct, &cockpit, false},
      {"Cockpit's deleting destructor (sub_820D6798, on the game's list)", __imp__sub_820D6798,
       hooks::SceneLoadersCockpitDeletingDestruct, kDestruct, &cockpit, true},
      {"TrafficCar::TrafficCar (sub_820D8908, on the game's list)", __imp__sub_820D8908,
       hooks::SceneLoadersTrafficConstruct, kConstruct, &traffic, false},
      {"TrafficCar::Release (sub_820D7B90, on the game's list)", __imp__sub_820D7B90,
       hooks::SceneLoadersTrafficRelease, kRelease, &traffic, false},
      {"TrafficCar::~TrafficCar (sub_820D7E90, on the game's list)", __imp__sub_820D7E90,
       hooks::SceneLoadersTrafficDestruct, kDestruct, &traffic, false},
      {"TrafficCar's deleting destructor (sub_820D89B8, on the game's list)", __imp__sub_820D89B8,
       hooks::SceneLoadersTrafficDeletingDestruct, kDestruct, &traffic, true},
  };
  for (const Function& f : functions) {
    const ObjectLayout& l = *f.layout;
    const uint32_t list = (object + l.bytes + 0xFF) & ~0xFFu;
    const uint32_t headers = list + 0x200;
    const uint32_t set = headers + 0x400;
    const uint32_t records = set + 0x100;
    const uint32_t bytes = records + 0x200 - block;
    std::vector<uint32_t> providers;
    for (const uint32_t offset : l.providers) {
      providers.push_back(object + offset);
    }
    for (const uint32_t offset : l.models) {
      providers.push_back(object + offset + models::kModelTextures);
    }
    Results results(f.name);
    const int cases = &l == &parts ? 8 : 24;
    for (int c = 0; c < cases; c++) {
      const uint64_t seed = r.Next();
      const uint32_t first_set = uint32_t(r.Next());
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random in(seed);
        RandomBytes(h.base, block, bytes, in);
        Put(h.base, sl::kFirstSet, first_set);
        std::unordered_set<uint32_t> held;
        std::vector<uint32_t> storages;
        MakeList(h.base, list, list + 0x100, f.kind == kConstruct ? in.Below(4) : 0);
        if (f.kind != kConstruct) {
          if (!l.make(h.base, object, list)) {
            ITC_LOG_WARN("harness: scene_loaders: an object's constructor refused");
            return;
          }
          for (const uint32_t provider : providers) {
            storages.push_back(guest::LoadU32(
                h.base, provider + models::kTextureProviderVector + models::kVectorFirst));
          }
          for (uint32_t i = 0; i < l.xprs.size(); i++) {
            for (uint32_t b = 0; b < 2; b++) {
              const uint32_t words[3] = {uint32_t(in.Next()), uint32_t(in.Next()),
                                         uint32_t(in.Next())};
              Put(h.base, object + l.xprs[i] + 8 * b,
                  pending_frees::WriteBlock(h.base, in.Below(3), headers + 32 * i + 16 * b,
                                            words));
            }
          }
          if (!l.models.empty() && in.Below(2) == 0) {
            PutBlocks(h.base, object + l.models[in.Below(uint32_t(l.models.size()))],
                      headers + 0x100, in);
          }
          for (const uint32_t offset : l.slots) {
            Put(h.base, object + offset + 12, in.Below(4) == 0 ? 1 : 0);
            uint32_t pages = 0;
            if (in.Below(l.pages_one_in) == 0) {
              pages = memory::AllocPages(1 + in.Below(0x4000), 4096);
              held.insert(pages);
            }
            Put(h.base, object + offset + 16, pages);
          }
          if (l.values != 0) {
            uint32_t values = 0;
            if (in.Below(2) == 0) {
              values = memory::Alloc(48, 0);
              held.insert(values);
            }
            Put(h.base, object + l.values, values);
          }
          if (l.defer_set != 0) {
            const uint32_t a = in.Below(3), b = in.Below(3);
            Put(h.base, object + l.defer_set, set);
            Put(h.base, set + 40, a);
            Put(h.base, set + 12, b);
            Put(h.base, set + 52, records);
            for (uint32_t i = 0; i < 4 * (a + b); i++) {
              Put(h.base, records + 20 * i + 16, in.Below(4) == 0 ? 0 : 2);
            }
          }
        }
        const pending_frees::ListState was = pending_frees::SaveList(h.base);
        PPCContext t = CallContext(h);
        t.r1.u64 &= ~0xFull;
        t.r3.u64 = object;
        t.r4.u64 = f.deleting ? 2 * in.Below(2) : list;
        RunSide(v, f.recompiled, f.native, t, h.base);
        pending_frees::TakeAddedNodes(h.base, was, out[v]);
        for (const uint32_t address : held) {
          const bool pages = memory::IsPages(address);
          const bool alive = pages || memory::IsHeapBlock(address);
          out[v].Word(alive);
          if (pages) {
            memory::FreePages(address);
          } else if (alive) {
            memory::Free(address);
          }
        }
        if (f.kind == kDestruct) {
          for (const uint32_t storage : storages) {
            const bool alive = storage != 0 && memory::IsHeapBlock(storage);
            out[v].Word(alive);
            if (alive) {
              memory::crt::OperatorDelete(storage);
            }
            held.insert(storage);
          }
        } else {
          for (const uint32_t provider : providers) {
            TakeProvider(h.base, provider, out[v]);
          }
        }
        held.erase(0);
        Mark(h.base, block, bytes, held);
        out[v].Bytes(Read(h.base, block, bytes));
        out[v].Word(guest::LoadU32(h.base, sl::kFirstSet));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  Put(h.base, sl::kFirstSet, saved_set);
}

// --- ScenePools::CreateA ---
//
// A set (its lists empty, or now and then holding nodes in the block) and a
// record of small counts (now and then negative ones, outside the two that
// size the records), the first set's pointer random. Each side's objects are
// found by walking the lists; compared: the block (the set and the record),
// the records' pages and each object, with the pointers into them made
// offsets (each side has its own pages), the texture providers by
// TakeProvider, r3 the same way; then all freed.
std::vector<uint32_t> PoolProviders(uint32_t pool) {
  std::vector<uint32_t> at;
  auto models_from = [&](uint32_t first, uint32_t count) {
    for (uint32_t i = 0; i < count; i++) {
      at.push_back(first + sl::kPartModelBytes * i + models::kModelTextures);
    }
  };
  switch (pool) {
    case 0:
      for (uint32_t offset : sl::kCarModelMembers.providers) at.push_back(sl::kCarModelLoads + offset);
      break;
    case 1:
      models_from(sl::kPartsModels, sl::kPartsCount);
      break;
    case 2:
      for (uint32_t offset : sl::kCarModelMembers.providers) at.push_back(sl::kCockpitLoads + offset);
      models_from(sl::kCockpitLoads + sl::kCockpitLoadsParts, 6);
      break;
    case 3:
      models_from(304, 6);
      break;
    case 4:
      models_from(112, 1);
      break;
    case 5:
      for (uint32_t i = 0; i < 2; i++) at.push_back(sl::kWheelsLoads + sl::kWheelsProviders + 20 * i);
      for (uint32_t offset : {sl::kWheelsTires, sl::kWheelsTires + 464, sl::kWheelsRims,
                              sl::kWheelsRims + 464, sl::kWheelsCalipers,
                              sl::kWheelsCalipers + 464}) {
        at.push_back(sl::kWheelsLoads + offset + models::kModelTextures);
      }
      break;
    default:
      for (uint32_t offset : sl::kTrafficMembers.providers) at.push_back(sl::kTrafficLoads + offset);
      models_from(sl::kTrafficLoads + sl::kTrafficLoadsParts, 6);
      break;
  }
  return at;
}

struct PageRegion {
  uint32_t start, bytes;
};

// `bytes` from `start`, the words pointing into a region (or 16 past it)
// replaced by a marker of the region and the offset.
std::vector<uint8_t> Normalized(uint8_t* base, const std::vector<PageRegion>& regions,
                                uint32_t start, uint32_t bytes) {
  std::vector<uint8_t> data = Read(base, start, bytes);
  for (uint32_t at = 0; at + 4 <= bytes; at += 4) {
    const uint32_t word = guest::LoadU32(base, start + at);
    for (size_t k = 0; k < regions.size(); k++) {
      if (word >= regions[k].start && word - regions[k].start < regions[k].bytes + 16) {
        const uint32_t marker = 0xB0000000u + (uint32_t(k) << 20) + (word - regions[k].start);
        for (int b = 0; b < 4; b++) data[at + b] = uint8_t(marker >> (24 - 8 * b));
        break;
      }
    }
  }
  return data;
}

void CreateCases(Context& h, Random& r, uint32_t block) {
  if (!hooks::ModelsNativeLifetime()) {
    ITC_LOG_WARN("harness: scene_loaders: the allocation layer or --native_models is off: "
                 "ScenePools::CreateA's cases skipped");
    return;
  }
  std::lock_guard<std::recursive_mutex> lock(models::MemoryBlocksMutex());
  constexpr uint32_t kBytes = 0x400;
  const uint32_t set = block + 0x100;
  const uint32_t record = block + 0x200;
  const uint32_t nodes = block + 0x280;
  const uint32_t saved_set = guest::LoadU32(h.base, sl::kFirstSet);
  Results results("ScenePools::CreateA (sub_820C3FD8)");
  for (int c = 0; c < 16; c++) {
    const uint64_t seed = r.Next();
    const uint32_t first_set = uint32_t(r.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random in(seed);
      RandomBytes(h.base, block, kBytes, in);
      Put(h.base, sl::kFirstSet, first_set);
      Put(h.base, record + 0, RandomModelSet(in));
      for (uint32_t k = 1; k < 9; k++) {
        const uint32_t small[] = {0, 0, 1, 1, 2};
        uint32_t count = small[in.Below(5)];
        if (k == 2) {
          count = in.Below(2);  // the car parts: at most one (106,016 bytes)
        } else if (k != 1 && k != 8 && in.Below(8) == 0) {
          count = 0x80000000u | in.Below(3);  // negative
        }
        Put(h.base, record + 4 * k, count);
      }
      for (uint32_t pool = 0; pool < 7; pool++) {
        const uint32_t list = set + 56 + 12 * pool;
        MakeList(h.base, list, nodes + 16 * pool, c % 4 == 3 && pool == in.Below(7) ? 1 : 0);
      }
      PPCContext t = CallContext(h);
      t.r1.u64 &= ~0xFull;
      t.r3.u64 = set;
      t.r4.u64 = record;
      RunSide(v, __imp__sub_820C3FD8, hooks::SceneLoadersCreateA, t, h.base);
      // The objects, pool by pool along the lists (past a node already there).
      std::vector<PageRegion> regions;
      std::vector<std::pair<uint32_t, uint32_t>> objects;  // pool, object
      const uint32_t records = guest::LoadU32(h.base, set + 52);
      regions.push_back({records, 80 * (guest::LoadU32(h.base, set + 12) +
                                        guest::LoadU32(h.base, set + 40))});
      for (uint32_t pool = 0; pool < 7; pool++) {
        const uint32_t list = set + 56 + 12 * pool;
        for (uint32_t node = guest::LoadU32(h.base, list + 0); node != 0;
             node = guest::LoadU32(h.base, node + 8)) {
          if (memory::IsPages(node)) {
            objects.push_back({pool, node});
            regions.push_back({node, sl::kPoolObjectBytes[pool]});
          }
          if (objects.size() > 64) break;
        }
      }
      out[v].Word(uint32_t(objects.size()));
      for (const auto& [pool, object] : objects) {
        for (uint32_t offset : PoolProviders(pool)) {
          TakeProvider(h.base, object + offset, out[v]);
        }
      }
      out[v].Bytes(Normalized(h.base, regions, block, kBytes));
      if (memory::IsPages(records)) {
        out[v].Bytes(Normalized(h.base, regions, records, regions[0].bytes));
      }
      for (const auto& [pool, object] : objects) {
        out[v].Bytes(Normalized(h.base, regions, object, sl::kPoolObjectBytes[pool]));
      }
      const uint32_t r3 = t.r3.u32;
      std::vector<uint8_t> r3_bytes(4);
      for (int b = 0; b < 4; b++) r3_bytes[b] = uint8_t(r3 >> (24 - 8 * b));
      for (size_t k = 0; k < regions.size(); k++) {
        if (r3 >= regions[k].start && r3 - regions[k].start < regions[k].bytes + 16) {
          const uint32_t marker = 0xB0000000u + (uint32_t(k) << 20) + (r3 - regions[k].start);
          for (int b = 0; b < 4; b++) r3_bytes[b] = uint8_t(marker >> (24 - 8 * b));
          break;
        }
      }
      out[v].Bytes(r3_bytes);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      out[v].Word(guest::LoadU32(h.base, sl::kFirstSet));
      for (const auto& [pool, object] : objects) {
        memory::FreePages(object);
      }
      if (memory::IsPages(records)) {
        memory::FreePages(records);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
  Put(h.base, sl::kFirstSet, saved_set);
}

// --- Interior::OnPartLoaded ---
//
// An interior the plain C++ constructor made, its six slots' requests
// random and distinct; r3 one of them (or none now and then), whose slot's
// buffer is pages holding a minimal part file (no shapes, shader pairs or
// skeleton; the wrong version now and then; as the scenery lists' cases),
// shared now and then. Compared: the pending frees' nodes, whether the
// buffer's pages are alive, the part models' providers (TakeProvider), the
// interior's bytes and the model's header copy and vertex data with the
// pointers into them made offsets, and r3; then the blocks freed.
void PartLoadedCases(Context& h, Random& r, uint32_t block) {
  if (!hooks::ModelsNativeLifetime()) {
    ITC_LOG_WARN("harness: scene_loaders: Model::Load isn't native in this run: "
                 "Interior::OnPartLoaded's cases skipped");
    return;
  }
  std::lock_guard<std::recursive_mutex> lock(models::MemoryBlocksMutex());
  constexpr uint32_t kBytes = 0x1400;
  constexpr uint32_t kFileBytes = 0x5000;
  constexpr uint32_t kHeader = 0x4000;
  constexpr uint32_t kVertices = 0x800;
  const uint32_t interior = block + 0x100;
  const uint32_t list = block + 0x1000;
  const uint32_t saved_set = guest::LoadU32(h.base, sl::kFirstSet);
  Results results("Interior::OnPartLoaded (sub_820D7578)");
  for (int c = 0; c < 24; c++) {
    const uint64_t seed = r.Next();
    const uint32_t first_set = uint32_t(r.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random in(seed);
      RandomBytes(h.base, block, kBytes, in);
      Put(h.base, sl::kFirstSet, first_set);
      MakeList(h.base, list, list + 0x100, 0);
      if (!sl::InteriorConstruct(h.base, interior, list)) {
        ITC_LOG_WARN("harness: scene_loaders: the interior's constructor refused");
        return;
      }
      for (uint32_t i = 0; i < 6; i++) {
        Put(h.base, interior + 164 + sl::kSlotBytes * i, 0x10000u * (i + 1) + in.Below(0x100));
      }
      const uint32_t which = in.Below(7);  // 6: none matches
      const uint32_t request =
          which < 6 ? guest::LoadU32(h.base, interior + 164 + sl::kSlotBytes * which) : 0x123;
      uint32_t pages = 0;
      if (which < 6) {
        const uint32_t slot = interior + 160 + sl::kSlotBytes * which;
        pages = memory::AllocPages(kFileBytes, 4096);
        std::vector<uint32_t> words(kFileBytes / 4, 0);
        auto put = [&](uint32_t at, uint32_t value) { words[at / 4] = value; };
        put(0, in.Below(5) == 0 ? 0x00140000 : 0x00150000);
        put(4, 7);
        put(8, 9);
        put(16, 3);
        put(20, 0x80);
        put(24, 0x90);
        put(28, 0x100);
        put(32, 0xA0);
        put(44, 0x2000);
        put(64, kHeader);
        put(68, kVertices);
        put(72, kHeader + kVertices);
        for (uint32_t k = kHeader; k < kHeader + kVertices; k += 4) {
          put(k, uint32_t(in.Next()) & 0x0FFFFFFFu);
        }
        for (uint32_t k = 0; k < words.size(); k++) {
          guest::StoreU32(h.base, pages + 4 * k, words[k]);
        }
        Put(h.base, slot + 12, in.Below(4) == 0 ? 1 : 0);
        Put(h.base, slot + 16, pages);
      }
      const pending_frees::ListState was = pending_frees::SaveList(h.base);
      PPCContext t = CallContext(h);
      t.r1.u64 &= ~0xFull;
      t.r3.u64 = request;
      t.r6.u64 = interior;
      RunSide(v, __imp__sub_820D7578, hooks::SceneLoadersInteriorOnPartLoaded, t, h.base);
      pending_frees::TakeAddedNodes(h.base, was, out[v]);
      const bool alive = pages != 0 && memory::IsPages(pages);
      out[v].Word(alive);
      for (uint32_t i = 0; i < 6; i++) {
        TakeProvider(h.base, interior + 304 + sl::kPartModelBytes * i + models::kModelTextures,
                     out[v]);
      }
      std::vector<PageRegion> regions = {{interior, 3104}};
      uint32_t header = 0, vertices = 0;
      if (which < 6) {
        const uint32_t model = interior + 304 + sl::kPartModelBytes * which;
        header = guest::LoadU32(h.base, model + 16);
        vertices = guest::LoadU32(h.base, model + 20);
        if (header != 0) regions.push_back({header, kHeader});
        if (vertices != 0) regions.push_back({vertices, kVertices});
      }
      if (pages != 0) regions.push_back({pages, kFileBytes});
      out[v].Bytes(Normalized(h.base, regions, block, kBytes));
      for (size_t k = 1; k < regions.size(); k++) {
        if (regions[k].start != pages) {
          out[v].Bytes(Normalized(h.base, regions, regions[k].start, regions[k].bytes));
        }
      }
      out[v].Word(t.r3.u32 == pages && pages != 0 ? 0xB0B0B0B0u : t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      if (header != 0) memory::FreePages(header - models::kBlockHeaderBytes);
      if (vertices != 0) memory::FreePhysical(vertices - models::kBlockHeaderBytes);
      if (alive) memory::FreePages(pages);
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
  Put(h.base, sl::kFirstSet, saved_set);
}


// --- The texture binds ---
//
// The cockpit body (or the traffic body) at the block, its XPR's records at
// +0x2000 (16 bytes each, +12 a name at +0x2100 + 48i, now and then the name
// the bind prints, its case varied), each side from the same random bytes,
// the recompiled side with the flags off (RunSide). The block, the
// printed name (the frame's +80, up to its terminator) and r3 compared.

// A guest string (up to 64 bytes, without the terminator).
std::string GuestString(uint8_t* base, uint32_t address) {
  std::string s;
  for (uint32_t i = 0; i < 64; i++) {
    const char ch = char(guest::LoadU8(base, address + i));
    if (ch == 0) break;
    s.push_back(ch);
  }
  return s;
}

// A name table's index below `below` whose entry points into the xex (else
// 0).
uint32_t NameIndex(uint8_t* base, uint32_t table, uint32_t below, Random& r) {
  for (int tries = 0; tries < 8; tries++) {
    const uint32_t i = r.Below(below);
    const uint32_t at = guest::LoadU32(base, table + 4 * i);
    if (at >= 0x82000000u && at < 0x83000000u) return i;
  }
  return 0;
}

// The XPR object at `xpr`: records at block + 0x2000, `count` of them (0-6),
// their names random or `wanted` varied.
void BindXpr(uint8_t* base, uint32_t block, uint32_t xpr, const std::string& wanted, Random& r) {
  const uint32_t records = block + 0x2000;
  Put(base, xpr + 16, records);
  Put(base, xpr + 20, r.Below(7));
  for (uint32_t i = 0; i < 6; i++) {
    const uint32_t name = block + 0x2100 + 48 * i;
    Put(base, records + 16 * i + 12, name);
    PutString(base, name,
              r.Below(2) == 0 ? (r.Below(2) == 0 ? wanted : Variant(r, wanted))
                              : RandomName(r, r.Below(10)));
  }
}

void BindCase(Context& h, Results& results, uint32_t block, Random& r,
              const std::function<void(PPCContext&)>& setup, uint32_t frame_bytes,
              GuestFunction recompiled, GuestFunction native, const std::string& what) {
  constexpr uint32_t kBytes = 0x4000;
  RandomBytes(h.base, block, kBytes, r);
  PPCContext start = CallContext(h);
  start.r1.u64 &= ~0xFull;
  setup(start);
  const std::vector<uint8_t> inputs = Read(h.base, block, kBytes);
  const uint32_t text = start.r1.u32 - frame_bytes + 80;
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Write(h.base, block, inputs.data(), kBytes);
    Put(h.base, text, 0);
    PPCContext t = start;
    RunSide(v, recompiled, native, t, h.base);
    out[v].Bytes(Read(h.base, block, kBytes));
    out[v].Bytes(Read(h.base, text, uint32_t(GuestString(h.base, text).size())));
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  results.Check(out[0], out[1], what);
}

void Binds(Context& h, Random& r, uint32_t block) {
  {
    Results results("Cockpit::BindTextures (sub_820D6108)");
    for (int c = 0; c < 64; c++) {
      BindCase(
          h, results, block, r,
          [&](PPCContext& t) {
            for (uint32_t i = 0; i < 6; i++) Put(h.base, block + 556 + 24 * i, RandomState(r, 40));
            for (uint32_t i = 0; i < 2; i++) Put(h.base, block + 420 + 24 * i, RandomState(r, 40));
            Put(h.base, block + 196, RandomState(r, 40));
            const uint32_t record = block + 0x1000;
            Put(h.base, block + 24, record);
            const uint32_t index = r.Below(4) == 0 ? 18 + r.Below(8) : NameIndex(h.base, 0x826018C8, 18, r);
            Put(h.base, record + 32, index);
            const uint32_t number = r.Below(2) == 0 ? 0 : r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(10);
            Put(h.base, block + 48, number);
            std::string wanted = GuestString(h.base, guest::LoadU32(h.base, 0x826018C8 + 4 * index));
            wanted += number == 0 ? GuestString(h.base, 0x82005008)
                                  : GuestString(h.base, 0x82004FFC) + std::to_string(int32_t(number));
            BindXpr(h.base, block, block + 448, wanted, r);
            t.r3.u64 = block;
          },
          sl::kCockpitBindFrameBytes, __imp__sub_820D6108, hooks::SceneLoadersCockpitBindTextures,
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("TrafficCar::BindTextures (sub_820D7D38)");
    for (int c = 0; c < 64; c++) {
      BindCase(
          h, results, block, r,
          [&](PPCContext& t) {
            Put(h.base, block + 3764, RandomState(r, 8));
            const uint32_t index = NameIndex(h.base, 0x82601980, 8, r);
            Put(h.base, block + 32, index);
            const uint32_t number = r.Below(4) == 0 ? uint32_t(-int32_t(r.Below(3))) : r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(10);
            Put(h.base, block + 192, number);
            Put(h.base, block + 7056, r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(10));
            const std::string wanted = GuestString(h.base, guest::LoadU32(h.base, 0x82601980 + 4 * index)) +
                                       GuestString(h.base, 0x820050AC) + std::to_string(int32_t(number));
            BindXpr(h.base, block, block + 4016, wanted, r);
            t.r3.u64 = block;
          },
          sl::kTrafficBindFrameBytes, __imp__sub_820D7D38, hooks::SceneLoadersTrafficBindTextures,
          fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- The skeletons' searches ---

// A small float (eighths from -4 to 4).
uint32_t SmallFloat(Random& r) {
  return std::bit_cast<uint32_t>(float(int32_t(r.Below(65)) - 32) / 8.0f);
}

// A node file (4 KB at `file`, as car_models' harness builds them) of
// `names.size()` nodes named `names`, with random parents, types (none
// UpdateMatrices hands back), sources and matrices (small floats).
void MakeNodeFile(uint8_t* base, uint32_t file, const std::vector<std::string>& names,
                  Random& r) {
  const uint32_t nodes = uint32_t(names.size());
  const uint32_t bones = r.Below(nodes + 1);
  const uint32_t first_bone = r.Below(nodes - bones + 1);
  RandomBytes(base, file, 4096, r);
  Put(base, file + 4, nodes);
  Put(base, file + 8, bones);
  Put(base, file + 12, first_bone);
  const uint32_t kRecords = 256, kSources = 1536, kMatrices = 1920, kTail = 2432;
  Put(base, file + 16, kRecords);
  Put(base, file + 20, kSources);
  Put(base, file + 24, kSources + 128);
  Put(base, file + 28, kSources + 256);
  Put(base, file + 32, kTail + 64);
  Put(base, file + 36, kMatrices);
  Put(base, file + 40, kTail + 128);
  Put(base, file + 44, kTail);
  Put(base, file + 48, kTail + 192);
  for (uint32_t n = 0; n < nodes; n++) {
    const uint32_t record = file + kRecords + 160 * n;
    PutString(base, record, names[n]);
    Put(base, record + 92, n == 0 || r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(n));
    static const uint32_t kTypes[] = {0, 1, 2, 3, 4, 5, 0x80, 0x81, 32};
    Put(base, record + 132, kTypes[r.Below(std::size(kTypes))]);
  }
  for (uint32_t at = kSources; at < kTail; at += 4) Put(base, file + at, SmallFloat(r));
}

// A skeleton's arrays (offset, bytes, header bytes before the address) as
// Skeleton::Bind makes them for `nodes` nodes and `bones` bones.
struct SkeletonArray {
  uint32_t offset, bytes, header;
};
std::vector<SkeletonArray> SkeletonArrays(uint8_t* base, uint32_t file) {
  const uint32_t nodes = guest::LoadU32(base, file + 4);
  const uint32_t bones = guest::LoadU32(base, file + 8);
  return {{128, 20, 0},
          {136, 16 + 144 * nodes, 16},
          {132, 16 + 64 * nodes, 16},
          {144, 16 + 64 * bones, 16},
          {176, 16 * nodes, 0}};
}

// A skeleton at `skeleton` bound to `file` (its array words cleared first).
bool BindSkeleton(uint8_t* base, uint32_t skeleton, uint32_t file) {
  for (const SkeletonArray& a : SkeletonArrays(base, file)) Put(base, skeleton + a.offset, 0);
  return model_animation::SkeletonBind(base, skeleton, file, 0);
}

// The skeleton's arrays' contents into `out` (past their headers), each
// then freed, and their words in the skeleton cleared.
void TakeSkeleton(uint8_t* base, uint32_t skeleton, uint32_t file, Outputs& out) {
  for (const SkeletonArray& a : SkeletonArrays(base, file)) {
    const uint32_t at = guest::LoadU32(base, skeleton + a.offset);
    out.Word(at != 0);
    if (at == 0) continue;
    if (a.offset != 128) {
      out.Bytes(Read(base, at, a.bytes - a.header));
    }
    memory::crt::OperatorDeleteArray(at - a.header);
    Put(base, skeleton + a.offset, 0);
  }
}

// ModelPoint::Find (sub_820D02D0): a point record (240 bytes at the block)
// and a part model (at +0x400, its skeleton at +112 bound to a node file of
// 1 to 8 nodes at +0x1000), the record's name one of the nodes', a variant
// of one (case, `*`) or another; the model's +408 0 now and then. Each side
// from the same random bytes (the skeleton bound again), the recompiled side
// with the flags off. The record (its matrices and translation as floats),
// the model (the skeleton's array words left out), the skeleton's arrays,
// the frame's +80 to +224 and r3 compared.
void ModelPoints(Context& h, Random& r, uint32_t block) {
  if (!hooks::ModelsNativeLifetime()) {
    ITC_LOG_WARN("harness: scene_loaders: ModelPoint::Find skipped (models' lifetime not native)");
    return;
  }
  static const char* kNames[] = {"tire_fl", "seat_L", "acce_null", "Body", "lamp_*", "x"};
  Results results("ModelPoint::Find (sub_820D02D0)");
  const uint32_t point = block, model = block + 0x400, skeleton = model + 112;
  const uint32_t file = block + 0x1000;
  for (int c = 0; c < 48; c++) {
    std::vector<std::string> names(1 + r.Below(8));
    for (auto& n : names) n = kNames[r.Below(std::size(kNames))];
    std::string wanted = names[r.Below(uint32_t(names.size()))];
    switch (r.Below(4)) {
      case 0: wanted = Variant(r, wanted); break;
      case 1: wanted = RandomName(r, r.Below(9)); break;
      default: break;
    }
    const bool loaded = r.Below(6) != 0;
    const uint64_t seed = r.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random g(seed);
      RandomBytes(h.base, block, 0x2000, g);
      for (uint32_t at = 16; at < 160; at += 4) Put(h.base, point + at, SmallFloat(g));
      PutString(h.base, point + 160, wanted);
      MakeNodeFile(h.base, file, names, g);
      if (!BindSkeleton(h.base, skeleton, file)) {
        ITC_LOG_WARN("harness: scene_loaders: ModelPoint::Find case {}: Bind declined", c);
      }
      Put(h.base, model + 408, loaded ? 1 + g.Below(4) : 0);
      PPCContext t = CallContext(h);
      t.r1.u64 &= ~0xFull;
      const uint32_t frame = t.r1.u32 - sl::kModelPointFrameBytes;
      RandomBytes(h.base, frame - 512, sl::kModelPointFrameBytes + 512, g);
      t.r3.u64 = point;
      t.r4.u64 = model;
      RunSide(v, __imp__sub_820D02D0, hooks::SceneLoadersModelPointFind, t, h.base);
      out[v].Bytes(Read(h.base, point, 16));
      out[v].Floats(h.base, point + 16, 36);
      out[v].Bytes(Read(h.base, point + 160, 80));
      out[v].Floats(h.base, frame + 80, 36);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      std::vector<uint8_t> m = Read(h.base, model, 464);
      for (const SkeletonArray& a : SkeletonArrays(h.base, file)) {
        std::fill_n(m.begin() + 112 + a.offset, 4, 0);
      }
      out[v].Bytes(m);
      TakeSkeleton(h.base, skeleton, file, out[v]);
    }
    results.Check(out[0], out[1], fmt::format("case {} ({} nodes, '{}')", c, names.size(), wanted));
  }
  results.Report();
}

// --- The file-loaded callbacks ---

constexpr uint32_t kPartFileBytes = 0x5000;
constexpr uint32_t kPartHeader = 0x4000;
constexpr uint32_t kPartVertices = 0x800;

// A minimal part file in new pages (as Interior::OnPartLoaded's cases).
void WritePartFile(uint8_t* base, uint32_t pages, Random& in) {
  std::vector<uint32_t> words(kPartFileBytes / 4, 0);
  auto put = [&](uint32_t at, uint32_t value) { words[at / 4] = value; };
  put(0, in.Below(5) == 0 ? 0x00140000 : 0x00150000);
  put(4, 7);
  put(8, 9);
  put(16, 3);
  put(20, 0x80);
  put(24, 0x90);
  put(28, 0x100);
  put(32, 0xA0);
  put(44, 0x2000);
  put(64, kPartHeader);
  put(68, kPartVertices);
  put(72, kPartHeader + kPartVertices);
  for (uint32_t k = kPartHeader; k < kPartHeader + kPartVertices; k += 4) {
    put(k, uint32_t(in.Next()) & 0x0FFFFFFFu);
  }
  for (uint32_t k = 0; k < words.size(); k++) {
    guest::StoreU32(base, pages + 4 * k, words[k]);
  }
}

uint32_t MakePartFile(uint8_t* base, Random& in) {
  const uint32_t pages = memory::AllocPages(kPartFileBytes, 4096);
  WritePartFile(base, pages, in);
  return pages;
}

// An XPR2 file at `file` of 0 to 8 records named from `names` or others (as
// car_models' harness builds them); its header and data sizes out.
void MakeXprFile(uint8_t* base, uint32_t file, const std::vector<std::string>& wanted,
                 Random& in, uint32_t* header_bytes, uint32_t* data_bytes) {
  const uint32_t records = in.Below(9);
  std::vector<std::string> names;
  for (uint32_t i = 0; i < records; i++) {
    names.push_back(in.Below(2) == 0 && !wanted.empty() ? wanted[in.Below(uint32_t(wanted.size()))]
                                                         : RandomName(in, 1 + in.Below(8)));
  }
  const uint32_t names_at = 4 + 16 * records;
  uint32_t header = names_at;
  for (const auto& n : names) header += uint32_t(n.size()) + 1;
  header = (header + 3) & ~3u;
  *header_bytes = header;
  *data_bytes = 64;
  RandomBytes(base, file, (12 + header + 64 + 3) & ~3u, in);
  Put(base, file + 0, 0x58505232);
  Put(base, file + 4, header);
  Put(base, file + 8, 64);
  Put(base, file + 12, records);
  uint32_t name_at = names_at;
  for (uint32_t i = 0; i < records; i++) {
    const uint32_t record = file + 16 + 16 * i;
    Put(base, record + 0, 0x56420000u | (guest::LoadU32(base, record) & 0xFFFF));
    Put(base, record + 12, name_at);
    PutString(base, file + 12 + name_at, names[i]);
    name_at += uint32_t(names[i].size()) + 1;
  }
}

// Cockpit::OnFileLoaded (sub_820D6528): a cockpit the plain C++ constructor
// made (at the block's +0x100), its slots' requests distinct, r3 one of a
// part's (its buffer a minimal part file in pages, shared now and then), an
// XPR2's (r4 an XPR2 file in the block, the real provider), the node
// file's (a node file of 1 to 8 nodes, "acce_null" among them now and
// then; +3472 0 or an earlier matrix in the block) or none. The recompiled
// side with the flags off. Compared: the pending frees' nodes, the buffer's
// pages alive, the providers (TakeProvider), the cockpit's bytes and the
// blocks the callees made (pointers into them as offsets), the skeleton's
// arrays, the frame's +80 to +224, r3.
void CockpitLoadedCases(Context& h, Random& r, uint32_t block) {
  if (!hooks::ModelsNativeLifetime()) {
    ITC_LOG_WARN("harness: scene_loaders: Cockpit::OnFileLoaded's cases skipped (models' "
                 "lifetime not native)");
    return;
  }
  std::lock_guard<std::recursive_mutex> lock(models::MemoryBlocksMutex());
  constexpr uint32_t kBytes = 0x2000;
  const uint32_t cockpit = block + 0x100;
  const uint32_t list = block + 0x1000;
  const uint32_t file = block + 0x3000;   // the node file (4 KB) or the XPR2
  const uint32_t earlier = block + 0x1800;  // an earlier +3472 matrix
  const uint32_t skeleton = cockpit + 208;
  const uint32_t saved_set = guest::LoadU32(h.base, sl::kFirstSet);
  static const char* kNames[] = {"acce_null", "acce_nul", "ACCE_NULL", "body", "seat_L"};
  Results results("Cockpit::OnFileLoaded (sub_820D6528)");
  for (int c = 0; c < 40; c++) {
    const uint64_t seed = r.Next();
    const uint32_t first_set = uint32_t(r.Next());
    const uint32_t which = c % 10;  // 0-5 a part, 6-7 an XPR2, 8 the node file, 9 none
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random in(seed);
      RandomBytes(h.base, block, kBytes, in);
      Put(h.base, sl::kFirstSet, first_set);
      MakeList(h.base, list, list + 0x100, 0);
      if (!sl::CockpitConstruct(h.base, cockpit, list)) {
        ITC_LOG_WARN("harness: scene_loaders: the cockpit's constructor refused");
        return;
      }
      for (uint32_t i = 0; i < 6; i++) Put(h.base, cockpit + 540 + 24 * i, 0x10000u * (i + 1));
      for (uint32_t i = 0; i < 2; i++) Put(h.base, cockpit + 404 + 24 * i, 0x100000u * (i + 1));
      Put(h.base, cockpit + 180, 0x1000000u);
      for (uint32_t at = 0; at < 64; at += 4) Put(h.base, earlier + at, SmallFloat(in));
      Put(h.base, cockpit + 3472, in.Below(3) == 0 ? earlier : 0);
      uint32_t request = 0x123, buffer = uint32_t(in.Next()), pages = 0;
      uint32_t header_bytes = 0, data_bytes = 0;
      if (which < 6) {
        const uint32_t slot = cockpit + 536 + 24 * which;
        pages = MakePartFile(h.base, in);
        Put(h.base, slot + 12, in.Below(4) == 0 ? 1 : 0);
        Put(h.base, slot + 16, pages);
        request = guest::LoadU32(h.base, slot + 4);
      } else if (which < 8) {
        MakeXprFile(h.base, file, {"tex", "body"}, in, &header_bytes, &data_bytes);
        buffer = file;
        request = 0x100000u * (which - 5);
      } else if (which == 8) {
        std::vector<std::string> names(1 + in.Below(8));
        for (auto& n : names) n = kNames[in.Below(std::size(kNames))];
        MakeNodeFile(h.base, file, names, in);
        Put(h.base, cockpit + 192, file);
        for (const SkeletonArray& a : SkeletonArrays(h.base, file)) {
          Put(h.base, skeleton + a.offset, 0);
        }
        request = 0x1000000u;
      }
      const pending_frees::ListState was = pending_frees::SaveList(h.base);
      PPCContext t = CallContext(h);
      t.r1.u64 &= ~0xFull;
      const uint32_t frame = t.r1.u32 - sl::kCockpitLoadedFrameBytes;
      RandomBytes(h.base, frame - 512, sl::kCockpitLoadedFrameBytes + 512, in);
      t.r3.u64 = request;
      t.r4.u64 = buffer;
      t.r6.u64 = cockpit;
      RunSide(v, __imp__sub_820D6528, hooks::SceneLoadersCockpitOnFileLoaded, t, h.base);
      pending_frees::TakeAddedNodes(h.base, was, out[v]);
      const bool alive = pages != 0 && memory::IsPages(pages);
      out[v].Word(alive);
      std::vector<PageRegion> regions = {{block, kBytes}, {file, 4096}};
      std::vector<std::pair<uint32_t, bool>> blocks;  // made: (block, physical)
      if (which < 6) {
        const uint32_t model = cockpit + 688 + sl::kPartModelBytes * which;
        for (uint32_t k = 0; k < 2; k++) {
          const uint32_t at = guest::LoadU32(h.base, model + 16 + 4 * k);
          if (at != 0) {
            regions.push_back({at, k == 0 ? kPartHeader : kPartVertices});
            blocks.push_back({at, k == 1});
          }
        }
      }
      uint32_t xpr_header = 0, xpr_data = 0;
      if (which == 6 || which == 7) {
        const uint32_t xpr = cockpit + 448 + 24 * (which - 6);
        xpr_header = guest::LoadU32(h.base, xpr + 0);
        xpr_data = guest::LoadU32(h.base, xpr + 8);
        if (xpr_header != 0) regions.push_back({xpr_header, header_bytes});
        if (xpr_data != 0) regions.push_back({xpr_data, data_bytes});
      }
      if (pages != 0) regions.push_back({pages, kPartFileBytes});
      if (which == 8) {
        for (const SkeletonArray& a : SkeletonArrays(h.base, file)) {
          const uint32_t at = guest::LoadU32(h.base, skeleton + a.offset);
          if (at != 0) regions.push_back({at - a.header, a.bytes});
        }
      }
      for (uint32_t i = 0; i < 2; i++) TakeProvider(h.base, cockpit + 496 + 20 * i, out[v]);
      for (uint32_t i = 0; i < 6; i++) {
        TakeProvider(h.base, cockpit + 688 + sl::kPartModelBytes * i + models::kModelTextures,
                     out[v]);
      }
      out[v].Bytes(Normalized(h.base, regions, block, kBytes));
      for (size_t k = 2; k < regions.size(); k++) {
        if (regions[k].start != pages) {
          out[v].Bytes(Normalized(h.base, regions, regions[k].start, regions[k].bytes));
        }
      }
      out[v].Floats(h.base, frame + 80, 36);
      const uint32_t r3 = t.r3.u32;
      uint32_t r3_marker = r3;
      for (size_t k = 0; k < regions.size(); k++) {
        if (r3 >= regions[k].start && r3 - regions[k].start < regions[k].bytes + 16) {
          r3_marker = 0xB0000000u + (uint32_t(k) << 20) + (r3 - regions[k].start);
          break;
        }
      }
      out[v].Word(r3_marker);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      for (const auto& [at, physical] : blocks) {
        if (physical) {
          memory::FreePhysical(at - models::kBlockHeaderBytes);
        } else {
          memory::FreePages(at - models::kBlockHeaderBytes);
        }
      }
      if (xpr_header != 0) memory::FreePages(xpr_header - models::kBlockHeaderBytes);
      if (xpr_data != 0) {
        memory::crt::XMemFree(xpr_data - models::kBlockHeaderBytes, models::kXprDataAllocation);
      }
      if (alive) memory::FreePages(pages);
      if (which == 8) {
        Outputs ignored;
        TakeSkeleton(h.base, skeleton, file, ignored);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {} (path {})", c, which));
  }
  results.Report();
  Put(h.base, sl::kFirstSet, saved_set);
}

// TrafficCar::OnFileLoaded (sub_820D8430): a traffic car the plain
// constructor made (at the block's +0x100, its record's +0 the name 0-7),
// r3 its file's request (+3748) or another; the file a container in pages
// (the slot's buffer, shared now and then): an XPR2 (names from the
// traffic car's name + "_ex" or others), a node file of 1 to 8 nodes (the
// tires' names among them), six minimal part files; the plate's images and
// words as car_models' MakePlate cases. The recompiled side with the flags
// off. Compared: the pending frees' nodes, the container's pages alive, the
// providers, the traffic car's bytes and the blocks the callees made
// (pointers into them as offsets; the plate's texture data by its bytes),
// the skeleton's arrays, the frame's +80 to +240 and the printed name, r3.
void TrafficLoadedCases(Context& h, Random& r, uint32_t block) {
  if (!hooks::ModelsNativeLifetime()) {
    ITC_LOG_WARN("harness: scene_loaders: TrafficCar::OnFileLoaded's cases skipped (models' "
                 "lifetime not native)");
    return;
  }
  std::lock_guard<std::recursive_mutex> lock(models::MemoryBlocksMutex());
  constexpr uint32_t kBytes = 0x2400;
  constexpr uint32_t kContainerBytes = 0x24000;
  constexpr uint32_t kXprAt = 0x40, kNodesAt = 0x1000, kPartsAt = 0x2000;
  const uint32_t traffic = block + 0x100;
  const uint32_t list = block + 0x2000;
  const uint32_t skeleton = traffic + 3776;
  const uint32_t saved_set = guest::LoadU32(h.base, sl::kFirstSet);
  const uint32_t saved_scene = guest::LoadU32(h.base, car_models::kSceneSet);
  const uint32_t set_block = memory::Alloc(4096, 0);
  const uint32_t scene = set_block - 3600;
  static constexpr uint32_t kJ[5] = {0, 4, 7, 11, 15};
  static const char* kNames[] = {"tire_fl", "tire_fr", "tire_rl", "tire_rr", "body", "Tire_fl"};
  const uint32_t texture_bytes = graphics::XGSetTextureHeader(
      h.base, 300, 150, 1, 2, car_models::kSheetFormat, 0, 0, 0, 0, 0, 0);
  Results results("TrafficCar::OnFileLoaded (sub_820D8430)");
  for (int c = 0; c < 16; c++) {
    const uint64_t seed = r.Next();
    const uint32_t first_set = uint32_t(r.Next());
    const bool matches = c % 4 != 3;
    Random g(r.Next());
    // The plate's images (as MakePlate's cases).
    uint32_t w[5] = {30 + g.Below(35), 300, 1521 + g.Below(20), 12 * (1 + g.Below(60)),
                     10 * (1 + g.Below(25))};
    uint32_t hh[5] = {6 * (33 + g.Below(13)), 150, 1 + g.Below(80), 10 * (1 + g.Below(10)),
                      1 + g.Below(100)};
    uint32_t pixels[5] = {};
    for (int k = 0; k < 5; k++) {
      pixels[k] = memory::Alloc(w[k] * hh[k] * 4, 0);
      RandomBytes(h.base, pixels[k], w[k] * hh[k] * 4, g);
    }
    uint32_t words[12];
    words[0] = uint32_t(g.Next());
    words[1] = g.Below(101);
    for (int k = 2; k < 5; k++) words[k] = g.Below(4) == 0 ? 38 : g.Below(10);
    for (int k = 5; k < 12; k++) words[k] = g.Below(4) == 0 ? 37 : g.Below(37);
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random in(seed);
      RandomBytes(h.base, block, kBytes, in);
      Put(h.base, sl::kFirstSet, first_set);
      MakeList(h.base, list, list + 0x100, 0);
      if (!sl::TrafficConstruct(h.base, traffic, list)) {
        ITC_LOG_WARN("harness: scene_loaders: the traffic car's constructor refused");
        return;
      }
      const uint32_t index = NameIndex(h.base, 0x82601980, 8, in);
      Put(h.base, traffic + 32, index);
      for (uint32_t k = 0; k < 12; k++) Put(h.base, traffic + 3664 + 4 * k, words[k]);
      // The container.
      const uint32_t container = memory::AllocPages(kContainerBytes, 4096);
      RandomBytes(h.base, container, 0x2000, in);
      const auto le = [&](uint32_t at, uint32_t value) {
        Put(h.base, at, __builtin_bswap32(value));
      };
      le(container, 8);
      le(container + 4, kXprAt);
      le(container + 8, kNodesAt);
      for (uint32_t i = 0; i < 6; i++) le(container + 12 + 4 * i, kPartsAt + kPartFileBytes * i);
      const std::string name =
          GuestString(h.base, guest::LoadU32(h.base, 0x82601980 + 4 * index)) +
          GuestString(h.base, 0x820050B8);
      uint32_t header_bytes = 0, data_bytes = 0;
      MakeXprFile(h.base, container + kXprAt, {name, name + "_ex_01", "x"}, in, &header_bytes,
                  &data_bytes);
      std::vector<std::string> nodes(1 + in.Below(8));
      for (auto& n : nodes) n = kNames[in.Below(std::size(kNames))];
      MakeNodeFile(h.base, container + kNodesAt, nodes, in);
      for (uint32_t i = 0; i < 6; i++) {
        WritePartFile(h.base, container + kPartsAt + kPartFileBytes * i, in);
      }
      for (const SkeletonArray& a : SkeletonArrays(h.base, container + kNodesAt)) {
        Put(h.base, skeleton + a.offset, 0);
      }
      Put(h.base, traffic + 3748, 0x7770);
      Put(h.base, traffic + 3756, in.Below(4) == 0 ? 1 : 0);
      Put(h.base, traffic + 3760, container);
      const uint32_t request = matches ? 0x7770 : 0x7771;
      // The plate's images.
      RandomBytes(h.base, set_block, 4096, in);
      for (int k = 0; k < 5; k++) {
        const uint32_t header = scene + 3732 + 20 * kJ[k];
        guest::StoreU16(h.base, header + 14, uint16_t(w[k]));
        guest::StoreU16(h.base, header + 16, uint16_t(hh[k]));
        Put(h.base, scene + 3656 + 4 * kJ[k], pixels[k]);
      }
      guest::StoreU32(h.base, car_models::kSceneSet, scene);
      const pending_frees::ListState was = pending_frees::SaveList(h.base);
      PPCContext t = CallContext(h);
      t.r1.u64 &= ~0xFull;
      const uint32_t frame = t.r1.u32 - sl::kTrafficLoadedFrameBytes;
      RandomBytes(h.base, frame - 1024, sl::kTrafficLoadedFrameBytes + 1024, in);
      t.r3.u64 = request;
      t.r4.u64 = uint32_t(in.Next());
      t.r6.u64 = traffic;
      RunSide(v, __imp__sub_820D8430, hooks::SceneLoadersTrafficOnFileLoaded, t, h.base);
      pending_frees::TakeAddedNodes(h.base, was, out[v]);
      const bool alive = memory::IsPages(container);
      out[v].Word(alive);
      std::vector<PageRegion> regions = {{block, kBytes}, {container, kContainerBytes}};
      std::vector<std::pair<uint32_t, int>> blocks;  // made: 0 pages, 1 physical, 2 XPR data
      if (matches) {
        for (uint32_t i = 0; i < 6; i++) {
          const uint32_t model = traffic + 4256 + sl::kPartModelBytes * i;
          for (uint32_t k = 0; k < 2; k++) {
            const uint32_t at = guest::LoadU32(h.base, model + 16 + 4 * k);
            if (at != 0) {
              regions.push_back({at, k == 0 ? kPartHeader : kPartVertices});
              blocks.push_back({at, int(k)});
            }
          }
        }
        const uint32_t xpr_header = guest::LoadU32(h.base, traffic + 4016);
        const uint32_t xpr_data = guest::LoadU32(h.base, traffic + 4024);
        if (xpr_header != 0) {
          regions.push_back({xpr_header, header_bytes});
          blocks.push_back({xpr_header, 0});
        }
        if (xpr_data != 0) {
          regions.push_back({xpr_data, data_bytes});
          blocks.push_back({xpr_data, 2});
        }
        for (const SkeletonArray& a : SkeletonArrays(h.base, container + kNodesAt)) {
          const uint32_t at = guest::LoadU32(h.base, skeleton + a.offset);
          if (at != 0) regions.push_back({at - a.header, a.bytes});
        }
      }
      // The plate's textures: headers (52 bytes) as regions, data by bytes.
      std::vector<uint32_t> plate_data;
      for (uint32_t k = 0; k < 4; k += 2) {
        const uint32_t header = guest::LoadU32(h.base, traffic + 7040 + 4 * k);
        const uint32_t data = guest::LoadU32(h.base, traffic + 7044 + 4 * k);
        if (matches && header != 0) regions.push_back({header, 52});
        if (matches && data != 0) {
          regions.push_back({data, texture_bytes});
          plate_data.push_back(data);
        }
      }
      for (uint32_t i = 0; i < 2; i++) TakeProvider(h.base, traffic + 4064 + 20 * i, out[v]);
      for (uint32_t i = 0; i < 6; i++) {
        TakeProvider(h.base, traffic + 4256 + sl::kPartModelBytes * i + models::kModelTextures,
                     out[v]);
      }
      out[v].Bytes(Normalized(h.base, regions, block, kBytes));
      for (size_t k = 2; k < regions.size(); k++) {
        const bool data = std::find(plate_data.begin(), plate_data.end(), regions[k].start) !=
                          plate_data.end();
        out[v].Bytes(data ? Read(h.base, regions[k].start, regions[k].bytes)
                          : Normalized(h.base, regions, regions[k].start, regions[k].bytes));
      }
      out[v].Bytes(Read(h.base, frame + 80, 16));
      out[v].Floats(h.base, frame + 96, 36);
      out[v].Bytes(Read(h.base, frame + 240, uint32_t(GuestString(h.base, frame + 240).size())));
      const uint32_t r3 = t.r3.u32;
      uint32_t r3_marker = r3;
      for (size_t k = 0; k < regions.size(); k++) {
        if (r3 >= regions[k].start && r3 - regions[k].start < regions[k].bytes + 16) {
          r3_marker = 0xB0000000u + (uint32_t(k) << 20) + (r3 - regions[k].start);
          break;
        }
      }
      out[v].Word(r3_marker);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      for (const auto& [at, kind] : blocks) {
        if (kind == 0) {
          memory::FreePages(at - models::kBlockHeaderBytes);
        } else if (kind == 1) {
          memory::FreePhysical(at - models::kBlockHeaderBytes);
        } else {
          memory::crt::XMemFree(at - models::kBlockHeaderBytes, models::kXprDataAllocation);
        }
      }
      if (matches) {
        for (uint32_t k = 0; k < 4; k += 2) {
          const uint32_t header = guest::LoadU32(h.base, traffic + 7040 + 4 * k);
          const uint32_t data = guest::LoadU32(h.base, traffic + 7044 + 4 * k);
          if (header != 0) memory::FreePages(header);
          if (data != 0) memory::FreePhysical(data);
        }
        Outputs ignored;
        TakeSkeleton(h.base, skeleton, container + kNodesAt, ignored);
      }
      if (alive) memory::FreePages(container);
    }
    for (int k = 0; k < 5; k++) memory::Free(pixels[k]);
    results.Check(out[0], out[1], fmt::format("case {} ({})", c, matches ? "its file" : "another"));
  }
  results.Report();
  memory::Free(set_block);
  Put(h.base, sl::kFirstSet, saved_set);
  guest::StoreU32(h.base, car_models::kSceneSet, saved_scene);
}
}  // namespace

void SceneLoaders(Context& h) {
  Random r(0x6C6F61646572ull);  // "loader"
  const uint32_t allocation = memory::Alloc(kBlockBytes + 16, 0);
  const uint32_t block = (allocation + 15) & ~15u;
  Pools(h, r, block);
  PoolNodes(h, r, block);
  Queries(h, r, block);
  Names(h, r, block);
  Loads(h, r, block);
  Resets(h, r, block);
  Parts(h, r, block);
  Cockpits(h, r, block);
  InteriorReleases(h, r, block);
  WheelsAndTraffic(h, r, block);
  Lifetimes(h, r, block);
  CreateCases(h, r, block);
  PartLoadedCases(h, r, block);
  Binds(h, r, block);
  ModelPoints(h, r, block);
  CockpitLoadedCases(h, r, block);
  TrafficLoadedCases(h, r, block);
  memory::Free(allocation);
}

}  // namespace harness
