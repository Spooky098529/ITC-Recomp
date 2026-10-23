// The data tables (src/data_tables/, docs/functions/data-tables.md) against
// the recompiled functions. The words swapped are data, not floats (the
// originals only pass them through a float register), so the whole scratch
// area is compared exactly, signaling-NaN bit patterns among the inputs,
// then r3. The originals run with --native_data_tables off, so their callees
// are the recompiled code too. The table object's global ([0x826C5CA0]) is
// pointed at the scratch area's object for a case, compared, and put back.

#include "harness.h"

#include "base/flags.h"
#include "data_tables/data_tables.h"

#include <fmt/format.h>

#include <iterator>
#include <string>

ITC_FLAG_DECLARE(bool, native_data_tables);

namespace harness {

namespace {

namespace dt = data_tables;

// The register steps: the cases run the plain C++ through these (not the
// adapters), with the registers each adapter leaves.
#define ITC_NATIVE_STEP(name, body)           \
  void name(PPCContext& ctx, uint8_t* base) { \
    ctx.fpscr.disableFlushMode();             \
    body;                                     \
  }

ITC_NATIVE_STEP(NativeSwapWord, dt::SwapWord(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeSwapEntry20, ctx.r3.u64 = dt::SwapEntry20(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeSwapEntry56, ctx.r3.u64 = dt::SwapEntry56(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeSwapEntry40, ctx.r3.u64 = dt::SwapEntry40(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeOn56Loaded, ctx.r3.u64 = dt::On56Loaded(base, ctx.r6.u32))
ITC_NATIVE_STEP(NativeOn20Loaded, ctx.r3.u64 = dt::On20Loaded(base, ctx.r6.u32))
ITC_NATIVE_STEP(NativeOn40Loaded, ctx.r3.u64 = dt::On40Loaded(base, ctx.r6.u32))
ITC_NATIVE_STEP(NativeOnItemsLoaded, dt::OnItemsLoaded(base, ctx.r6.u32))
ITC_NATIVE_STEP(NativeOnLoaded84, dt::OnLoaded84(base))
ITC_NATIVE_STEP(NativeIsLoading, ctx.r3.u64 = dt::IsLoading(base))
ITC_NATIVE_STEP(NativeItem, ctx.r3.u64 = dt::Item(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeRecordsItem, ctx.r3.u64 = dt::RecordsItem(base, ctx.r4.u32))
ITC_NATIVE_STEP(NativeEntry56, ctx.r3.u64 = dt::Entry56(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeFind20, ctx.r3.s64 = dt::Find20(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeEntry20, ctx.r3.u64 = dt::Entry20(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeEntry40, ctx.r3.u64 = dt::Entry40(base, ctx.r3.u32))

#undef ITC_NATIVE_STEP

// One case through both versions: the area filled, `setup()` writes the
// inputs (the same both times: it's handed a copy of the generator's state),
// `args(t)` sets the registers, the original (flag off) or the native step
// runs, and the area, the global and r3 are compared exactly. The global is
// put back afterwards.
template <typename Setup, typename Args>
void Case(Context& h, Results& results, Setup setup, Args args, GuestFunction recompiled,
          GuestFunction native, const std::string& what) {
  Outputs out[2];
  const Random start = h.random;
  Random after;
  const uint32_t tables_global = guest::LoadU32(h.base, dt::kTables);
  for (int v = 0; v < 2; v++) {
    h.random = start;
    Fill(h.base, h.data, kScratchBytes);
    setup();
    PPCContext t = CallContext(h);
    args(t);
    ITC_FLAG(native_data_tables) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_data_tables) = true;
    out[v].Bytes(Read(h.base, h.data, kScratchBytes));
    out[v].Word(guest::LoadU32(h.base, dt::kTables));
    guest::StoreU32(h.base, dt::kTables, tables_global);
    out[v].Word(t.r3.u32);
    after = h.random;
  }
  h.random = after;
  results.Check(out[0], out[1], what);
}

// A word to swap: often one whose bits read as a float edge value (the
// signaling NaNs first), else random bits.
uint32_t RandomWord(Random& r) {
  static const uint32_t kEdges[] = {
      0x7F800001u, 0x7FBFFFFFu, 0xFF800001u, 0xFFBFFFFFu, 0x7FA00000u, 0x7F812345u,
      0xFF9ABCDEu, 0x7FC00000u, 0xFFC00000u, 0x7FFFFFFFu, 0x7F800000u, 0xFF800000u,
      0x00000001u, 0x80000001u, 0x007FFFFFu, 0x00000000u, 0x80000000u, 0x3F800000u,
  };
  if (r.Below(3) == 0) {
    return kEdges[r.Below(uint32_t(std::size(kEdges)))];
  }
  return uint32_t(r.Next());
}

// Random words, none -1 (an entry's end marker).
void RandomWords(Context& h, uint32_t address, uint32_t count) {
  for (uint32_t i = 0; i < count; i++) {
    uint32_t word = RandomWord(h.random);
    if (word == 0xFFFFFFFFu) {
      word = 0;
    }
    guest::StoreU32(h.base, address + 4 * i, word);
  }
}

void Words(Context& h) {
  Results results("sub_820C97D8 SwapWord");
  for (int i = 0; i < 600; i++) {
    // The first 256: signaling NaNs over their payloads, both signs.
    Case(
        h, results,
        [&] {
          const uint32_t sign = (i & 1) ? 0x80000000u : 0;
          guest::StoreU32(h.base, h.data + 8,
                          i < 256 ? sign | (0x7F800001u + uint32_t(i) * 0x3FC1u)
                                  : RandomWord(h.random));
        },
        [&](PPCContext& t) { t.r3.u64 = h.data + 8; }, __imp__sub_820C97D8, NativeSwapWord,
        fmt::format("case {}", i));
  }
  results.Report();
}

void Entries(Context& h) {
  struct Kind {
    const char* name;
    uint32_t size;
    GuestFunction recompiled;
    GuestFunction native;
  };
  const Kind kKinds[] = {
      {"sub_82111AE0 Entry20::Swap", 20, __imp__sub_82111AE0, NativeSwapEntry20},
      {"sub_82111B30 Entry56::Swap", 56, __imp__sub_82111B30, NativeSwapEntry56},
      {"sub_821128E0 Entry40::Swap", 40, __imp__sub_821128E0, NativeSwapEntry40},
  };
  for (const Kind& kind : kKinds) {
    Results results(kind.name);
    for (int i = 0; i < 200; i++) {
      const uint32_t entry = h.data + 64 + 4 * uint32_t(i % 8);
      Case(
          h, results, [&] { RandomWords(h, entry, kind.size / 4); },
          [&](PPCContext& t) { t.r3.u64 = entry; }, kind.recompiled, kind.native,
          fmt::format("case {}", i));
    }
    results.Report();
  }
}

// The table object at the area's start, its global pointed at it, +4 the
// loads pending.
void SetUpObject(Context& h) {
  guest::StoreU32(h.base, dt::kTables, h.data);
  guest::StoreU32(h.base, h.data + 4, h.random.Below(13));
  guest::StoreU32(h.base, h.data + 8, uint32_t(h.random.Next()));
}

void Callbacks(Context& h) {
  struct Kind {
    const char* name;
    uint32_t size;
    uint32_t end;  // the end marker's offset
    GuestFunction recompiled;
    GuestFunction native;
  };
  const Kind kKinds[] = {
      {"sub_821170F0 DataTables::On56Loaded", 56, 0, __imp__sub_821170F0, NativeOn56Loaded},
      {"sub_82116FF0 DataTables::On20Loaded", 20, 0, __imp__sub_82116FF0, NativeOn20Loaded},
      {"sub_82117058 DataTables::On40Loaded", 40, 4, __imp__sub_82117058, NativeOn40Loaded},
  };
  const uint32_t table = h.data + 64;
  for (const Kind& kind : kKinds) {
    Results results(kind.name);
    for (int i = 0; i < 120; i++) {
      // 0 to 39 entries before the one ending the table (the last cases up
      // to the area's end).
      const uint32_t room = (kScratchBytes - 64) / kind.size - 1;
      const uint32_t entries = i >= 110 ? room - uint32_t(i - 110) : h.random.Below(40);
      Case(
          h, results,
          [&] {
            SetUpObject(h);
            RandomWords(h, table, (entries + 1) * kind.size / 4);
            guest::StoreU32(h.base, table + entries * kind.size + kind.end, 0xFFFFFFFFu);
          },
          [&](PPCContext& t) {
            t.r3.u64 = h.random.Next();
            t.r4.u64 = table;
            t.r6.u64 = table;
          },
          kind.recompiled, kind.native, fmt::format("case {} ({} entries)", i, entries));
    }
    results.Report();
  }
  {
    Results results("sub_821170D8 DataTables::OnItemsLoaded");
    for (int i = 0; i < 50; i++) {
      Case(
          h, results, [&] { SetUpObject(h); },
          [&](PPCContext& t) {
            t.r3.u64 = h.random.Next();
            t.r6.u64 = h.data;
          },
          __imp__sub_821170D8, NativeOnItemsLoaded, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("sub_821170C0 DataTables::OnLoaded84");
    for (int i = 0; i < 50; i++) {
      Case(
          h, results, [&] { SetUpObject(h); },
          [&](PPCContext& t) {
            t.r3.u64 = h.random.Next();
            t.r6.u64 = h.data + 512;
          },
          __imp__sub_821170C0, NativeOnLoaded84, fmt::format("case {}", i));
    }
    results.Report();
  }
}

// The object's words at +0 to +60 random (its tables' pointers), +4 the
// loads pending often 0, the global pointed at it.
void RandomObject(Context& h) {
  guest::StoreU32(h.base, dt::kTables, h.data);
  for (uint32_t offset = 0; offset < 64; offset += 4) {
    guest::StoreU32(h.base, h.data + offset, uint32_t(h.random.Next()));
  }
  if (h.random.Below(2) == 0) {
    guest::StoreU32(h.base, h.data + 4, 0);
  }
}

// An index: the game's range, an edge, or any word; its upper half random
// (the register's, which the originals carry and the game doesn't read).
uint64_t RandomIndex(Random& r, uint32_t count) {
  static const uint32_t kEdges[] = {0, 1, 399, 400, 0x7FFFFFFFu, 0x80000000u, 0xFFFFFFFFu};
  uint32_t low;
  switch (r.Below(3)) {
    case 0:
      low = kEdges[r.Below(uint32_t(std::size(kEdges)))];
      break;
    case 1:
      low = uint32_t(r.Next());
      break;
    default:
      low = r.Below(count);
      break;
  }
  return (r.Below(2) == 0 ? 0 : r.Next() << 32) | low;
}

void Accessors(Context& h) {
  {
    Results results("sub_82117158 DataTables::IsLoading");
    for (int i = 0; i < 100; i++) {
      Case(
          h, results, [&] { RandomObject(h); },
          [&](PPCContext& t) { t.r3.u64 = h.random.Next(); }, __imp__sub_82117158,
          NativeIsLoading, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("sub_82117178 DataTables::Item");
    for (int i = 0; i < 300; i++) {
      Case(
          h, results, [&] { RandomObject(h); },
          [&](PPCContext& t) { t.r3.u64 = RandomIndex(h.random, 400); }, __imp__sub_82117178,
          NativeItem, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("sub_82116DA8 ItemRecords::Item");
    for (int i = 0; i < 300; i++) {
      Case(
          h, results, [&] { RandomObject(h); },
          [&](PPCContext& t) {
            t.r3.u64 = h.random.Next();
            t.r4.u64 = RandomIndex(h.random, 400);
          },
          __imp__sub_82116DA8, NativeRecordsItem, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // Tables -5 to 10 (the object's words), the game's 0 to 7 most often.
    Results results("sub_82117190 DataTables::Entry56");
    for (int i = 0; i < 300; i++) {
      Case(
          h, results, [&] { RandomObject(h); },
          [&](PPCContext& t) {
            const uint32_t table = h.random.Below(3) == 0 ? h.random.Below(16) - 5
                                                          : h.random.Below(8);
            t.r3.u64 = (h.random.Below(2) == 0 ? 0 : h.random.Next() << 32) | table;
            t.r4.u64 = RandomIndex(h.random, 64);
          },
          __imp__sub_82117190, NativeEntry56, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("sub_82117210 DataTables::Entry20");
    for (int i = 0; i < 300; i++) {
      Case(
          h, results, [&] { RandomObject(h); },
          [&](PPCContext& t) { t.r3.u64 = RandomIndex(h.random, 64); }, __imp__sub_82117210,
          NativeEntry20, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("sub_82117228 DataTables::Entry40");
    for (int i = 0; i < 300; i++) {
      Case(
          h, results, [&] { RandomObject(h); },
          [&](PPCContext& t) { t.r3.u64 = RandomIndex(h.random, 64); }, __imp__sub_82117228,
          NativeEntry40, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // A table of 0 to 59 entries at +64 (keys from a few values, so some
    // repeat), searched for keys in it, near it or anywhere.
    Results results("sub_821171B0 DataTables::Find20");
    const uint32_t table = h.data + 64;
    for (int i = 0; i < 400; i++) {
      const uint32_t entries = h.random.Below(60);
      Case(
          h, results,
          [&] {
            RandomObject(h);
            guest::StoreU32(h.base, h.data + 52, table);
            for (uint32_t e = 0; e < entries; e++) {
              const uint32_t entry = table + 20 * e;
              RandomWords(h, entry, 5);
              guest::StoreU32(h.base, entry, h.random.Below(6));
              guest::StoreU32(h.base, entry + 8, h.random.Below(6));
            }
            guest::StoreU32(h.base, table + 20 * entries, 0xFFFFFFFFu);
          },
          [&](PPCContext& t) {
            const bool any = h.random.Below(4) == 0;
            t.r3.u64 = (h.random.Below(2) == 0 ? 0 : h.random.Next() << 32) |
                       (any ? uint32_t(h.random.Next()) : h.random.Below(7));
            t.r4.u64 = (h.random.Below(2) == 0 ? 0 : h.random.Next() << 32) |
                       (any ? uint32_t(h.random.Next()) : h.random.Below(7));
          },
          __imp__sub_821171B0, NativeFind20, fmt::format("case {} ({} entries)", i, entries));
    }
    results.Report();
  }
}

}  // namespace

void DataTables(Context& h) {
  Words(h);
  Entries(h);
  Callbacks(h);
  Accessors(h);
}

}  // namespace harness
