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
#include "hooks/models.h"
#include "hooks/scene_loaders.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "models/texture_provider.h"
#include "scene_loaders/loads.h"
#include "scene_loaders/objects.h"
#include "models/memory_blocks.h"
#include "models/model.h"
#include "models/part_model.h"
#include "models/xpr.h"
#include "scene_loaders/pools.h"

#include <fmt/format.h>

#include <functional>
#include <mutex>
#include <unordered_set>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_scene_loaders);
ITC_FLAG_DECLARE(bool, native_car_models);
ITC_FLAG_DECLARE(bool, native_models);

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
// +0x400, its list at +0x2400, the blocks' headers at +0x2800. The
// constructors run on random bytes (r4 a list of 0 to 3 nodes); the
// releases and destructors on an object the plain C++ constructor made on
// an empty list, with blocks for the releases to put off (the XPRs' and,
// now and then, one part model's: at most eight nodes), the slots' buffers
// (pages each side allocates, shared or not) and the skeleton's node values.
// The deleting destructors with flags 0 or 2 (bit 0, operator delete, never
// seen in play, is left to the original).
struct ObjectLayout {
  bool (*make)(uint8_t*, uint32_t object, uint32_t list);
  std::vector<uint32_t> providers;  // in the object
  std::vector<uint32_t> xprs;       // in the object
  std::vector<uint32_t> models;     // part models in the object
  std::vector<uint32_t> slots;      // the slots the release frees
  uint32_t values;                  // the skeleton's node values (0: none)
};

void Lifetimes(Context& h, Random& r, uint32_t block) {
  if (!hooks::ModelsHostLock()) {
    ITC_LOG_WARN("harness: scene_loaders: --native_models was off at start-up: the lifetimes' "
                 "cases skipped");
    return;
  }
  std::lock_guard<std::recursive_mutex> lock(models::MemoryBlocksMutex());
  constexpr uint32_t kBytes = 0x3000;
  const uint32_t object = block + 0x400;
  const uint32_t list = block + 0x2400;
  const uint32_t headers = block + 0x2800;
  const uint32_t saved_set = guest::LoadU32(h.base, sl::kFirstSet);
  ObjectLayout pool4{sl::Pool4Construct, {}, {}, {112}, {80}, 0};
  ObjectLayout wheels{sl::WheelsConstruct, {}, {}, {}, {}, 0};
  for (uint32_t i = 0; i < 2; i++) {
    wheels.providers.push_back(sl::kWheelsLoads + sl::kWheelsProviders + 20 * i);
    wheels.xprs.push_back(sl::kWheelsLoads + sl::kWheelsXprs + 24 * i);
  }
  for (uint32_t offset : {sl::kWheelsTires, sl::kWheelsTires + 464, sl::kWheelsRims,
                          sl::kWheelsRims + 464, sl::kWheelsCalipers, sl::kWheelsCalipers + 464}) {
    wheels.models.push_back(sl::kWheelsLoads + offset);
  }
  wheels.slots = {192, 1168, 2144, 2280, 216, 1192, 2168, 2304};
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
  };
  for (const Function& f : functions) {
    const ObjectLayout& l = *f.layout;
    std::vector<uint32_t> providers;
    for (const uint32_t offset : l.providers) {
      providers.push_back(object + offset);
    }
    for (const uint32_t offset : l.models) {
      providers.push_back(object + offset + models::kModelTextures);
    }
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
            if (in.Below(3) == 0) {
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
  WheelsAndTraffic(h, r, block);
  Lifetimes(h, r, block);
  memory::Free(allocation);
}

}  // namespace harness
