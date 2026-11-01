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
#include "pending_frees.h"

#include "accessories/accessories.h"
#include "base/flags.h"
#include "base/log.h"
#include "hooks/accessories.h"
#include "hooks/heap.h"
#include "hooks/models.h"
#include "hooks/scene.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "models/memory_blocks.h"
#include "models/model.h"
#include "models/texture_provider.h"
#include "models/xpr.h"
#include "scene/scene.h"
#include "scene_loaders/pools.h"

#include <fmt/format.h>

#include <algorithm>
#include <functional>
#include <mutex>
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

// --- The shared assets and the scene ---

constexpr uint32_t kStorageMarker = 0x53544F52;  // a provider's storage, taken

// A texture provider's storage after a case: compared as a marker, its
// size and capacity and its words (nulls after the constructors and
// ClearAll), then freed and the vector's words cleared.
void TakeProvider(uint8_t* base, uint32_t provider, Outputs& out) {
  const uint32_t vector = provider + models::kTextureProviderVector;
  const uint32_t first = Load(base, vector + models::kVectorFirst);
  const uint32_t last = Load(base, vector + models::kVectorLast);
  const uint32_t end = Load(base, vector + models::kVectorEnd);
  const bool heap = first != 0 && memory::IsHeapBlock(first);
  out.Word(heap ? kStorageMarker : first);
  out.Word(last - first);
  out.Word(end - first);
  if (heap && last - first <= 4096) {
    for (uint32_t p = first; p != last; p += 4) {
      out.Word(Load(base, p));
    }
  }
  if (heap) {
    memory::crt::OperatorDelete(first);
  }
  for (uint32_t w = 0; w < 3; w++) {
    Store(base, vector + 4 + 4 * w, 0);
  }
}

// Every provider in the assets (theirs and the part models'), taken.
void TakeAssetProviders(uint8_t* base, uint32_t assets, Outputs& out) {
  for (const uint32_t provider : scene::kAssetsProviders) {
    TakeProvider(base, assets + provider, out);
  }
  for (const uint32_t model : scene::kAssetsModels) {
    TakeProvider(base, assets + model + models::kModelTextures, out);
  }
}

// The register steps of the constructors without the flag (the harness's
// own way to make the assets the other cases start from).
void MakeAssets(PPCContext& ctx, uint8_t* base, uint32_t assets) {
  ctx.r3.u64 = assets;
  hooks::SceneAssetsConstruct(ctx, base);
}

// Blocks for the releases: some XPRs' header and data words, and the part
// models' +16, a tagged block (its header in the block from `headers`) or
// 0, from `r`.
void WriteAssetBlocks(uint8_t* base, uint32_t assets, uint32_t headers, Random& r) {
  uint32_t next = 0;
  auto block_for = [&]() {
    const uint32_t words[3] = {uint32_t(r.Next()), uint32_t(r.Next()), uint32_t(r.Next())};
    const uint32_t kind = r.Below(3) == 0 ? 2 : 0;
    const uint32_t header = headers + 16 * next++;
    return pending_frees::WriteBlock(base, kind, header, words);
  };
  for (const uint32_t xpr : scene::kAssetsXprs) {
    Store(base, assets + xpr + models::kXprHeader, block_for());
    Store(base, assets + xpr + models::kXprData, block_for());
  }
  for (const uint32_t model : scene::kAssetsModels) {
    Store(base, assets + model + 16, block_for());
  }
}

