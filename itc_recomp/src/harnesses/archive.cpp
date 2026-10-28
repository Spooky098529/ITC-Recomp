// The archive's functions (src/archive/, docs/functions/archive.md) against
// the recompiled ones. The TOC lookups read the game's own TOCs (only read:
// the loader thread may be using them), with ids across both archives and
// their edges; the loose files' lookup the image's tables. The list's
// functions run on lists made in the scratch area. The slot lookup reads the
// game's slots, and the static set-ups write the game's queue and slots (put
// back after each side), both under the game's queue lock (the Semaphore at
// 0x826BAAE0, waited and signaled through the translated code, as the
// queueing and the picker take it), so no request is queued or picked
// meanwhile; the set-ups' clean-ups are taken back off the exit table
// (exit_table.h). The originals run with --native_archive off, so their
// callees are the recompiled code too.

#include "harness.h"
#include "exit_table.h"

#include "archive/archive.h"
#include "base/flags.h"
#include "base/log.h"
#include "hooks/c_runtime.h"

#include <fmt/format.h>

#include <mutex>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_archive);

namespace harness {

namespace {

// The register steps: the cases run the plain C++ through these (not the
// adapters), with the registers each adapter leaves.
#define ITC_NATIVE_STEP(name, body)           \
  void name(PPCContext& ctx, uint8_t* base) { \
    body;                                     \
  }

ITC_NATIVE_STEP(NativeToc, ctx.r3.u64 = archive::Toc(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeAssetSize, ctx.r3.u64 = archive::AssetSize(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeListConstruct, archive::ListConstruct(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeListAppend,
                ctx.r3.u64 = archive::ListAppend(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeListRemove,
                ctx.r3.u64 = archive::ListRemove(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeNodeConstruct, archive::NodeConstruct(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeFindSlot, ctx.r3.s64 = int32_t(archive::FindSlot(base, ctx.r3.u32)))
ITC_NATIVE_STEP(NativeFindLooseFile, ctx.r3.u64 = archive::FindLooseFile(base, ctx.r3.u32))

void NativeQueueStaticInit(PPCContext& ctx, uint8_t* base) {
  hooks::GuestAtexit guest{ctx, base};
  ctx.r3.s64 = archive::QueueStaticInit(base, guest.Call());
}
void NativeSlotsStaticInit(PPCContext& ctx, uint8_t* base) {
  hooks::GuestAtexit guest{ctx, base};
  ctx.r3.s64 = archive::SlotsStaticInit(base, guest.Call());
}

#undef ITC_NATIVE_STEP

// One case through both versions: `setup()` writes the inputs into the
// scratch area (filled first), `args(t)` sets the registers, the original
// (flag off) or the native step runs, and `outputs(t, out)` adds what's
// compared (r3 always, exactly).
template <typename Setup, typename Args, typename Out>
void Case(Context& h, Results& results, Setup setup, Args args, GuestFunction recompiled,
          GuestFunction native, Out outputs, const std::string& what) {
  Outputs out[2];
  const Random start = h.random;
  Random after;
  for (int v = 0; v < 2; v++) {
    h.random = start;
    Fill(h.base, h.data, kScratchBytes);
    setup();
    PPCContext t = CallContext(h);
    args(t);
    ITC_FLAG(native_archive) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_archive) = true;
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
    outputs(t, out[v]);
    after = h.random;
  }
  h.random = after;
  results.Check(out[0], out[1], what);
}

auto NoSetup = [] {};
auto NoOutputs = [](const PPCContext&, Outputs&) {};

// An id in the archives' tables: often an edge, else random in either.
uint32_t TableId(Random& r) {
  const uint32_t crs_count = 1359;
  static const uint32_t kEdges[] = {0, 1, archive::kFirstCrsId - 1, archive::kFirstCrsId,
                                    archive::kFirstCrsId + 1,
                                    archive::kFirstCrsId + crs_count - 1, 22887, 22888, 22889};
  switch (r.Below(4)) {
    case 0:
      return kEdges[r.Below(uint32_t(sizeof(kEdges) / sizeof(kEdges[0])))];
    case 1:
      return archive::kFirstCrsId + r.Below(crs_count);
    default:
      return r.Below(archive::kFirstCrsId);
  }
}

void Tocs(Context& h) {
  {
    Results results("sub_8209D868 Archive::Toc");
    for (int i = 0; i < 300; i++) {
      // Any id (only the two pointers are read), the edges and the tables'.
      uint32_t id = i < 100 ? TableId(h.random) : uint32_t(h.random.Next());
      if (i >= 280) {
        id = 0xFFFFFFFFu - uint32_t(i - 280);
      }
      Case(
          h, results, NoSetup,
          [&](PPCContext& t) {
            t.r3.u64 = (h.random.Next() << 32) | id;  // the upper half: not read
          },
          __imp__sub_8209D868, NativeToc, NoOutputs, fmt::format("case {} (id {})", i, id));
    }
    results.Report();
  }
  {
    Results results("sub_820A0678 Archive::AssetSize");
    for (int i = 0; i < 400; i++) {
      const uint32_t id = TableId(h.random);
      Case(
          h, results, NoSetup, [&](PPCContext& t) { t.r3.u64 = id; }, __imp__sub_820A0678,
          NativeAssetSize, NoOutputs, fmt::format("case {} (id {})", i, id));
    }
    results.Report();
  }
}

// The game's queue lock, held while alive (Semaphore::Wait and Signal,
// sub_82592670 and sub_825926E8, through the translated code).
constexpr uint32_t kQueueLock = 0x826BAAE0;
struct QueueLock {
  Context& h;
  explicit QueueLock(Context& h_) : h(h_) {
    PPCContext t = CallContext(h);
    t.r3.u64 = kQueueLock;
    sub_82592670(t, h.base);
  }
  ~QueueLock() {
    PPCContext t = CallContext(h);
    t.r3.u64 = kQueueLock;
    sub_825926E8(t, h.base);
  }
};

// The list's cases: a list at the area's start (+0 to +20) and up to 24
// nodes after it (16 bytes apart from +64), linked in a random order; the
// whole area is compared.
constexpr uint32_t kNodes = 64;
constexpr uint32_t kMaxNodes = 24;

uint32_t Node(Context& h, uint32_t i) {
  return h.data + kNodes + 16 * i;
}

// A list of `count` of the nodes in a random order, its link words as made
// (the last 0), the count word `count` or random, the tail the last node or,
// sometimes, another node or 0.
void MakeList(Context& h, uint32_t count, std::vector<uint32_t>& order) {
  order.clear();
  for (uint32_t i = 0; i < kMaxNodes; i++) {
    order.push_back(i);
  }
  for (uint32_t i = kMaxNodes - 1; i > 0; i--) {
    std::swap(order[i], order[h.random.Below(i + 1)]);
  }
  order.resize(count);
  const uint32_t list = h.data;
  guest::StoreU32(h.base, list + 0, uint32_t(h.random.Next()));
  guest::StoreU32(h.base, list + 4, uint32_t(h.random.Next()));
  guest::StoreU32(h.base, list + 8, h.random.Below(4) ? count : uint32_t(h.random.Next()));
  guest::StoreU32(h.base, list + 12, count ? Node(h, order[0]) : 0);
  uint32_t tail = count ? Node(h, order[count - 1]) : 0;
  switch (h.random.Below(8)) {
    case 0:
      tail = Node(h, h.random.Below(kMaxNodes));
      break;
    case 1:
      tail = 0;
      break;
    default:
      break;
  }
  guest::StoreU32(h.base, list + 16, tail);
  for (uint32_t i = 0; i < count; i++) {
    guest::StoreU32(h.base, Node(h, order[i]) + 4, i + 1 < count ? Node(h, order[i + 1]) : 0);
  }
}

auto AreaOutputs(Context& h) {
  return [&h](const PPCContext&, Outputs& out) { out.Bytes(Read(h.base, h.data, kScratchBytes)); };
}

void Lists(Context& h) {
  std::vector<uint32_t> order;
  {
    Results results("sub_82294B80 List::List");
    for (int i = 0; i < 40; i++) {
      Case(
          h, results, NoSetup,
          [&](PPCContext& t) {
            t.r3.u64 = h.data + 4 * h.random.Below(32);
            t.r4.u64 = i < 10 ? uint32_t(i & 1) : h.random.Next();
          },
          __imp__sub_82294B80, NativeListConstruct, AreaOutputs(h), fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("sub_82294B68 ListNode::ListNode");
    for (int i = 0; i < 40; i++) {
      Case(
          h, results, NoSetup,
          [&](PPCContext& t) {
            t.r3.u64 = h.data + 4 * h.random.Below(32);
            t.r4.u64 = i < 10 ? 0 : h.random.Next();
          },
          __imp__sub_82294B68, NativeNodeConstruct, AreaOutputs(h), fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("sub_82294BA8 List::Append");
    for (int i = 0; i < 300; i++) {
      Case(
          h, results, [&] { MakeList(h, h.random.Below(kMaxNodes), order); },
          [&](PPCContext& t) {
            // One of two nodes (listed or not: Append walks nothing), or 0.
            t.r4.u64 = i % 10 == 0 ? 0 : Node(h, kMaxNodes - 1 - h.random.Below(2));
            t.r3.u64 = h.data;
          },
          __imp__sub_82294BA8, NativeListAppend, AreaOutputs(h),
          fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("sub_82294C00 List::Remove");
    for (int i = 0; i < 400; i++) {
      Case(
          h, results, [&] { MakeList(h, h.random.Below(kMaxNodes + 1), order); },
          [&](PPCContext& t) {
            t.r3.u64 = h.data;
            // A listed node (often the head or the tail), one not listed, or 0.
            switch (h.random.Below(8)) {
              case 0:
                t.r4.u64 = 0;
                break;
              case 1:
                t.r4.u64 = Node(h, h.random.Below(kMaxNodes));
                break;
              case 2:
                t.r4.u64 = order.empty() ? h.data + 2048 : Node(h, order[0]);
                break;
              case 3:
                t.r4.u64 = order.empty() ? h.data + 2048 : Node(h, order.back());
                break;
              default:
                t.r4.u64 = order.empty() ? h.data + 2048
                                         : Node(h, order[h.random.Below(uint32_t(order.size()))]);
                break;
            }
          },
          __imp__sub_82294C00, NativeListRemove, AreaOutputs(h), fmt::format("case {}", i));
    }
    results.Report();
  }
}

void Slots(Context& h) {
  Results results("sub_8209D818 the slot lookup");
  QueueLock lock(h);
  // The serials in the slots now (some), -1, and others.
  std::vector<uint32_t> serials;
  for (uint32_t i = 0; i < archive::kSlotCount; i++) {
    const uint32_t serial = guest::LoadU32(h.base, archive::kSlots + i * archive::kSlotBytes + 8);
    if (serial != 0xFFFFFFFFu) {
      serials.push_back(serial);
    }
  }
  for (int i = 0; i < 120; i++) {
    uint32_t serial;
    if (i < 20) {
      serial = 0xFFFFFFFFu;
    } else if (i < 60 && !serials.empty()) {
      serial = serials[h.random.Below(uint32_t(serials.size()))];
    } else {
      serial = i < 80 ? h.random.Below(4096) : uint32_t(h.random.Next());
    }
    Case(
        h, results, NoSetup,
        [&](PPCContext& t) { t.r3.u64 = (h.random.Next() << 32) | serial; }, __imp__sub_8209D818,
        NativeFindSlot, NoOutputs, fmt::format("case {} (serial {:08X})", i, serial));
  }
  results.Report();
}

void LooseFiles(Context& h) {
  Results results("sub_8209D8D8 Archive::FindLooseFile");
  // The tables' ids, then others.
  std::vector<uint32_t> ids;
  for (uint32_t e = archive::kLooseFiles; guest::LoadU32(h.base, e + 4) != 0; e += 8) {
    ids.push_back(guest::LoadU32(h.base, e));
  }
  ITC_LOG_INFO("harness: archive: {} loose files", ids.size());
  for (int i = 0; i < 150; i++) {
    uint32_t id;
    if (i < 60 && !ids.empty()) {
      id = ids[uint32_t(i) % ids.size()];
    } else {
      id = i < 120 ? TableId(h.random) : uint32_t(h.random.Next());
    }
    Case(
        h, results, NoSetup, [&](PPCContext& t) { t.r3.u64 = id; }, __imp__sub_8209D8D8,
        NativeFindLooseFile, NoOutputs, fmt::format("case {} (id {})", i, id));
  }
  results.Report();
}

// The static set-ups, on the game's queue and slots: their words before a
// case (random for most cases), the set-up run, the queue's 20 bytes and the
// slots' first two words compared with what it added to the exit table and
// r3, all put back after each side.
void StaticInits(Context& h) {
  if (!hooks::NativeAtexit()) {
    ITC_LOG_WARN("harness: archive: native atexit off: the static set-ups' cases skipped");
    return;
  }
  struct Kind {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
  };
  const Kind kKinds[] = {
      {"sub_825D9160 RequestQueue::StaticInit", __imp__sub_825D9160, NativeQueueStaticInit},
      {"sub_825D91B0 RequestSlots::StaticInit", __imp__sub_825D91B0, NativeSlotsStaticInit},
  };
  QueueLock lock(h);
  std::lock_guard<std::recursive_mutex> exit_lock(c_runtime::ExitMutex());
  const auto saved_queue = Read(h.base, archive::kQueue, 20);
  std::vector<uint32_t> saved_slots;
  for (uint32_t i = 0; i < archive::kSlotCount; i++) {
    const uint32_t slot = archive::kSlots + i * archive::kSlotBytes;
    saved_slots.push_back(guest::LoadU32(h.base, slot));
    saved_slots.push_back(guest::LoadU32(h.base, slot + 4));
  }
  auto put_back = [&] {
    Write(h.base, archive::kQueue, saved_queue.data(), 20);
    for (uint32_t i = 0; i < archive::kSlotCount; i++) {
      const uint32_t slot = archive::kSlots + i * archive::kSlotBytes;
      guest::StoreU32(h.base, slot, saved_slots[2 * i]);
      guest::StoreU32(h.base, slot + 4, saved_slots[2 * i + 1]);
    }
  };
  for (const Kind& kind : kKinds) {
    Results results(kind.name);
    for (int c = 0; c < 6; c++) {
      std::vector<uint32_t> start(5 + 2 * archive::kSlotCount);
      for (auto& word : start) {
        word = c == 0 ? 0 : uint32_t(h.random.Next());
      }
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        for (uint32_t w = 0; w < 5; w++) {
          guest::StoreU32(h.base, archive::kQueue + 4 * w, start[w]);
        }
        for (uint32_t i = 0; i < archive::kSlotCount; i++) {
          const uint32_t slot = archive::kSlots + i * archive::kSlotBytes;
          guest::StoreU32(h.base, slot, start[5 + 2 * i]);
          guest::StoreU32(h.base, slot + 4, start[6 + 2 * i]);
        }
        const uint32_t mark = exit_table::Mark(h.base);
        PPCContext t = CallContext(h);
        ITC_FLAG(native_archive) = v == 1;
        (v == 0 ? kind.recompiled : kind.native)(t, h.base);
        ITC_FLAG(native_archive) = true;
        out[v].Bytes(Read(h.base, archive::kQueue, 20));
        for (uint32_t i = 0; i < archive::kSlotCount; i++) {
          const uint32_t slot = archive::kSlots + i * archive::kSlotBytes;
          out[v].Word(guest::LoadU32(h.base, slot));
          out[v].Word(guest::LoadU32(h.base, slot + 4));
        }
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        exit_table::Take(h.base, mark, out[v]);
        put_back();
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
}

}  // namespace

void Archive(Context& h) {
  Tocs(h);
  Lists(h);
  Slots(h);
  LooseFiles(h);
  StaticInits(h);
}

}  // namespace harness
