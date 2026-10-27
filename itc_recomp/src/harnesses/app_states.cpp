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
  uint32_t Add(uint32_t block) {
    list.push_back(block);
    return block;
  }
  uint32_t New(uint8_t* base, uint32_t bytes) {
    const auto block = memory::crt::OperatorNew(base, bytes);
    return Add(block ? *block : 0);
  }
  // A word into a block as 0xB1000000 + 0x1000 x its number + the offset;
  // others as they are.
  uint32_t Of(uint32_t word) const {
    for (uint32_t i = 0; i < list.size(); i++) {
      if (list[i] != 0 && word >= list[i] && word - list[i] < 0x400) {
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

}  // namespace

void AppStates(Context& h) {
  MessageVector(h);
  Allocators(h);
  IncSize(h);
  Lists(h);
  Small(h);
}

}  // namespace harness
