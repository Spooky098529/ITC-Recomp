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
// (exit_table.h). The request finish runs on requests in the scratch area
// with ListNode::ListNode (0x82294B68, a leaf storing r4 at r3 + 4) as the
// callback, its serial a scratch address, so the call shows; the staging
// half in use (0x826A511C, the loader thread's) is only ever written with
// its own value, and the callback serial (0x82600AA4, written only by the
// finish) put back after each side; the data it decommits are pages of the
// allocation layer (a decommit there does nothing). LoaderFunctor::Invoke
// calls the same leaf. Ucl::Unpack and its block's unpacking run on streams
// made by an NRV2B encoder here (literals, matches near and far, the last
// offset reused, long lengths; stored blocks; sizes that stop it early) and
// on compressed assets read from BUILD.DAT, in pages of the allocation
// layer. The originals run with --native_archive off, so their
// callees are the recompiled code too.

#include "harness.h"
#include "exit_table.h"

#include "archive/archive.h"
#include "base/flags.h"
#include "base/log.h"
#include "hooks/archive.h"
#include "hooks/c_runtime.h"
#include "game_data/game_data.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <algorithm>
#include <bit>
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

void Finishes(Context& h) {
  Results results("sub_8209D688 Archive::FinishRequest");
  const uint32_t pages = memory::AllocPages(4096, 4096);
  constexpr uint32_t kLeaf = 0x82294B68;
  const uint32_t request = h.data + 256;
  const uint32_t serial_area = h.data + 2048;
  for (int i = 0; i < 300; i++) {
    const uint32_t saved_serial = guest::LoadU32(h.base, archive::kCallbackSerial);
    const uint32_t staging = guest::LoadU32(h.base, archive::kStagingInUse);
    // The flags the finish reads, each on or off, over random bits.
    uint32_t flags = uint32_t(h.random.Next()) & ~0x310Au & ~0x2000u;
    for (uint32_t bit : {0x100u, 0x8u, 0x2000u, 0x2u, 0x1000u}) {
      if (h.random.Below(2)) {
        flags |= bit;
      }
    }
    if (i < 32) {  // every combination first
      flags = (flags & ~0x310Au & ~0x2000u) | ((i & 1) ? 0x100u : 0) | ((i & 2) ? 0x8u : 0) |
              ((i & 4) ? 0x2000u : 0) | ((i & 8) ? 0x2u : 0) | ((i & 16) ? 0x1000u : 0);
    }
    const bool restages = (flags & 0x100) && !(flags & 0x8);
    const uint32_t serial = serial_area + 8 * h.random.Below(64);
    Case(
        h, results,
        [&] {
          for (uint32_t w = 0; w < 23; w++) {
            guest::StoreU32(h.base, request + 4 * w, uint32_t(h.random.Next()));
          }
          guest::StoreU32(h.base, request + 8, serial);
          guest::StoreU32(h.base, request + 12, flags);
          guest::StoreU32(h.base, request + 48, kLeaf);
          // The data: the staging half in use when the finish restages it
          // (so the global keeps its value), the destination the pages.
          guest::StoreU32(h.base, request + 76, restages ? staging : pages);
          guest::StoreU32(h.base, request + 88, pages);
        },
        [&](PPCContext& t) { t.r3.u64 = request; }, __imp__sub_8209D688,
        hooks::ArchiveFinishRequest,
        [&](const PPCContext&, Outputs& out) {
          out.Bytes(Read(h.base, h.data, kScratchBytes));
          out.Word(guest::LoadU32(h.base, archive::kCallbackSerial));
          out.Word(guest::LoadU32(h.base, archive::kStagingInUse));
          guest::StoreU32(h.base, archive::kCallbackSerial, saved_serial);
        },
        fmt::format("case {} (flags {:08X})", i, flags));
  }
  memory::FreePages(pages);
  results.Report();
}

void Invokes(Context& h) {
  Results results("sub_8209DD80 LoaderFunctor::Invoke");
  for (int i = 0; i < 60; i++) {
    const uint32_t functor = h.data + 8 * h.random.Below(16);
    const uint32_t argument = h.data + 512 + 8 * h.random.Below(64);
    Case(
        h, results,
        [&] {
          guest::StoreU32(h.base, functor, uint32_t(h.random.Next()));
          guest::StoreU32(h.base, functor + 4, 0x82294B68);
        },
        [&](PPCContext& t) {
          t.r3.u64 = functor;
          t.r4.u64 = argument;
        },
        __imp__sub_8209DD80, hooks::ArchiveInvokeFunctor,
        [&](const PPCContext& t, Outputs& out) {
          out.Bytes(Read(h.base, h.data, kScratchBytes));
          out.Word(t.r11.u32);
          out.Word(t.ctr.u32);
        },
        fmt::format("case {}", i));
  }
  results.Report();
}

// --- Ucl::Unpack ---

