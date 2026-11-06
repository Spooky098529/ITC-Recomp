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
#include "hooks/archive.h"
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
#include <optional>
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


// CarScene::LoadAssets with its loads recorded (each case's own recorder):
// the scene in the block and [0x826C2E44] pointing at it (LoadShared reads
// the shared buffer's size there), set a or b, +5044 0 or not, the shared
// buffer in the block and its size small or large; the part files' pages
// compared as markers.
void LoadAssetsCases(Context& h, const Block& block) {
  if (!hooks::NativeHeapActive() || !hooks::NativeArchiveQueue()) {
    return;
  }
  Results results("CarScene::LoadAssets (sub_820C8B40)");
  const uint32_t scene = block.address + 0x100;
  constexpr uint32_t kOwn[] = {452, 4112, 1152, 1176, 1200, 1224};
  for (int c = 0; c < 8; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Globals globals(h.base);
      Random r(seed);
      WriteRandom(h.base, block.address, block.bytes, r);
      Store(h.base, scene::kScene, scene);
      Store(h.base, scene + 4, r.Below(2));
      Store(h.base, scene + 5044, r.Below(2));
      Store(h.base, scene + 5056, block.address + 0x4000);
      Store(h.base, scene + 5060, r.Below(2) ? 0x100000 : r.Below(0x20000));
      for (const uint32_t at : kOwn) {
        Store(h.base, scene + at + 16, 0);
      }
      std::vector<int64_t> serials;
      for (int k = 0; k < 64; k++) {
        serials.push_back(r.Below(8) == 0 ? -1 : int64_t(r.Below(0x7FFFFFFF)));
      }
      std::optional<ArchiveLoads> loads;
      loads.emplace(serials);
      PPCContext t = CallContext(h);
      t.r3.u64 = scene;
      (v == 0 ? Original<__imp__sub_820C8B40> : hooks::SceneLoadAssets)(t, h.base);
      std::vector<uint32_t> calls = loads->calls();
      for (uint32_t& w : calls) {
        if (w != 0 && memory::IsPages(w)) {
          w = kStorageMarker;
        }
      }
      out[v].Word(uint32_t(calls.size()));
      for (const uint32_t w : calls) {
        out[v].Word(w);
      }
      for (const uint32_t at : kOwn) {
        TakePagesWord(h.base, scene + at + 16, out[v]);
      }
      out[v].Bytes(Read(h.base, block.address, block.bytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}


// CarScene::OnFileLoaded: the assets made (the constructor's register step)
// in a scene at the block's +0x100, every slot's request distinct; r3 one
// of an XPR2 slot's (r4 random words: XPR::Create's error path, the
// providers' TextureProvider::Set), a part file's, a TGA's (a small
// uncompressed one) or none. The texture slots'
// branches (Model::Load on a part file) aren't among the cases: they run
// at every scene's set-up in play. Compared: the pending frees, the
// providers, the slots' pages, the block, the word at the caller's frame's
// +28, r3.
void FileLoadedCases(Context& h, const Block& block) {
  if (!hooks::ModelsNativeLifetime() || !hooks::ModelsHostLock() || !hooks::NativeHeapActive()) {
    return;
  }
  std::lock_guard<std::recursive_mutex> lock(models::MemoryBlocksMutex());
  Results results("CarScene::OnFileLoaded (sub_820C52A0)");
  const uint32_t scene = block.address + 0x100;
  const uint32_t assets = scene + scene::kAssets;
  std::vector<uint32_t> slots = {0, 48, 72, 184, 252, 320, 388, 904, 952, 1020, 4048, 4560};
  for (uint32_t i = 0; i < 4; i++) {
    slots.push_back(1088 + 24 * i);
    slots.push_back(4608 + 24 * i);
  }
  for (uint32_t i = 0; i < 19; i++) {
    slots.push_back(3136 + 24 * i);
  }
  const uint32_t others[] = {880, 3040, 3064, 3088, 3112, 4072};
  for (int c = 0; c < 48; c++) {
    const uint64_t seed = h.random.Next();
    const uint32_t which = uint32_t(c) % uint32_t(slots.size() + 1);
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      WriteRandom(h.base, block.address, block.bytes, r);
      PPCContext t = CallContext(h);
      MakeAssets(t, h.base, assets);
      uint32_t k = 1;
      for (const uint32_t slot : slots) {
        Store(h.base, assets + slot + 4, 0x10000u * k++);
        Store(h.base, assets + slot + 12, 0);
        Store(h.base, assets + slot + 16, 0);
      }
      for (const uint32_t slot : others) {
        Store(h.base, assets + slot + 4, 0x10000u * k++);
      }
      uint32_t request = 0x123;
      uint32_t pages = 0;
      if (which < slots.size()) {
        const uint32_t slot = assets + slots[which];
        request = Load(h.base, slot + 4);
        if (r.Below(4) == 0) {
          Store(h.base, slot + 12, 1);
          Store(h.base, slot + 16, r.Next() | 1);
        } else {
          pages = memory::AllocPages(4096, 4096);
          WriteRandom(h.base, pages, 4096, r);
          Store(h.base, slot + 16, pages);
        }
        Store(h.base, slot + 8, 64 + r.Below(1024));
      }
      const uint32_t data = block.address + 0xC000;
      if (which < slots.size() && (slots[which] == 904 || slots[which] >= 3136 && slots[which] < 3592)) {
        // A TGA (uncompressed, 32 bits a pixel, 1 to 8 pixels square, its
        // origin at the top or bottom), its pixels random.
        const uint32_t w = 1 + r.Below(8), hgt = 1 + r.Below(8);
        const uint8_t header[18] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, uint8_t(w), 0,
                                    uint8_t(hgt), 0, 32, uint8_t(r.Below(2) ? 0x28 : 0x08)};
        Write(h.base, data, header, 18);
        Store(h.base, assets + slots[which] + 8, 18 + 4 * w * hgt);
      }
      for (uint32_t i = 0; i < 20; i++) {
        Store(h.base, assets + (i == 0 ? 948 : 3592 + 4 * (i - 1)), 0);
      }
      const pending_frees::ListState was = pending_frees::SaveList(h.base);
      t.r3.u64 = request;
      t.r4.u64 = data;
      t.r6.u64 = scene;
      const uint32_t sp = t.r1.u32;
      (v == 0 ? Original<__imp__sub_820C52A0> : hooks::SceneOnFileLoaded)(t, h.base);
      pending_frees::TakeAddedNodes(h.base, was, out[v]);
      TakeAssetProviders(h.base, assets, out[v]);
      if (pages != 0) {
        const bool freed = !memory::IsPages(pages);
        out[v].Word(freed);
        if (!freed) {
          memory::FreePages(pages);
        }
        for (const uint32_t slot : slots) {
          if (Load(h.base, assets + slot + 16) == pages) {
            Store(h.base, assets + slot + 16, kStorageMarker);
          }
        }
      }
      for (uint32_t i = 0; i < 20; i++) {
        const uint32_t word = assets + (i == 0 ? 948 : 3592 + 4 * (i - 1));
        if (Load(h.base, word) != 0 && memory::IsPages(Load(h.base, word))) {
          out[v].Bytes(Read(h.base, Load(h.base, word), 256));
        }
        TakePagesWord(h.base, word, out[v]);
      }
      out[v].Word(Load(h.base, sp + 28));
      out[v].Bytes(Read(h.base, block.address, block.bytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("case {} (slot {})", c,
                                              which < slots.size() ? slots[which] : 0xFFFFu));
  }
  results.Report();
}

// CarScene::LoadsDone: a scene in a block of its own, its eight asset slots
// loading (2) now and then, its eight pools' lists of up to 3 objects
// (0x800 apart, so the fields each pool reads don't overlap), each in use
// (+16 = 3) or not, with their finishers' finished words set (so each
// answers 1 at once: the finishers are the load poll harness's) and the
// answers of 0 from pool 0's car models (CarModel::IsReady over a model
// set word of 0 to 2 and slots loading or not, the decals' state) and pool
// 7's deferred build.
void LoadsDoneCases(Context& h) {
  Block block(0x19000);
  const uint32_t scene = block.address;
  Cases(h, block, "CarScene::LoadsDone (sub_820C5798)", 60, Original<__imp__sub_820C5798>,
        hooks::SceneLoadsDone, [&](PPCContext& t, Random& r) {
          static constexpr uint32_t kSlots[8] = {84, 132, 156, 268, 472, 964, 4156, 4764};
          for (const uint32_t slot : kSlots) {
            Store(h.base, scene + slot, r.Below(24) == 0 ? 2 : r.Below(2));
          }
          for (uint32_t m = 0; m < 2; m++) {
            const uint32_t model = block.address + 0x14000 + 0x2000 * m;
            Store(h.base, model + 7256, block.address + 0x18000 + 16 * r.Below(4) - 8);
            Store(h.base, model + 3748, r.Below(6) == 0 ? 2 : 1);
            Store(h.base, model + 3972, r.Below(6) == 0 ? 2 : 0);
            Store(h.base, model + 3996, r.Below(6) == 0 ? 2 : 3);
            Store(h.base, model + 4088, r.Below(6) == 0 ? 2 : 1);
          }
          for (uint32_t j = 0; j < 4; j++) {
            Store(h.base, block.address + 0x18000 + 16 * j, r.Below(4) == 0 ? 2 : r.Below(2));
          }
          static constexpr uint32_t kFinished[8] = {0, 100, 4312, 11660, 13156, 16768, 9792, 0};
          for (uint32_t pool = 0; pool < 8; pool++) {
            const uint32_t list = scene + 4944 + 12 * pool;
            const uint32_t count = r.Below(4);
            // Now and then a count past the nodes (List::At answers the
            // first node again; not for an empty list, where it answers 0).
            Store(h.base, list + 8, count != 0 && r.Below(10) == 0 ? count + 1 : count);
            uint32_t previous = list;  // the list's first-node word at +0
            for (uint32_t k = 0; k < count; k++) {
              const uint32_t object = block.address + 0x2000 + 0x800 * (3 * pool + k);
              Store(h.base, previous == list ? previous : previous + 8, object);
              previous = object;
              Store(h.base, object + 16, r.Below(4) == 0 ? r.Below(3) : 3);
              if (pool == 0) {
                Store(h.base, object + 24, block.address + 0x14000 + 0x2000 * r.Below(2));
              } else if (pool == 7) {
                Store(h.base, object + 5040, r.Below(4) == 0 ? 1 : 0);
              } else {
                Store(h.base, object + kFinished[pool], 1 + r.Below(3));
              }
            }
            Store(h.base, previous == list ? list : previous + 8, 0);
          }
          t.r3.u64 = scene;
        });
}


// --- The scene's update ---

// Accessories::Free's fake destructor for the set (a leaf storing r4 and r5
// at +444 and +448; the accessories harness's).
constexpr uint32_t kRecordR4R5 = 0x820CE9F8;
// The word CarScene::Update sets to 3 (Modes::Tick counts it down).
constexpr uint32_t kSceneCountdown = 0x8260185C;

// Pool nodes (the case's pages, not in use) in some of the second set's
// lists, their slot 0 ~PoolNode.
std::vector<uint32_t> AddPoolObjects(uint8_t* base, uint32_t scene, uint32_t vtable, Random& r) {
  std::vector<uint32_t> objects;
  Store(base, vtable, kPoolNodeDestruct);
  for (uint32_t pool = 0; pool < 8; pool++) {
    const uint32_t count = r.Below(3);
    for (uint32_t k = 0; k < count; k++) {
      const uint32_t object = memory::AllocPages(4096, 4096);
      scene_loaders::PoolNodeConstruct(base, object, scene + 4944 + 12 * pool);
      Store(base, object, vtable);
      objects.push_back(object);
    }
  }
  return objects;
}

void TakeFreed(uint8_t* base, const std::vector<uint32_t>& pages, Outputs& out) {
  for (const uint32_t p : pages) {
    const bool freed = !memory::IsPages(p);
    out.Word(freed);
    if (!freed) {
      memory::FreePages(p);
    }
  }
}

// Pool 7's scene models for CarScene::UpdateModels: 0 to 4 in the block at
// +0x4000 + 0x1400 i (in use or not), the count -1 to 5 (past the nodes
// List::At answers the first again), each one's update as the scene models'
// harness builds it (no build due, the accessory none: the accessories'
// set at +0x3F00, its entries 0).
void AddSceneModels(uint8_t* base, uint32_t scene, uint32_t block, Random& r) {
  const uint32_t list = scene + 5028;
  const uint32_t set = block + 0x3F00;
  Store(base, accessories::kSet, set);
  for (uint32_t i = 0; i < 3; i++) {
    Store(base, set + 4 + 4 * i, 0);
  }
  const uint32_t nodes = r.Below(5);
  Store(base, list + 0, 0);
  uint32_t previous = 0;
  for (uint32_t k = 0; k < nodes; k++) {
    const uint32_t model = block + 0x4000 + 0x1400 * k;
    Store(base, previous == 0 ? list : previous + 8, model);
    Store(base, model + 8, 0);
    previous = model;
    Store(base, model + 16, r.Below(4) == 0 ? r.Below(3) : 3);
    const int32_t count = int32_t(r.Below(5)) - 1;
    Store(base, model + 5044, uint32_t(count));
    Store(base, model + 5040, count == 1 ? 0 : r.Below(2));
    Store(base, model + 5116, scene);
    for (const uint32_t at : {5072u, 5048u, 5076u, 4296u}) {
      Store(base, model + at, r.Below(5) == 0 ? 0 : uint32_t(r.Next()) | 1);
    }
    const uint32_t interior = block + 0x3E00;
    Store(base, model + 5068, interior);
    Store(base, interior + 36, interior + 0x40);
    Store(base, interior + 0x40 + 140, r.Below(4));
  }
  Store(base, scene + 5048, r.Below(4) == 0 ? 1 : 0);
  Store(base, scene + 4, r.Below(2));
  Store(base, scene + 5036, nodes == 0 ? uint32_t(int32_t(r.Below(2)) - 1)
                                       : nodes + (r.Below(6) == 0 ? 1 : 0));
}

// ScenePools::DestroyB: the scene in the block, its shared assets' slots
// loading or not, pool nodes (pages, not in use: LoadsDone's finishers not
// called) in its lists; compared: which were freed, the block, r3.
void DestroyBCases(Context& h, const Block& block) {
  if (!hooks::NativeHeapActive()) {
    return;
  }
  const uint32_t scene = block.address + 0x100;
  const uint32_t vtable = block.address + 0x40;
  std::vector<uint32_t> objects;
  Cases(
      h, block, "ScenePools::DestroyB (sub_820C59B0)", 12, Original<__imp__sub_820C59B0>,
      hooks::SceneDestroyB,
      [&](PPCContext& t, Random& r) {
        static constexpr uint32_t kSlots[8] = {84, 132, 156, 268, 472, 964, 4156, 4764};
        for (const uint32_t slot : kSlots) {
          Store(h.base, scene + slot, r.Below(3));
        }
        for (uint32_t pool = 0; pool < 8; pool++) {
          for (uint32_t w = 0; w < 3; w++) {
            Store(h.base, scene + 4944 + 12 * pool + 4 * w, 0);
          }
        }
        objects = AddPoolObjects(h.base, scene, vtable, r);
        t.r3.u64 = scene;
      },
      [&](const PPCContext&, Outputs& out) { TakeFreed(h.base, objects, out); });
}

// sub_820C5A60 (CarScene::UpdateModels): AddSceneModels' models; compared:
// the block (the models and their scene), the globals, r3.
void UpdateModelsCases(Context& h, const Block& block) {
  const uint32_t scene = block.address + 0x100;
  Cases(h, block, "CarScene::UpdateModels (sub_820C5A60)", 12, __imp__sub_820C5A60,
        hooks::SceneUpdateModels, [&](PPCContext& t, Random& r) {
          AddSceneModels(h.base, scene, block.address, r);
          t.r3.u64 = scene;
        });
}

// CarScene::Update: no scene; a teardown counting down; the scene's models
// updated (AddSceneModels) with the first set none or counting down no
// releases; or, with the models' lifetime native, a teardown due: the scene
// the case's pages, made by CarScene::CarScene's register step, pool nodes
// in its lists, +40 0 or not, then the accessories' set none or the case's
// pages (its slot 0 the leaf recorder). Compared: the block, which pages
// were freed, the pending frees, the globals and kSceneCountdown, r3.
void UpdateCases(Context& h, const Block& block) {
  const bool due_ok = hooks::NativeHeapActive() && hooks::ModelsNativeLifetime() &&
                      hooks::ModelsHostLock();
  std::unique_lock<std::recursive_mutex> lock(models::MemoryBlocksMutex(), std::defer_lock);
  if (due_ok) {
    lock.lock();
  }
  Results results("CarScene::Update (sub_820C9608)");
  const uint32_t vtable = block.address + 0x40, set_vtable = block.address + 0x60;
  const uint32_t first_set = block.address + 0x3C00;
  const uint32_t saved_countdown = Load(h.base, kSceneCountdown);
  for (int c = 0; c < 24; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Globals globals(h.base);
      Random r(seed);
      WriteRandom(h.base, block.address, block.bytes, r);
      Store(h.base, kSceneCountdown, uint32_t(r.Next()));
      PPCContext t = CallContext(h);
      const uint32_t kind = c < 2 ? 0 : (due_ok ? 1 + r.Below(3) : 1 + r.Below(2));
      std::vector<uint32_t> pages;
      uint32_t scene = 0;
      Store(h.base, scene::kFirstSet, 0);
      Store(h.base, accessories::kSet, 0);
      if (kind == 1) {
        scene = block.address + 0x100;
        Store(h.base, scene + 0, 4);
        Store(h.base, scene + 48, 1 + r.Below(3));
      } else if (kind == 2) {
        scene = block.address + 0x100;
        Store(h.base, scene + 0, r.Below(4) == 0 ? 4 + 1 + r.Below(3) : r.Below(4));
        AddSceneModels(h.base, scene, block.address, r);
        if (r.Below(2) == 0) {
          // A first set with no releases and no teardown.
          Store(h.base, scene::kFirstSet, first_set);
          Store(h.base, first_set + 4, r.Below(4));
          Store(h.base, first_set + 40, 0);
          Store(h.base, first_set + 12, 0);
        }
      } else if (kind == 3) {
        scene = memory::AllocPages(scene::kSceneBytes, 4096);
        pages.push_back(scene);
        t.r3.u64 = scene;
        hooks::SceneCarSceneConstruct(t, h.base);
        Store(h.base, scene + 0, 4);
        Store(h.base, scene + 48, 0);
        Store(h.base, scene + 40, r.Below(2) == 0 ? 0 : uint32_t(r.Next()) | 1);
        const std::vector<uint32_t> objects = AddPoolObjects(h.base, scene, vtable, r);
        pages.insert(pages.end(), objects.begin(), objects.end());
        if (r.Below(2) == 0) {
          const uint32_t set = memory::AllocPages(accessories::kSetBytes, 4096);
          Store(h.base, set_vtable, kRecordR4R5);
          Store(h.base, set, set_vtable);
          Store(h.base, accessories::kSet, set);
          pages.push_back(set);
        }
      }
      Store(h.base, scene::kScene, scene);
      const pending_frees::ListState was = pending_frees::SaveList(h.base);
      t.r3.u64 = r.Next();
      (v == 0 ? Original<__imp__sub_820C9608> : hooks::SceneUpdate)(t, h.base);
      pending_frees::TakeAddedNodes(h.base, was, out[v]);
      out[v].Word(Load(h.base, scene::kScene) == scene);
      out[v].Word(Load(h.base, kSceneCountdown));
      TakeFreed(h.base, pages, out[v]);
      out[v].Bytes(Read(h.base, block.address, block.bytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  Store(h.base, kSceneCountdown, saved_countdown);
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
  LoadAssetsCases(h, block);
  FileLoadedCases(h, block);
  LoadsDoneCases(h);
  DestroyBCases(h, block);
  UpdateModelsCases(h, block);
  UpdateCases(h, block);
}

}  // namespace harness