void Assets(Context& h, const Block& block) {
  if (!hooks::ModelsNativeLifetime() || !hooks::ModelsHostLock()) {
    ITC_LOG_WARN("harness: scene: the models' lifetime isn't native: the assets' cases skipped");
    return;
  }
  std::lock_guard<std::recursive_mutex> lock(models::MemoryBlocksMutex());
  const uint32_t assets = block.address + 0x100;
  const uint32_t headers = block.address + 0x8000;
  struct Case {
    const char* name;
    GuestFunction recompiled, native;
    bool made;  // the assets made first (the constructor's register step)
  };
  const Case cases[] = {
      {"SceneAssets::SceneAssets (sub_820C5008)", Original<__imp__sub_820C5008>,
       hooks::SceneAssetsConstruct, false},
      {"SceneAssets::Reset (sub_820C4A70, on the game's list)", Original<__imp__sub_820C4A70>,
       hooks::SceneAssetsReset, true},
      {"SceneAssets::~SceneAssets (sub_820C5168, on the game's list)",
       Original<__imp__sub_820C5168>, hooks::SceneAssetsDestruct, true},
  };
  for (const Case& f : cases) {
    Results results(f.name);
    for (int c = 0; c < 8; c++) {
      const uint64_t seed = h.random.Next();
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed);
        WriteRandom(h.base, block.address, block.bytes, r);
        PPCContext t = CallContext(h);
        if (f.made) {
          MakeAssets(t, h.base, assets);
          if (c > 0) {
            WriteAssetBlocks(h.base, assets, headers, r);
          }
        }
        const pending_frees::ListState was = pending_frees::SaveList(h.base);
        t.r3.u64 = assets;
        (v == 0 ? f.recompiled : f.native)(t, h.base);
        pending_frees::TakeAddedNodes(h.base, was, out[v]);
        TakeAssetProviders(h.base, assets, out[v]);
        out[v].Bytes(Read(h.base, block.address, block.bytes));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
}

// List::DeleteAll and ~List: 0 to 4 pool nodes in the list (in the block),
// each one's vtable slot 0 ~PoolNode (it unlinks the node; r4 ignored).
void Lists(Context& h, const Block& block) {
  const uint32_t list = block.address + 0x400;
  const uint32_t vtable = block.address + 0x300;
  const struct {
    const char* name;
    GuestFunction recompiled, native;
  } cases[] = {
      {"List::DeleteAll (sub_82593FF8)", Original<__imp__sub_82593FF8>,
       hooks::SceneListDeleteAll},
      {"List::~List (sub_82594188)", Original<__imp__sub_82594188>, hooks::SceneListDestruct},
  };
  for (const auto& f : cases) {
    Cases(h, block, f.name, 12, f.recompiled, f.native, [&](PPCContext& t, Random& r) {
      Store(h.base, vtable, kPoolNodeDestruct);
      Store(h.base, list + 0, 0);
      Store(h.base, list + 4, 0);
      Store(h.base, list + 8, 0);
      const uint32_t count = r.Below(5);
      for (uint32_t k = 0; k < count; k++) {
        const uint32_t node = block.address + 0x1000 + 0x40 * k;
        scene_loaders::PoolNodeConstruct(h.base, node, list);
        Store(h.base, node, vtable);
      }
      t.r3.u64 = list;
    });
  }
}

// CarScene::CarScene and ~CarScene (r4 = 0: the scene's pages are freed by
// its caller): the scene in the block, made first for the destructor, its
// lists empty or holding pool nodes (their slot 0 ~PoolNode).
void Scenes(Context& h, const Block& block) {
  if (!hooks::ModelsNativeLifetime() || !hooks::ModelsHostLock()) {
    return;
  }
  std::lock_guard<std::recursive_mutex> lock(models::MemoryBlocksMutex());
  const uint32_t scene = block.address + 0x100;
  const uint32_t headers = block.address + 0x8000;
  const uint32_t vtable = block.address + 0x40;
  for (int which = 0; which < 2; which++) {
    Results results(which == 0 ? "CarScene::CarScene (sub_820C89A8)"
                               : "CarScene::~CarScene (sub_820C9110, on the game's list)");
    for (int c = 0; c < 8; c++) {
      const uint64_t seed = h.random.Next();
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed);
        WriteRandom(h.base, block.address, block.bytes, r);
        PPCContext t = CallContext(h);
        if (which == 1) {
          t.r3.u64 = scene;
          hooks::SceneCarSceneConstruct(t, h.base);
          if (c > 0) {
            WriteAssetBlocks(h.base, scene + scene::kAssets, headers, r);
          }
          Store(h.base, vtable, kPoolNodeDestruct);
          for (uint32_t i = 0; i < 8; i++) {
            if (r.Below(4) == 0) {
              const uint32_t node = block.address + 0x2000 + 0x40 * i;
              scene_loaders::PoolNodeConstruct(h.base, node, scene + 4944 + 12 * i);
              Store(h.base, node, vtable);
            }
          }
          t.r4.u64 = 0;
        }
        const pending_frees::ListState was = pending_frees::SaveList(h.base);
        t.r3.u64 = scene;
        if (which == 0) {
          (v == 0 ? Original<__imp__sub_820C89A8> : hooks::SceneCarSceneConstruct)(t, h.base);
          TakeAssetProviders(h.base, scene + scene::kAssets, out[v]);
        } else {
          (v == 0 ? Original<__imp__sub_820C9110> : hooks::SceneCarSceneDestruct)(t, h.base);
        }
        pending_frees::TakeAddedNodes(h.base, was, out[v]);
        out[v].Bytes(Read(h.base, block.address, block.bytes));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
}

// ScenePools::UpdateA: the first set none, counting down its releases (in
// the block, the records' textures' words 0 or the case's pages for the
// headers, as Texture::Free takes them), or in a teardown (+4 = 4) with its
// count above 0 or at 0: then the set, its records and its pool objects
// the case's pages (the objects pool nodes whose slot 0 is ~PoolNode).
void UpdateA(Context& h, const Block& block) {
  if (!hooks::NativeHeapActive()) {
    return;
  }
  Results results("ScenePools::UpdateA (sub_820C3DA0)");
  const uint32_t vtable = block.address + 0x40;
  for (int c = 0; c < 32; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Globals globals(h.base);
      Random r(seed);
      WriteRandom(h.base, block.address, block.bytes, r);
      Store(h.base, vtable, kPoolNodeDestruct);
      const uint32_t kind = c < 4 ? 0 : 1 + r.Below(3);  // none, releases, teardown, due
      const bool due = kind == 3;
      const uint32_t set = due ? memory::AllocPages(4096, 4096) : block.address + 0x100;
      if (due) {
        WriteRandom(h.base, set, 4096, r);
      }
      Store(h.base, scene::kFirstSet, kind == 0 ? 0 : set);
      Store(h.base, set + 4, kind == 1 ? r.Below(4) : 4);
      Store(h.base, set + 0, kind == 2 ? 1 + r.Below(3) : 0);
      Store(h.base, set + 40, r.Below(3));
      Store(h.base, set + 12, r.Below(3));
      const uint32_t n = (Load(h.base, set + 40) + Load(h.base, set + 12)) * 4;
      const uint32_t records = due ? memory::AllocPages(4096, 4096) : block.address + 0x1000;
      Store(h.base, set + 52, records);
      std::vector<uint32_t> pages;
      for (uint32_t i = 0; i < n; i++) {
        const uint32_t record = records + 20 * i;
        for (uint32_t w = 0; w < 4; w++) {
          const bool header = (w & 1) == 0;
          // A header's word holds the case's pages or 0; a data word 0.
          uint32_t value = 0;
          if (header && r.Below(3) == 0) {
            value = memory::AllocPages(4096, 4096);
            pages.push_back(value);
          }
          Store(h.base, record + 4 * w, value);
        }
        Store(h.base, record + 16, r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(3));
      }
      std::vector<uint32_t> objects;
      for (uint32_t pool = 0; pool < 7; pool++) {
        const uint32_t list = set + 56 + 12 * pool;
        Store(h.base, list + 0, 0);
        Store(h.base, list + 4, 0);
        Store(h.base, list + 8, 0);
        const uint32_t count = due ? r.Below(3) : 0;
        for (uint32_t k = 0; k < count; k++) {
          const uint32_t object = memory::AllocPages(4096, 4096);
          scene_loaders::PoolNodeConstruct(h.base, object, list);
          Store(h.base, object, vtable);
          objects.push_back(object);
        }
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = r.Next();
      (v == 0 ? Original<__imp__sub_820C3DA0> : hooks::SceneUpdateA)(t, h.base);
      out[v].Word(Load(h.base, scene::kFirstSet) == set);
      for (const auto* list : {&pages, &objects}) {
        for (const uint32_t p : *list) {
          const bool freed = !memory::IsPages(p);
          out[v].Word(freed);
          if (!freed) {
            memory::FreePages(p);
          }
        }
      }
      if (due) {
        out[v].Word(!memory::IsPages(set));
        out[v].Word(!memory::IsPages(records));
        for (const uint32_t p : {set, records}) {
          if (memory::IsPages(p)) {
            memory::FreePages(p);
          }
        }
      } else {
        // The records' header words: the case's pages as a marker.
        for (uint32_t i = 0; i < n; i++) {
          for (uint32_t w = 0; w < 4; w++) {
            const uint32_t at = records + 20 * i + 4 * w;
            const uint32_t value = Load(h.base, at);
            if (value != 0 && std::find(pages.begin(), pages.end(), value) != pages.end()) {
              Store(h.base, at, kStorageMarker);
            }
          }
        }
        out[v].Bytes(Read(h.base, block.address, block.bytes));
      }
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}


// CarScene::InitLighting: the block in the scratch block, the frame below
// the call's stack compared too (its float4s are built there; the saved
// registers above +192 left out).
void Lighting(Context& h, const Block& block) {
  Results results("CarScene::InitLighting (sub_820C8528)");
  for (int c = 0; c < 4; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      WriteRandom(h.base, block.address, block.bytes, r);
      PPCContext t = CallContext(h);
      const uint32_t frame = t.r1.u32 - 288;
      WriteRandom(h.base, frame, 288, r);
      const uint32_t target = block.address + 0x200 + 16 * r.Below(4);
      t.r3.u64 = target;
      (v == 0 ? Original<__imp__sub_820C8528> : hooks::SceneInitLighting)(t, h.base);
      out[v].Floats(h.base, target, 69);
      out[v].Floats(h.base, frame + 4, 47);
      out[v].Word(Load(h.base, frame));
      out[v].Bytes(Read(h.base, block.address, 0x200));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// A word that may hold the case's pages: compared as whether it does (its
// pages freed then), else the word.
void TakePagesWord(uint8_t* base, uint32_t word, Outputs& out) {
  const uint32_t value = Load(base, word);
  if (value != 0 && memory::IsPages(value)) {
    out.Word(kStorageMarker);
    memory::FreePages(value);
    Store(base, word, kStorageMarker);
  } else {
    out.Word(value);
  }
}

// SceneAssets::Release: the assets made, their slots shared, empty or
// holding the case's pages (each its own), the TGA pixels' words the case's
// pages; compared: which pages were freed, the pending frees, the block.
void Releases(Context& h, const Block& block) {
  if (!hooks::ModelsNativeLifetime() || !hooks::ModelsHostLock() || !hooks::NativeHeapActive()) {
    return;
  }
  std::lock_guard<std::recursive_mutex> lock(models::MemoryBlocksMutex());
  Results results("SceneAssets::Release (sub_820C4C80, on the game's list)");
  const uint32_t assets = block.address + 0x100;
  const uint32_t headers = block.address + 0x8000;
  std::vector<uint32_t> slots = {0, 48, 72, 184, 252, 320, 388, 880, 904, 952, 1020,
                                 4048, 4072, 4560};
  for (uint32_t i = 0; i < 4; i++) {
    slots.push_back(1088 + 24 * i);
    slots.push_back(3040 + 24 * i);
    slots.push_back(4608 + 24 * i);
  }
  for (uint32_t i = 0; i < 19; i++) {
    slots.push_back(3136 + 24 * i);
  }
  for (int c = 0; c < 8; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      WriteRandom(h.base, block.address, block.bytes, r);
      PPCContext t = CallContext(h);
      MakeAssets(t, h.base, assets);
      if (c > 0) {
        WriteAssetBlocks(h.base, assets, headers, r);
      }
      std::vector<uint32_t> pages;
      for (const uint32_t slot : slots) {
        const uint32_t kind = r.Below(3);
        Store(h.base, assets + slot + 12, kind == 0 ? 1 : 0);
        uint32_t buffer = 0;
        if (kind == 2) {
          buffer = memory::AllocPages(4096, 4096);
          pages.push_back(buffer);
        } else if (kind == 0) {
          buffer = r.Next() | 1;
        }
        Store(h.base, assets + slot + 16, buffer);
      }
      for (uint32_t i = 0; i < 20; i++) {
        const uint32_t pixels = memory::AllocPages(4096, 4096);
        pages.push_back(pixels);
        Store(h.base, assets + (i == 0 ? 948 : 3592 + 4 * (i - 1)), pixels);
      }
      const pending_frees::ListState was = pending_frees::SaveList(h.base);
      t.r3.u64 = assets;
      (v == 0 ? Original<__imp__sub_820C4C80> : hooks::SceneAssetsRelease)(t, h.base);
      pending_frees::TakeAddedNodes(h.base, was, out[v]);
      TakeAssetProviders(h.base, assets, out[v]);
      for (const uint32_t p : pages) {
        const bool freed = !memory::IsPages(p);
        out[v].Word(freed);
        if (!freed) {
          memory::FreePages(p);
        }
      }
      out[v].Bytes(Read(h.base, block.address, block.bytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// CarScene::Create: the scene, the first set and the accessories' set none
// before (their globals and the palette and draw words put back after each
// side); compared: the scene's 5,536 bytes (its first set's pointer and the
// providers' storage as markers), the first set's 140 and the accessories'
// set's 32, the palette, the two words, r3.
void Creates(Context& h, const Block& block) {
  if (!hooks::ModelsNativeLifetime() || !hooks::ModelsHostLock() || !hooks::NativeHeapActive()) {
    return;
  }
  std::lock_guard<std::recursive_mutex> lock(models::MemoryBlocksMutex());
  Results results("CarScene::Create (sub_820C8A50)");
  constexpr uint32_t kPalette = 0x826C5110, kPaletteBytes = 1024, kWords = 0x82601854;
  for (int c = 0; c < 2; c++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Globals globals(h.base);
      const std::vector<uint8_t> palette = Read(h.base, kPalette, kPaletteBytes);
      const std::vector<uint8_t> words = Read(h.base, kWords, 8);
      Store(h.base, scene::kScene, 0);
      Store(h.base, scene::kFirstSet, 0);
      Store(h.base, accessories::kSet, 0);
      PPCContext t = CallContext(h);
      (v == 0 ? Original<__imp__sub_820C8A50> : hooks::SceneCreate)(t, h.base);
      const uint32_t scene = Load(h.base, scene::kScene);
      const uint32_t first = Load(h.base, scene::kFirstSet);
      const uint32_t set = Load(h.base, accessories::kSet);
      out[v].Word(memory::IsPages(scene));
      out[v].Word(memory::IsPages(first));
      out[v].Word(memory::IsPages(set));
      out[v].Word(t.r3.u32 == set);
      if (memory::IsPages(scene)) {
        TakeAssetProviders(h.base, scene + scene::kAssets, out[v]);
        out[v].Word(Load(h.base, scene + 5040) == first);
        Store(h.base, scene + 5040, 0);
        out[v].Bytes(Read(h.base, scene, scene::kSceneBytes));
        memory::FreePages(scene);
      }
      if (memory::IsPages(first)) {
        out[v].Bytes(Read(h.base, first, 140));
        memory::FreePages(first);
      }
      if (memory::IsPages(set)) {
        out[v].Bytes(Read(h.base, set, 32));
        memory::FreePages(set);
      }
      out[v].Bytes(Read(h.base, kPalette, kPaletteBytes));
      out[v].Bytes(Read(h.base, kWords, 8));
      Write(h.base, kPalette, palette.data(), kPaletteBytes);
      Write(h.base, kWords, words.data(), 8);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
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
  Assets(h, block);
  Lists(h, block);
  Scenes(h, block);
  UpdateA(h, block);
  Lighting(h, block);
  Releases(h, block);
  Creates(h, block);
}

}  // namespace harness