// UCL's NRV2B stream, as the decoder reads it: bits in 32-bit little-endian
// words (taken where the decoder needs its next bit), bytes in between.
class Nrv2b {
 public:
  void Bit(uint32_t b) {
    if (count_ == 0) {
      word_at_ = out.size();
      out.resize(out.size() + 4, 0);
      count_ = 32;
    }
    count_--;
    if (b) {
      out[word_at_ + (count_ >> 3)] |= uint8_t(1u << (count_ & 7));
    }
  }
  void Byte(uint8_t b) { out.push_back(b); }
  // A number of 2 or more: each bit below the top one, then 1 after the
  // last and 0 after the others.
  void Gamma(uint32_t v) {
    const int top = 31 - std::countl_zero(v);
    for (int i = top - 1; i >= 0; i--) {
      Bit((v >> i) & 1);
      Bit(i == 0 ? 1 : 0);
    }
  }
  std::vector<uint8_t> out;

 private:
  size_t word_at_ = 0;
  int count_ = 0;
};

// A random block: its stream and the bytes it unpacks to (counted only).
std::vector<uint8_t> RandomBlock(Random& r, uint32_t target, uint32_t* unpacked) {
  Nrv2b w;
  uint32_t olen = 0, last = 1;
  const bool far = r.Below(2);
  while (olen < target) {
    if (olen == 0 || r.Below(3) == 0) {
      for (uint32_t n = 1 + r.Below(8); n; n--) {
        w.Bit(1);
        w.Byte(uint8_t(r.Next()));
        olen++;
      }
      continue;
    }
    w.Bit(0);
    uint32_t off;
    if (r.Below(5) == 0) {
      off = last;
      w.Gamma(2);
    } else {
      const uint32_t most = std::min(olen, far ? 0x3000u : 0x400u);
      off = 1 + r.Below(most);
      const uint32_t x = off - 1;
      w.Gamma((x >> 8) + 3);
      w.Byte(uint8_t(x & 0xFF));
      last = off;
    }
    const uint32_t bonus = off > 0xD00 ? 1 : 0;
    uint32_t length = 2 + bonus + r.Below(r.Below(6) == 0 ? 300 : 12);
    const uint32_t m_len = length - 1 - bonus;
    if (m_len <= 3) {
      w.Bit(m_len >> 1);
      w.Bit(m_len & 1);
    } else {
      w.Bit(0);
      w.Bit(0);
      w.Gamma(m_len - 2);
    }
    olen += length;
  }
  w.Bit(0);
  w.Gamma(0x1000002);
  w.Byte(0xFF);
  *unpacked = olen;
  return w.out;
}

void PutBe32(std::vector<uint8_t>& v, uint32_t word) {
  for (int i = 0; i < 4; i++) {
    v.push_back(uint8_t(word >> (24 - 8 * i)));
  }
}

// The block's native step: its r3 as the original leaves it (-201 from li,
// sign-extended; -205 from a word rlwinm cleared the upper half of), its
// output size stored at r6.
void NativeUclBlock(PPCContext& ctx, uint8_t* base) {
  const auto result = archive::UclBlock(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
  guest::StoreU32(base, ctx.r6.u32, result.out);
  ctx.r3.u64 = result.r3 == -205 ? uint64_t(uint32_t(-205)) : uint64_t(int64_t(result.r3));
}

// Runs one Unpack or block case on pages: the source copied in, the
// destination filled, each side run, the destination (and r3) compared.
struct UclPages {
  uint32_t src = 0, dst = 0, src_bytes = 0, dst_bytes = 0;
  void Make(uint32_t s, uint32_t d) {
    src_bytes = s;
    dst_bytes = d;
    src = memory::AllocPages(s, 4096);
    dst = memory::AllocPages(d, 4096);
  }
  void Free() {
    memory::FreePages(src);
    memory::FreePages(dst);
  }
};

// How many cases' recompiled side returned 0 (a block: the input used up
// exactly; Unpack: 1, its output as expected), for the log: the streams made
// here have to be ones the game's decoder reads as meant.
int g_ucl_whole = 0;

void UclCase(Context& h, Results& results, const std::vector<uint8_t>& stream, uint32_t out_bytes,
             bool block, uint32_t r4, uint32_t r5, const std::string& what) {
  UclPages pages;
  pages.Make(uint32_t(stream.size()) + 64, out_bytes + 4096);
  Write(h.base, pages.src, stream.data(), uint32_t(stream.size()));
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Fill(h.base, pages.dst, pages.dst_bytes, 0xA5);
    Fill(h.base, h.data, 64);
    PPCContext t = CallContext(h);
    if (block) {
      t.r3.u64 = pages.src;
      t.r4.u64 = r4;
      t.r5.u64 = pages.dst;
      t.r6.u64 = h.data;
      t.r7.u64 = 0;
    } else {
      t.r3.u64 = pages.dst;
      t.r4.u64 = pages.src;
      t.r5.u64 = r5;
    }
    ITC_FLAG(native_archive) = v == 1;
    if (block) {
      (v == 0 ? __imp__sub_822947F8 : NativeUclBlock)(t, h.base);
    } else {
      (v == 0 ? __imp__sub_82294610 : hooks::ArchiveUclUnpack)(t, h.base);
    }
    ITC_FLAG(native_archive) = true;
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
    if (v == 0 && t.r3.u32 == (block ? 0u : 1u)) {
      g_ucl_whole++;
    }
    if (block) {
      out[v].Word(guest::LoadU32(h.base, h.data));
    }
    out[v].Bytes(Read(h.base, pages.dst, pages.dst_bytes));
  }
  pages.Free();
  results.Check(out[0], out[1], what);
}

