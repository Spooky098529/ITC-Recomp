// The app's update and its state stack (src/app_states/,
// docs/functions/app-states.md) against the recompiled functions. The
// originals run with --native_app_states off (so their callees are the
// originals too), the plain C++ through the register steps
// (hooks/app_states.h). Inputs go in the scratch area; list nodes and the
// blocks the allocators give come from the allocation layer (operator new's
// plain C++), each version building its own, and words pointing into them
// are compared by the block's number and the offset, so the two versions'
// heap addresses don't matter. Compared with harness::Outputs (no floats
// here: every value exactly).

#include "harness.h"

#include "app_states/containers.h"
#include "base/flags.h"
#include "hooks/app_states.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "std_list/ptr_list.h"

#include <fmt/format.h>

#include <algorithm>
#include <optional>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_app_states);

namespace harness {

namespace {

namespace as = app_states;

struct FlagOff {
  FlagOff() : saved_(ITC_FLAG(native_app_states)) { ITC_FLAG(native_app_states) = false; }
  ~FlagOff() { ITC_FLAG(native_app_states) = saved_; }
  bool saved_;
};

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// The heap blocks a case made or met, numbered in the order added.
struct Blocks {
  std::vector<uint32_t> list;
  std::vector<uint32_t> sizes;  // as allocated (a word past the end counts)
  uint32_t Add(uint32_t block) {
    list.push_back(block);
    sizes.push_back(block != 0 && memory::IsHeapBlock(block) ? memory::Size(block) : 4);
    return block;
  }
  uint32_t New(uint8_t* base, uint32_t bytes) {
    const auto block = memory::crt::OperatorNew(base, bytes);
    return Add(block ? *block : 0);
  }
  // A word into a block (or just past its end) as 0xB1000000 + 0x1000 x its
  // number + the offset; others as they are.
  uint32_t Of(uint32_t word) const {
    for (uint32_t i = 0; i < list.size(); i++) {
      if (list[i] != 0 && word >= list[i] && word - list[i] <= sizes[i]) {
        return 0xB1000000u + 0x1000 * i + (word - list[i]);
      }
    }
    return word;
  }
  void Words(uint8_t* base, Outputs& out, uint32_t address, uint32_t bytes) const {
    for (uint32_t at = 0; at < bytes; at += 4) {
      out.Word(Of(Load(base, address + at)));
    }
  }
  // Each block's state (a live heap block and its size, or freed), then the
  // live ones freed.
  void Out(Outputs& out) const {
    for (uint32_t block : list) {
      const bool live = block != 0 && memory::IsHeapBlock(block);
      out.Word(live ? 1 : 0);
      if (live) {
        out.Word(memory::Size(block));
      }
    }
    FreeAll();
  }
  void FreeAll() const {
    for (uint32_t block : list) {
      if (block != 0 && memory::IsHeapBlock(block)) {
        memory::crt::OperatorDelete(block);
      }
    }
  }
};

// One case through both versions: `setup(blocks)` builds the inputs (the
// same both times: it's handed a copy of the generator's state), `args(t)`
// sets the registers, the original (flag off) or the native step runs, then
// `collect(out, t, blocks)` keeps what's compared (and frees the blocks:
// Blocks::Out). The caller's words at r1 + 32 to + 48 are compared by number.
template <typename Setup, typename Args, typename Collect>
void Case(Context& h, Results& results, Setup setup, Args args, GuestFunction recompiled,
          GuestFunction native, Collect collect, const std::string& what) {
  Outputs out[2];
  const Random start = h.random;
  Random after;
  for (int v = 0; v < 2; v++) {
    h.random = start;
    Fill(h.base, h.data, kScratchBytes);
    PPCContext t = CallContext(h);
    Fill(h.base, t.r1.u32 + 32, 16);
    Blocks blocks;
    setup(blocks);
    args(t);
    {
      std::optional<FlagOff> off;
      if (v == 0) {
        off.emplace();
      }
      (v == 0 ? recompiled : native)(t, h.base);
    }
    blocks.Words(h.base, out[v], t.r1.u32 + 32, 16);
    collect(out[v], t, blocks);
    after = h.random;
  }
  h.random = after;
  results.Check(out[0], out[1], what);
}

// --- The message vector ---

void MessageVector(Context& h) {
  const uint32_t vector = h.data;
  const uint32_t result = h.data + 16;
  const uint32_t elements = h.data + 64;
  // A vector of `size` random words in the scratch area with room for
  // `spare` more.
  auto build = [&](uint32_t size, uint32_t spare) {
    for (uint32_t i = 0; i < size + spare; i++) {
      Store(h.base, elements + 4 * i, uint32_t(h.random.Next()));
    }
    Store(h.base, vector + as::kVectorFirst, elements);
    Store(h.base, vector + as::kVectorLast, elements + 4 * size);
    Store(h.base, vector + as::kVectorEnd, elements + 4 * (size + spare));
  };
  {
    Results results("MessageVector::Erase (sub_82092E70)");
    for (int c = 0; c < 300; c++) {
      uint32_t first = 0, last = 0, tag = 0;
      Case(
          h, results,
          [&](Blocks&) {
            const uint32_t size = h.random.Below(40);
            build(size, h.random.Below(4));
            const uint32_t i = h.random.Below(size + 1);
            const uint32_t j = i + h.random.Below(size + 1 - i);
            first = elements + 4 * i;
            last = elements + 4 * j;
            // The iterators' vector: the vector's own, or another nonzero
            // word (only checked to be the same and not 0).
            tag = c % 5 == 4 ? (uint32_t(h.random.Next()) | 1) : vector;
          },
          [&](PPCContext& t) {
            t.r3.u64 = result;
            t.r4.u64 = vector;
            t.r5.u64 = (uint64_t(tag) << 32) | first;
            t.r6.u64 = (uint64_t(tag) << 32) | last;
          },
          __imp__sub_82092E70, hooks::AppMessagesErase,
          [&](Outputs& out, const PPCContext& t, const Blocks&) {
            out.Bytes(Read(h.base, h.data, 512));
            out.Word(t.r3.u32);
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("MessageVector::Clear (sub_82092F98)");
    for (int c = 0; c < 100; c++) {
      Case(
          h, results, [&](Blocks&) { build(h.random.Below(40), h.random.Below(4)); },
          [&](PPCContext& t) { t.r3.u64 = vector; }, __imp__sub_82092F98,
          hooks::AppMessagesClear,
          [&](Outputs& out, const PPCContext& t, const Blocks&) {
            out.Bytes(Read(h.base, h.data, 512));
            out.Word(t.r3.u32 - t.r1.u32);
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- The allocators ---

void Allocators(Context& h) {
  struct Allocator {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
  };
  const Allocator allocators[] = {
      {"MessageVector::Allocate (sub_82092B78)", __imp__sub_82092B78,
       hooks::AppMessagesAllocate},
      {"OpList::AllocateNodes (sub_820ACF10)", __imp__sub_820ACF10, hooks::AppOpAllocateNodes},
  };
  for (const Allocator& a : allocators) {
    Results results(a.name);
    for (int c = 0; c < 60; c++) {
      const uint32_t count = c < 3 ? uint32_t(c) : c % 10 == 9 ? 200 + h.random.Below(2000)
                                                               : h.random.Below(64);
      Case(
          h, results, [&](Blocks&) {}, [&](PPCContext& t) { t.r3.u64 = count; }, a.recompiled,
          a.native,
          [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
            out.Word(t.r3.u32 != 0 ? 1 : 0);
            blocks.Add(t.r3.u32);
            out.Word(blocks.Of(t.r3.u32));
            blocks.Out(out);
          },
          fmt::format("count {} case {}", count, c));
    }
    results.Report();
  }
}

// --- The operation list's size ---

void IncSize(Context& h) {
  Results results("OpList::IncSize (sub_820AD768)");
  for (int c = 0; c < 100; c++) {
    Case(
        h, results,
        [&](Blocks&) {
          const uint32_t count = c % 4 == 0 ? 0 : h.random.Below(c % 3 == 0 ? 0x10000 : 8);
          const uint32_t room = as::kOpListMaxSize - count;
          // The size: small, or at the limit's edge (the original's
          // length_error is never taken).
          const uint32_t size = c % 5 == 0 ? room : c % 5 == 1 ? room - h.random.Below(16)
                                                               : h.random.Below(1000);
          Store(h.base, h.data + 8, size);
          Store(h.base, h.data + 64, count);
        },
        [&](PPCContext& t) {
          t.r3.u64 = h.data;
          t.r4.u64 = Load(h.base, h.data + 64);
        },
        __imp__sub_820AD768, hooks::AppOpIncSize,
        [&](Outputs& out, const PPCContext& t, const Blocks&) {
          out.Bytes(Read(h.base, h.data, 128));
          out.Word(t.r3.u32);
        },
        fmt::format("case {}", c));
  }
  results.Report();
}

// --- The lists ---

// A list at `list` of `count` nodes of `node_bytes` (from operator new),
// each with random values, its head node first in `blocks`; returns the
// nodes in order.
std::vector<uint32_t> BuildList(Context& h, Blocks& blocks, uint32_t list, uint32_t count,
                                uint32_t node_bytes) {
  const uint32_t head = blocks.New(h.base, node_bytes);
  std::vector<uint32_t> nodes;
  for (uint32_t i = 0; i < count; i++) {
    nodes.push_back(blocks.New(h.base, node_bytes));
  }
  std::vector<uint32_t> ring = {head};
  ring.insert(ring.end(), nodes.begin(), nodes.end());
  for (uint32_t i = 0; i < ring.size(); i++) {
    const uint32_t node = ring[i];
    Store(h.base, node + as::kNodeNext, ring[(i + 1) % ring.size()]);
    Store(h.base, node + as::kNodePrevious, ring[(i + ring.size() - 1) % ring.size()]);
    for (uint32_t at = 8; at < node_bytes; at += 4) {
      Store(h.base, node + at, uint32_t(h.random.Next()));
    }
  }
  Store(h.base, list + 0, uint32_t(h.random.Next()));
  Store(h.base, list + as::kListHead, head);
  Store(h.base, list + as::kListSize, count);
  return nodes;
}

// Each live block's words by number.
void LiveBlocks(uint8_t* base, Outputs& out, const Blocks& blocks, uint32_t bytes) {
  for (uint32_t block : blocks.list) {
    if (block != 0 && memory::IsHeapBlock(block)) {
      blocks.Words(base, out, block, bytes);
    }
  }
}

void Lists(Context& h) {
  {
    Results results("List::Clear (sub_820ACF88)");
    for (int c = 0; c < 100; c++) {
      const uint32_t node_bytes = c % 2 == 0 ? 16 : 12;
      Case(
          h, results,
          [&](Blocks& blocks) { BuildList(h, blocks, h.data, h.random.Below(12), node_bytes); },
          [&](PPCContext& t) { t.r3.u64 = h.data; }, __imp__sub_820ACF88, hooks::AppListClear,
          [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
            blocks.Words(h.base, out, h.data, 64);
            out.Word(blocks.Of(t.r3.u32));
            LiveBlocks(h.base, out, blocks, node_bytes);
            blocks.Out(out);
          },
          fmt::format("{}-byte nodes case {}", node_bytes, c));
    }
    results.Report();
  }
  {
    Results results("PtrList::Erase (sub_82098E80)");
    const uint32_t list = h.data;
    const uint32_t other = h.data + 16;
    const uint32_t result = h.data + 32;
    for (int c = 0; c < 200; c++) {
      uint32_t iterator_list = 0, node = 0;
      Case(
          h, results,
          [&](Blocks& blocks) {
            const std::vector<uint32_t> nodes =
                BuildList(h, blocks, list, 1 + h.random.Below(10), std_list::kNodeBytes);
            iterator_list = list;
            node = nodes[h.random.Below(uint32_t(nodes.size()))];
            if (c % 8 == 7) {
              // The iterator of another list at this list's head: unlinked
              // nothing (the original's check against r4's head).
              BuildList(h, blocks, other, h.random.Below(3), std_list::kNodeBytes);
              iterator_list = other;
              node = Load(h.base, list + as::kListHead);
            }
          },
          [&](PPCContext& t) {
            t.r3.u64 = result;
            t.r4.u64 = list;
            t.r5.u64 = (uint64_t(iterator_list) << 32) | node;
          },
          __imp__sub_82098E80, hooks::AppPtrListErase,
          [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
            blocks.Words(h.base, out, h.data, 64);
            out.Word(blocks.Of(t.r3.u32));
            LiveBlocks(h.base, out, blocks, std_list::kNodeBytes);
            blocks.Out(out);
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- The delegate's clone and the layer's emptiness ---

void Small(Context& h) {
  {
    Results results("Delegate::Clone (sub_820ACA48)");
    for (int c = 0; c < 50; c++) {
      Case(
          h, results,
          [&](Blocks&) {
            for (uint32_t at = 0; at < 16; at += 4) {
              Store(h.base, h.data + at, uint32_t(h.random.Next()));
            }
          },
          [&](PPCContext& t) { t.r3.u64 = h.data; }, __imp__sub_820ACA48, hooks::AppDelegateClone,
          [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
            blocks.Add(t.r3.u32);
            out.Word(blocks.Of(t.r3.u32));
            if (t.r3.u32 != 0) {
              out.Bytes(Read(h.base, t.r3.u32, as::kDelegateBytes));
            }
            out.Bytes(Read(h.base, h.data, 64));
            blocks.Out(out);
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("StateLayer::IsEmpty (sub_820ACE50)");
    for (int c = 0; c < 64; c++) {
      Case(
          h, results,
          [&](Blocks&) {
            for (uint32_t at = 0; at < 64; at += 4) {
              Store(h.base, h.data + at, uint32_t(h.random.Next()));
            }
            // The counts: 0 or not, in each combination.
            Store(h.base, h.data + 28, c & 1 ? 0 : uint32_t(h.random.Next()) | 1);
            Store(h.base, h.data + 16, c & 2 ? 0 : 1 + h.random.Below(5));
          },
          [&](PPCContext& t) { t.r3.u64 = h.data; }, __imp__sub_820ACE50, hooks::AppLayerIsEmpty,
          [&](Outputs& out, const PPCContext& t, const Blocks&) {
            out.Word(t.r3.u32);
            out.Bytes(Read(h.base, h.data, 64));
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- The operation list and the layer's push ---

// The nodes reachable from `head` (by +0 and +4) not yet in `blocks` added
// to it.
void MeetNodes(uint8_t* base, Blocks& blocks, uint32_t head) {
  std::vector<uint32_t> queue = {head};
  std::vector<uint32_t> seen;
  while (!queue.empty()) {
    const uint32_t node = queue.back();
    queue.pop_back();
    if (node == 0 || !memory::IsHeapBlock(node) ||
        std::find(seen.begin(), seen.end(), node) != seen.end()) {
      continue;
    }
    seen.push_back(node);
    if (std::find(blocks.list.begin(), blocks.list.end(), node) == blocks.list.end()) {
      blocks.Add(node);
    }
    queue.push_back(Load(base, node + as::kNodeNext));
    queue.push_back(Load(base, node + as::kNodePrevious));
  }
}

void OpLists(Context& h) {
  {
    Results results("OpList::OpList (sub_820AD0B8)");
    for (int c = 0; c < 30; c++) {
      Case(
          h, results, [&](Blocks&) {}, [&](PPCContext& t) { t.r3.u64 = h.data; },
          __imp__sub_820AD0B8, hooks::AppOpConstruct,
          [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
            blocks.Add(Load(h.base, h.data + as::kListHead));
            blocks.Words(h.base, out, h.data, 64);
            blocks.Words(h.base, out, blocks.list[0], 8);  // +8 to +15 never set
            out.Word(t.r3.u32);
            blocks.Out(out);
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("OpList::BuyNode (sub_820ACFF8)");
    for (int c = 0; c < 50; c++) {
      Case(
          h, results,
          [&](Blocks&) {
            for (uint32_t at = 0; at < 128; at += 4) {
              Store(h.base, h.data + at, uint32_t(h.random.Next()));
            }
          },
          [&](PPCContext& t) {
            t.r4.u64 = Load(h.base, h.data + 0);
            t.r5.u64 = Load(h.base, h.data + 4);
            t.r6.u64 = h.data + 64;
          },
          __imp__sub_820ACFF8, hooks::AppOpBuyNode,
          [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
            blocks.Add(t.r3.u32);
            out.Word(blocks.Of(t.r3.u32));
            if (t.r3.u32 != 0) {
              out.Bytes(Read(h.base, t.r3.u32, 16));
            }
            out.Bytes(Read(h.base, h.data, 128));
            blocks.Out(out);
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // A layer at data (48 bytes, random words), its operation list (+36)
    // holding 0 to 6 operations.
    Results results("StateLayer::Push (sub_820ADAD0)");
    for (int c = 0; c < 100; c++) {
      Case(
          h, results,
          [&](Blocks& blocks) {
            for (uint32_t at = 0; at < 64; at += 4) {
              Store(h.base, h.data + at, uint32_t(h.random.Next()));
            }
            BuildList(h, blocks, h.data + as::kLayerOps, h.random.Below(7), as::kOpNodeBytes);
          },
          [&](PPCContext& t) {
            t.r3.u64 = h.data;
            t.r4.u64 = uint32_t(h.random.Next());
          },
          __imp__sub_820ADAD0, hooks::AppLayerPush,
          [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
            MeetNodes(h.base, blocks, Load(h.base, h.data + as::kLayerOps + as::kListHead));
            blocks.Words(h.base, out, h.data, 64);
            LiveBlocks(h.base, out, blocks, as::kOpNodeBytes);
            out.Word(t.r3.u32);
            blocks.Out(out);
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- The message vector's growth ---

void MessageGrowth(Context& h) {
  const uint32_t vector = h.data;
  const uint32_t result = h.data + 16;
  const uint32_t value = h.data + 32;
  // A vector of `size` random words in a heap block with room for `spare`
  // more (none when both are 0, half the time); returns the element at
  // `index` (clamped to the size).
  auto build = [&](Blocks& blocks, uint32_t size, uint32_t spare, uint32_t index) {
    uint32_t block = 0;
    if (size + spare != 0 || h.random.Below(2) != 0) {
      block = blocks.New(h.base, 4 * (size + spare));
      for (uint32_t i = 0; i < size + spare; i++) {
        Store(h.base, block + 4 * i, uint32_t(h.random.Next()));
      }
    }
    Store(h.base, vector + as::kVectorFirst, block);
    Store(h.base, vector + as::kVectorLast, block + 4 * size);
    Store(h.base, vector + as::kVectorEnd, block + 4 * (size + spare));
    Store(h.base, value, uint32_t(h.random.Next()));
    return block + 4 * std::min(index, size);
  };
  // The vector by number, its block's words to its end, the blocks' state.
  auto collect = [&](Outputs& out, Blocks& blocks) {
    const uint32_t first = Load(h.base, vector + as::kVectorFirst);
    const uint32_t end = Load(h.base, vector + as::kVectorEnd);
    if (first != 0 && memory::IsHeapBlock(first)) {
      bool known = false;
      for (uint32_t b : blocks.list) {
        known = known || b == first;
      }
      if (!known) {
        blocks.Add(first);
      }
      if (end >= first && end - first <= 0x400) {
        // Only the words up to last are set (the rest heap leftovers).
        const uint32_t last = Load(h.base, vector + as::kVectorLast);
        blocks.Words(h.base, out, first, std::min(last, end) - first);
      }
    }
    blocks.Words(h.base, out, h.data, 48);
    blocks.Out(out);
  };
  {
    Results results("MessageVector::InsertN (sub_820938C0)");
    for (int c = 0; c < 300; c++) {
      uint32_t where = 0, count = 0;
      Case(
          h, results,
          [&](Blocks& blocks) {
            const uint32_t size = h.random.Below(c % 3 == 0 ? 3 : 20);
            count = h.random.Below(c % 4 == 0 ? 30 : 6);
            where = build(blocks, size, h.random.Below(c % 2 == 0 ? 4 : 40),
                          h.random.Below(size + 1));
          },
          [&](PPCContext& t) {
            Fill(h.base, t.r1.u32 + 24, 8);
            t.r3.u64 = vector;
            t.r4.u64 = (uint64_t(vector) << 32) | where;
            t.r5.u64 = count;
            t.r6.u64 = value;
          },
          __imp__sub_820938C0, hooks::AppMessagesInsertN,
          [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
            out.Bytes(Read(h.base, t.r1.u32 + 24, 8));
            collect(out, blocks);
          },
          fmt::format("count {} case {}", count, c));
    }
    results.Report();
  }
  {
    Results results("MessageVector::Insert (sub_82094110)");
    for (int c = 0; c < 200; c++) {
      uint32_t where = 0;
      Case(
          h, results,
          [&](Blocks& blocks) {
            const uint32_t size = h.random.Below(20);
            where = build(blocks, size, h.random.Below(3), h.random.Below(size + 1));
          },
          [&](PPCContext& t) {
            t.r3.u64 = result;
            t.r4.u64 = vector;
            t.r5.u64 = (uint64_t(vector) << 32) | where;
            t.r6.u64 = value;
          },
          __imp__sub_82094110, hooks::AppMessagesInsert,
          [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
            out.Word(t.r3.u32);
            collect(out, blocks);
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("MessageVector::PushBack (sub_820944E0)");
    for (int c = 0; c < 200; c++) {
      Case(
          h, results,
          [&](Blocks& blocks) {
            const uint32_t size = h.random.Below(20);
            build(blocks, size, h.random.Below(3), 0);
          },
          [&](PPCContext& t) {
            t.r3.u64 = vector;
            t.r4.u64 = value;
          },
          __imp__sub_820944E0, hooks::AppMessagesPushBack,
          [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
            out.Word(t.r3.u32 - t.r1.u32);
            collect(out, blocks);
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // The app's +20 interface at data - 56 + 0 (its message vector at +56 is
    // the vector at data).
    Results results("App::SendMessage (sub_82094AB0)");
    for (int c = 0; c < 100; c++) {
      Case(
          h, results,
          [&](Blocks& blocks) {
            const uint32_t size = h.random.Below(20);
            build(blocks, size, h.random.Below(3), 0);
          },
          [&](PPCContext& t) {
            t.r3.u64 = vector - as::kInterfaceMessages;
            t.r4.u64 = value;
          },
          __imp__sub_82094AB0, hooks::AppSendMessage,
          [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
            out.Word(t.r3.u32 - t.r1.u32);
            collect(out, blocks);
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- The app's running test ---
//
// The interface at data, its state stack at data + 256 (vtable at + 320),
// the root layer at data + 512 (its state's vtable at + 640), random words
// elsewhere. The slots are the game's in play (AppStates::IsEmpty,
// StateLayer::IsEmpty) or guest getters (sub_82090760, the byte +133;
// sub_82090770, the word +60) for the guest calls.

constexpr uint32_t kByte133 = 0x82090760;
constexpr uint32_t kWord60 = 0x82090770;

void Running(Context& h) {
  const uint32_t interface = h.data;
  const uint32_t states = h.data + 256;
  const uint32_t root = h.data + 512;
  auto build = [&](int c) {
    for (uint32_t at = 0; at < 1024; at += 4) {
      Store(h.base, h.data + at, uint32_t(h.random.Next()));
    }
    const uint32_t pick[] = {kByte133, kWord60};
    Store(h.base, interface + as::kInterfaceStates, states);
    Store(h.base, states + 0, states + 64);
    Store(h.base, states + 64 + 12, c % 4 == 3 ? pick[h.random.Below(2)] : as::kAppStatesIsEmpty);
    Store(h.base, states + as::kStatesRoot, root);
    Store(h.base, root + 4, root + 128);
    Store(h.base, root + 128 + 44, c % 3 == 2 ? pick[h.random.Below(2)] : as::kLayerIsEmpty);
    // The counts (the root's +20 and +32): 0 or not; the getters' byte and
    // word: 0 or not.
    Store(h.base, root + 20, h.random.Below(2) ? 0 : 1 + h.random.Below(3));
    Store(h.base, root + 32, h.random.Below(2) ? 0 : 1 + h.random.Below(3));
    guest::StoreU8(h.base, root + 4 + 133, h.random.Below(2) ? 0 : uint8_t(h.random.Next()));
    guest::StoreU8(h.base, states + 133, h.random.Below(2) ? 0 : uint8_t(h.random.Next()));
    if (h.random.Below(2)) {
      Store(h.base, root + 4 + 60, Load(h.base, root + 4 + 60) & 0xFFFFFF00);
      Store(h.base, states + 60, Load(h.base, states + 60) & 0xFFFFFF00);
    }
  };
  {
    Results results("AppStates::IsEmpty (sub_820AE1A0)");
    for (int c = 0; c < 100; c++) {
      Case(
          h, results, [&](Blocks&) { build(c); }, [&](PPCContext& t) { t.r3.u64 = states; },
          __imp__sub_820AE1A0, hooks::AppStatesIsEmpty,
          [&](Outputs& out, const PPCContext& t, const Blocks&) {
            out.Word(t.r3.u32);
            out.Bytes(Read(h.base, h.data, 1024));
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("App::IsRunning (sub_82092928)");
    for (int c = 0; c < 100; c++) {
      Case(
          h, results, [&](Blocks&) { build(c); }, [&](PPCContext& t) { t.r3.u64 = interface; },
          __imp__sub_82092928, hooks::AppIsRunning,
          [&](Outputs& out, const PPCContext& t, const Blocks&) {
            out.Word(t.r3.u32);
            out.Word(t.r11.u32);
            out.Bytes(Read(h.base, h.data, 1024));
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- The app's walks, handlers and requests ---
//
// Objects in the scratch area whose vtables point at small guest functions:
// sub_8245F860 adds one to the word at r3 + 4 (a counter), sub_820A70A8
// stores r4 at r3 + 4 (a recorder), sub_82090770 answers the word at r3 +
// 60 (a getter). The functors are those, or the game's own (App::RunHandler,
// RunListener, DeliverMessage, ApplyRequest) over delegates whose slot 1 is
// the counter. The handler maps are built by the game's HandlerMap::HandlerMap
// and Insert (sub_82099FB0, sub_82093740) and walked from their heads, every
// node met numbered.

constexpr uint32_t kCount4 = 0x8245F860;
constexpr uint32_t kStoreR4At4 = 0x820A70A8;

// The app at data + 1024 (its maps at +28 to +64, its vectors at +76 and
// +96), objects at data + 2048 + 64 x i (their vtables at their + 32, every
// slot `target`).
uint32_t AppAt(const Context& h) { return h.data + 1024; }
uint32_t ObjectAt(const Context& h, uint32_t i) { return h.data + 2048 + 64 * i; }
void Objects(Context& h, uint32_t count, uint32_t target) {
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t object = ObjectAt(h, i);
    Store(h.base, object, object + 32);
    Store(h.base, object + 4, 0);
    for (uint32_t slot = 0; slot < 8; slot++) {
      Store(h.base, object + 32 + 4 * slot, target);
    }
  }
}

// A handler map at `map` (the game's functions) with `count` random keys,
// each with a delegate: one of the objects (`objects` of them) or a random
// word when 0.
void BuildHandlerMap(Context& h, uint32_t map, uint32_t count, uint32_t objects) {
  PPCContext t = CallContext(h);
  t.r3.u64 = map;
  __imp__sub_82099FB0(t, h.base);
  const uint32_t value = h.data + 4000;
  const uint32_t result = h.data + 4016;
  const bool small = h.random.Below(2) != 0;
  for (uint32_t i = 0; i < count; i++) {
    Store(h.base, value, small ? h.random.Below(24) : uint32_t(h.random.Next()));
    Store(h.base, value + 4,
          objects != 0 ? ObjectAt(h, h.random.Below(objects)) : uint32_t(h.random.Next()));
    t = CallContext(h);
    t.r3.u64 = result;
    t.r4.u64 = map;
    t.r5.u64 = value;
    __imp__sub_82093740(t, h.base);
  }
  Fill(h.base, value, 32);
}

// A map's nodes in key order (its head last).
std::vector<uint32_t> MapInOrder(uint8_t* base, uint32_t map) {
  const uint32_t head = Load(base, map + 4);
  auto nil = [&](uint32_t node) { return guest::LoadU8(base, node + 21) != 0; };
  std::vector<uint32_t> nodes, stack;
  uint32_t node = Load(base, head + 4);
  while (!nil(node) || !stack.empty()) {
    while (!nil(node)) {
      stack.push_back(node);
      node = Load(base, node + 0);
    }
    node = stack.back();
    stack.pop_back();
    nodes.push_back(node);
    node = Load(base, node + 8);
  }
  nodes.push_back(head);
  return nodes;
}

// The tree's nodes from each map's head (left, parent, right) added to
// `blocks`.
void MeetTree(uint8_t* base, Blocks& blocks, const std::vector<uint32_t>& maps) {
  std::vector<uint32_t> queue;
  for (uint32_t map : maps) {
    queue.push_back(Load(base, map + 4));
  }
  while (!queue.empty()) {
    const uint32_t node = queue.back();
    queue.pop_back();
    if (node == 0 || !memory::IsHeapBlock(node) ||
        std::find(blocks.list.begin(), blocks.list.end(), node) != blocks.list.end()) {
      continue;
    }
    blocks.Add(node);
    for (uint32_t at : {0u, 4u, 8u}) {
      queue.push_back(Load(base, node + at));
    }
  }
}

// The scratch area's first 3072 bytes by number, the tree nodes' words (24
// bytes: links, key, delegate; the color and isnil bytes) by number, r3.
void AppOut(Context& h, Outputs& out, Blocks& blocks, const std::vector<uint32_t>& maps,
            uint32_t r3) {
  MeetTree(h.base, blocks, maps);
  blocks.Words(h.base, out, h.data, 3072);
  for (uint32_t block : blocks.list) {
    if (block != 0 && memory::IsHeapBlock(block) && memory::Size(block) == 24) {
      blocks.Words(h.base, out, block, 12);
      if (guest::LoadU8(h.base, block + 21) == 0) {
        blocks.Words(h.base, out, block + 12, 8);
      }
      out.Bytes(Read(h.base, block + 20, 2));
    }
  }
  out.Word(blocks.Of(r3));
  blocks.Out(out);
}

std::vector<uint32_t> AppMaps(const Context& h) {
  std::vector<uint32_t> maps;
  for (uint32_t at : as::kAppMaps) {
    maps.push_back(AppAt(h) + at);
  }
  return maps;
}

void Walks(Context& h) {
  const uint32_t app = AppAt(h);
  const uint32_t result = h.data + 64;
  {
    Results results("HandlerMap::ForEach (sub_82092BF0)");
    for (int c = 0; c < 200; c++) {
      // 0: the counter, 1: the recorder, 2: App::RunHandler, 3: RunListener.
      const int kind = c % 4;
      uint64_t first = 0, last = 0, functor = 0, adjust = 0;
      Case(
          h, results,
          [&](Blocks&) {
            for (uint32_t at = 0; at < 3072; at += 4) {
              Store(h.base, h.data + at, uint32_t(h.random.Next()));
            }
            Objects(h, 6, kind == 1 ? kStoreR4At4 : kCount4);
            const uint32_t map = app + as::kAppMaps[h.random.Below(4)];
            BuildHandlerMap(h, map, h.random.Below(12), kind >= 2 ? 5 : 0);
            const std::vector<uint32_t> nodes = MapInOrder(h.base, map);
            const uint32_t i = h.random.Below(uint32_t(nodes.size()));
            const uint32_t j = i + h.random.Below(uint32_t(nodes.size()) - i);
            first = (uint64_t(map) << 32) | nodes[i];
            last = (uint64_t(map) << 32) | nodes[j];
            const uint32_t function = kind == 0 ? kCount4 : kind == 1 ? kStoreR4At4
                                      : kind == 2 ? as::kRunHandler : as::kRunListener;
            // The object and the adjustment: an object, or the app as its
            // adjustment (the game's functors).
            const uint32_t object = kind >= 2 ? 0 : ObjectAt(h, 5);
            const uint32_t add = kind >= 2 ? app : 0;
            functor = (uint64_t(function) << 32) | object;
            adjust = (uint64_t(add) << 32) | uint32_t(h.random.Next());
          },
          [&](PPCContext& t) {
            Fill(h.base, t.r1.u32 + 48, 16);
            t.r3.u64 = result;
            t.r4.u64 = first;
            t.r5.u64 = last;
            t.r6.u64 = functor;
            t.r7.u64 = adjust;
          },
          __imp__sub_82092BF0, hooks::AppHandlerMapForEach,
          [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
            MeetTree(h.base, blocks, AppMaps(h));
            blocks.Words(h.base, out, t.r1.u32 + 48, 16);
            AppOut(h, out, blocks, AppMaps(h), t.r3.u32);
          },
          fmt::format("functor {} case {}", kind, c));
    }
    results.Report();
  }
  // The vectors' walks: the words (or requests) in the scratch area at data
  // + 128.
  struct VectorWalk {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
    uint32_t step;
    uint32_t game;  // the game's functor
  };
  const VectorWalk walks[] = {
      {"App::ForEachMessage (sub_82092CD0)", __imp__sub_82092CD0, hooks::AppForEachMessage, 4,
       as::kDeliverMessage},
      {"App::ForEachRequest (sub_82091C48)", __imp__sub_82091C48, hooks::AppForEachRequest, 12,
       as::kApplyRequest},
  };
  for (const VectorWalk& w : walks) {
    Results results(w.name);
    for (int c = 0; c < 150; c++) {
      const int kind = c % 3;  // 0: the counter, 1: the recorder, 2: the game's
      uint64_t first = 0, last = 0, functor = 0, adjust = 0;
      Case(
          h, results,
          [&](Blocks&) {
            for (uint32_t at = 0; at < 3072; at += 4) {
              Store(h.base, h.data + at, uint32_t(h.random.Next()));
            }
            Objects(h, 6, kind == 1 ? kStoreR4At4 : kCount4);
            for (uint32_t at : as::kAppMaps) {
              BuildHandlerMap(h, app + at, kind == 2 ? h.random.Below(5) : 0, 5);
            }
            const uint32_t count = h.random.Below(10);
            const uint32_t elements = h.data + 128;
            for (uint32_t i = 0; i < count; i++) {
              if (w.step == 12) {
                // The type: an insert (0, 2, 4, 6) or nothing (above 7).
                const uint32_t types[] = {0, 2, 4, 6, 9};
                Store(h.base, elements + 12 * i, types[h.random.Below(5)]);
                Store(h.base, elements + 12 * i + 4, h.random.Below(24));
                Store(h.base, elements + 12 * i + 8, ObjectAt(h, h.random.Below(5)));
              } else {
                Store(h.base, elements + 4 * i, h.random.Below(8));
              }
            }
            const uint32_t vector = h.data + 112;
            first = (uint64_t(vector) << 32) | elements;
            last = (uint64_t(vector) << 32) | (elements + w.step * count);
            const uint32_t function = kind == 0 ? kCount4 : kind == 1 ? kStoreR4At4 : w.game;
            const uint32_t object = kind == 2 ? 0 : ObjectAt(h, 5);
            const uint32_t add = kind == 2 ? app : 0;
            functor = (uint64_t(function) << 32) | object;
            adjust = (uint64_t(add) << 32) | uint32_t(h.random.Next());
          },
          [&](PPCContext& t) {
            Fill(h.base, t.r1.u32 + 24, 8);
            Fill(h.base, t.r1.u32 + 48, 16);
            t.r3.u64 = result;
            t.r4.u64 = first;
            t.r5.u64 = last;
            t.r6.u64 = functor;
            t.r7.u64 = adjust;
          },
          w.recompiled, w.native,
          [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
            MeetTree(h.base, blocks, AppMaps(h));
            blocks.Words(h.base, out, t.r1.u32 + 24, 8);
            blocks.Words(h.base, out, t.r1.u32 + 48, 16);
            AppOut(h, out, blocks, AppMaps(h), t.r3.u32);
          },
          fmt::format("functor {} case {}", kind, c));
    }
    results.Report();
  }
}

void Handlers(Context& h) {
  const uint32_t app = AppAt(h);
  struct Runner {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
  };
  const Runner runners[] = {
      {"App::RunHandler (sub_82092968)", __imp__sub_82092968, hooks::AppRunHandler},
      {"App::RunListener (sub_820929B0)", __imp__sub_820929B0, hooks::AppRunListener},
  };
  for (const Runner& r : runners) {
    Results results(r.name);
    for (int c = 0; c < 60; c++) {
      Case(
          h, results,
          [&](Blocks&) {
            for (uint32_t at = 0; at < 3072; at += 4) {
              Store(h.base, h.data + at, uint32_t(h.random.Next()));
            }
            Objects(h, 1, c % 2 == 0 ? kCount4 : kStoreR4At4);
          },
          [&](PPCContext& t) {
            Fill(h.base, t.r1.u32 + 24, 8);
            t.r3.u64 = c % 7 == 6 ? 0 : app;
            t.r4.u64 = (h.random.Next() << 32) | ObjectAt(h, 0);
            t.r5.u64 = h.random.Next();
          },
          r.recompiled, r.native,
          [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
            blocks.Words(h.base, out, t.r1.u32 + 24, 8);
            out.Word(t.r4.u32);
            out.Word(t.r5.u32);
            AppOut(h, out, blocks, {}, t.r3.u32);
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("App::DeliverMessage (sub_82092D50)");
    for (int c = 0; c < 60; c++) {
      Case(
          h, results,
          [&](Blocks&) {
            for (uint32_t at = 0; at < 3072; at += 4) {
              Store(h.base, h.data + at, uint32_t(h.random.Next()));
            }
            Objects(h, 6, kCount4);
            BuildHandlerMap(h, app + as::kAppMaps[3], h.random.Below(8), 5);
          },
          [&](PPCContext& t) {
            t.r3.u64 = app;
            t.r4.u64 = h.random.Below(8);
          },
          __imp__sub_82092D50, hooks::AppDeliverMessage,
          [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
            out.Word(t.r3.u32 - t.r1.u32);
            AppOut(h, out, blocks, AppMaps(h), 0);
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("App::ApplyRequest (sub_82093BE0)");
    for (int c = 0; c < 200; c++) {
      uint32_t type = 0;
      Case(
          h, results,
          [&](Blocks&) {
            for (uint32_t at = 0; at < 3072; at += 4) {
              Store(h.base, h.data + at, uint32_t(h.random.Next()));
            }
            for (uint32_t at : as::kAppMaps) {
              BuildHandlerMap(h, app + at, h.random.Below(6), 0);
            }
            const uint32_t types[] = {0, 2, 4, 6, 8, 0xFFFFFFFF};
            type = types[h.random.Below(6)];
          },
          [&](PPCContext& t) {
            t.r3.u64 = app;
            t.r4.u64 = (uint64_t(type) << 32) | h.random.Below(24);
            t.r5.u64 = h.random.Next();
          },
          __imp__sub_82093BE0, hooks::AppApplyRequest,
          [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
            out.Word(t.r3.u32 - t.r1.u32);
            AppOut(h, out, blocks, AppMaps(h), 0);
          },
          fmt::format("type {} case {}", int32_t(type), c));
    }
    results.Report();
  }
  // The add slots: the interface at app + 20 (its request vector at +76, in
  // a heap block), the key at data + 96, the delegate an object whose slot 2
  // is the game's Delegate::Clone or the getter (a word for a copy).
  struct Slot {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
  };
  const Slot slots[] = {
      {"App::AddHandler (sub_82094830)", __imp__sub_82094830, hooks::AppAddHandler},
      {"App::AddToggledHandler (sub_820948D0)", __imp__sub_820948D0, hooks::AppAddToggledHandler},
      {"App::AddAlwaysHandler (sub_82094970)", __imp__sub_82094970, hooks::AppAddAlwaysHandler},
      {"App::AddListener (sub_82094A10)", __imp__sub_82094A10, hooks::AppAddListener},
  };
  for (const Slot& slot : slots) {
    Results results(slot.name);
    for (int c = 0; c < 60; c++) {
      Case(
          h, results,
          [&](Blocks& blocks) {
            for (uint32_t at = 0; at < 3072; at += 4) {
              Store(h.base, h.data + at, uint32_t(h.random.Next()));
            }
            Objects(h, 1, c % 4 == 3 ? 0x82090770 : 0x820ACA48);  // the getter, or Delegate::Clone
            const uint32_t size = h.random.Below(5);
            const uint32_t capacity = size + h.random.Below(3);
            const uint32_t vector = app + as::kAppRequests;
            uint32_t block = 0;
            if (capacity != 0) {
              block = blocks.New(h.base, 12 * capacity);
              for (uint32_t at = 0; at < 12 * capacity; at += 4) {
                Store(h.base, block + at, uint32_t(h.random.Next()));
              }
            }
            Store(h.base, vector + 4, block);
            Store(h.base, vector + 8, block + 12 * size);
            Store(h.base, vector + 12, block + 12 * capacity);
          },
          [&](PPCContext& t) {
            t.r3.u64 = app + as::kAppInterface;
            t.r4.u64 = h.data + 96;
            t.r5.u64 = ObjectAt(h, 0);
          },
          slot.recompiled, slot.native,
          [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
            const uint32_t vector = app + as::kAppRequests;
            const uint32_t first = Load(h.base, vector + 4);
            const uint32_t last = Load(h.base, vector + 8);
            if (first != 0 && std::find(blocks.list.begin(), blocks.list.end(), first) ==
                                  blocks.list.end()) {
              blocks.Add(first);
            }
            // The copies (heap blocks of 12 bytes) the requests hold.
            for (uint32_t at = first; first != 0 && at < last && at - first < 0x400; at += 12) {
              const uint32_t copy = Load(h.base, at + 8);
              if (copy != 0 && memory::IsHeapBlock(copy) &&
                  std::find(blocks.list.begin(), blocks.list.end(), copy) == blocks.list.end()) {
                blocks.Add(copy);
                blocks.Words(h.base, out, copy, 12);
              }
            }
            if (first != 0 && last >= first && last - first < 0x400) {
              blocks.Words(h.base, out, first, last - first);
            }
            out.Word(t.r3.u32 == app + as::kAppRequests ? 1 : t.r3.u32 - t.r1.u32);
            AppOut(h, out, blocks, {}, 0);
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- The app's update ---
//
// The app at data + 1024: its +24 source and the manager are objects whose
// slot 1 is the getter of their word +60 (the manager, then player 1's
// processor at data + 3072, random words with Start and Back set or not in
// its pressed and held words); the +20 interface's slot +40 the game's
// App::SendMessage (the message vector in a heap block); the handler maps
// with delegates whose slot 1 is the counter; requests to apply (types 0,
// 2, 4, 6 and 9) in a heap block; the toggle and the step 0 or 1.

void Update(Context& h) {
  const uint32_t app = AppAt(h);
  const uint32_t processor = h.data + 3072;
  const uint32_t source = h.data + 3400;
  const uint32_t manager = h.data + 3480;
  const uint32_t vtable = h.data + 3600;
  Results results("App::Update (sub_82093DC0)");
  for (int c = 0; c < 200; c++) {
    Case(
        h, results,
        [&](Blocks& blocks) {
          for (uint32_t at = 0; at < 3800; at += 4) {
            Store(h.base, h.data + at, uint32_t(h.random.Next()));
          }
          Objects(h, 6, kCount4);
          for (uint32_t at : as::kAppMaps) {
            BuildHandlerMap(h, app + at, h.random.Below(5), 5);
          }
          Store(h.base, app + 24, source);
          Store(h.base, source, source + 16);
          Store(h.base, source + 20, 0x82090770);
          Store(h.base, source + 60, manager);
          Store(h.base, manager, manager + 16);
          Store(h.base, manager + 20, 0x82090770);
          Store(h.base, manager + 60, processor);
          // Start (0x100000) and Back (0x200000) pressed and held, or not.
          for (uint32_t at : {264u, 268u, 272u, 276u}) {
            uint32_t word = uint32_t(h.random.Next()) & ~0x300000u;
            word |= h.random.Below(2) ? 0x100000 : 0;
            word |= h.random.Below(2) ? 0x200000 : 0;
            Store(h.base, processor + at, word);
          }
          Store(h.base, app + as::kAppInterface, vtable);
          Store(h.base, vtable + 40, 0x82094AB0);  // App::SendMessage
          guest::StoreU8(h.base, app + 112, uint8_t(h.random.Below(2)));
          guest::StoreU8(h.base, app + 113, uint8_t(h.random.Below(2)));
          // The message vector: empty, or a few messages with room or not.
          const uint32_t messages = app + as::kAppMessages;
          const uint32_t count = h.random.Below(3);
          const uint32_t capacity = count + h.random.Below(3);
          const uint32_t block = capacity != 0 ? blocks.New(h.base, 4 * capacity) : 0;
          for (uint32_t i = 0; i < capacity; i++) {
            Store(h.base, block + 4 * i, h.random.Below(4));
          }
          Store(h.base, messages + 4, block);
          Store(h.base, messages + 8, block + 4 * count);
          Store(h.base, messages + 12, block + 4 * capacity);
          // The requests.
          const uint32_t requests = app + as::kAppRequests;
          const uint32_t pending = h.random.Below(4);
          const uint32_t room = pending + h.random.Below(2);
          const uint32_t list = room != 0 ? blocks.New(h.base, 12 * room) : 0;
          const uint32_t types[] = {0, 2, 4, 6, 9};
          for (uint32_t i = 0; i < room; i++) {
            Store(h.base, list + 12 * i, types[h.random.Below(5)]);
            Store(h.base, list + 12 * i + 4, h.random.Below(24));
            Store(h.base, list + 12 * i + 8, ObjectAt(h, h.random.Below(5)));
          }
          Store(h.base, requests + 4, list);
          Store(h.base, requests + 8, list + 12 * pending);
          Store(h.base, requests + 12, list + 12 * room);
        },
        [&](PPCContext& t) { t.r3.u64 = app; }, __imp__sub_82093DC0, hooks::AppUpdate,
        [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
          // The vectors' blocks (old and new) by number, their words to last.
          for (uint32_t vector : {app + as::kAppMessages, app + as::kAppRequests}) {
            const uint32_t first = Load(h.base, vector + 4);
            const uint32_t last = Load(h.base, vector + 8);
            if (first != 0 && memory::IsHeapBlock(first) &&
                std::find(blocks.list.begin(), blocks.list.end(), first) == blocks.list.end()) {
              blocks.Add(first);
            }
            if (first != 0 && last >= first && last - first < 0x400) {
              blocks.Words(h.base, out, first, last - first);
            }
          }
          out.Word(t.r3.u32 - t.r1.u32);
          AppOut(h, out, blocks, AppMaps(h), 0);
        },
        fmt::format("case {}", c));
  }
  results.Report();
}

}  // namespace

void AppStates(Context& h) {
  MessageVector(h);
  Allocators(h);
  IncSize(h);
  Lists(h);
  Small(h);
  OpLists(h);
  MessageGrowth(h);
  Running(h);
  Walks(h);
  Handlers(h);
  Update(h);
}

}  // namespace harness
