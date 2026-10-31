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
#include "hooks/heap.h"
#include "hooks/models.h"
#include "memory/c_runtime_heap.h"
#include "native/buffers.h"
#include "memory/heap.h"
#include "model_animation/model_animation.h"
#include "models/model.h"
#include "models/texture_provider.h"

#include <fmt/format.h>

#include <functional>
#include <string>
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

// The register steps of the functions only native code calls (their
// adapters removed): the plain C++ with the adapters' checks, else the
// original.
void NativeFileConstruct(PPCContext& ctx, uint8_t* base) {
  ac::FileConstruct(base, ctx.r3.u32);
}
void NativeSetBaseConstruct(PPCContext& ctx, uint8_t* base) {
  ac::SetBaseConstruct(base, ctx.r3.u32);
}
void NativeSetConstruct(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ac::SetConstruct(base, ctx.r3.u32);
}
void NativeSetNew(PPCContext& ctx, uint8_t* base) {
  if (!hooks::NativeHeapActive()) {
    __imp__sub_820F0F48(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ac::SetNew(base);
}
void NativeSetFree(PPCContext& ctx, uint8_t* base) {
  if (!hooks::NativeHeapActive() || !hooks::AccessoriesSetFreeable(base, ctx.r3.u32)) {
    __imp__sub_820F0F90(ctx, base);
    return;
  }
  ctx.r3.u64 =
      ac::SetFree(base, ctx.r3.u32, ctx.r1.u32, hooks::AccessoriesSetDestructor(ctx, base));
}
void NativeSkeletonConstruct(PPCContext& ctx, uint8_t* base) {
  if (!native::Buffers_NativeMemcpy()) {
    __imp__sub_820F03F0(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ac::SkeletonConstruct(base, ctx.r3.u32, ctx.r1.u32);
}
void NativeAccessoryConstruct(PPCContext& ctx, uint8_t* base) {
  if (!hooks::ModelsNativeLifetime() || !native::Buffers_NativeMemcpy() ||
      !ac::AccessoryConstructible(base)) {
    __imp__sub_820EF258(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ac::AccessoryConstruct(base, ctx.r3.u32, ctx.r1.u32);
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
        Original<__imp__sub_820F0C50>, NativeFileConstruct,
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
        Original<__imp__sub_820F07B8>, NativeSetBaseConstruct,
        [&](PPCContext& t, Random&) { t.r3.u64 = set; });
  Cases(h, block, "Accessories::Accessories (sub_820F0E60)", 8, Original<__imp__sub_820F0E60>,
        NativeSetConstruct, [&](PPCContext& t, Random&) { t.r3.u64 = set; });
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
      NativeSetNew, [&](PPCContext&, Random&) {},
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

// AccessoryFile::Reset: owned or not, each state (0 to 3), a loaded block
// the case's pages or none, a loading serial no request has (Cancel answers
// 1 without touching the queue).
void FileResets(Context& h, const Block& block) {
  const uint32_t file = block.address + 0x100;
  uint32_t pages = 0;
  Cases(
      h, block, "AccessoryFile::Reset (sub_820F0D30)", 32, Original<__imp__sub_820F0D30>,
      hooks::AccessoryFileReset,
      [&](PPCContext& t, Random& r) {
        guest::StoreU32(h.base, file + 24, r.Below(4) == 0 ? 0 : 1);
        guest::StoreU32(h.base, file + 12, r.Below(4));
        guest::StoreU32(h.base, file + 8, 0x70000000u + r.Below(0x1000000));
        pages = r.Below(4) == 0 ? 0 : memory::AllocPages(4096, 4096);
        guest::StoreU32(h.base, file + 16, pages);
        t.r3.u64 = file;
      },
      [&](const PPCContext&, Outputs& out) {
        // Freed or not; still the case's pages, freed here.
        out.Word(pages != 0 && memory::IsPages(pages) ? 1 : 0);
        if (pages != 0 && memory::IsPages(pages)) {
          memory::FreePages(pages);
        }
      });
}

// A set (the case's pages) whose vtable's slot 0 is a leaf recorder (r4, r5
// to the set's +444, +448: read after, though freed); the slot empty now
// and then.
void SetFrees(Context& h, const Block& block) {
  const uint32_t slot = block.address + 0x40, vtable = block.address + 0x80;
  const auto lay = [&](Random& r, uint32_t at) {
    const uint32_t set = r.Below(6) == 0 ? 0 : memory::AllocPages(ac::kSetBytes, 4096);
    guest::StoreU32(h.base, at, set);
    guest::StoreU32(h.base, vtable, kRecordR4R5);
    if (set != 0) {
      guest::StoreU32(h.base, set, vtable);
    }
    return set;
  };
  uint32_t set = 0;
  const auto collect = [&](const PPCContext& t, Outputs& out) {
    if (set != 0) {
      out.Word(memory::IsPages(set) ? 1 : 0);
      out.Bytes(Read(h.base, set + 444, 8));
      if (memory::IsPages(set)) {
        memory::FreePages(set);
      }
    }
    out.Word(t.r4.u32);
  };
  Cases(
      h, block, "Accessories::Free (sub_820F0F90)", 16, Original<__imp__sub_820F0F90>,
      NativeSetFree,
      [&](PPCContext& t, Random& r) {
        set = lay(r, slot);
        t.r3.u64 = slot;
      },
      collect);
  const uint32_t saved = guest::LoadU32(h.base, ac::kSet);
  Cases(
      h, block, "Accessories::Destroy (sub_820F1078)", 16, Original<__imp__sub_820F1078>,
      hooks::AccessoriesDestroy,
      [&](PPCContext& t, Random& r) {
        set = lay(r, ac::kSet);
        t.r3.u64 = r.Next();
      },
      [&](const PPCContext& t, Outputs& out) {
        out.Word(guest::LoadU32(h.base, ac::kSet));
        collect(t, out);
        guest::StoreU32(h.base, ac::kSet, saved);
      });
}

// A part model's texture provider storage (operator new): compared by its
// size, the pointer as a marker, then freed.
void TakeTextures(uint8_t* base, uint32_t model, Outputs& out) {
  const uint32_t vector = model + models::kModelTextures + models::kTextureProviderVector;
  const uint32_t first = guest::LoadU32(base, vector + models::kVectorFirst);
  out.Word(guest::LoadU32(base, vector + models::kVectorLast) - first);
  out.Word(guest::LoadU32(base, vector + models::kVectorEnd) - first);
  out.Bytes(Read(base, first, 4 * models::kTextureProviderSlots));
  guest::StoreU32(base, vector + models::kVectorFirst, kMarker);
  guest::StoreU32(base, vector + models::kVectorLast, 0);
  guest::StoreU32(base, vector + models::kVectorEnd, 0);
  memory::crt::OperatorDelete(first);
}

// The constructors: the skeleton alone, the accessory and the entry (the
// case's pages), the frame's words the skeleton's constructor leaves (+80
// and +84, the identity at +96 to +159 of its 224-byte frame) compared.
void Constructors(Context& h, const Block& block) {
  const uint32_t object = block.address + 0x200;
  const auto frame_words = [&](const PPCContext& t, uint32_t below, Outputs& out) {
    // +88 to +95 aren't written (the part model's calls leave words there).
    const uint32_t frame = t.r1.u32 - below - 224;
    out.Bytes(Read(h.base, frame + 80, 8));
    out.Bytes(Read(h.base, frame + 96, 64));
  };
  Cases(
      h, block, "AccessorySkeleton::AccessorySkeleton (sub_820F03F0)", 8,
      Original<__imp__sub_820F03F0>, NativeSkeletonConstruct,
      [&](PPCContext& t, Random&) { t.r3.u64 = object; },
      [&](const PPCContext& t, Outputs& out) { frame_words(t, 0, out); });
  Cases(
      h, block, "Accessory::Accessory (sub_820EF258)", 8, Original<__imp__sub_820EF258>,
      NativeAccessoryConstruct, [&](PPCContext& t, Random&) { t.r3.u64 = object; },
      [&](const PPCContext& t, Outputs& out) {
        frame_words(t, 96, out);
        TakeTextures(h.base, object, out);
      });
  if (!hooks::AccessoriesNativeLoads()) {
    return;
  }
  Cases(
      h, block, "AccessoryEntry::New (sub_820F0920)", 8, Original<__imp__sub_820F0920>,
      hooks::AccessoryEntryNew, [&](PPCContext&, Random&) {},
      [&](const PPCContext& t, Outputs& out) {
        frame_words(t, 192, out);
        const uint32_t entry = t.r3.u32;
        if (entry != 0 && memory::IsPages(entry)) {
          TakeTextures(h.base, entry + 48, out);
        }
        guest::StoreU32(h.base, block.address, entry);
        TakePages(h.base, block.address, ac::kEntryBytes, out);
      });
}

// The destructors, on an entry made by AccessoryEntry::New's plain C++ (the
// case's pages, the same on both sides: the layer is first fit and each
// side frees what it took): its four files (the entry's, the model's two,
// the skeleton's) each empty, loading (a serial no request has) or loaded
// (owned pages or data not owned). Run on the entry's slot, the accessory,
// the skeleton or the entry's file; the entry's bytes (read after, though
// freed), whether each block is still pages, and r3 compared; what's left
// freed after.
void Destructors(Context& h, const Block& block) {
  if (!hooks::AccessoriesNativeLoads() || !hooks::ModelsNativeLifetime() ||
      !native::Buffers_NativeMemcpy() || !ac::AccessoryConstructible(h.base)) {
    ITC_LOG_WARN("harness: accessories: the models' lifetimes aren't native: the "
                 "destructors' cases skipped");
    return;
  }
  struct Kind {
    const char* name;
    GuestFunction recompiled, native;
    int target;  // 0 the slot, 1 the accessory, 2 the skeleton, 3 the entry's file
  };
  const Kind kinds[] = {
      {"AccessoryEntry::Destroy (sub_820F0988)", Original<__imp__sub_820F0988>,
       hooks::AccessoryEntryDestroy, 0},
      {"Accessory::~Accessory (sub_820EF310)", Original<__imp__sub_820EF310>,
       hooks::AccessoryDestruct, 1},
      {"AccessorySkeleton::~AccessorySkeleton (sub_820EFFD8)", Original<__imp__sub_820EFFD8>,
       hooks::AccessorySkeletonDestruct, 2},
      {"AccessoryFile::~AccessoryFile (sub_820F0DB8)", Original<__imp__sub_820F0DB8>,
       hooks::AccessoryFileDestruct, 3},
  };
  const uint32_t slot = block.address + 0x40;
  for (const Kind& kind : kinds) {
    Results results(kind.name);
    for (int c = 0; c < 16; c++) {
      const uint64_t seed = h.random.Next();
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed);
        WriteRandom(h.base, block.address, block.bytes, r);
        PPCContext t = CallContext(h);
        const bool none = kind.target == 0 && r.Below(8) == 0;
        const uint32_t entry = none ? 0 : ac::EntryNew(h.base, t.r1.u32);
        const uint32_t accessory = entry + 48;
        const uint32_t files[4] = {entry + 4, accessory + 464, accessory + 500,
                                   accessory + 544 + 180};
        uint32_t pages[4] = {};
        for (uint32_t i = 0; i < 4 && entry != 0; i++) {
          const uint32_t file = files[i];
          const uint32_t state = r.Below(3);
          const uint32_t owned = r.Below(4) == 0 ? 0 : 1;
          guest::StoreU32(h.base, file + 12, state);
          guest::StoreU32(h.base, file + 24, owned);
          guest::StoreU32(h.base, file + 8, 0x70000000u + r.Below(0x1000000));
          guest::StoreU32(h.base, file + 4, r.Below(23011));
          if (state == 2 && owned) {
            pages[i] = memory::AllocPages(4096, 4096);
            guest::StoreU32(h.base, file + 16, pages[i]);
          } else {
            guest::StoreU32(h.base, file + 16, block.address + 0x1000);
          }
        }
        guest::StoreU32(h.base, slot, entry);
        const uint32_t targets[4] = {slot, accessory, accessory + 544, entry + 4};
        t.r3.u64 = targets[kind.target];
        (v == 0 ? kind.recompiled : kind.native)(t, h.base);
        out[v].Word(t.r3.u32 == entry && entry != 0 ? kMarker : t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        out[v].Bytes(Read(h.base, block.address, block.bytes));
        if (entry == 0) {
          continue;
        }
        const bool entry_pages = memory::IsPages(entry);
        out[v].Word(entry_pages ? 1 : 0);
        out[v].Bytes(Read(h.base, entry, ac::kEntryBytes));
        for (uint32_t i = 0; i < 4; i++) {
          const bool still = pages[i] != 0 && memory::IsPages(pages[i]);
          out[v].Word(still ? 1 : 0);
          if (still) {
            memory::FreePages(pages[i]);
          }
        }
        if (entry_pages) {
          if (kind.target >= 2) {  // the model wasn't destroyed: its storage
            const uint32_t first = guest::LoadU32(
                h.base, accessory + models::kModelTextures + models::kTextureProviderVector +
                            models::kVectorFirst);
            memory::crt::OperatorDelete(first);
          }
          memory::FreePages(entry);
        }
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
}

// Accessories::ReleaseAll: each slot none or an entry made by
// AccessoryEntry::New's plain C++ with a count of 1 to 3. The slots (an
// entry as a marker with its count), the block and r3 compared; the entries
// left destroyed after.
void ReleaseAlls(Context& h, const Block& block) {
  const uint32_t set = block.address + 0x40;
  Results results("Accessories::ReleaseAll (sub_820F0AD8)");
  for (int c = 0; c < 24; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      WriteRandom(h.base, block.address, block.bytes, r);
      PPCContext t = CallContext(h);
      for (uint32_t i = 0; i < 3; i++) {
        uint32_t entry = 0;
        if (r.Below(4) != 0) {
          entry = ac::EntryNew(h.base, t.r1.u32);
          guest::StoreU32(h.base, entry, 1 + r.Below(3));
        }
        guest::StoreU32(h.base, set + 4 + 4 * i, entry);
      }
      t.r3.u64 = set;
      (v == 0 ? Original<__imp__sub_820F0AD8> : hooks::AccessoriesReleaseAll)(t, h.base);
      out[v].Word(t.r3.u32 == set ? kMarker : t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      for (uint32_t i = 0; i < 3; i++) {
        const uint32_t slot = set + 4 + 4 * i;
        const uint32_t entry = guest::LoadU32(h.base, slot);
        out[v].Word(entry == 0 ? 0 : memory::IsPages(entry) ? kMarker : 1);
        if (entry != 0 && memory::IsPages(entry)) {
          out[v].Word(guest::LoadU32(h.base, entry));
          ac::EntryDestroy(h.base, slot);
          guest::StoreU32(h.base, slot, kMarker);
        }
      }
      out[v].Bytes(Read(h.base, block.address, block.bytes));
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// A node file in the block at `file` for the skeleton's set-up and reset:
// `nodes` nodes (160-byte records whose names are random letters, some
// holding the swinging node's string), their three rows, random flags;
// `entries` entries; the offsets from the file.
void NodeFile(uint8_t* base, uint32_t file, int32_t nodes, uint32_t entries, Random& r) {
  WriteRandom(base, file, 0x600, r);
  guest::StoreU32(base, file + 4, uint32_t(nodes));
  guest::StoreU32(base, file + 8, entries);
  guest::StoreU32(base, file + 12, nodes > 0 ? r.Below(uint32_t(nodes) + 1) : 0);
  const uint32_t offsets[] = {0x60, 0x300, 0x340, 0x380, 0x3C0, 0x3E0, 0x400, 0x440, 0x480};
  for (uint32_t k = 0; k < 9; k++) {
    guest::StoreU32(base, file + 16 + 4 * k, offsets[k]);
  }
  std::string swing;
  for (uint32_t a = ac::kSwingNodeName; guest::LoadU8(base, a) != 0 && swing.size() < 64; a++) {
    swing.push_back(char(guest::LoadU8(base, a)));
  }
  for (int32_t i = 0; i < nodes && i < 4; i++) {
    const uint32_t record = file + 0x60 + 160 * uint32_t(i);
    std::string name;
    for (uint32_t k = r.Below(6); k > 0; k--) {
      name.push_back(char('a' + r.Below(26)));
    }
    if (r.Below(3) == 0) {
      name += swing;
    }
    for (uint32_t k = 0; k <= name.size(); k++) {
      guest::StoreU8(base, record + k, k < name.size() ? uint8_t(name[k]) : 0);
    }
  }
}

// The skeleton's set-up and reset, on a skeleton AccessorySkeleton's plain
// C++ made. SetUp: set up already or not, its file loaded or not, the node
// file's nodes 0 to 4 (a negative count now and then); the skeleton, the
// arrays SkeletonBase::SetUp made (their contents; the pointers the same on
// both sides: first fit, each side frees them), the block, Reset's frame
// rows and r3 compared. Reset alone on the skeleton's views pointed into a
// node file.
void SkeletonSetUps(Context& h, const Block& block) {
  if (!hooks::ModelsNativeLifetime() || !native::Buffers_NativeMemcpy()) {
    ITC_LOG_WARN("harness: accessories: the models' lifetimes aren't native: the skeleton's "
                 "set-up cases skipped");
    return;
  }
  const uint32_t skeleton = block.address + 0x100;
  const uint32_t file = block.address + 0x800;
  const auto frame_rows = [&](uint32_t sp, Outputs& out) {
    out.Bytes(Read(h.base, sp - 192 + 80, 64));
  };
  Cases(
      h, block, "AccessorySkeleton::Reset (sub_820EFB30)", 24, Original<__imp__sub_820EFB30>,
      hooks::AccessorySkeletonReset,
      [&](PPCContext& t, Random& r) {
        ac::SkeletonConstruct(h.base, skeleton, t.r1.u32);
        const int32_t nodes = r.Below(6) == 0 ? -1 : int32_t(r.Below(5));
        NodeFile(h.base, file, nodes, 0, r);
        guest::StoreU32(h.base, skeleton + 80, file);
        guest::StoreU32(h.base, skeleton + 84, file + 0x60);
        for (uint32_t k = 0; k < 3; k++) {
          guest::StoreU32(h.base, skeleton + 88 + 4 * k, file + 0x300 + 0x40 * k);
        }
        guest::StoreU32(h.base, skeleton + 136, block.address + 0x1000);
        guest::StoreU32(h.base, skeleton + 152, r.Below(2));
        t.r3.u64 = skeleton;
      },
      [&](const PPCContext& t, Outputs& out) { frame_rows(t.r1.u32, out); });
  Cases(
      h, block, "AccessorySkeleton::SetUp (sub_820F0070)", 32, Original<__imp__sub_820F0070>,
      hooks::AccessorySkeletonSetUp,
      [&](PPCContext& t, Random& r) {
        ac::SkeletonConstruct(h.base, skeleton, t.r1.u32);
        const int32_t nodes = r.Below(8) == 0 ? -1 : int32_t(r.Below(5));
        NodeFile(h.base, file, nodes < 0 ? 0 : nodes, r.Below(3), r);
        if (nodes < 0) {
          guest::StoreU32(h.base, file + 4, 0);
        }
        guest::StoreU32(h.base, skeleton + 192, r.Below(5) == 0 ? 1 : 2);
        guest::StoreU32(h.base, skeleton + 196, file);
        guest::StoreU32(h.base, skeleton + 260, r.Below(5) == 0 ? 1 : 0);
        t.r3.u64 = skeleton;
      },
      [&](const PPCContext& t, Outputs& out) {
        frame_rows(t.r1.u32 - 128, out);
        const uint32_t nodes = guest::LoadU32(h.base, file + 4);
        const uint32_t entries = guest::LoadU32(h.base, skeleton + 136);
        if (entries != 0) {
          out.Bytes(Read(h.base, entries, 144 * nodes));
        }
        const uint32_t matrices = guest::LoadU32(h.base, skeleton + 148);
        if (matrices != 0) {
          out.Bytes(Read(h.base, matrices, 64 * nodes));
        }
        model_animation::SkeletonBaseFree(h.base, skeleton);
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
  Constructors(h, block);
  SkeletonSetUps(h, block);
  if (hooks::AccessoriesNativeLoads()) {
    FileResets(h, block);
    SetFrees(h, block);
    Destructors(h, block);
    ReleaseAlls(h, block);
  }
}

}  // namespace harness
