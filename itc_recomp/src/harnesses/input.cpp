// The input manager's set-up and the std::list of pointers
// (src/input/input_manager.cpp, src/std_list/ptr_list.cpp,
// docs/functions/input.md) against the recompiled functions. The objects are
// built in the scratch area, its other bytes left at the fill, and the whole
// area is compared after each case (floats where the function stores
// floats, bytes everywhere else), then r3. The originals run with
// --native_input off, so their callees are the recompiled code too. The
// functions that allocate run only with --native_heap (the adapters'
// condition): a node they make is compared by its contents and size, its
// address replaced by a marker wherever it's stored, and freed after.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/heap.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "std_list/ptr_list.h"

#include <fmt/format.h>

#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_input);

namespace harness {

namespace {

namespace sl = std_list;

// Cases the plain C++ handed to the original (none expected).
int g_fallbacks = 0;

// --- The register steps, as the adapters (src/hooks/input_manager.cpp) ---

void NativeAllocateNodes(PPCContext& ctx, uint8_t* base) {
  if (const auto block = sl::AllocateNodes(base, ctx.r3.u32)) {
    ctx.r3.u64 = *block;
    return;
  }
  g_fallbacks++;
  __imp__sub_82099C08(ctx, base);
}
void NativeIncSize(PPCContext& ctx, uint8_t* base) {
  if (!sl::IncSize(base, ctx.r3.u32, ctx.r4.u32)) {
    g_fallbacks++;
    __imp__sub_820958B8(ctx, base);
  }
}
void NativeConstruct(PPCContext& ctx, uint8_t* base) {
  if (!sl::Construct(base, ctx.r3.u32)) {
    g_fallbacks++;
    __imp__sub_820AD058(ctx, base);
  }
}
void NativeInsert(PPCContext& ctx, uint8_t* base) {
  if (!sl::Insert(base, ctx.r3.u32, ctx.r4.u32, guest::LoadU32(base, ctx.r5.u32))) {
    g_fallbacks++;
    __imp__sub_82095958(ctx, base);
  }
}

// A heap block a case made: its address replaced by this marker wherever
// it's stored.
constexpr uint32_t kNodeMarker = 0xB10C0001;

uint32_t Normalize(uint32_t word, uint32_t node) {
  return node != 0 && word == node ? kNodeMarker : word;
}

// The scratch area as words (exact), with the node's address replaced.
void AreaWords(Context& h, Outputs& out, uint32_t node) {
  for (uint32_t at = 0; at < kScratchBytes; at += 4) {
    out.Word(Normalize(guest::LoadU32(h.base, h.data + at), node));
  }
}

// The node: whether it's a heap block, its size and its first `words` words
// (normalized); then freed.
void NodeOut(Context& h, Outputs& out, uint32_t node, uint32_t words) {
  const bool block = node != 0 && memory::IsHeapBlock(node);
  out.Word(block);
  if (!block) {
    return;
  }
  out.Word(memory::Size(node));
  for (uint32_t i = 0; i < words; i++) {
    out.Word(Normalize(guest::LoadU32(h.base, node + 4 * i), node));
  }
  memory::crt::OperatorDelete(node);
}

// One case through both versions: the area filled, `setup()` writes the
// inputs (the same both times: it's handed a copy of the generator's
// state), `args(t)` sets the registers, the original (flag off) or the
// native step runs, then `collect(out, t)` keeps what's compared.
template <typename Setup, typename Args, typename Collect>
void Case(Context& h, Results& results, Setup setup, Args args, GuestFunction recompiled,
          GuestFunction native, Collect collect, const std::string& what) {
  Outputs out[2];
  const Random start = h.random;
  Random after;
  for (int v = 0; v < 2; v++) {
    h.random = start;
    Fill(h.base, h.data, kScratchBytes);
    setup();
    PPCContext t = CallContext(h);
    args(t);
    ITC_FLAG(native_input) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_input) = true;
    collect(out[v], t);
    after = h.random;
  }
  h.random = after;
  results.Check(out[0], out[1], what);
}

// --- The std::list of pointers ---

void AllocateNodesCases(Context& h) {
  Results results("sub_82099C08 PtrList::AllocateNodes");
  for (uint32_t i = 0; i < 64; i++) {
    // 1 (every call in play), 0, and small counts.
    const uint32_t count = i < 16 ? 1 : i < 20 ? 0 : h.random.Below(200);
    Case(
        h, results, [] {}, [&](PPCContext& t) { t.r3.u64 = count; }, __imp__sub_82099C08,
        NativeAllocateNodes,
        [&](Outputs& out, const PPCContext& t) {
          const uint32_t node = t.r3.u32;
          AreaWords(h, out, node);
          out.Word(Normalize(node, node));
          NodeOut(h, out, node, 0);  // its contents aren't set
        },
        fmt::format("count {}", count));
  }
  results.Report();
}

void IncSizeCases(Context& h) {
  Results results("sub_820958B8 PtrList::IncSize");
  const uint32_t list = h.data + 64;
  for (int i = 0; i < 200; i++) {
    uint32_t size = 0, count = 1;
    Case(
        h, results,
        [&] {
          Random& r = h.random;
          // Up to the limit (0x3FFFFFFF in all), never past it: past it is
          // the original's length_error, which the plain C++ hands back.
          size = i < 50 ? r.Below(8) : i < 100 ? 0x3FFFFFFFu - r.Below(4) : r.Below(0x3FFFFFFF);
          const uint32_t room = 0x3FFFFFFFu - size;
          count = i % 3 == 0 ? 1 : i % 3 == 1 ? 0 : uint32_t(r.Next() % (uint64_t(room) + 1));
          if (count > room) {
            count = room;
          }
          guest::StoreU32(h.base, list + sl::kSize, size);
        },
        [&](PPCContext& t) {
          t.r3.u64 = list;
          t.r4.u64 = count;
        },
        __imp__sub_820958B8, NativeIncSize,
        [&](Outputs& out, const PPCContext& t) {
          AreaWords(h, out, 0);
          out.Word(t.r3.u32);
        },
        fmt::format("case {}", i));
  }
  results.Report();
}

void ConstructCases(Context& h) {
  Results results("sub_820AD058 PtrList::PtrList");
  for (int i = 0; i < 32; i++) {
    const uint32_t list = h.data + 64 + 4 * uint32_t(i);
    Case(
        h, results, [] {}, [&](PPCContext& t) { t.r3.u64 = list; }, __imp__sub_820AD058,
        NativeConstruct,
        [&](Outputs& out, const PPCContext& t) {
          const uint32_t node = guest::LoadU32(h.base, list + sl::kHead);
          AreaWords(h, out, node);
          out.Word(t.r3.u32);
          NodeOut(h, out, node, 2);  // the head's value isn't set
        },
        fmt::format("list at +{}", 64 + 4 * i));
  }
  results.Report();
}

void InsertCases(Context& h) {
  Results results("sub_82095958 PtrList::Insert");
  const uint32_t list = h.data + 16;
  const uint32_t value = h.data + 32;
  const uint32_t nodes = h.data + 64;  // the head, then up to 7 nodes
  for (int i = 0; i < 200; i++) {
    uint32_t where = 0;
    Case(
        h, results,
        [&] {
          Random& r = h.random;
          // A list of 0-7 nodes (the input manager's are empty, the sound
          // manager's grow), linked in order around the head.
          const uint32_t count = i < 40 ? 0 : r.Below(8);
          for (uint32_t k = 0; k <= count; k++) {
            const uint32_t node = nodes + sl::kNodeBytes * k;
            const uint32_t next = nodes + sl::kNodeBytes * (k == count ? 0 : k + 1);
            const uint32_t previous = nodes + sl::kNodeBytes * (k == 0 ? count : k - 1);
            guest::StoreU32(h.base, node + sl::kNext, next);
            guest::StoreU32(h.base, node + sl::kPrevious, previous);
            guest::StoreU32(h.base, node + sl::kValue, uint32_t(r.Next()));
          }
          guest::StoreU32(h.base, list + sl::kHead, nodes);
          guest::StoreU32(h.base, list + sl::kSize, i % 5 == 4 ? r.Below(0x3FFFFFFF) : count);
          guest::StoreU32(h.base, value, uint32_t(r.Next()));
          // Before the head (at the end: the input manager's) or any node.
          where = nodes + sl::kNodeBytes * (i < 100 ? 0 : r.Below(count + 1));
        },
        [&](PPCContext& t) {
          t.r3.u64 = list;
          // The iterator by value: its list in the high word, as
          // InputManager::AddListener passes it.
          t.r4.u64 = (uint64_t(list) << 32) | where;
          t.r5.u64 = value;
        },
        __imp__sub_82095958, NativeInsert,
        [&](Outputs& out, const PPCContext& t) {
          const uint32_t node = guest::LoadU32(h.base, where + sl::kPrevious);
          AreaWords(h, out, node);
          out.Word(t.r3.u32);
          NodeOut(h, out, node, 3);
        },
        fmt::format("case {}", i));
  }
  results.Report();
}

}  // namespace

void Input(Context& h) {
  g_fallbacks = 0;
  if (!hooks::NativeHeapActive()) {
    ITC_LOG_INFO("harness: input: the std::list cases need --native_heap; skipped");
  } else {
    AllocateNodesCases(h);
    ConstructCases(h);
    InsertCases(h);
  }
  IncSizeCases(h);
  if (g_fallbacks != 0) {
    ITC_LOG_ERROR("harness: input: {} cases handed to the original", g_fallbacks);
  }
}

}  // namespace harness
