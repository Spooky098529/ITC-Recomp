// The build screen's register steps (src/build_screen/,
// docs/functions/build-screen.md), shared by the adapters
// (src/hooks/build_screen.cpp, behind --native_build_screen) and the
// harness (without the flag). Each leaves r3 as the original does. Steps
// whose functions have no adapter (only native callers reach them) are the
// harness's alone: InitBuildList, CopyBuilds, ListBuilds, DrawBuildRows,
// SetUpGroups, FilterGroups, MarkGroups, InitGroupLists, DrawGroupRows,
// ShowGroup, ShowNumber and GroupArrows.

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

// BuildScreen::GroupArrows sub_8219D1B0 and BuildArrows sub_8219D4B8 (r3 =
// the scroll, r4 = the count), StandArrows sub_8219D7B0, ShowBuildMark
// sub_8219D440 (r3 = a build), SetPrompts sub_8219DA58 (r3 to r8),
// SetHelp sub_8219DB50 (r3 = a string id), ShowNumber sub_8219D090 (r3 =
// the number), DrawGroupRows sub_8219FA28.
void BuildScreenGroupArrows(PPCContext& ctx, uint8_t* base);
void BuildScreenBuildArrows(PPCContext& ctx, uint8_t* base);
void BuildScreenStandArrows(PPCContext& ctx, uint8_t* base);
void BuildScreenShowBuildMark(PPCContext& ctx, uint8_t* base);
void BuildScreenSetPrompts(PPCContext& ctx, uint8_t* base);
void BuildScreenSetHelp(PPCContext& ctx, uint8_t* base);
void BuildScreenShowNumber(PPCContext& ctx, uint8_t* base);
void BuildScreenDrawGroupRows(PPCContext& ctx, uint8_t* base);

// The transitions (r3 = the step word): HighlightGroups sub_8219C528,
// HighlightGroups2 sub_8219C6D0, FadeOut sub_8219C850, HighlightBuilds
// sub_8219C8F8, HighlightStand sub_8219CA08, HighlightStand2 sub_8219CB18,
// HighlightDetails sub_8219CBD8.
void BuildScreenHighlightGroupsA(PPCContext& ctx, uint8_t* base);
void BuildScreenHighlightGroupsB(PPCContext& ctx, uint8_t* base);
void BuildScreenFadeOut(PPCContext& ctx, uint8_t* base);
void BuildScreenHighlightBuilds(PPCContext& ctx, uint8_t* base);
void BuildScreenHighlightStand(PPCContext& ctx, uint8_t* base);
void BuildScreenHighlightStand2(PPCContext& ctx, uint8_t* base);
void BuildScreenHighlightDetails(PPCContext& ctx, uint8_t* base);

// The builds level: InitBuildList sub_8219BFE8, CopyBuilds sub_821A0128 (r3
// = a group's index), ListBuilds sub_821A0D38 (r3 = a group's index),
// DrawBuildRows sub_8219D3E8, DrawStandRow sub_8219D550 (r3 = a row, r4 = a
// position), ShowGroup sub_8219CF08 (r3 = a group), SetUpBuilds
// sub_821A1008. InitBuildList, CopyBuilds, ListBuilds and DrawBuildRows
// have no adapters (the harness's steps).
void BuildScreenInitBuildList(PPCContext& ctx, uint8_t* base);
void BuildScreenCopyBuilds(PPCContext& ctx, uint8_t* base);
void BuildScreenListBuilds(PPCContext& ctx, uint8_t* base);
void BuildScreenDrawBuildRows(PPCContext& ctx, uint8_t* base);
void BuildScreenDrawStandRow(PPCContext& ctx, uint8_t* base);
void BuildScreenShowGroup(PPCContext& ctx, uint8_t* base);
void BuildScreenSetUpBuilds(PPCContext& ctx, uint8_t* base);

// The groups level: SetUpGroups sub_821A0060; the transitions (r3 = the
// step word) Open sub_821A08D8, BackToGroups sub_821A0A38, ChangeCategory
// sub_8219FC60.
void BuildScreenSetUpGroups(PPCContext& ctx, uint8_t* base);
void BuildScreenOpen(PPCContext& ctx, uint8_t* base);
void BuildScreenBackToGroups(PPCContext& ctx, uint8_t* base);
void BuildScreenChangeCategory(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
