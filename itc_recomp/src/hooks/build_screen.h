// The build screen's register steps (src/build_screen/,
// docs/functions/build-screen.md), shared by the adapters
// (src/hooks/build_screen.cpp, behind --native_build_screen) and the
// harness (without the flag). Each leaves r3 as the original does.

#pragma once

#include "guest.h"

namespace hooks {

// No arguments: BuildScreen::KeepGroupList sub_8219BA50, KeepBuildList
// sub_8219BB20, KeepStandList sub_8219BBF0, RestoreBuildList sub_8219BB80,
// RestoreStandList sub_8219BC50, InitGroupLists sub_8219BF08,
// InitStandList sub_8219C078, MarkGroups sub_8219EC50, FilterGroups
// sub_8219CC70.
void BuildScreenKeepGroupList(PPCContext& ctx, uint8_t* base);
void BuildScreenKeepBuildList(PPCContext& ctx, uint8_t* base);
void BuildScreenKeepStandList(PPCContext& ctx, uint8_t* base);
void BuildScreenRestoreBuildList(PPCContext& ctx, uint8_t* base);
void BuildScreenRestoreStandList(PPCContext& ctx, uint8_t* base);
void BuildScreenInitGroupLists(PPCContext& ctx, uint8_t* base);
void BuildScreenInitStandList(PPCContext& ctx, uint8_t* base);
void BuildScreenMarkGroups(PPCContext& ctx, uint8_t* base);
void BuildScreenFilterGroups(PPCContext& ctx, uint8_t* base);
// BuildScreen::ValueText sub_8219BCC0: r3 = a value.
void BuildScreenValueText(PPCContext& ctx, uint8_t* base);
// BuildList::Insert sub_8219C230: r3 = a build list, r4 = a pointer's
// address, r5 = a position.
void BuildListInsert(PPCContext& ctx, uint8_t* base);
// GarageStage::SetDark sub_8217D0C8: r3 = the stage's holder, r4 = dark.
void GarageStageSetDark(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
