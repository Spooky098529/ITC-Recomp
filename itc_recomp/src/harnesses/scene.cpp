// The car scene (src/scene/, docs/functions/scene.md) against the
// recompiled functions. The originals run with --native_scene off (their
// callees' own flags as they are), the plain C++ through the register steps
// (hooks/scene.h). The inputs go in a block from the allocation layer
// filled with the case's random words; the scene's globals ([0x826C2E44],
// [0x826C2E40], the accessories' set [0x826C5510]) point into it during a
// case and are put back after each side; pages a function frees are the
// case's own (allocated alike on both sides) and compared as whether they
// were freed. Compared with harness::Outputs.

#include "harness.h"

#include "accessories/accessories.h"
#include "base/flags.h"
#include "base/log.h"
#include "hooks/accessories.h"
#include "hooks/heap.h"
#include "hooks/models.h"
#include "hooks/scene.h"
#include "memory/heap.h"
#include "scene/scene.h"
#include "scene_loaders/pools.h"

#include <fmt/format.h>

#include <functional>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_scene);
ITC_FLAG_DECLARE(bool, native_accessories);

namespace harness {

namespace {

constexpr uint32_t kBlockBytes = 0x10000;
// ~PoolNode (sub_825940B0): unlinks the node; the fake destructor in DestroyA's
// pool objects' vtables (it ignores r4).
constexpr uint32_t kPoolNodeDestruct = 0x825940B0;

// A block of guest memory for the cases, freed at the end.
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

void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }

// The scene's globals, saved and put back.
struct Globals {
  explicit Globals(uint8_t* base) : base(base) {
    for (int i = 0; i < 3; i++) {
      words[i] = Load(base, kAddresses[i]);
    }
  }
  ~Globals() { Restore(); }
  void Restore() const {
    for (int i = 0; i < 3; i++) {
      Store(base, kAddresses[i], words[i]);
    }
  }
  static constexpr uint32_t kAddresses[3] = {scene::kScene, scene::kFirstSet,
                                             accessories::kSet};
  uint8_t* base;
  uint32_t words[3];
};

// The original with --native_scene off.
template <GuestFunction recompiled>
void Original(PPCContext& ctx, uint8_t* base) {
  ITC_FLAG(native_scene) = false;
  recompiled(ctx, base);
  ITC_FLAG(native_scene) = true;
}

// `count` cases of `recompiled` against `native`: the block random, then
// `setup` (registers and inputs), the call, then `collect`; the block, r3
// (both halves) and what `collect` adds compared. The globals put back
// after each side.
void Cases(Context& h, const Block& block, const std::string& name, int count,
           GuestFunction recompiled, GuestFunction native,
           const std::function<void(PPCContext&, Random&)>& setup,
           const std::function<void(const PPCContext&, Outputs&)>& collect = nullptr) {
  Results results(name);
  for (int c = 0; c < count; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Globals globals(h.base);
      Random r(seed);
      WriteRandom(h.base, block.address, block.bytes, r);
      PPCContext t = CallContext(h);
      setup(t, r);
      (v == 0 ? recompiled : native)(t, h.base);
      if (collect) {
        collect(t, out[v]);
      }
      out[v].Bytes(Read(h.base, block.address, block.bytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      for (const uint32_t address : Globals::kAddresses) {
        out[v].Word(Load(h.base, address));
      }
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// --- The small pieces ---

void SmallPieces(Context& h, const Block& block) {
  Cases(h, block, "SceneSetUp::SceneSetUp (sub_820D0D50)", 8, Original<__imp__sub_820D0D50>,
        hooks::SceneSetUpConstruct,
        [&](PPCContext& t, Random& r) { t.r3.u64 = block.address + 0x100 + 4 * r.Below(64); });
  Cases(h, block, "ScenePools::StartTeardownA (sub_820C3C80)", 8,
        Original<__imp__sub_820C3C80>, hooks::SceneStartTeardownA, [&](PPCContext& t, Random& r) {
          Store(h.base, scene::kFirstSet, block.address + 0x200 + 4 * r.Below(64));
          t.r3.u64 = r.Next();
        });
  // The scene none or in the block; the accessories' set in the block, its
  // entries none or words in the block whose counts stay above 0 (the
  // entries' destruction is the accessories harness's).
  Cases(
      h, block, "CarScene::StartTeardown (sub_820C4D88)", 32, Original<__imp__sub_820C4D88>,
      hooks::SceneStartTeardown, [&](PPCContext& t, Random& r) {
        Store(h.base, scene::kScene, r.Below(4) == 0 ? 0 : block.address + 0x1000);
        Store(h.base, scene::kFirstSet, block.address + 0x3000);
        const uint32_t set = block.address + 0x3400;
        Store(h.base, accessories::kSet, set);
        for (uint32_t i = 0; i < 3; i++) {
          const uint32_t entry = block.address + 0x3500 + 0x40 * i;
          Store(h.base, entry, 2 + r.Below(4));
          Store(h.base, set + 4 + 4 * i, r.Below(3) == 0 ? 0 : entry);
        }
        t.r3.u64 = r.Next();
      });
  // Pool 7's count 0 to 6 (or negative), its nodes in the block linked by
  // +8, each one's interior ready or not and its interior in the block.
  Cases(h, block, "CarScene::SetInteriorWord (sub_820C8950)", 32, Original<__imp__sub_820C8950>,
        hooks::SceneSetInteriorWord, [&](PPCContext& t, Random& r) {
          const uint32_t scene = block.address;
          const uint32_t nodes = 5 + r.Below(3);
          Store(h.base, scene + 5036, r.Below(8) == 0 ? 0xFFFFFFFFu : r.Below(nodes + 1));
          uint32_t previous = scene + 5028;  // the list's first-node word at +0
          for (uint32_t k = 0; k < nodes; k++) {
            const uint32_t node = block.address + 0x2000 + 0x100 * k;
            Store(h.base, previous == scene + 5028 ? previous : previous + 8, node);
            previous = node;
            Store(h.base, node + 5072, r.Below(3) == 0 ? 0 : r.Next() | 1);
            Store(h.base, node + 5068, block.address + 0x100 + 4 * r.Below(64));
          }
          Store(h.base, previous + 8, 0);
          t.r3.u64 = scene;
          t.r4.u64 = r.Next();
        });
}

// --- The pages ---

// FreePagesIn: the word holds the case's pages (allocated alike on both
// sides); compared as whether they were freed.
void Pages(Context& h, const Block& block) {
  if (!hooks::NativeHeapActive()) {
    ITC_LOG_WARN("harness: scene: the allocation layer isn't native: the frees' cases skipped");
    return;
  }
  Results results("FreePagesIn (sub_820D09A0)");
  const uint32_t word = block.address + 0x200;
  for (int c = 0; c < 8; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      WriteRandom(h.base, block.address, block.bytes, r);
      const uint32_t pages = memory::AllocPages(4096 * (1 + r.Below(3)), 4096);
      Store(h.base, word, pages);
      PPCContext t = CallContext(h);
      t.r3.u64 = word;
      (v == 0 ? Original<__imp__sub_820D09A0> : hooks::SceneFreePagesIn)(t, h.base);
      const bool freed = !memory::IsPages(pages);
      if (!freed) {
        memory::FreePages(pages);
      }
      out[v].Word(freed);
      out[v].Word(Load(h.base, word) == pages);
      Store(h.base, word, 0);
      out[v].Bytes(Read(h.base, block.address, block.bytes));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// DestroyA: the first set in the block with 0 to 3 objects in each of its
// seven pools, each the case's pages made a pool node (PoolNode::PoolNode)
// whose vtable's slot 0 is ~PoolNode (it unlinks the node). Compared: the
// block, which objects were freed, r3.
void DestroyA(Context& h, const Block& block) {
  if (!hooks::NativeHeapActive()) {
    return;
  }
  Results results("ScenePools::DestroyA (sub_820C3CF8)");
  const uint32_t set = block.address + 0x400;
  const uint32_t vtable = block.address + 0x300;
  for (int c = 0; c < 16; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      WriteRandom(h.base, block.address, block.bytes, r);
      Store(h.base, vtable, kPoolNodeDestruct);
      std::vector<uint32_t> objects;
      for (uint32_t pool = 0; pool < 7; pool++) {
        const uint32_t list = set + 56 + 12 * pool;
        Store(h.base, list + 0, 0);
        Store(h.base, list + 4, 0);
        Store(h.base, list + 8, 0);
        const uint32_t count = r.Below(4);
        for (uint32_t k = 0; k < count; k++) {
          const uint32_t object = memory::AllocPages(4096, 4096);
          WriteRandom(h.base, object, 64, r);
          scene_loaders::PoolNodeConstruct(h.base, object, list);
          Store(h.base, object, vtable);
          objects.push_back(object);
        }
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = set;
      (v == 0 ? Original<__imp__sub_820C3CF8> : hooks::SceneDestroyA)(t, h.base);
      out[v].Word(uint32_t(objects.size()));
      for (const uint32_t object : objects) {
        const bool freed = !memory::IsPages(object);
        out[v].Word(freed);
        if (!freed) {
          memory::FreePages(object);
        }
      }
      out[v].Bytes(Read(h.base, block.address, block.bytes));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

}  // namespace

void Scene(Context& h) {
  Block block(kBlockBytes);
  SmallPieces(h, block);
  Pages(h, block);
  DestroyA(h, block);
}

}  // namespace harness
