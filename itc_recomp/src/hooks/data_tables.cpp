// The data tables natively: adapters in front of
// src/data_tables/data_tables.cpp (docs/functions/data-tables.md). The
// loads' callbacks are reached through Archive::Load's function pointer, so
// their adapters stay until it's native. SwapWord (sub_820C97D8) and the
// entries' swaps (sub_82111AE0, sub_82111B30, sub_821128E0) have no adapters:
// only the callbacks' plain C++ calls them (the fallback flag switches the
// callbacks).

#include "guest.h"
#include "lifted/originals.h"
#include "hooks/native_entry.h"

#include "base/flags.h"
#include "data_tables/data_tables.h"
#include "hooks/app_start_up.h"
#include "hooks/archive.h"
#include "hooks/heap.h"

ITC_FLAG_BOOL(native_data_tables, true, "ITC",
              "Run the data tables (docs/functions/data-tables.md: SwapWord, sub_820C97D8; "
              "the entries' swaps, sub_82111AE0, sub_82111B30 and sub_821128E0; the loads' "
              "callbacks, sub_821170D8, sub_821170C0, sub_821170F0, sub_82116FF0 and "
              "sub_82117058; the accessors, sub_82117158, sub_82117178, sub_82116DA8, "
              "sub_82117190, sub_821171B0, sub_82117210 and sub_82117228; with --native_heap, "
              "DataTables::Init sub_82116DB0, and with the archive's queue native, "
              "DataTables::Load sub_82117240) natively instead of through the translated code (same results; "
              "checked by --run_harnesses=data_tables)");

// The flag's fallback. The swapping adapters then turn flush-to-zero off, as
// the originals leave it (SwapWord's lfs sets it); its f0 (volatile, read by
// no caller) isn't set.
#define ITC_DATA_TABLES_FALLBACK(address)  \
  REX_FUNC_PROLOGUE();                     \
  if (!ITC_FLAG(native_data_tables)) {     \
    lifted::original::c##address(ctx); \
    return;                                \
  }

// An adapter whose original swaps words: r3 as the original leaves it (the
// last word swapped).
#define ITC_DATA_TABLES_SWAP(address, expr) \
  ITC_NATIVE_ENTRY_ALWAYS(address) {     \
    ITC_DATA_TABLES_FALLBACK(address)      \
    ctx.fpscr.disableFlushMode();          \
    ctx.r3.u64 = uint32_t(expr);           \
  }

using namespace data_tables;

ITC_DATA_TABLES_SWAP(821170F0, On56Loaded(ctx.r6.u32))
ITC_DATA_TABLES_SWAP(82116FF0, On20Loaded(ctx.r6.u32))
ITC_DATA_TABLES_SWAP(82117058, On40Loaded(ctx.r6.u32))

ITC_NATIVE_ENTRY_ALWAYS(821170D8) {
  ITC_DATA_TABLES_FALLBACK(821170D8)
  OnItemsLoaded(ctx.r6.u32);
}
ITC_NATIVE_ENTRY_ALWAYS(821170C0) {
  ITC_DATA_TABLES_FALLBACK(821170C0)
  OnLoaded84();
}

// The accessors: r3 a word (zero-extended; the originals' carry the
// arguments' upper halves, which the game's 32-bit code doesn't read), or
// Find20's index (-1 sign-extended, as the original's li).
#define ITC_DATA_TABLES_WORD(address, expr) \
  ITC_NATIVE_ENTRY_ALWAYS(address) {     \
    ITC_DATA_TABLES_FALLBACK(address)      \
    ctx.r3.u64 = uint32_t(expr);           \
  }

ITC_DATA_TABLES_WORD(82117158, IsLoading())
ITC_DATA_TABLES_WORD(82117178, Item(ctx.r3.u32))
// ItemRecords::Item (probably): r3 (the records) not read, r4 the id.
ITC_DATA_TABLES_WORD(82116DA8, RecordsItem(ctx.r4.u32))
ITC_DATA_TABLES_WORD(82117190, Entry56(ctx.r3.u32, ctx.r4.u32))
ITC_DATA_TABLES_WORD(82117210, Entry20(ctx.r3.u32))
ITC_DATA_TABLES_WORD(82117228, Entry40(ctx.r3.u32))

ITC_NATIVE_ENTRY_ALWAYS(821171B0) {
  ITC_DATA_TABLES_FALLBACK(821171B0)
  ctx.r3.s64 = Find20(ctx.r3.u32, ctx.r4.u32);
}

// DataTables::Init: r3 as the original leaves it (Archive::Toc's, lwz's).
ITC_NATIVE_ENTRY_ALWAYS(82116DB0) {
  
  if (hooks::StartUpRecorded(ctx, 0x82116DB0)) {
    return;
  }
  if (ITC_FLAG(native_data_tables) && hooks::NativeHeapActive()) {
    if (const auto r3 = Init()) {
      ctx.r3.u64 = *r3;
      return;
    }
  }
  lifted::original::c82116DB0(ctx);
}

// DataTables::Load: the loads through the archive's queue's plain C++.
ITC_NATIVE_ENTRY_ALWAYS(82117240) {
  
  if (hooks::StartUpRecorded(ctx, 0x82117240)) {
    return;
  }
  if (!ITC_FLAG(native_data_tables) || !hooks::NativeArchiveQueue()) {
    lifted::original::c82117240(ctx);
    return;
  }
  hooks::SetArchiveSerial(ctx, Load(hooks::NativeArchiveLoad()));
}
