// The app framework (src/framework/, docs/functions/framework.md) against
// the recompiled functions. The getters and thunks read objects the cases
// make in the scratch area (random words); the debug font's position
// (0x8296BCE0, the game's) is kept and put back around each case.
//
// The run loop's containers: each version builds its own handler map with
// the game's functions (HandlerMap::HandlerMap and Insert, with
// --native_framework off), then the function runs (the original with the
// flag off, so its callees are the originals too, or its plain C++), and
// what it leaves is compared: the tree walked from its head (every node met
// numbered in the order met, so the two versions' heap addresses don't
// matter: each node's links by number, key, delegate, color, isnil, and
// whether it's a heap block and its size), the map's size, the iterators
// and r3 by number. The nodes are freed after each version. The functions
// that allocate run only with --native_heap (the adapters' condition).

#include "harness.h"

#include "base/flags.h"
#include "framework/framework.h"
#include "framework/run_loop.h"
#include "hooks/framework.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"
#include "base/log.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"

#include <rex/memory/utils.h>

#include <fmt/format.h>

#include <algorithm>
#include <array>
#include <deque>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

ITC_FLAG_DECLARE(bool, native_framework);

namespace harness {

namespace {

namespace fw = framework;

// The register steps: the plain C++ through these, not the adapters.
#define ITC_NATIVE_WORD(name, function)       \
  void name(PPCContext& ctx, uint8_t* base) { \
    ctx.r3.u64 = fw::function(base, ctx.r3.u32); \
  }

ITC_NATIVE_WORD(NativeFlag133, Flag133)
ITC_NATIVE_WORD(NativeFlag134, Flag134)
ITC_NATIVE_WORD(NativeWord60, Word60)
ITC_NATIVE_WORD(NativeWord64, Word64)
ITC_NATIVE_WORD(NativeWord88, Word88)
ITC_NATIVE_WORD(NativeWord68, Word68)
ITC_NATIVE_WORD(NativeWord44, Word44)
ITC_NATIVE_WORD(NativeInputManagerFrom36, InputManagerFrom36)
ITC_NATIVE_WORD(NativeRendererFrom52, RendererFrom52)
ITC_NATIVE_WORD(NativeWorkerPoolFrom52, WorkerPoolFrom52)
ITC_NATIVE_WORD(NativeRendererOrTextFrom24, RendererOrTextFrom24)
ITC_NATIVE_WORD(NativeSceneFrom52, SceneFrom52)
ITC_NATIVE_WORD(NativeRendererFrom36, RendererFrom36)
ITC_NATIVE_WORD(NativeWorkerPoolFrom36, WorkerPoolFrom36)
ITC_NATIVE_WORD(NativeSceneFrom36, SceneFrom36)

void NativeDebugFontSetPosition(PPCContext& ctx, uint8_t* base) {
  fw::DebugFontSetPosition(base, ctx.r3.u32, ctx.r4.u32);
  ctx.r11.u64 = fw::kDebugFontPosition;
}

void NativeDebugTextSetPosition(PPCContext& ctx, uint8_t* base) {
  const uint32_t x = guest::LoadU32(base, ctx.r4.u32);
  const uint32_t y = guest::LoadU32(base, ctx.r5.u32);
  fw::DebugTextSetPosition(base, ctx.r4.u32, ctx.r5.u32);
  ctx.r3.u64 = x;
  ctx.r4.u64 = y;
  ctx.r11.u64 = fw::kDebugFontPosition;
}

struct Getter {
  const char* name;
  GuestFunction recompiled;
  GuestFunction native;
  uint32_t adjust;  // the interface's offset in the object (0 for a getter)
};

// Random words in the scratch area's first 512 bytes, then r3 at an object
// (or an interface `adjust` bytes into it) placed at a random word.
void Getters(Context& h) {
  const Getter getters[] = {
      {"Flag133 (sub_82090760)", __imp__sub_82090760, NativeFlag133, 0},
      {"Flag134 (sub_82090768)", __imp__sub_82090768, NativeFlag134, 0},
      {"Word60 (sub_82090770)", __imp__sub_82090770, NativeWord60, 0},
      {"Word64 (sub_82090778)", __imp__sub_82090778, NativeWord64, 0},
      {"Word88 (sub_82090788)", __imp__sub_82090788, NativeWord88, 0},
      {"Word68 (sub_8248F030)", __imp__sub_8248F030, NativeWord68, 0},
      {"Word44 (sub_824892F8)", __imp__sub_824892F8, NativeWord44, 0},
      {"InputManagerFrom36 (sub_82090830)", __imp__sub_82090830, NativeInputManagerFrom36, 12},
      {"RendererFrom52 (sub_82090828)", __imp__sub_82090828, NativeRendererFrom52, 44},
      {"WorkerPoolFrom52 (sub_82090838)", __imp__sub_82090838, NativeWorkerPoolFrom52, 28},
      {"RendererOrTextFrom24 (sub_82090860)", __imp__sub_82090860, NativeRendererOrTextFrom24, 16},
      {"SceneFrom52 (sub_82090868)", __imp__sub_82090868, NativeSceneFrom52, 32},
      {"RendererFrom36 (sub_820908E0)", __imp__sub_820908E0, NativeRendererFrom36, 28},
      {"WorkerPoolFrom36 (sub_820908E8)", __imp__sub_820908E8, NativeWorkerPoolFrom36, 12},
      {"SceneFrom36 (sub_82090908)", __imp__sub_82090908, NativeSceneFrom36, 16},
  };
  for (const Getter& g : getters) {
    Results results(g.name);
    for (int c = 0; c < 16; c++) {
      std::vector<uint32_t> words(128);
      for (auto& w : words) {
        w = uint32_t(h.random.Next());
      }
      const uint32_t object = h.data + 4 * h.random.Below(64);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        for (uint32_t i = 0; i < words.size(); i++) {
          guest::StoreU32(h.base, h.data + 4 * i, words[i]);
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = object + g.adjust;
        (v == 0 ? g.recompiled : g.native)(t, h.base);
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("{} case {}", g.name, c));
    }
    results.Report();
  }
}

void Positions(Context& h) {
  Results font("DebugFont::SetPosition (sub_8258BE08)");
  Results text("DebugText::SetPosition (sub_82091050)");
  const std::vector<uint8_t> kept = Read(h.base, fw::kDebugFontPosition, 16);
  const uint32_t edge[][2] = {{64, 64}, {0, 0}, {0xFFFFFFFF, 0x80000000}};
  for (int c = 0; c < 19; c++) {
    const uint32_t x = c < 3 ? edge[c][0] : uint32_t(h.random.Next());
    const uint32_t y = c < 3 ? edge[c][1] : uint32_t(h.random.Next());
    for (int which = 0; which < 2; which++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        Fill(h.base, fw::kDebugFontPosition, 16);
        PPCContext t = CallContext(h);
        if (which == 0) {
          t.r3.u64 = x;
          t.r4.u64 = y;
          (v == 0 ? __imp__sub_8258BE08 : NativeDebugFontSetPosition)(t, h.base);
        } else {
          guest::StoreU32(h.base, h.data + 8, x);
          guest::StoreU32(h.base, h.data + 20, y);
          t.r3.u64 = h.data + 100;
          t.r4.u64 = h.data + 8;
          t.r5.u64 = h.data + 20;
          (v == 0 ? __imp__sub_82091050 : NativeDebugTextSetPosition)(t, h.base);
        }
        out[v].Bytes(Read(h.base, fw::kDebugFontPosition, 16));
        out[v].Bytes(Read(h.base, h.data, 64));
        out[v].Word(t.r3.u32);
        out[v].Word(t.r4.u32);
        out[v].Word(t.r11.u32);
      }
      (which == 0 ? font : text).Check(out[0], out[1], fmt::format("case {}", c));
    }
  }
  Write(h.base, fw::kDebugFontPosition, kept.data(), 16);
  font.Report();
  text.Report();
}

// --- The run loop's containers ---

namespace rl = framework;

struct FlagOff {
  FlagOff() : saved_(ITC_FLAG(native_framework)) { ITC_FLAG(native_framework) = false; }
  ~FlagOff() { ITC_FLAG(native_framework) = saved_; }
  bool saved_;
};

// Cases the plain C++ handed to the original (none expected), and those
// expected (InsertN's copy_backward path).
int g_fallbacks = 0;
int g_fallbacks_expected = 0;

// Where a case keeps things in the scratch area.
struct Places {
  uint32_t map, iterator, result, value, vector, elements;
};
Places PlacesOf(const Context& h) {
  return {h.data, h.data + 16, h.data + 32, h.data + 48, h.data + 64, h.data + 128};
}

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }

// A map built by the game's own functions with the keys and delegates
// given; duplicates aren't added.
void BuildMapOf(Context& h, uint32_t map,
                const std::vector<std::pair<uint32_t, uint32_t>>& entries) {
  FlagOff off;
  const Places p = PlacesOf(h);
  PPCContext t = CallContext(h);
  t.r3.u64 = map;
  __imp__sub_82099FB0(t, h.base);
  for (const auto& [key, delegate] : entries) {
    guest::StoreU32(h.base, p.value, key);
    guest::StoreU32(h.base, p.value + 4, delegate);
    t = CallContext(h);
    t.r3.u64 = p.result;
    t.r4.u64 = map;
    t.r5.u64 = p.value;
    __imp__sub_82093740(t, h.base);
  }
  Fill(h.base, p.result, 16);
  Fill(h.base, p.value, 16);
}

// A map with the keys given, each with a delegate made from it.
void BuildMap(Context& h, uint32_t map, const std::vector<uint32_t>& keys) {
  std::vector<std::pair<uint32_t, uint32_t>> entries;
  for (uint32_t key : keys) {
    entries.emplace_back(key, key ^ 0xDE1E0000u);
  }
  BuildMapOf(h, map, entries);
}

// The nodes reachable from a map's head, numbered in the order met.
struct Walk {
  std::unordered_map<uint32_t, uint32_t> number;
  std::vector<uint32_t> nodes;
  uint32_t Of(uint32_t address) const {
    const auto it = number.find(address);
    return it == number.end() ? address : 0xB10C0000u + it->second;
  }
};

