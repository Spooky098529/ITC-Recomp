// The players' save data's register steps (src/save_data/,
// docs/functions/save-data.md), shared by the adapters
// (src/hooks/save_data.cpp, behind --native_save_data) and the harness
// (without the flag). Each leaves r3 as the original does.

#pragma once

#include "guest.h"

namespace hooks {

// Stats::Stats sub_821056C0, Stats::ResetRecords sub_821136D8: r3 = the
// record; r3 left.
void SaveDataStatsConstruct(PPCContext& ctx, uint8_t* base);
void SaveDataStatsResetRecords(PPCContext& ctx, uint8_t* base);
// Stats::Reset sub_82113730: r3 = the record; r3 = the record + 4948.
void SaveDataStatsReset(PPCContext& ctx, uint8_t* base);
// Progress::Clear sub_82113BD8: r3 = the save data; r3 = Stats::Reset's.
void SaveDataProgressClear(PPCContext& ctx, uint8_t* base);
// BitSet400::SetAll sub_821133A8: r3 = the set; r3 left.
void SaveDataBitSetAll(PPCContext& ctx, uint8_t* base);
// The default tables' getters: r3 (and r11, 0x826C0000) the table, sign
// extended as lis and addi leave them.
void SaveDataTable50A8(PPCContext& ctx, uint8_t* base);
void SaveDataTable3044(PPCContext& ctx, uint8_t* base);
void SaveDataTable3E34(PPCContext& ctx, uint8_t* base);
void SaveDataTable3C24(PPCContext& ctx, uint8_t* base);
void SaveDataTable50E4(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
