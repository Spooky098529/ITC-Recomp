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
// Progress::SetDefaults sub_82113E60: r3 = the save data; r3 = the unlock
// set (+0x1ACE4). The original where memcpy's plain C++ can't copy.
void SaveDataProgressSetDefaults(PPCContext& ctx, uint8_t* base);
// ControlMap::SetDefault sub_821058A8: r3 = the map; r3 left.
void SaveDataControlMapSetDefault(PPCContext& ctx, uint8_t* base);
// Options::SetDefaults sub_82105940: r3 = the options; r3 = the options +
// 224, r7 the options (the original's).
void SaveDataOptionsSetDefaults(PPCContext& ctx, uint8_t* base);
// CourseRecords::Reset sub_82105D08: r3 = the records; r3 = the last
// record + 332.
void SaveDataCourseRecordsReset(PPCContext& ctx, uint8_t* base);
// SaveData::Delete sub_821055D8: r3 = the player; r3 = the old block. The
// original unless --native_heap routes the game's heap to the allocation
// layer, or where the block isn't the layer's.
void SaveDataDelete(PPCContext& ctx, uint8_t* base);
// The default tables' getters: r3 (and r11, 0x826C0000) the table, sign
// extended as lis and addi leave them.
void SaveDataTable50A8(PPCContext& ctx, uint8_t* base);
void SaveDataTable3044(PPCContext& ctx, uint8_t* base);
void SaveDataTable3E34(PPCContext& ctx, uint8_t* base);
void SaveDataTable3C24(PPCContext& ctx, uint8_t* base);
void SaveDataTable50E4(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