Walk WalkFrom(uint8_t* base, uint32_t head, const std::vector<uint32_t>& extra = {}) {
  Walk w;
  std::deque<uint32_t> queue;
  auto meet = [&](uint32_t node) {
    if (node == 0 || w.number.count(node) != 0 || !memory::IsHeapBlock(node)) {
      return;
    }
    w.number[node] = uint32_t(w.nodes.size());
    w.nodes.push_back(node);
    queue.push_back(node);
  };
  meet(head);
  for (uint32_t node : extra) {
    meet(node);
  }
  while (!queue.empty()) {
    const uint32_t node = queue.front();
    queue.pop_front();
    for (uint32_t at : {rl::kNodeLeft, rl::kNodeParent, rl::kNodeRight}) {
      meet(Load(base, node + at));
    }
  }
  return w;
}

// The walk's nodes into `out`, then freed.
void NodesOut(uint8_t* base, Outputs& out, const Walk& w) {
  out.Word(uint32_t(w.nodes.size()));
  for (uint32_t node : w.nodes) {
    out.Word(memory::Size(node));
    for (uint32_t at : {rl::kNodeLeft, rl::kNodeParent, rl::kNodeRight}) {
      out.Word(w.Of(Load(base, node + at)));
    }
    // A head's key and delegate are never set (heap leftovers).
    if (guest::LoadU8(base, node + rl::kNodeIsNil) == 0) {
      out.Word(Load(base, node + rl::kNodeKey));
      out.Word(Load(base, node + rl::kNodeDelegate));
    }
    out.Bytes(Read(base, node + rl::kNodeColor, 2));  // +22, +23 are never set
  }
  for (uint32_t node : w.nodes) {
    memory::crt::OperatorDelete(node);
  }
}

// The map (its head by number and size), the iterator, the result's 12
// bytes and r3, the scratch area's words by number, then the nodes.
void MapOut(Context& h, Outputs& out, const PPCContext& t, const std::vector<uint32_t>& extra = {}) {
  const Places p = PlacesOf(h);
  const Walk w = WalkFrom(h.base, Load(h.base, p.map + rl::kMapHead), extra);
  for (uint32_t at = 0; at < 512; at += 4) {
    out.Word(w.Of(Load(h.base, h.data + at)));
  }
  out.Word(w.Of(t.r3.u32));
  NodesOut(h.base, out, w);
}

// The map's nodes in key order (by walking left, parent and right).
std::vector<uint32_t> InOrder(uint8_t* base, uint32_t map) {
  std::vector<uint32_t> nodes;
  const uint32_t head = Load(base, map + rl::kMapHead);
  auto nil = [&](uint32_t node) { return guest::LoadU8(base, node + rl::kNodeIsNil) != 0; };
  std::vector<uint32_t> stack;
  uint32_t node = Load(base, head + rl::kNodeParent);
  while (!nil(node) || !stack.empty()) {
    while (!nil(node)) {
      stack.push_back(node);
      node = Load(base, node + rl::kNodeLeft);
    }
    node = stack.back();
    stack.pop_back();
    nodes.push_back(node);
    node = Load(base, node + rl::kNodeRight);
  }
  return nodes;
}

std::vector<uint32_t> RandomKeys(Random& r) {
  std::vector<uint32_t> keys(r.Below(14));
  const uint32_t range = r.Below(2) ? 32 : 0xFFFFFFFFu;
  for (auto& key : keys) {
    key = range == 0xFFFFFFFFu ? uint32_t(r.Next()) : r.Below(range);
  }
  return keys;
}

// One case through both versions: `setup()` builds the inputs (the same
// both times: it's handed a copy of the generator's state), `args(t)` sets
// the registers, the original (flag off) or the native step runs, then
// `collect(out, t)` keeps what's compared. The caller's words at r1 + 32 to
// + 48 are compared as they are unless `raw_stack` is false (the case then
// compares them itself, as they may hold heap addresses).
template <typename Setup, typename Args, typename Collect>
void MapCase(Context& h, Results& results, Setup setup, Args args, GuestFunction recompiled,
             GuestFunction native, Collect collect, const std::string& what,
             bool raw_stack = true) {
  Outputs out[2];
  const Random start = h.random;
  Random after;
  for (int v = 0; v < 2; v++) {
    h.random = start;
    Fill(h.base, h.data, kScratchBytes);
    PPCContext t = CallContext(h);
    Fill(h.base, t.r1.u32 + 32, 16);
    setup();
    args(t);
    {
      std::optional<FlagOff> off;
      if (v == 0) {
        off.emplace();
      }
      (v == 0 ? recompiled : native)(t, h.base);
    }
    if (raw_stack) {
      out[v].Bytes(Read(h.base, t.r1.u32 + 32, 16));
    }
    collect(out[v], t);
    after = h.random;
  }
  h.random = after;
  results.Check(out[0], out[1], what);
}