void UclBlocks(Context& h) {
  Results results("sub_822947F8 Ucl::UnpackBlock");
  g_ucl_whole = 0;
  for (int i = 0; i < 80; i++) {
    uint32_t unpacked;
    const auto stream = RandomBlock(h.random, 1 + h.random.Below(i < 40 ? 600 : 20000), &unpacked);
    // The input's length as given, or a little off (r3 -201, -205).
    uint32_t length = uint32_t(stream.size());
    if (i % 4 == 1) {
      length += 1 + h.random.Below(8);
    } else if (i % 4 == 2) {
      length -= 1 + h.random.Below(std::min<uint32_t>(length - 1, 8));
    }
    UclCase(h, results, stream, unpacked, true, length, 0,
            fmt::format("case {} ({} bytes to {})", i, stream.size(), unpacked));
  }
  ITC_LOG_INFO("harness: archive: {} of 80 blocks read to their end exactly (40 expected)",
               g_ucl_whole);
  results.Report();
}

void UclStreams(Context& h) {
  Results results("sub_82294610 Ucl::Unpack");
  g_ucl_whole = 0;
  for (int i = 0; i < 60; i++) {
    std::vector<uint8_t> stream;
    for (int b = 0; b < 18; b++) {
      stream.push_back(uint8_t(h.random.Next()));
    }
    uint32_t total = 0;
    for (uint32_t blocks = 1 + h.random.Below(5); blocks; blocks--) {
      if (h.random.Below(3) == 0) {
        // Stored as is: as many bytes as its size, or more (copied all).
        const uint32_t size = 1 + h.random.Below(3000);
        const uint32_t stored = size + (h.random.Below(3) == 0 ? h.random.Below(64) : 0);
        PutBe32(stream, size);
        PutBe32(stream, stored);
        for (uint32_t n = 0; n < stored; n++) {
          stream.push_back(uint8_t(h.random.Next()));
        }
        total += stored;
      } else {
        uint32_t unpacked;
        auto block = RandomBlock(h.random, 1 + h.random.Below(8000), &unpacked);
        if (block.size() >= unpacked) {
          // Not smaller: it would be copied; make it a stored block of its
          // own bytes instead.
          PutBe32(stream, uint32_t(block.size()));
          PutBe32(stream, uint32_t(block.size()));
          total += uint32_t(block.size());
        } else {
          PutBe32(stream, unpacked);
          PutBe32(stream, uint32_t(block.size()));
          total += unpacked;
        }
        stream.insert(stream.end(), block.begin(), block.end());
      }
    }
    PutBe32(stream, 0);
    // The expected size: the total, or less (stops early: 0), or more (0).
    uint32_t expected = total;
    if (i % 5 == 3) {
      expected = total - h.random.Below(total);
    } else if (i % 5 == 4) {
      expected = total + 1 + h.random.Below(100);
    }
    UclCase(h, results, stream, std::max(total, expected), false, 0, expected,
            fmt::format("case {} ({} bytes to {}, expected {})", i, stream.size(), total,
                        expected));
  }
  // Compressed assets from BUILD.DAT (the TOC's +8 not 0), under 256 KiB
  // stored.
  const uint32_t handle = guest::LoadU32(h.base, 0x82600A9C);
  const uint32_t toc = guest::LoadU32(h.base, archive::kBuildToc);
  int found = 0;
  for (int tries = 0; tries < 4000 && found < 16 && game_data::IsHandle(handle); tries++) {
    const uint32_t id = h.random.Below(archive::kFirstCrsId);
    const uint32_t entry = toc + (id + 1) * 16;
    const uint32_t sector = guest::LoadU32(h.base, entry);
    const uint32_t stored = guest::LoadU32(h.base, entry + 4);
    const uint32_t unpacked = guest::LoadU32(h.base, entry + 8);
    if (unpacked == 0 || stored == 0 || stored > 256 * 1024 || unpacked > 4 * 1024 * 1024) {
      continue;
    }
    std::vector<uint8_t> stream(stored);
    auto read = game_data::ReadAt(handle, uint64_t(sector) * 2048, stream.data(), stored);
    if (!read || *read != stored) {
      continue;
    }
    found++;
    UclCase(h, results, stream, unpacked, false, 0, unpacked,
            fmt::format("asset {} ({} bytes to {})", id, stored, unpacked));
  }
  ITC_LOG_INFO("harness: archive: {} compressed assets from BUILD.DAT; {} of {} streams unpacked "
               "to their expected size (36 made + the assets expected)",
               found, g_ucl_whole, 60 + found);
  results.Report();
}

}  // namespace

void Archive(Context& h) {
  Tocs(h);
  Lists(h);
  Slots(h);
  LooseFiles(h);
  StaticInits(h);
  Finishes(h);
  Invokes(h);
  UclBlocks(h);
  UclStreams(h);
}

}  // namespace harness
