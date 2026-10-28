// The players' save data natively: adapters in front of src/save_data/
// (docs/functions/save-data.md), with the register steps the harness shares
// (hooks/save_data.h).

#include "hooks/save_data.h"

#include "base/flags.h"
#include "hooks/heap.h"
#include "save_data/save_data.h"

ITC_FLAG_BOOL(native_save_data, true, "ITC",
              "Run the players' save data's set-ups (docs/functions/save-data.md: Stats::Stats "
              "sub_821056C0, Stats::ResetRecords sub_821136D8, Stats::Reset sub_82113730, "
              "Progress::Clear sub_82113BD8, BitSet400::SetAll sub_821133A8, the default "
              "tables' getters sub_820CCCC0, sub_820CCCD0, sub_820CCCE0, sub_820CCCF0 and "
              "sub_820CCD00, Progress::SetDefaults sub_82113E60 (its copies through memcpy's "
              "plain C++), ControlMap::SetDefault sub_821058A8, Options::SetDefaults "
              "sub_82105940, CourseRecords::Reset sub_82105D08, Players::ResetControllers "
              "sub_8209F5E0 and, with --native_heap, SaveData::Delete sub_821055D8, "
              "SaveData::Create sub_821057C8, MenuList::Init sub_8220BAC0 and "
              "TitleScreen::ResetPlayer sub_82221D00) natively instead of through the translated code (the same writes; "
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

void SaveDataProgressSetDefaults(PPCContext& ctx, uint8_t* base) {
  if (!sd::SetDefaultsNative(ctx.r3.u32)) {
    __imp__sub_82113E60(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = sd::ProgressSetDefaults(base, ctx.r3.u32);
}

void SaveDataControlMapSetDefault(PPCContext& ctx, uint8_t* base) {
  sd::ControlMapSetDefault(base, ctx.r3.u32);
}

void SaveDataOptionsSetDefaults(PPCContext& ctx, uint8_t* base) {
  ctx.r7.u64 = ctx.r3.u64;
  ctx.r3.u64 = sd::OptionsSetDefaults(base, ctx.r3.u32);
}

void SaveDataCourseRecordsReset(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = sd::CourseRecordsReset(base, ctx.r3.u32);
}

void SaveDataDelete(PPCContext& ctx, uint8_t* base) {
  if (hooks::NativeHeapActive()) {
    if (const auto block = sd::SaveDataDelete(base, ctx.r3.u32)) {
      ctx.r3.u64 = *block;
      return;
    }
  }
  __imp__sub_821055D8(ctx, base);
}

void SaveDataCreate(PPCContext& ctx, uint8_t* base) {
  if (hooks::NativeHeapActive() && sd::CreateNative()) {
    ctx.fpscr.disableFlushMode();
    if (const auto holder = sd::SaveDataCreate(base, ctx.r3.u32)) {
      ctx.r3.u64 = *holder;
      return;
    }
  }
  __imp__sub_821057C8(ctx, base);
}

void SaveDataResetControllers(PPCContext& ctx, uint8_t* base) {
  sd::PlayersResetControllers(base);
  ctx.r9.u64 = 3;
  ctx.r10.u64 = 0xFFFFFFFF826C0000ull;
  ctx.r11.u64 = 1;
}

void SaveDataMenuListInit(PPCContext& ctx, uint8_t* base) {
  if (!hooks::NativeHeapActive() || !sd::MenuListInitNative(base, ctx.r3.u32)) {
    __imp__sub_8220BAC0(ctx, base);
    return;
  }
  const sd::MenuListArgs args{ctx.r4.u32,
                              ctx.r5.u32,
                              ctx.r6.u32,
                              ctx.r7.u32,
                              ctx.r8.u32,
                              ctx.r9.u32,
                              ctx.r10.u32,
                              guest::LoadU32(base, ctx.r1.u32 + 84),
                              guest::LoadU32(base, ctx.r1.u32 + 92)};
  ctx.r3.u64 = sd::MenuListInit(base, ctx.r3.u32, args);
}

void SaveDataResetPlayer(PPCContext& ctx, uint8_t* base) {
  if (!hooks::NativeHeapActive() || !sd::ResetPlayerNative(base)) {
    __imp__sub_82221D00(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  if (const auto r3 = sd::TitleScreenResetPlayer(base)) {
    ctx.r3.u64 = *r3;
    return;
  }
  // SaveData::Create's plain C++ refused after the delete (operator new's
  // error paths, never seen): the rest as the original does it.
  ctx.r3.u64 = 0;
  sub_821057C8(ctx, base);
  sub_8209F5E0(ctx, base);
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
ITC_SAVE_DATA_ADAPTER(82113E60, SaveDataProgressSetDefaults)
ITC_SAVE_DATA_ADAPTER(821058A8, SaveDataControlMapSetDefault)
ITC_SAVE_DATA_ADAPTER(82105940, SaveDataOptionsSetDefaults)
ITC_SAVE_DATA_ADAPTER(82105D08, SaveDataCourseRecordsReset)
ITC_SAVE_DATA_ADAPTER(821055D8, SaveDataDelete)
ITC_SAVE_DATA_ADAPTER(821057C8, SaveDataCreate)
ITC_SAVE_DATA_ADAPTER(8209F5E0, SaveDataResetControllers)
ITC_SAVE_DATA_ADAPTER(8220BAC0, SaveDataMenuListInit)
ITC_SAVE_DATA_ADAPTER(82221D00, SaveDataResetPlayer)