// The register steps.
void NativeIncrement(PPCContext& ctx, uint8_t* base) {
  rl::Iterator it{Load(base, ctx.r3.u32), Load(base, ctx.r3.u32 + 4)};
  if (!rl::MapIncrement(base, it)) {
    g_fallbacks++;
    __imp__sub_82099B18(ctx, base);
    return;
  }
  guest::StoreU32(base, ctx.r3.u32 + 4, it.node);
}
void NativeDecrement(PPCContext& ctx, uint8_t* base) {
  rl::Iterator it{Load(base, ctx.r3.u32), Load(base, ctx.r3.u32 + 4)};
  if (!rl::MapDecrement(base, it)) {
    g_fallbacks++;
    __imp__sub_82091AE8(ctx, base);
    return;
  }
  guest::StoreU32(base, ctx.r3.u32 + 4, it.node);
}
void NativeRotateLeft(PPCContext& ctx, uint8_t* base) {
  rl::MapRotateLeft(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeRotateRight(PPCContext& ctx, uint8_t* base) {
  rl::MapRotateRight(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeAllocateNodes(PPCContext& ctx, uint8_t* base) {
  if (const auto block = rl::MapAllocateNodes(base, ctx.r3.u32)) {
    ctx.r3.u64 = *block;
    return;
  }
  g_fallbacks++;
  __imp__sub_82099C80(ctx, base);
}
void NativeNewHead(PPCContext& ctx, uint8_t* base) { ctx.r3.u64 = rl::MapNewHead(base); }
void NativeConstruct(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rl::MapConstruct(base, ctx.r3.u32);
}
void NativeNewNode(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rl::MapNewNode(base, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32, ctx.r8.u8);
}
void NativeInsertNode(PPCContext& ctx, uint8_t* base) {
  const auto it = rl::MapInsert(base, ctx.r4.u32, ctx.r5.u8 != 0, ctx.r6.u32, ctx.r7.u32);
  if (!it) {
    g_fallbacks++;
    __imp__sub_82093400(ctx, base);
    return;
  }
  guest::StoreU32(base, ctx.r3.u32 + 4, it->node);
  guest::StoreU32(base, ctx.r3.u32 + 0, it->container);
}
void NativeInsert(PPCContext& ctx, uint8_t* base) {
  const auto result = rl::MapInsertUnique(base, ctx.r4.u32, ctx.r5.u32);
  if (!result) {
    g_fallbacks++;
    __imp__sub_82093740(ctx, base);
    return;
  }
  guest::StoreU32(base, ctx.r3.u32 + 0, result->where.container);
  guest::StoreU32(base, ctx.r3.u32 + 4, result->where.node);
  guest::StoreU8(base, ctx.r3.u32 + 8, result->inserted ? 1 : 0);
}
rl::Iterator IteratorOf(uint64_t reg) { return {uint32_t(reg >> 32), uint32_t(reg)}; }
void NativeErase(PPCContext& ctx, uint8_t* base) {
  if (!rl::RequestsErase(base, ctx.r4.u32, IteratorOf(ctx.r5.u64), IteratorOf(ctx.r6.u64))) {
    g_fallbacks++;
    __imp__sub_82092EF8(ctx, base);
    return;
  }
  REX_STORE_U64(ctx.r1.u32 + 32, ctx.r5.u64);
  REX_STORE_U64(ctx.r1.u32 + 40, ctx.r6.u64);
  REX_STORE_U64(ctx.r3.u32, ctx.r5.u64);
}
void NativeClear(PPCContext& ctx, uint8_t* base) {
  if (!rl::RequestsClear(base, ctx.r3.u32)) {
    g_fallbacks++;
    __imp__sub_8209A080(ctx, base);
    return;
  }
  ctx.r3.u64 = ctx.r1.u32 - 16;
}

// A node of the map picked at random (or its head when `head` allows it).
uint32_t PickNode(Context& h, uint32_t map, bool head) {
  const std::vector<uint32_t> nodes = InOrder(h.base, map);
  if (nodes.empty() || (head && h.random.Below(6) == 0)) {
    return Load(h.base, map + rl::kMapHead);
  }
  return nodes[h.random.Below(uint32_t(nodes.size()))];
}

void Iterators(Context& h) {
  const Places p = PlacesOf(h);
  for (int which = 0; which < 2; which++) {
    Results results(which == 0 ? "HandlerMap::Iterator::operator++ (sub_82099B18)"
                               : "HandlerMap::Iterator::operator-- (sub_82091AE8)");
    for (int c = 0; c < 300; c++) {
      MapCase(
          h, results,
          [&] {
            std::vector<uint32_t> keys = RandomKeys(h.random);
            if (keys.empty()) {
              keys.push_back(h.random.Below(32));  // an empty map's head traps both ways
            }
            BuildMap(h, p.map, keys);
            const uint32_t node = PickNode(h, p.map, which == 1);
            guest::StoreU32(h.base, p.iterator, p.map);
            guest::StoreU32(h.base, p.iterator + 4, node);
          },
          [&](PPCContext& t) { t.r3.u64 = p.iterator; },
          which == 0 ? __imp__sub_82099B18 : __imp__sub_82091AE8,
          which == 0 ? NativeIncrement : NativeDecrement,
          [&](Outputs& out, const PPCContext& t) { MapOut(h, out, t); }, fmt::format("case {}", c));
    }
    results.Report();
  }
}

void Rotations(Context& h) {
  const Places p = PlacesOf(h);
  for (int which = 0; which < 2; which++) {
    Results results(which == 0 ? "HandlerMap::RotateLeft (sub_82092B10)"
                               : "HandlerMap::RotateRight (sub_82092A00)");
    const uint32_t child = which == 0 ? rl::kNodeRight : rl::kNodeLeft;
    for (int c = 0; c < 300; c++) {
      MapCase(
          h, results,
          [&] {
            std::vector<uint32_t> keys = RandomKeys(h.random);
            keys.push_back(h.random.Below(32));
            keys.push_back(h.random.Below(32) + 32);
            BuildMap(h, p.map, keys);
            // A node with a child on that side (the root, else).
            std::vector<uint32_t> nodes;
            for (uint32_t node : InOrder(h.base, p.map)) {
              if (guest::LoadU8(h.base, Load(h.base, node + child) + rl::kNodeIsNil) == 0) {
                nodes.push_back(node);
              }
            }
            const uint32_t head = Load(h.base, p.map + rl::kMapHead);
            const uint32_t node = nodes.empty() ? Load(h.base, head + rl::kNodeParent)
                                                : nodes[h.random.Below(uint32_t(nodes.size()))];
            guest::StoreU32(h.base, p.iterator, node);
          },
          [&](PPCContext& t) {
            t.r3.u64 = p.map;
            t.r4.u64 = Load(h.base, p.iterator);
          },
          which == 0 ? __imp__sub_82092B10 : __imp__sub_82092A00,
          which == 0 ? NativeRotateLeft : NativeRotateRight,
          [&](Outputs& out, const PPCContext& t) { MapOut(h, out, t); }, fmt::format("case {}", c));
    }
    results.Report();
  }
}

// A block the function made: its size and first `words` words (by number),
// then freed.
void BlockOut(Context& h, Outputs& out, uint32_t block, uint32_t words) {
  const bool heap = block != 0 && memory::IsHeapBlock(block);
  out.Word(heap);
  out.Word(block == 0);
  if (!heap) {
    return;
  }
  out.Word(memory::Size(block));
  Walk w;
  w.number[block] = 0;
  for (uint32_t i = 0; i < words; i++) {
    out.Word(w.Of(Load(h.base, block + 4 * i)));
  }
  memory::crt::OperatorDelete(block);
}

void Allocations(Context& h) {
  const Places p = PlacesOf(h);
  {
    Results results("HandlerMap::AllocateNodes (sub_82099C80)");
    for (int c = 0; c < 64; c++) {
      const uint32_t count = c < 16 ? 1 : c < 20 ? 0 : h.random.Below(200);
      MapCase(
          h, results, [] {}, [&](PPCContext& t) { t.r3.u64 = count; }, __imp__sub_82099C80,
          NativeAllocateNodes,
          [&](Outputs& out, const PPCContext& t) { BlockOut(h, out, t.r3.u32, 0); },
          fmt::format("count {}", count));
    }
    results.Report();
  }
  {
    Results results("HandlerMap::NewHead (sub_82099E88)");
    for (int c = 0; c < 20; c++) {
      MapCase(
          h, results, [] {}, [](PPCContext&) {}, __imp__sub_82099E88, NativeNewHead,
          [&](Outputs& out, const PPCContext& t) { BlockOut(h, out, t.r3.u32, 6); },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("HandlerMap::HandlerMap (sub_82099FB0)");
    for (int c = 0; c < 20; c++) {
      MapCase(
          h, results, [] {}, [&](PPCContext& t) { t.r3.u64 = p.map; }, __imp__sub_82099FB0,
          NativeConstruct, [&](Outputs& out, const PPCContext& t) { MapOut(h, out, t); },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("HandlerMap::NewNode (sub_82099E20)");
    for (int c = 0; c < 100; c++) {
      MapCase(
          h, results,
          [&] {
            guest::StoreU32(h.base, p.value, uint32_t(h.random.Next()));
            guest::StoreU32(h.base, p.value + 4, uint32_t(h.random.Next()));
          },
          [&](PPCContext& t) {
            t.r3.u64 = p.map;
            t.r4.u64 = uint32_t(h.random.Next());
            t.r5.u64 = uint32_t(h.random.Next());
            t.r6.u64 = uint32_t(h.random.Next());
            t.r7.u64 = p.value;
            t.r8.u64 = h.random.Next();  // only its low byte is the color
          },
          __imp__sub_82099E20, NativeNewNode,
          [&](Outputs& out, const PPCContext& t) { BlockOut(h, out, t.r3.u32, 6); },
          fmt::format("case {}", c));
    }
    results.Report();
  }
}

void Inserts(Context& h) {
  const Places p = PlacesOf(h);
  {
    Results results("HandlerMap::InsertNode (sub_82093400)");
    for (int c = 0; c < 500; c++) {
      MapCase(
          h, results,
          [&] {
            BuildMap(h, p.map, RandomKeys(h.random));
            // A key not in the map, and where std::map::insert would put it.
            const std::vector<uint32_t> nodes = InOrder(h.base, p.map);
            uint32_t key = 0;
            for (;;) {
              key = h.random.Below(2) ? h.random.Below(40) : uint32_t(h.random.Next());
              if (std::none_of(nodes.begin(), nodes.end(), [&](uint32_t node) {
                    return Load(h.base, node + rl::kNodeKey) == key;
                  })) {
                break;
              }
            }
            const uint32_t head = Load(h.base, p.map + rl::kMapHead);
            uint32_t where = head;
            bool left = true;
            for (uint32_t x = Load(h.base, head + rl::kNodeParent);
                 guest::LoadU8(h.base, x + rl::kNodeIsNil) == 0;) {
              where = x;
              left = key < Load(h.base, x + rl::kNodeKey);
              x = Load(h.base, x + (left ? rl::kNodeLeft : rl::kNodeRight));
            }
            guest::StoreU32(h.base, p.value, key);
            guest::StoreU32(h.base, p.value + 4, uint32_t(h.random.Next()));
            guest::StoreU32(h.base, p.iterator, where);
            guest::StoreU32(h.base, p.iterator + 4, left ? 0x01FFFF01u : 0xFF00FF00u);
          },
          [&](PPCContext& t) {
            t.r3.u64 = p.result;
            t.r4.u64 = p.map;
            t.r5.u64 = Load(h.base, p.iterator + 4);  // add on the left: its low byte
            t.r6.u64 = Load(h.base, p.iterator);
            t.r7.u64 = p.value;
          },
          __imp__sub_82093400, NativeInsertNode,
          [&](Outputs& out, const PPCContext& t) { MapOut(h, out, t); }, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("HandlerMap::Insert (sub_82093740)");
    for (int c = 0; c < 1000; c++) {
      MapCase(
          h, results,
          [&] {
            const std::vector<uint32_t> keys = RandomKeys(h.random);
            BuildMap(h, p.map, keys);
            uint32_t key = h.random.Below(2) ? h.random.Below(40) : uint32_t(h.random.Next());
            if (!keys.empty() && h.random.Below(3) == 0) {
              key = keys[h.random.Below(uint32_t(keys.size()))];  // one already there
            }
            guest::StoreU32(h.base, p.value, key);
            guest::StoreU32(h.base, p.value + 4, uint32_t(h.random.Next()));
          },
          [&](PPCContext& t) {
            t.r3.u64 = p.result;
            t.r4.u64 = p.map;
            t.r5.u64 = p.value;
          },
          __imp__sub_82093740, NativeInsert,
          [&](Outputs& out, const PPCContext& t) { MapOut(h, out, t); }, fmt::format("case {}", c));
    }
    results.Report();
  }
}

void Requests(Context& h) {
  const Places p = PlacesOf(h);
  {
    Results results("RequestVector::Erase (sub_82092EF8)");
    for (int c = 0; c < 300; c++) {
      MapCase(
          h, results,
          [&] {
            const uint32_t count = h.random.Below(20);
            for (uint32_t i = 0; i < 3 * count; i++) {
              guest::StoreU32(h.base, p.elements + 4 * i, uint32_t(h.random.Next()));
            }
            guest::StoreU32(h.base, p.vector + 4, p.elements);
            guest::StoreU32(h.base, p.vector + 8, p.elements + 12 * count);
            guest::StoreU32(h.base, p.vector + 12, p.elements + 12 * (count + h.random.Below(4)));
            uint32_t a = h.random.Below(count + 1), b = h.random.Below(count + 1);
            if (a > b) {
              std::swap(a, b);
            }
            guest::StoreU32(h.base, p.iterator, p.elements + 12 * a);
            guest::StoreU32(h.base, p.iterator + 4, p.elements + 12 * b);
          },
          [&](PPCContext& t) {
            t.r3.u64 = p.result;
            t.r4.u64 = p.vector;
            t.r5.u64 = (uint64_t(p.vector) << 32) | Load(h.base, p.iterator);
            t.r6.u64 = (uint64_t(p.vector) << 32) | Load(h.base, p.iterator + 4);
          },
          __imp__sub_82092EF8, NativeErase,
          [&](Outputs& out, const PPCContext& t) {
            out.Bytes(Read(h.base, h.data, 512));
            out.Word(t.r3.u32);
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("RequestVector::Clear (sub_8209A080)");
    for (int c = 0; c < 100; c++) {
      MapCase(
          h, results,
          [&] {
            const uint32_t count = c < 10 ? 0 : h.random.Below(20);
            for (uint32_t i = 0; i < 3 * count; i++) {
              guest::StoreU32(h.base, p.elements + 4 * i, uint32_t(h.random.Next()));
            }
            guest::StoreU32(h.base, p.vector + 4, p.elements);
            guest::StoreU32(h.base, p.vector + 8, p.elements + 12 * count);
            guest::StoreU32(h.base, p.vector + 12, p.elements + 12 * count);
          },
          [&](PPCContext& t) { t.r3.u64 = p.vector; }, __imp__sub_8209A080, NativeClear,
          [&](Outputs& out, const PPCContext& t) {
            out.Bytes(Read(h.base, h.data, 512));
            out.Word(t.r3.u32);
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
}


// --- The request vector's growth and the run loop ---

void NativeInsertN(PPCContext& ctx, uint8_t* base) {
  if (!rl::RequestsInsertN(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32)) {
    g_fallbacks_expected++;
    __imp__sub_8209A0E8(ctx, base);
    return;
  }
  REX_STORE_U64(ctx.r1.u32 + 24, ctx.r4.u64);
}
// Whether Insert of one at `where` takes InsertN's copy_backward path (a
// fallback expected).
bool CopyBackward(uint8_t* base, uint32_t vector, uint32_t where) {
  const uint32_t first = Load(base, vector + rl::kVectorFirst);
  const uint32_t last = Load(base, vector + rl::kVectorLast);
  const uint32_t end = Load(base, vector + rl::kVectorEnd);
  return first != 0 && last != end && where != last;
}
void NativeRequestInsert(PPCContext& ctx, uint8_t* base) {
  const bool expected = CopyBackward(base, ctx.r4.u32, ctx.r5.u32);
  const auto it =
      rl::RequestsInsert(base, ctx.r4.u32, {uint32_t(ctx.r5.u64 >> 32), ctx.r5.u32}, ctx.r6.u32);
  if (!it) {
    (expected ? g_fallbacks_expected : g_fallbacks)++;
    __imp__sub_8209A600(ctx, base);
    return;
  }
  REX_STORE_U64(ctx.r1.u32 + 32, ctx.r5.u64);
  guest::StoreU32(base, ctx.r3.u32 + 0, it->container);
  guest::StoreU32(base, ctx.r3.u32 + 4, it->node);
}
void NativePushBack(PPCContext& ctx, uint8_t* base) {
  const auto inserted = rl::RequestsPushBack(base, ctx.r3.u32, ctx.r4.u32);
  if (!inserted) {
    g_fallbacks++;
    __imp__sub_8209A930(ctx, base);
    return;
  }
  if (*inserted) {
    ctx.r3.u64 = ctx.r1.u32 - 16;
  }
}
void NativeRunLoop(PPCContext& ctx, uint8_t* base) { rl::RunLoopConstruct(base, ctx.r3.u32); }
void NativeApplyRequest(PPCContext& ctx, uint8_t* base) {
  const auto inserted = rl::RunLoopApplyRequest(base, ctx.r3.u32, uint32_t(ctx.r4.u64 >> 32),
                                                ctx.r4.u32, uint32_t(ctx.r5.u64 >> 32));
  if (!inserted) {
    g_fallbacks++;
    __imp__sub_8209A458(ctx, base);
    return;
  }
  REX_STORE_U64(ctx.r1.u32 + 32, ctx.r4.u64);
  REX_STORE_U64(ctx.r1.u32 + 40, ctx.r5.u64);
  if (*inserted) {
    ctx.r3.u64 = ctx.r1.u32 - 32;
  }
}

// A request vector at p.vector with `size` random requests in a heap block
// of `capacity` (none when 0); returns the block.
uint32_t BuildVector(Context& h, uint32_t size, uint32_t capacity) {
  const Places p = PlacesOf(h);
  uint32_t block = 0;
  if (capacity != 0) {
    block = *memory::crt::OperatorNew(h.base, capacity * rl::kRequestBytes);
    for (uint32_t i = 0; i < 3 * capacity; i++) {
      guest::StoreU32(h.base, block + 4 * i, i < 3 * size ? uint32_t(h.random.Next()) : 0xCDCDCDCDu);
    }
  }
  guest::StoreU32(h.base, p.vector + rl::kVectorFirst, block);
  guest::StoreU32(h.base, p.vector + rl::kVectorLast, block + size * rl::kRequestBytes);
  guest::StoreU32(h.base, p.vector + rl::kVectorEnd, block + capacity * rl::kRequestBytes);
  return block;
}

// The vector after a case: its words with the blocks by number (the one it
// started with 0, a new one 1), its requests, whether the first block is
// still a heap block; the scratch area by number. Its block(s) freed.
void VectorOut(Context& h, Outputs& out, uint32_t old_block, const std::vector<uint32_t>& words) {
  const Places p = PlacesOf(h);
  const uint32_t first = Load(h.base, p.vector + rl::kVectorFirst);
  const uint32_t last = Load(h.base, p.vector + rl::kVectorLast);
  auto number = [&](uint32_t word) {
    if (old_block != 0 && word - old_block < 0x10000) {
      return 0xB10C0000u + (word - old_block);
    }
    if (first != old_block && first != 0 && word - first < 0x10000) {
      return 0xB1100000u + (word - first);
    }
    return word;
  };
  for (uint32_t at = 0; at < 512; at += 4) {
    out.Word(number(Load(h.base, h.data + at)));
  }
  for (uint32_t word : words) {
    out.Word(number(word));
  }
  out.Word(first != 0 && memory::IsHeapBlock(first) ? memory::Size(first) : 0);
  if (first != 0 && last >= first && last - first < 0x10000) {
    out.Bytes(Read(h.base, first, last - first));
  }
  const bool old_alive = old_block != 0 && memory::IsHeapBlock(old_block);
  out.Word(old_alive);
  if (first != 0 && memory::IsHeapBlock(first)) {
    memory::crt::OperatorDelete(first);
  }
  if (old_alive && old_block != first) {
    memory::crt::OperatorDelete(old_block);
  }
}

void VectorGrowth(Context& h) {
  const Places p = PlacesOf(h);
  uint32_t block = 0;
  // A vector of 0 to 6 requests, its capacity 0 to 4 more (none at all now
  // and then: an empty vector with no block), and the request inserted from
  // the scratch area or (now and then) one of the vector's own.
  auto setup = [&](uint32_t& size, uint32_t& where) {
    const bool none = h.random.Below(8) == 0;
    size = none ? 0 : h.random.Below(7);
    const uint32_t capacity = none ? 0 : size + h.random.Below(5);
    block = BuildVector(h, size, capacity);
    where = block + h.random.Below(size + 1) * rl::kRequestBytes;
    for (uint32_t i = 0; i < 3; i++) {
      guest::StoreU32(h.base, p.value + 4 * i, uint32_t(h.random.Next()));
    }
    guest::StoreU32(h.base, p.result + 12, size != 0 && h.random.Below(4) == 0
                                               ? block + h.random.Below(size) * rl::kRequestBytes
                                               : p.value);
  };
  {
    Results results("RequestVector::InsertN (sub_8209A0E8)");
    for (int c = 0; c < 500; c++) {
      uint32_t count = 0;
      MapCase(
          h, results,
          [&] {
            uint32_t size = 0, where = 0;
            setup(size, where);
            count = h.random.Below(5);
            guest::StoreU32(h.base, p.iterator, where);
          },
          [&](PPCContext& t) {
            t.r3.u64 = p.vector;
            t.r4.u64 = (uint64_t(p.vector) << 32) | Load(h.base, p.iterator);
            t.r5.u64 = count;
            t.r6.u64 = Load(h.base, p.result + 12);
            Fill(h.base, t.r1.u32 + 24, 8);
          },
          __imp__sub_8209A0E8, NativeInsertN,
          [&](Outputs& out, const PPCContext& t) {
            // r3 isn't compared (see the adapter).
            VectorOut(h, out, block, {Load(h.base, t.r1.u32 + 24), Load(h.base, t.r1.u32 + 28)});
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("RequestVector::Insert (sub_8209A600)");
    for (int c = 0; c < 500; c++) {
      MapCase(
          h, results,
          [&] {
            uint32_t size = 0, where = 0;
            setup(size, where);
            guest::StoreU32(h.base, p.iterator, where);
          },
          [&](PPCContext& t) {
            t.r3.u64 = p.result;
            t.r4.u64 = p.vector;
            t.r5.u64 = (uint64_t(p.vector) << 32) | Load(h.base, p.iterator);
            t.r6.u64 = Load(h.base, p.result + 12);
          },
          __imp__sub_8209A600, NativeRequestInsert,
          [&](Outputs& out, const PPCContext& t) { VectorOut(h, out, block, {t.r3.u32}); },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("RequestVector::PushBack (sub_8209A930)");
    for (int c = 0; c < 500; c++) {
      MapCase(
          h, results,
          [&] {
            uint32_t size = 0, where = 0;
            setup(size, where);
          },
          [&](PPCContext& t) {
            t.r3.u64 = p.vector;
            t.r4.u64 = Load(h.base, p.result + 12);
          },
          __imp__sub_8209A930, NativePushBack,
          [&](Outputs& out, const PPCContext& t) { VectorOut(h, out, block, {t.r3.u32}); },
          fmt::format("case {}", c));
    }
    results.Report();
  }
}

// A run loop at p.map's place (56 bytes: through p.value's).
void RunLoops(Context& h) {
  const uint32_t loop = h.data + 64;
  {
    Results results("RunLoop::RunLoop (sub_8209A9F0)");
    for (int c = 0; c < 20; c++) {
      MapCase(
          h, results, [] {}, [&](PPCContext& t) { t.r3.u64 = loop; }, __imp__sub_8209A9F0,
          NativeRunLoop,
          [&](Outputs& out, const PPCContext& t) {
            const uint32_t listeners = Load(h.base, loop + rl::kListeners + rl::kMapHead);
            const Walk w =
                WalkFrom(h.base, Load(h.base, loop + rl::kTickHandlers + rl::kMapHead), {listeners});
            for (uint32_t at = 0; at < 512; at += 4) {
              out.Word(w.Of(Load(h.base, h.data + at)));
            }
            out.Word(w.Of(t.r3.u32));
            NodesOut(h.base, out, w);
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("RunLoop::ApplyRequest (sub_8209A458)");
    for (int c = 0; c < 600; c++) {
      uint32_t type = 0, key = 0, delegate = 0;
      MapCase(
          h, results,
          [&] {
            {
              FlagOff off;
              PPCContext t = CallContext(h);
              t.r3.u64 = loop;
              __imp__sub_8209A9F0(t, h.base);
            }
            BuildMap(h, loop + rl::kTickHandlers, RandomKeys(h.random));
            BuildMap(h, loop + rl::kListeners, RandomKeys(h.random));
            const uint32_t kinds[] = {0, 2, 0, 2, 4, 7, 0xFFFFFFFF};
            type = kinds[h.random.Below(7)];
            key = h.random.Below(2) ? h.random.Below(32) : uint32_t(h.random.Next());
            delegate = uint32_t(h.random.Next());
          },
          [&](PPCContext& t) {
            t.r3.u64 = loop;
            t.r4.u64 = (uint64_t(type) << 32) | key;
            t.r5.u64 = (uint64_t(delegate) << 32) | uint32_t(h.random.Next());
          },
          __imp__sub_8209A458, NativeApplyRequest,
          [&](Outputs& out, const PPCContext& t) {
            const uint32_t listeners = Load(h.base, loop + rl::kListeners + rl::kMapHead);
            const Walk w =
                WalkFrom(h.base, Load(h.base, loop + rl::kTickHandlers + rl::kMapHead), {listeners});
            for (uint32_t at = 0; at < 512; at += 4) {
              out.Word(w.Of(Load(h.base, h.data + at)));
            }
            out.Word(w.Of(t.r3.u32));
            NodesOut(h.base, out, w);
          },
          fmt::format("type {} case {}", type, c));
    }
    results.Report();
  }
}


// --- The thunks and dispatchers ---
//
// Their targets are guest calls, so the cases run the adapters themselves
// (with the flag on; the originals with it off) on objects in the scratch
// area whose vtables point at small guest functions that leave a trace:
// sub_820A70A8 stores r4 at r3 + 4, the getters sub_82090760 (r3's byte
// +133) and sub_82090770 (r3's word +60) answer from the object.

constexpr uint32_t kStoreR4At4 = 0x820A70A8;
constexpr uint32_t kByte133 = 0x82090760;
constexpr uint32_t kWord60 = 0x82090770;


// Random words in the scratch area's first 512 bytes, and `objects` objects
// (at data + 512 + 160 * i) whose vtables (at their + 0x80) hold `targets`.
void DispatchObjects(Context& h, uint32_t objects, const std::vector<uint32_t>& targets) {
  for (uint32_t at = 0; at < 1024; at += 4) {
    guest::StoreU32(h.base, h.data + at, uint32_t(h.random.Next()) & 0x7FFFFFFF);
  }
  for (uint32_t i = 0; i < objects; i++) {
    const uint32_t object = h.data + 512 + 160 * i;
    guest::StoreU32(h.base, object, object + 0x80);
    for (uint32_t slot = 0; slot < targets.size(); slot++) {
      guest::StoreU32(h.base, object + 0x80 + 4 * slot, targets[slot]);
    }
  }
}
uint32_t ObjectAt(const Context& h, uint32_t i) { return h.data + 512 + 160 * i; }

void DispatchOut(Context& h, Outputs& out, const PPCContext& t, bool r4 = true) {
  out.Bytes(Read(h.base, h.data, 2048));
  out.Word(t.r3.u32);
  if (r4) {
    out.Word(t.r4.u32);
  }
}

void Dispatchers(Context& h) {
  // Each slot's target picked from the three.
  auto targets = [&] {
    const uint32_t pick[] = {kStoreR4At4, kByte133, kWord60};
    std::vector<uint32_t> list(16);
    for (auto& target : list) {
      target = pick[h.random.Below(3)];
    }
    return list;
  };
  struct Thunk {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
  };
  const Thunk thunks[] = {
      {"Delegate slot 0 thunk (sub_82096330)", __imp__sub_82096330, sub_82096330},
      {"Delegate slot 1 thunk (sub_82096340)", __imp__sub_82096340, hooks::DelegateSlot1Thunk},
      {"Framework::Start (sub_820902B0)", __imp__sub_820902B0, sub_820902B0},
      {"App input source (sub_82094350)", __imp__sub_82094350, sub_82094350},
      {"App +16 slot +36 (sub_820944C8)", __imp__sub_820944C8, sub_820944C8},
  };
  for (const Thunk& thunk : thunks) {
    Results results(thunk.name);
    for (int c = 0; c < 100; c++) {
      MapCase(
          h, results,
          [&] {
            std::vector<uint32_t> list = targets();
            if (c % 3 == 0) {
              list[1] = 0x82090830;  // the input source in play (InputManagerFrom36)
            }
            DispatchObjects(h, 4, list);
            // The framework's app (+120) and the app's input source (+20 of
            // app + 4) are objects.
            guest::StoreU32(h.base, h.data + 120, ObjectAt(h, 1));
            guest::StoreU32(h.base, h.data + 4 + 20, ObjectAt(h, 2));
            guest::StoreU32(h.base, h.data + 12 + 4 + 20, ObjectAt(h, 3));
          },
          [&](PPCContext& t) {
            const bool framework = thunk.recompiled == __imp__sub_820902B0;
            const bool app = thunk.recompiled == __imp__sub_82094350;
            const bool app16 = thunk.recompiled == __imp__sub_820944C8;
            t.r3.u64 = framework ? h.data : app ? h.data + 4 : app16 ? h.data + 28 : ObjectAt(h, 0);
            t.r4.u64 = uint32_t(h.random.Next());
          },
          thunk.recompiled, thunk.native,
          [&](Outputs& out, const PPCContext& t) { DispatchOut(h, out, t); },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("TickHandler::Invoke (sub_82099A98)");
    for (int c = 0; c < 100; c++) {
      MapCase(
          h, results, [&] { DispatchObjects(h, 1, targets()); },
          [&](PPCContext& t) {
            t.r3.u64 = h.data + 64;
            t.r4.u64 = (h.random.Next() << 32) | ObjectAt(h, 0);
            Fill(h.base, t.r1.u32 + 24, 8);
          },
          __imp__sub_82099A98, hooks::TickHandlerInvoke,
          [&](Outputs& out, const PPCContext& t) {
            DispatchOut(h, out, t);
            out.Bytes(Read(h.base, t.r1.u32 + 24, 8));
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // The framework at data: the input manager (+88), the app (+120) and
    // the run loop objects; the app's answer its byte +133.
    Results results("Framework::OnTick (sub_820902C8)");
    for (int c = 0; c < 200; c++) {
      MapCase(
          h, results,
          [&] {
            std::vector<uint32_t> list = targets();
            list[1] = kStoreR4At4;   // the manager's slot 1
            list[11] = kByte133;     // the app's slot +44
            list[4] = kStoreR4At4;   // the run loop's slot 4
            DispatchObjects(h, 3, list);
            guest::StoreU32(h.base, h.data + 88, ObjectAt(h, 0));
            guest::StoreU32(h.base, h.data + 120, ObjectAt(h, 1));
            guest::StoreU8(h.base, ObjectAt(h, 1) + 133, h.random.Below(2) ? 0 : uint8_t(h.random.Next()));
          },
          [&](PPCContext& t) {
            t.r3.u64 = h.data;
            t.r4.u64 = ObjectAt(h, 2);
            Fill(h.base, t.r1.u32 - 128 + 80, 4);
          },
          __imp__sub_820902C8, sub_820902C8,
          [&](Outputs& out, const PPCContext& t) {
            DispatchOut(h, out, t);
            out.Bytes(Read(h.base, t.r1.u32 - 128 + 80, 4));
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  if (!hooks::NativeHeapActive()) {
    return;
  }
  for (int which = 0; which < 2; which++) {
    Results results(which == 0 ? "RunLoop::AddTickHandler (sub_8209AAA8)"
                               : "RunLoop::AddListener (sub_8209AB48)");
    // The run loop at data + 2048 - 64 (its vector at + 28, a block for
    // 0 to 6 requests with room for up to 4 more, or none); the delegate an
    // object whose slot 2 answers its word +60 (its copy).
    const uint32_t loop = h.data + 2048 - 64;
    uint32_t block = 0;
    for (int c = 0; c < 200; c++) {
      MapCase(
          h, results,
          [&] {
            std::vector<uint32_t> list = targets();
            list[2] = kWord60;
            DispatchObjects(h, 1, list);
            const bool none = h.random.Below(6) == 0;
            const uint32_t size = none ? 0 : h.random.Below(7);
            const uint32_t capacity = none ? 0 : size + h.random.Below(5);
            block = 0;
            if (capacity != 0) {
              block = *memory::crt::OperatorNew(h.base, capacity * rl::kRequestBytes);
              for (uint32_t i = 0; i < 3 * capacity; i++) {
                guest::StoreU32(h.base, block + 4 * i, uint32_t(h.random.Next()));
              }
            }
            const uint32_t vector = loop + rl::kRequests;
            guest::StoreU32(h.base, vector + rl::kVectorFirst, block);
            guest::StoreU32(h.base, vector + rl::kVectorLast, block + size * rl::kRequestBytes);
            guest::StoreU32(h.base, vector + rl::kVectorEnd, block + capacity * rl::kRequestBytes);
          },
          [&](PPCContext& t) {
            t.r3.u64 = loop;
            t.r4.u64 = h.data + 8;  // the key's word
            t.r5.u64 = ObjectAt(h, 0);
            Fill(h.base, t.r1.u32 - 112 + 80, 12);
          },
          which == 0 ? __imp__sub_8209AAA8 : __imp__sub_8209AB48,
          which == 0 ? sub_8209AAA8 : sub_8209AB48,
          [&](Outputs& out, const PPCContext& t) {
            const uint32_t vector = loop + rl::kRequests;
            const uint32_t first = Load(h.base, vector + rl::kVectorFirst);
            const uint32_t last = Load(h.base, vector + rl::kVectorLast);
            const uint32_t end = Load(h.base, vector + rl::kVectorEnd);
            out.Bytes(Read(h.base, h.data, 2048 - 64 + rl::kRequests));
            out.Word(last - first);
            out.Word(end - first);
            out.Word(first == block);
            const bool old_alive = block != 0 && memory::IsHeapBlock(block);
            out.Word(old_alive);
            out.Bytes(Read(h.base, first, last - first));
            out.Bytes(Read(h.base, t.r1.u32 - 112 + 80, 12));
            out.Word(t.r3.u32 == loop + rl::kRequests ? 0 : t.r3.u32 - t.r1.u32);
            if (first != 0) {
              memory::crt::OperatorDelete(first);
            }
            if (old_alive && block != first) {
              memory::crt::OperatorDelete(block);
            }
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
}


// --- The constructors ---
//
// As the dispatchers': the adapters against the originals, the services'
// interfaces objects in the scratch area whose slots point at small guest
// functions (sub_822A8C18 and sub_820CE9F8 store r4 and r5 at r3 + 60, + 64
// and + 444, + 448). The debug text's global (0x8266D080) is kept and put
// back around each case.

constexpr uint32_t kStoreR4R5At60 = 0x822A8C18;
constexpr uint32_t kStoreR4R5At444 = 0x820CE9F8;

// The heap block `block` (if one): its size and words by number (it 0xB10C0000),
// then freed.
void HeapBlockOut(Context& h, Outputs& out, uint32_t block, uint32_t bytes) {
  const bool heap = block != 0 && memory::IsHeapBlock(block);
  out.Word(heap);
  if (!heap) {
    return;
  }
  out.Word(memory::Size(block));
  for (uint32_t at = 0; at < bytes; at += 4) {
    const uint32_t word = Load(h.base, block + at);
    out.Word(word - block < 0x100 ? 0xB10C0000u + (word - block) : word);
  }
  memory::crt::OperatorDelete(block);
}

// The register steps of the constructors whose adapters went: the
// framework's, and the debug text's with its post from a 160-byte frame (the
// stack word read there) as the adapter did.
void NativeFrameworkConstruct(PPCContext& ctx, uint8_t* base) {
  rl::FrameworkConstruct(base, ctx.r3.u32);
}
// Framework::Create's, as its adapter did (the original where the plain C++
// hands back).
void NativeFrameworkCreate(PPCContext& ctx, uint8_t* base) {
  if (!rl::FrameworkCreate(base, ctx.r3.u32)) {
    __imp__sub_820909B8(ctx, base);
  }
}
void NativeDebugTextConstruct(PPCContext& ctx, uint8_t* base) {
  const uint32_t text = ctx.r3.u32;
  const uint32_t block = ctx.r4.u32;
  const uint32_t stack_word = guest::LoadU32(base, ctx.r1.u32 - 160 + 100);
  guest_call::InFrame(ctx, base, 160, 0x82090F78, [&](uint32_t) {
    rl::DebugTextConstruct(base, text, block, stack_word, [&](uint32_t interface, uint32_t job) {
      ctx.r3.u64 = interface;
      ctx.r4.u64 = job;
      ctx.r5.u64 = 1;
      ctx.ctr.u64 = rl::VirtualTarget(base, interface, 4);
      ctx.lr = 0x82090F78;
      REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
    });
  });
  ctx.r3.u64 = text;
}

void Constructors(Context& h) {
  {
    Results results("Framework::Framework (sub_82090420)");
    for (int c = 0; c < 20; c++) {
      MapCase(
          h, results, [&] { DispatchObjects(h, 0, {}); },
          [&](PPCContext& t) { t.r3.u64 = h.data + 64; }, __imp__sub_82090420,
          NativeFrameworkConstruct,
          // r4 to r11 aren't compared: the original leaves vtable addresses
          // there, which Framework::Create (its only caller) doesn't read.
          [&](Outputs& out, const PPCContext& t) { DispatchOut(h, out, t, false); },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  for (int which = 0; which < 2; which++) {
    Results results(which == 0 ? "PoolMaker::PoolMaker (sub_820AC8F8)"
                               : "PoolMaker2::PoolMaker2 (sub_820AC848)");
    for (int c = 0; c < 50; c++) {
      MapCase(
          h, results,
          [&] {
            std::vector<uint32_t> list(16, kWord60);
            list[1] = h.random.Below(2) ? kWord60 : kByte133;
            DispatchObjects(h, 1, list);
          },
          [&](PPCContext& t) {
            t.r3.u64 = h.data + 32;
            t.r4.u64 = ObjectAt(h, 0);
          },
          which == 0 ? __imp__sub_820AC8F8 : __imp__sub_820AC848,
          which == 0 ? sub_820AC8F8 : sub_820AC848,
          [&](Outputs& out, const PPCContext& t) { DispatchOut(h, out, t); },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // The block: the run loop (its slots 2 and 1 the recorders) and the
    // interface; the frame's key and delegates compared too.
    Results results("FrameStart::FrameStart (sub_82092710)");
    for (int c = 0; c < 50; c++) {
      MapCase(
          h, results,
          [&] {
            DispatchObjects(h, 0, {});
            const uint32_t loop = h.data + 2048;
            guest::StoreU32(h.base, loop, loop + 0x200);
            guest::StoreU32(h.base, loop + 0x200 + 4, kStoreR4R5At444);
            guest::StoreU32(h.base, loop + 0x200 + 8, kStoreR4R5At60);
            guest::StoreU32(h.base, h.data + 16, loop);
            guest::StoreU32(h.base, h.data + 20, uint32_t(h.random.Next()));
          },
          [&](PPCContext& t) {
            t.r3.u64 = h.data + 32;
            t.r4.u64 = h.data + 16;
            Fill(h.base, t.r1.u32 - 144, 144);
          },
          __imp__sub_82092710, sub_82092710,
          [&](Outputs& out, const PPCContext& t) {
            out.Bytes(Read(h.base, h.data, 2048 + 0x210));
            out.Bytes(Read(h.base, t.r1.u32 - 144 + 80, 36));
            out.Word(t.r3.u32);
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  const std::vector<uint8_t> kept = Read(h.base, rl::kDebugTextGlobal, 4);
  {
    // The block: a word, and the worker pool's interface (its slot 1 the
    // recorder); the stack below the call random (the word the constructor
    // reads there).
    Results results("DebugText::DebugText (sub_82090EA8)");
    for (int c = 0; c < 50; c++) {
      MapCase(
          h, results,
          [&] {
            std::vector<uint32_t> list(16, kStoreR4R5At60);
            DispatchObjects(h, 1, list);
            guest::StoreU32(h.base, h.data + 16, uint32_t(h.random.Next()));
            guest::StoreU32(h.base, h.data + 20, ObjectAt(h, 0));
          },
          [&](PPCContext& t) {
            t.r3.u64 = h.data + 64;
            t.r4.u64 = h.data + 16;
            for (uint32_t at = 4; at <= 512; at += 4) {
              guest::StoreU32(h.base, t.r1.u32 - at, uint32_t(h.random.Next()));
            }
          },
          __imp__sub_82090EA8, NativeDebugTextConstruct,
          [&](Outputs& out, const PPCContext& t) {
            DispatchOut(h, out, t);
            out.Bytes(Read(h.base, rl::kDebugTextGlobal, 4));
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  if (hooks::NativeHeapActive()) {
    {
      Results results("Framework::Create (sub_820909B8)");
      for (int c = 0; c < 20; c++) {
        MapCase(
            h, results, [&] { DispatchObjects(h, 0, {}); },
            [&](PPCContext& t) { t.r3.u64 = h.data + 8; }, __imp__sub_820909B8,
            NativeFrameworkCreate,
            [&](Outputs& out, const PPCContext& t) {
              const uint32_t block = Load(h.base, h.data + 8);
              guest::StoreU32(h.base, h.data + 8, 0);
              DispatchOut(h, out, t, false);  // r4: as Framework::Framework's
              HeapBlockOut(h, out, block, 136);
            },
            fmt::format("case {}", c));
      }
      results.Report();
    }
    {
      Results results("DebugText::Create (sub_82091160)");
      for (int c = 0; c < 50; c++) {
        MapCase(
            h, results,
            [&] {
              std::vector<uint32_t> list(16, kStoreR4R5At60);
              DispatchObjects(h, 1, list);
              guest::StoreU32(h.base, h.data + 16, uint32_t(h.random.Next()));
              guest::StoreU32(h.base, h.data + 20, ObjectAt(h, 0));
            },
            [&](PPCContext& t) {
              t.r3.u64 = h.data + 16;
              for (uint32_t at = 4; at <= 512; at += 4) {
                guest::StoreU32(h.base, t.r1.u32 - at, uint32_t(h.random.Next()));
              }
            },
            __imp__sub_82091160, sub_82091160,
            [&](Outputs& out, const PPCContext& t) {
              const uint32_t text = t.r3.u32;
              // The job's address recorded by the pool, and the global, by
              // number.
              const uint32_t pool = ObjectAt(h, 0);
              out.Word(Load(h.base, pool + 60) - text);
              guest::StoreU32(h.base, pool + 60, 0);
              out.Word(Load(h.base, rl::kDebugTextGlobal) == text);
              out.Bytes(Read(h.base, h.data, 2048));
              out.Word(t.r4.u32 - text);
              HeapBlockOut(h, out, text, 52);
            },
            fmt::format("case {}", c));
      }
      results.Report();
    }
  }
  Write(h.base, rl::kDebugTextGlobal, kept.data(), 4);
}

// --- The entry and the main loop (src/framework/main_loop.h) ---
//
// The adapters' register steps (hooks::ForEachTickHandler and the rest,
// src/hooks/framework.cpp) against the originals (with the flag off, so
// their callees are the originals too), on objects in the scratch area
// whose slots point at small guest functions: the recorder and getters
// above, and sub_8245F860 and sub_825A2370, which add one to the word at
// r3 + 4 and + 8 (counters). The functors are the recorder or the game's own
// (TickHandler::Invoke, RunLoop::ApplyRequest); App::Run's loop runs while
// the tick handler map isn't empty, so its tick handler is the game's
// Framework::OnTick (sub_820902C8) on an object whose app answers 0, which
// removes it (tick handler 7, through the run loop's slot 4 and
// ApplyRequest's removal, never seen in play): one, two or three passes.
// What they leave is compared with the heap's nodes and blocks by number.

constexpr uint32_t kCount4 = 0x8245F860;
constexpr uint32_t kCount8 = 0x825A2370;
constexpr uint32_t kTickHandlerInvoke = 0x82099A98;
constexpr uint32_t kApplyRequest = 0x8209A458;
constexpr uint32_t kFrameworkOnTick = 0x820902C8;

// Words numbered: a node met in the walk, or a word into one of `blocks`
// (by block and offset); others as they are.
struct Numbering {
  Walk walk;
  std::vector<uint32_t> blocks;
  uint32_t Of(uint32_t word) const {
    const uint32_t node = walk.Of(word);
    if (node != word) {
      return node;
    }
    for (uint32_t i = 0; i < blocks.size(); i++) {
      if (blocks[i] != 0 && word - blocks[i] < 0x400) {
        return 0xB1100000u + 0x1000 * i + (word - blocks[i]);
      }
    }
    return word;
  }
  void Words(uint8_t* base, Outputs& out, uint32_t address, uint32_t bytes) const {
    for (uint32_t at = 0; at < bytes; at += 4) {
      out.Word(Of(Load(base, address + at)));
    }
  }
};

// A run loop at `loop` built by the game's RunLoop::RunLoop.
void BuildRunLoop(Context& h, uint32_t loop) {
  FlagOff off;
  PPCContext t = CallContext(h);
  t.r3.u64 = loop;
  __imp__sub_8209A9F0(t, h.base);
}

// A request vector at `vector` holding `requests` (type, key, delegate) in a
// heap block with room for `spare` more; returns the block (0 when empty
// with no room).
uint32_t BuildRequests(Context& h, uint32_t vector, const std::vector<std::array<uint32_t, 3>>& requests,
                       uint32_t spare) {
  const uint32_t capacity = uint32_t(requests.size()) + spare;
  uint32_t block = 0;
  if (capacity != 0) {
    block = *memory::crt::OperatorNew(h.base, capacity * rl::kRequestBytes);
    Fill(h.base, block, capacity * rl::kRequestBytes);
    for (uint32_t i = 0; i < requests.size(); i++) {
      for (uint32_t w = 0; w < 3; w++) {
        guest::StoreU32(h.base, block + rl::kRequestBytes * i + 4 * w, requests[i][w]);
      }
    }
  }
  guest::StoreU32(h.base, vector + rl::kVectorFirst, block);
  guest::StoreU32(h.base, vector + rl::kVectorLast, block + uint32_t(requests.size()) * rl::kRequestBytes);
  guest::StoreU32(h.base, vector + rl::kVectorEnd, block + capacity * rl::kRequestBytes);
  return block;
}

// The run loop's maps walked (both heads), its vector's block, then
// `more()` adds what the case compares; the nodes and the block freed.
template <typename More>
void RunLoopOut(Context& h, Outputs& out, uint32_t loop, uint32_t old_block, More more) {
  Numbering n;
  n.walk = WalkFrom(h.base, Load(h.base, loop + rl::kTickHandlers + rl::kMapHead),
                    {Load(h.base, loop + rl::kListeners + rl::kMapHead)});
  const uint32_t first = Load(h.base, loop + rl::kRequests + rl::kVectorFirst);
  n.blocks = {old_block, first};
  more(n);
  const uint32_t last = Load(h.base, loop + rl::kRequests + rl::kVectorLast);
  if (first != 0 && last >= first && last - first < 0x1000) {
    n.Words(h.base, out, first, last - first);
  }
  NodesOut(h.base, out, n.walk);
  if (first != 0 && memory::IsHeapBlock(first)) {
    memory::crt::OperatorDelete(first);
  }
  if (old_block != 0 && old_block != first && memory::IsHeapBlock(old_block)) {
    memory::crt::OperatorDelete(old_block);
  }
}

void MainLoop(Context& h) {
  const uint32_t loop = h.data + 3072;
  {
    Results results("ForEachTickHandler (sub_82099D50)");
    for (int c = 0; c < 300; c++) {
      const bool invoke = c % 2 == 1;
      uint32_t function = 0, object = 0, first = 0, last = 0, map = 0;
      MapCase(
          h, results,
          [&] {
            DispatchObjects(h, 5, std::vector<uint32_t>(16, kCount4));
            BuildRunLoop(h, loop);
            map = loop + rl::kTickHandlers;
            std::vector<std::pair<uint32_t, uint32_t>> entries;
            for (uint32_t key : RandomKeys(h.random)) {
              entries.emplace_back(key, invoke ? ObjectAt(h, 1 + h.random.Below(4))
                                               : uint32_t(h.random.Next()));
            }
            Fill(h.base, h.data, 64);
            BuildMapOf(h, map, entries);
            // A range of the map in key order (the head its end).
            std::vector<uint32_t> nodes = InOrder(h.base, map);
            nodes.push_back(Load(h.base, map + rl::kMapHead));
            const uint32_t i = h.random.Below(uint32_t(nodes.size()));
            const uint32_t j = i + h.random.Below(uint32_t(nodes.size()) - i);
            first = nodes[i];
            last = nodes[j];
            function = invoke ? kTickHandlerInvoke : kStoreR4At4;
            object = invoke ? uint32_t(h.random.Next()) : ObjectAt(h, 0);
          },
          [&](PPCContext& t) {
            t.r3.u64 = h.data + 400;
            t.r4.u64 = (uint64_t(map) << 32) | first;
            t.r5.u64 = (uint64_t(map) << 32) | last;
            t.r6.u64 = (uint64_t(function) << 32) | object;
            Fill(h.base, t.r1.u32 - 128, 128 + 48);
          },
          __imp__sub_82099D50, hooks::ForEachTickHandler,
          [&](Outputs& out, const PPCContext& t) {
            RunLoopOut(h, out, loop, 0, [&](const Numbering& n) {
              n.Words(h.base, out, h.data, 3072 + 56);
              // The callee's r4 (TickHandler::Invoke's, at the frame's +24),
              // the iterator and the key and delegate at its +80, the
              // caller's words.
              n.Words(h.base, out, t.r1.u32 - 128 + 24, 8);
              n.Words(h.base, out, t.r1.u32 - 128 + 80, 16);
              n.Words(h.base, out, t.r1.u32 + 24, 24);
              out.Word(n.Of(t.r3.u32));
            });
          },
          fmt::format("{} case {}", invoke ? "TickHandler::Invoke" : "recorder", c), false);
    }
    results.Report();
  }
  {
    Results results("ForEachRequest (sub_82099EE8)");
    for (int c = 0; c < 300; c++) {
      const bool apply = c % 2 == 1;
      uint32_t function = 0, object = 0, first = 0, last = 0, block = 0;
      const uint32_t vector = h.data + 64;
      MapCase(
          h, results,
          [&] {
            DispatchObjects(h, 1, std::vector<uint32_t>(16, kCount4));
            BuildRunLoop(h, loop);
            BuildMap(h, loop + rl::kTickHandlers, RandomKeys(h.random));
            BuildMap(h, loop + rl::kListeners, RandomKeys(h.random));
            std::vector<std::array<uint32_t, 3>> requests(h.random.Below(9));
            const uint32_t types[] = {0, 2, 4, 7};
            for (auto& request : requests) {
              request = {types[h.random.Below(4)], h.random.Below(16), uint32_t(h.random.Next())};
            }
            Fill(h.base, h.data, 128);
            block = BuildRequests(h, vector, requests, h.random.Below(3));
            const uint32_t size = uint32_t(requests.size());
            const uint32_t i = h.random.Below(size + 1);
            const uint32_t j = i + h.random.Below(size + 1 - i);
            first = block + rl::kRequestBytes * i;
            last = block + rl::kRequestBytes * j;
            function = apply ? kApplyRequest : kStoreR4At4;
            object = apply ? loop : ObjectAt(h, 0);
          },
          [&](PPCContext& t) {
            t.r3.u64 = h.data + 400;
            t.r4.u64 = (uint64_t(vector) << 32) | first;
            t.r5.u64 = (uint64_t(vector) << 32) | last;
            t.r6.u64 = (uint64_t(function) << 32) | object;
            Fill(h.base, t.r1.u32 - 128, 128 + 48);
          },
          __imp__sub_82099EE8, hooks::ForEachRequest,
          [&](Outputs& out, const PPCContext& t) {
            // The vector's block is the run loop's for the walk: the case's
            // vector is at data + 64, so it's freed here.
            Numbering n;
            n.walk = WalkFrom(h.base, Load(h.base, loop + rl::kTickHandlers + rl::kMapHead),
                              {Load(h.base, loop + rl::kListeners + rl::kMapHead)});
            n.blocks = {block};
            n.Words(h.base, out, h.data, 3072 + 56);
            // ApplyRequest's r4 and r5 at the frame's +32, the caller's words.
            n.Words(h.base, out, t.r1.u32 - 128 + 32, 16);
            n.Words(h.base, out, t.r1.u32 + 24, 24);
            out.Word(n.Of(t.r3.u32));
            NodesOut(h.base, out, n.walk);
            if (block != 0) {
              memory::crt::OperatorDelete(block);
            }
          },
          fmt::format("{} case {}", apply ? "RunLoop::ApplyRequest" : "recorder", c), false);
    }
    results.Report();
  }
  {
    // The framework (at data + 2048, its vtable at + 2200, apart, as its
    // +133 would be in an object's slots): slot 1 (Init) a getter of +133 or
    // +60, 2 (Start) the +4 counter, 3 the +8 counter, 4 and 5 getters or
    // the +4 counter. Tick handler 7's delegate (at data + 2240, its vtable
    // at + 2400, as OnTick writes its +133 and +134): slot 0 (its release)
    // the +8 counter, 1 Framework::OnTick, its input manager (+88, object 2:
    // slot 1 the recorder) and app (+120, object 3: slot 11 the getter of
    // +133, 0). The listeners' delegates (object 4): slot 1 the +4 counter.
    Results results("App::Run (sub_8209A760)");
    for (int c = 0; c < 300; c++) {
      uint32_t block = 0;
      bool init = true;
      MapCase(
          h, results,
          [&] {
            const uint32_t getters[] = {kByte133, kWord60, kCount4};
            std::vector<uint32_t> list(16, kCount4);
            DispatchObjects(h, 5, list);
            const uint32_t framework = h.data + 2048;
            const uint32_t vtable = h.data + 2200;
            guest::StoreU32(h.base, framework, vtable);
            guest::StoreU32(h.base, vtable + 4, getters[h.random.Below(2)]);
            guest::StoreU32(h.base, vtable + 8, kCount4);
            guest::StoreU32(h.base, vtable + 12, kCount8);
            guest::StoreU32(h.base, vtable + 16, getters[h.random.Below(3)]);
            guest::StoreU32(h.base, vtable + 20, getters[h.random.Below(3)]);
            guest::StoreU8(h.base, framework + 133, h.random.Below(4) == 0 ? 0 : uint8_t(h.random.Next()));
            guest::StoreU32(h.base, framework + 60, h.random.Below(4) == 0 ? 0 : uint32_t(h.random.Next()));
            init = !(Load(h.base, vtable + 4) == kByte133
                         ? guest::LoadU8(h.base, framework + 133) == 0
                         : (Load(h.base, framework + 60) & 0xFF) == 0);
            const uint32_t handler = h.data + 2240;
            guest::StoreU32(h.base, handler, h.data + 2400);
            guest::StoreU32(h.base, h.data + 2400 + 0, kCount8);
            guest::StoreU32(h.base, h.data + 2400 + 4, kFrameworkOnTick);
            guest::StoreU32(h.base, handler + 88, ObjectAt(h, 2));
            guest::StoreU32(h.base, handler + 120, ObjectAt(h, 3));
            guest::StoreU32(h.base, ObjectAt(h, 2) + 0x80 + 4, kStoreR4At4);
            guest::StoreU32(h.base, ObjectAt(h, 3) + 0x80 + 44, kByte133);
            guest::StoreU8(h.base, ObjectAt(h, 3) + 133, 0);
            BuildRunLoop(h, loop);
            const uint32_t passes = h.random.Below(3);
            if (passes == 1) {
              BuildMapOf(h, loop + rl::kTickHandlers, {{7, handler}});
            }
            std::vector<std::pair<uint32_t, uint32_t>> listeners;
            for (uint32_t i = h.random.Below(4); i > 0; i--) {
              listeners.emplace_back(h.random.Below(8), ObjectAt(h, 4));
            }
            BuildMapOf(h, loop + rl::kListeners, listeners);
            std::vector<std::array<uint32_t, 3>> requests;
            for (uint32_t i = h.random.Below(4); i > 0; i--) {
              const uint32_t kinds[] = {2, 4};
              requests.push_back({kinds[h.random.Below(2)], h.random.Below(8), ObjectAt(h, 4)});
            }
            if (passes == 2 || (passes == 1 && h.random.Below(2))) {
              requests.insert(requests.begin() + h.random.Below(uint32_t(requests.size()) + 1),
                              {0, 7, handler});
            }
            Fill(h.base, h.data, 512);
            block = h.random.Below(3) == 0 && requests.empty()
                        ? 0
                        : BuildRequests(h, loop + rl::kRequests, requests, h.random.Below(3));
          },
          [&](PPCContext& t) {
            t.r3.u64 = loop;
            t.r4.u64 = h.data + 2048;
            Fill(h.base, t.r1.u32 - 208, 208 + 48);
          },
          __imp__sub_8209A760, hooks::AppRun,
          [&](Outputs& out, const PPCContext& t) {
            RunLoopOut(h, out, loop, block, [&](const Numbering& n) {
              n.Words(h.base, out, h.data, 3072 + 56);
              // The frame's words +80 to +152 (the event, the functors, the
              // iterators and the results); the last pass's first tick
              // handler (+116) as whether it's the head (+108), as it may
              // be a node freed since.
              const uint32_t frame = t.r1.u32 - 208;
              n.Words(h.base, out, frame + 80, 36);
              out.Word(Load(h.base, frame + 116) == Load(h.base, frame + 108));
              n.Words(h.base, out, frame + 120, 32);
              out.Word(t.r3.u32);
            });
          },
          fmt::format("{} case {}", init ? "init" : "no init", c), false);
    }
    results.Report();
  }
  for (int which = 0; which < 2; which++) {
    // The framework (data + 2048) as App::Run's, with an empty run loop:
    // one pass. sub_82090378 is given a holder (data + 8).
    Results results(which == 0 ? "sub_8209ABE8 (RunFramework)" : "sub_82090378 (RunFrameworkAt)");
    for (int c = 0; c < 50; c++) {
      MapCase(
          h, results,
          [&] {
            const uint32_t getters[] = {kByte133, kWord60, kCount4};
            DispatchObjects(h, 0, {});
            const uint32_t framework = h.data + 2048;
            const uint32_t vtable = h.data + 2200;
            guest::StoreU32(h.base, framework, vtable);
            guest::StoreU32(h.base, vtable + 4, getters[h.random.Below(2)]);
            guest::StoreU32(h.base, vtable + 8, kStoreR4At4);
            guest::StoreU32(h.base, vtable + 12, kCount8);
            guest::StoreU32(h.base, vtable + 16, getters[h.random.Below(3)]);
            guest::StoreU32(h.base, vtable + 20, getters[h.random.Below(3)]);
            guest::StoreU8(h.base, framework + 133, h.random.Below(3) == 0 ? 0 : uint8_t(h.random.Next()));
            guest::StoreU32(h.base, framework + 60, h.random.Below(3) == 0 ? 0 : uint32_t(h.random.Next()));
            guest::StoreU32(h.base, h.data + 8, framework);
          },
          [&](PPCContext& t) {
            t.r3.u64 = which == 0 ? h.data + 2048 : h.data + 8;
            Fill(h.base, t.r1.u32 - 160, 160 + 48);
          },
          which == 0 ? __imp__sub_8209ABE8 : __imp__sub_82090378,
          which == 0 ? hooks::RunFramework : hooks::RunFrameworkAt,
          [&](Outputs& out, const PPCContext& t) {
            out.Bytes(Read(h.base, h.data, 2400));
            // The run loop on the frame (+80) after its destructor: the
            // vtable, the maps' sizes and the vector (the freed heads' words,
            // +8 and +20, left out).
            const uint32_t run_loop = t.r1.u32 - 160 + 80;
            for (uint32_t at : {0u, 4u, 12u, 16u, 24u, 28u, 32u, 36u, 40u, 44u}) {
              out.Word(Load(h.base, run_loop + at));
            }
            out.Bytes(Read(h.base, run_loop + 48, 4));
            out.Word(t.r3.u32);
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // main with the game's own framework (Framework::Create): its vtable
    // (0x82000B90, in the image's read-only data: its page made writable
    // for the cases through the SDK's memory utilities, then given its
    // access back) has slots 0 to 5 swapped for the counters and getters for
    // the case (put back after), so Framework::Init (slot 1) answers its
    // +133 (0: App::Run returns at once) or +60 (a vtable: one pass with an
    // empty run loop), and the deletion (slot 0) counts at +8.
    constexpr uint32_t kFrameworkVtable = 0x82000B90;
    // The host page holding it (mprotect wants it aligned).
    const size_t page_size = rex::memory::page_size();
    uint8_t* const page = reinterpret_cast<uint8_t*>(
        reinterpret_cast<uintptr_t>(h.base + kFrameworkVtable) & ~uintptr_t(page_size - 1));
    rex::memory::PageAccess access = rex::memory::PageAccess::kReadOnly;
    if (!rex::memory::Protect(page, page_size, rex::memory::PageAccess::kReadWrite, &access)) {
      ITC_LOG_ERROR("harness: framework: main's cases: the framework's vtable can't be made "
                    "writable");
      return;
    }
    const std::vector<uint8_t> kept = Read(h.base, kFrameworkVtable, 24);
    Results results("main (sub_82090108)");
    for (int c = 0; c < 20; c++) {
      MapCase(
          h, results,
          [&] {
            DispatchObjects(h, 0, {});
            const uint32_t getters[] = {kByte133, kWord60};
            guest::StoreU32(h.base, kFrameworkVtable + 0, kCount8);
            guest::StoreU32(h.base, kFrameworkVtable + 4, getters[c % 2]);
            guest::StoreU32(h.base, kFrameworkVtable + 8, kCount4);
            guest::StoreU32(h.base, kFrameworkVtable + 12, kCount8);
            guest::StoreU32(h.base, kFrameworkVtable + 16, getters[h.random.Below(2)]);
            guest::StoreU32(h.base, kFrameworkVtable + 20, getters[h.random.Below(2)]);
          },
          [&](PPCContext& t) { Fill(h.base, t.r1.u32 - 112, 112 + 48); },
          __imp__sub_82090108, hooks::Main,
          [&](Outputs& out, const PPCContext& t) {
            Write(h.base, kFrameworkVtable, kept.data(), 24);
            out.Bytes(Read(h.base, h.data, 2048));
            out.Word(t.r3.u32);
            // The run loop on sub_8209ABE8's frame (below main's: sub_82090378
            // has none), as in sub_8209ABE8's cases.
            const uint32_t run_loop = t.r1.u32 - 112 - 160 + 80;
            for (uint32_t at : {0u, 4u, 12u, 16u, 24u, 28u, 32u, 36u, 40u, 44u}) {
              out.Word(Load(h.base, run_loop + at));
            }
            out.Bytes(Read(h.base, run_loop + 48, 4));
            HeapBlockOut(h, out, Load(h.base, t.r1.u32 - 112 + 80), 136);
          },
          fmt::format("{} case {}", c % 2 == 0 ? "no init" : "init", c));
    }
    Write(h.base, kFrameworkVtable, kept.data(), 24);
    rex::memory::Protect(page, page_size, access);
    results.Report();
  }
}

void Containers(Context& h) {
  Requests(h);
  Dispatchers(h);
  Constructors(h);
  if (!hooks::NativeHeapActive()) {
    ITC_LOG_INFO("harness: framework: the handler map's cases need --native_heap");
    return;
  }
  Iterators(h);
  Rotations(h);
  Allocations(h);
  Inserts(h);
  VectorGrowth(h);
  RunLoops(h);
  MainLoop(h);
  ITC_LOG_INFO("harness: framework: {} InsertN and Insert cases handed to the original (a tail "
               "at least the count long, its copy_backward never seen in play)",
               g_fallbacks_expected);
  if (g_fallbacks != 0) {
    ITC_LOG_INFO("harness: framework: {} cases handed to the original", g_fallbacks);
  }
}

}  // namespace

void Framework(Context& h) {
  Getters(h);
  Positions(h);
  Containers(h);
}

}  // namespace harness
