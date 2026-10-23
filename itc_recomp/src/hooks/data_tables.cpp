// The data tables natively: adapters in front of
// src/data_tables/data_tables.cpp (docs/functions/data-tables.md). The
// loads' callbacks are reached through Archive::Load's function pointer, so
// their adapters stay until it's native. SwapWord (sub_820C97D8) and the
// entries' swaps (sub_82111AE0, sub_82111B30, sub_821128E0) have no adapters:
// only the callbacks' plain C++ calls them (the fallback flag switches the
// callbacks).

#include "guest.h"

#include "base/flags.h"
#include "data_tables/data_tables.h"

ITC_FLAG_BOOL(native_data_tables, true, "ITC",
              "Run the data tables (docs/functions/data-tables.md: SwapWord, sub_820C97D8; "
              "the entries' swaps, sub_82111AE0, sub_82111B30 and sub_821128E0; the loads' "
              "callbacks, sub_821170D8, sub_821170C0, sub_821170F0, sub_82116FF0 and "
              "sub_82117058) natively instead of through the translated code (same results; "
              "checked by --run_harnesses=data_tables)");

// The flag's fallback. The swapping adapters then turn flush-to-zero off, as
// the originals leave it (SwapWord's lfs sets it); its f0 (volatile, read by
// no caller) isn't set.
#define ITC_DATA_TABLES_FALLBACK(address)  \
  REX_FUNC_PROLOGUE();                     \
  if (!ITC_FLAG(native_data_tables)) {     \
    __imp__sub_##address(ctx, base);       \
    return;                                \
  }

// An adapter whose original swaps words: r3 as the original leaves it (the
// last word swapped).
#define ITC_DATA_TABLES_SWAP(address, expr) \
  extern "C" REX_FUNC(sub_##address) {     \
    ITC_DATA_TABLES_FALLBACK(address)      \
    ctx.fpscr.disableFlushMode();          \
    ctx.r3.u64 = uint32_t(expr);           \
  }

using namespace data_tables;

ITC_DATA_TABLES_SWAP(821170F0, On56Loaded(base, ctx.r6.u32))
ITC_DATA_TABLES_SWAP(82116FF0, On20Loaded(base, ctx.r6.u32))
ITC_DATA_TABLES_SWAP(82117058, On40Loaded(base, ctx.r6.u32))

extern "C" REX_FUNC(sub_821170D8) {
  ITC_DATA_TABLES_FALLBACK(821170D8)
  OnItemsLoaded(base, ctx.r6.u32);
}
extern "C" REX_FUNC(sub_821170C0) {
  ITC_DATA_TABLES_FALLBACK(821170C0)
  OnLoaded84(base);
}
