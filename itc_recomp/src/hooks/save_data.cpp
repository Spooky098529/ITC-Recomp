// The players' save data natively: adapters in front of src/save_data/
// (docs/functions/save-data.md), with the register steps the harness shares
// (hooks/save_data.h).

#include "hooks/save_data.h"

#include "base/flags.h"
#include "save_data/save_data.h"

ITC_FLAG_BOOL(native_save_data, true, "ITC",
              "Run the players' save data's set-ups (docs/functions/save-data.md: Stats::Stats "
              "sub_821056C0, Stats::ResetRecords sub_821136D8, Stats::Reset sub_82113730, "
              "Progress::Clear sub_82113BD8, BitSet400::SetAll sub_821133A8 and the default "
              "tables' getters sub_820CCCC0, sub_820CCCD0, sub_820CCCE0, sub_820CCCF0 and "
              "sub_820CCD00) natively instead of through the translated code (the same writes; "
              "checked by --run_harnesses=save_data)");

namespace hooks {

namespace sd = save_data;

void SaveDataStatsConstruct(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  sd::StatsConstruct(base, ctx.r3.u32);
}

void SaveDataStatsResetRecords(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  sd::StatsResetRecords(base, ctx.r3.u32);
}

void SaveDataStatsReset(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = sd::StatsReset(base, ctx.r3.u32);
}

void SaveDataProgressClear(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = sd::ProgressClear(base, ctx.r3.u32);
}

void SaveDataBitSetAll(PPCContext& ctx, uint8_t* base) {
  sd::BitSet400SetAll(base, ctx.r3.u32);
}

#define ITC_SAVE_DATA_TABLE(name)                        \
  void SaveData##name(PPCContext& ctx, uint8_t*) {       \
    ctx.r11.u64 = 0xFFFFFFFF826C0000ull;                 \
    ctx.r3.s64 = int32_t(sd::Defaults##name());          \
  }

ITC_SAVE_DATA_TABLE(Table50A8)
ITC_SAVE_DATA_TABLE(Table3044)
ITC_SAVE_DATA_TABLE(Table3E34)
ITC_SAVE_DATA_TABLE(Table3C24)
ITC_SAVE_DATA_TABLE(Table50E4)

#undef ITC_SAVE_DATA_TABLE

}  // namespace hooks

#define ITC_SAVE_DATA_ADAPTER(address, step) \
  extern "C" REX_FUNC(sub_##address) {       \
    REX_FUNC_PROLOGUE();                     \
    if (!ITC_FLAG(native_save_data)) {       \
      __imp__sub_##address(ctx, base);       \
      return;                                \
    }                                        \
    hooks::step(ctx, base);                  \
  }

ITC_SAVE_DATA_ADAPTER(821056C0, SaveDataStatsConstruct)
ITC_SAVE_DATA_ADAPTER(821136D8, SaveDataStatsResetRecords)
ITC_SAVE_DATA_ADAPTER(82113730, SaveDataStatsReset)
ITC_SAVE_DATA_ADAPTER(82113BD8, SaveDataProgressClear)
ITC_SAVE_DATA_ADAPTER(821133A8, SaveDataBitSetAll)
ITC_SAVE_DATA_ADAPTER(820CCCC0, SaveDataTable50A8)
ITC_SAVE_DATA_ADAPTER(820CCCD0, SaveDataTable3044)
ITC_SAVE_DATA_ADAPTER(820CCCE0, SaveDataTable3E34)
ITC_SAVE_DATA_ADAPTER(820CCCF0, SaveDataTable3C24)
ITC_SAVE_DATA_ADAPTER(820CCD00, SaveDataTable50E4)
