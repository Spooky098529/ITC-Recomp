// The interior accessories (src/accessories/, docs/functions/accessories.md)
// against the recompiled functions. The originals run with
// --native_accessories off (their callees' own flags as they are), the
// plain C++ through the register steps (hooks/accessories.h). The inputs go
// in a block from the allocation layer filled with the case's random words;
// pages a function takes (VirtualAlloc through the layer) are compared as a
// marker, their contents read, and freed; Archive::Load's calls are
// recorded (ArchiveLoads), never queued. Compared with harness::Outputs.

#include "harness.h"

#include "accessories/accessories.h"
#include "base/flags.h"
#include "base/log.h"
#include "hooks/accessories.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <functional>
#include <optional>
#include <vector>

ITC_FLAG_DECLARE(bool, native_accessories);

namespace harness {

namespace {

namespace ac = accessories;

constexpr uint32_t kMarker = 0x50414745;  // a word that held the case's pages
constexpr uint32_t kBlockBytes = 0x2000;

// Leaf recorders for the file's callback: sub_820CE9F8 (r4, r5 to r3's +444
// and +448; r3 left), the getter sub_82090770 (r3 = [r3 + 60]).
constexpr uint32_t kRecordR4R5 = 0x820CE9F8, kGet60 = 0x82090770;

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

// The original with --native_accessories off.
template <GuestFunction recompiled>
void Original(PPCContext& ctx, uint8_t* base) {
  ITC_FLAG(native_accessories) = false;
  recompiled(ctx, base);
  ITC_FLAG(native_accessories) = true;
}

// A word that may hold the case's pages: whether it does as a marker, the
// pages' first `bytes` read into `out` and freed; else the word.
void TakePages(uint8_t* base, uint32_t word, uint32_t bytes, Outputs& out) {
  const uint32_t value = guest::LoadU32(base, word);
  if (value != 0 && memory::IsPages(value)) {
    out.Word(kMarker);
    if (bytes != 0) {
      out.Bytes(Read(base, value, bytes));
    }
    memory::FreePages(value);
    guest::StoreU32(base, word, kMarker);
  } else {
    out.Word(value);
  }
}

// `count` cases of `recompiled` against `native`: the block random, then
// `setup` (registers and inputs), the call, then `collect`; the block, r3
// (both halves) and what `collect` adds compared.
void Cases(Context& h, const Block& block, const std::string& name, int count,
           GuestFunction recompiled, GuestFunction native,
           const std::function<void(PPCContext&, Random&)>& setup,
           const std::function<void(const PPCContext&, Outputs&)>& collect = nullptr) {
  Results results(name);
  for (int c = 0; c < count; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      WriteRandom(h.base, block.address, block.bytes, r);
      PPCContext t = CallContext(h);
      setup(t, r);
      (v == 0 ? recompiled : native)(t, h.base);
      // r3 may be the case's pages (New's, Create's): compared as a marker.
      const bool r3_pages = t.r3.u32 != 0 && memory::IsPages(t.r3.u32);
      if (collect) {
        collect(t, out[v]);
      }
      out[v].Bytes(Read(h.base, block.address, block.bytes));
      out[v].Word(r3_pages ? kMarker : t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// The archive's serials for the recorded loads: -1 now and then, else any.
std::vector<int64_t> Serials(Random& r) {
  return {r.Below(8) == 0 ? -1 : int64_t(r.Below(0x7FFFFFFF))};
}

// --- The file ---

void Files(Context& h, const Block& block) {
  const uint32_t file = block.address + 0x100;
  Cases(h, block, "AccessoryFile::AccessoryFile (sub_820F0C50)", 8,
        Original<__imp__sub_820F0C50>, hooks::AccessoryFileConstruct,
        [&](PPCContext& t, Random&) { t.r3.u64 = file; });
  Cases(h, block, "AccessoryFile::SetData (sub_820F0BF0)", 16, Original<__imp__sub_820F0BF0>,
        hooks::AccessoryFileSetData, [&](PPCContext& t, Random& r) {
          t.r3.u64 = file;
          t.r4.u64 = r.Next();
          t.r5.u64 = r.Below(2) ? r.Below(2) : r.Next();
        });
  // The serial matching the file's +8 or not; the callback none or a leaf
  // recorder, with the serial (r3) an address in the block for it.
  Cases(
      h, block, "AccessoryFile::OnArchiveDone (sub_820F0C20)", 48,
      Original<__imp__sub_820F0C20>, hooks::AccessoryFileOnArchiveDone,
      [&](PPCContext& t, Random& r) {
        const uint32_t serial = block.address + 0x400 + 4 * r.Below(64);
        guest::StoreU32(h.base, file + 8, r.Below(4) == 0 ? serial + 4 : serial);
        const uint32_t callbacks[] = {0, kRecordR4R5, kGet60};
        guest::StoreU32(h.base, file + 28, callbacks[r.Below(3)]);
        t.r3.u64 = serial;
        t.r4.u64 = r.Next();
        t.r5.u64 = r.Next();
        t.r6.u64 = file;
      },
      [&](const PPCContext& t, Outputs& out) {
        out.Word(t.r4.u32);
        out.Word(t.r5.u32);
        out.Word(t.r6.u32);
      });
  if (!hooks::AccessoriesNativeLoads()) {
    ITC_LOG_WARN("harness: accessories: the allocation layer or the archive's queue isn't "
                 "native: the loads' and frees' cases skipped");
    return;
  }
  // The word empty, or the case's own pages (both sides take theirs).
  const uint32_t word = block.address + 0x200;
  Cases(
      h, block, "AccessoryFile::FreeData (sub_820F08D0)", 16, Original<__imp__sub_820F08D0>,
      hooks::AccessoryFileFreeData,
      [&](PPCContext& t, Random& r) {
        guest::StoreU32(h.base, word, r.Below(4) == 0 ? 0 : memory::AllocPages(4096, 4096));
        t.r3.u64 = word;
      },
      [&](const PPCContext&, Outputs& out) { TakePages(h.base, word, 0, out); });
}

// AccessoryFile::Load with its loads recorded (each case's own recorder):
// the accessories' own assets, or any below BUILD_CRS's first; with no
// buffer (the case's pages), or one in the block.
void FileLoads(Context& h, const Block& block) {
  const uint32_t file = block.address + 0x100;
  Results results("AccessoryFile::Load (sub_820F0C90)");
  for (int c = 0; c < 32; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      WriteRandom(h.base, block.address, block.bytes, r);
      PPCContext t = CallContext(h);
      t.r3.u64 = file;
      t.r4.u64 = r.Below(2) ? 23008 + r.Below(3) : r.Below(23011);
      t.r5.u64 = r.Below(3) == 0 ? block.address + 0x800 : 0;
      t.r6.u64 = r.Next();
      t.r7.u64 = r.Next();
      std::optional<ArchiveLoads> loads;
      loads.emplace(Serials(r));
      (v == 0 ? Original<__imp__sub_820F0C90> : hooks::AccessoryFileLoad)(t, h.base);
      std::vector<uint32_t> calls = loads->calls();
      const uint32_t data = guest::LoadU32(h.base, file + 16);
      for (size_t k = 1; k < calls.size(); k += 6) {
        if (calls[k] == data && data != 0 && memory::IsPages(data)) {
          calls[k] = kMarker;
        }
      }
      TakePages(h.base, file + 16, 0, out[v]);
      out[v].Word(uint32_t(calls.size()));
      for (uint32_t w : calls) {
        out[v].Word(w);
      }
      out[v].Bytes(Read(h.base, block.address, block.bytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// --- The set ---

void Sets(Context& h, const Block& block) {
  const uint32_t set = block.address + 0x100;
  Cases(h, block, "AccessoriesBase::AccessoriesBase (sub_820F07B8)", 8,
        Original<__imp__sub_820F07B8>, hooks::AccessoriesBaseConstruct,
        [&](PPCContext& t, Random&) { t.r3.u64 = set; });
  Cases(h, block, "Accessories::Accessories (sub_820F0E60)", 8, Original<__imp__sub_820F0E60>,
        hooks::AccessoriesConstruct, [&](PPCContext& t, Random&) { t.r3.u64 = set; });
  // Each entry none or one in the block; the scale any 16 bytes (overlapping
  // an entry's +1072 now and then).
  Cases(h, block, "Accessories::SetScale (sub_820F0F00)", 32, Original<__imp__sub_820F0F00>,
        hooks::AccessoriesSetScale, [&](PPCContext& t, Random& r) {
          for (uint32_t i = 0; i < 3; i++) {
            guest::StoreU32(h.base, set + 4 + 4 * i,
                            r.Below(4) == 0 ? 0 : block.address + 0x200 + 0x480 * i);
          }
          t.r3.u64 = set;
          t.r4.u64 = r.Below(4) == 0 ? block.address + 0x200 + 1072 + 4 * r.Below(4)
                                     : block.address + 0x1F00 + 4 * r.Below(16);
        });
  if (!hooks::AccessoriesNativeLoads()) {
    return;
  }
  Cases(
      h, block, "Accessories::New (sub_820F0F48)", 4, Original<__imp__sub_820F0F48>,
      hooks::AccessoriesNew, [&](PPCContext&, Random&) {},
      [&](const PPCContext& t, Outputs& out) {
        guest::StoreU32(h.base, block.address, t.r3.u32);
        TakePages(h.base, block.address, ac::kSetBytes, out);
      });
  // [0x826C5510] none, or a set already (any word); put back after each
  // side.
  const uint32_t saved = guest::LoadU32(h.base, ac::kSet);
  Cases(
      h, block, "Accessories::Create (sub_820F1038)", 8, Original<__imp__sub_820F1038>,
      hooks::AccessoriesCreate,
      [&](PPCContext& t, Random& r) {
        guest::StoreU32(h.base, ac::kSet, r.Below(2) ? 0 : uint32_t(r.Next()) | 1);
        t.r3.u64 = r.Next();
      },
      [&](const PPCContext&, Outputs& out) {
        TakePages(h.base, ac::kSet, ac::kSetBytes, out);
        guest::StoreU32(h.base, ac::kSet, saved);
      });
}

}  // namespace

void Accessories(Context& h) {
  const Block block(kBlockBytes);
  if (!block.address) {
    ITC_LOG_ERROR("harness: accessories: no block from the allocation layer");
    return;
  }
  Files(h, block);
  if (hooks::AccessoriesNativeLoads()) {
    FileLoads(h, block);
  }
  Sets(h, block);
}

}  // namespace harness
