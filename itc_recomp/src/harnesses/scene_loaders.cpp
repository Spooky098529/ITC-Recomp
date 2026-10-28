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
#include "scene_loaders/pools.h"

#include <fmt/format.h>

#include <functional>
#include <mutex>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_scene_loaders);
ITC_FLAG_DECLARE(bool, native_car_models);

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
  memory::Free(allocation);
}

}  // namespace harness
