// The build screen's register steps (src/build_screen/,
// docs/functions/build-screen.md), shared by the adapters
// (src/hooks/build_screen.cpp, behind --native_build_screen) and the
// harness (without the flag). Each leaves r3 as the original does. Steps
// whose functions have no adapter (only native callers reach them) are the
// harness's alone: InitBuildList, CopyBuilds, ListBuilds, DrawBuildRows,
// SetUpGroups, FilterGroups, MarkGroups, InitGroupLists, DrawGroupRows,
// ShowGroup, ShowNumber, GroupArrows, SetUpDetails, ShowDetails,
// KeepGroupList, KeepStandList, RestoreStandList, ValueText, BuildArrows,
// ShowBuildMark and RestoreStand.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// No arguments: BuildScreen::KeepGroupList sub_8219BA50, KeepBuildList
// sub_8219BB20, KeepStandList sub_8219BBF0, RestoreBuildList sub_8219BB80,
// RestoreStandList sub_8219BC50, InitGroupLists sub_8219BF08,
// InitStandList sub_8219C078, MarkGroups sub_8219EC50, FilterGroups
// sub_8219CC70.
void BuildScreenKeepGroupList(lifted::Regs& ctx);
void BuildScreenKeepBuildList(lifted::Regs& ctx);
void BuildScreenKeepStandList(lifted::Regs& ctx);
void BuildScreenRestoreBuildList(lifted::Regs& ctx);
void BuildScreenRestoreStandList(lifted::Regs& ctx);
void BuildScreenInitGroupLists(lifted::Regs& ctx);
void BuildScreenInitStandList(lifted::Regs& ctx);
void BuildScreenMarkGroups(lifted::Regs& ctx);
void BuildScreenFilterGroups(lifted::Regs& ctx);
// BuildScreen::ValueText sub_8219BCC0: r3 = a value.
void BuildScreenValueText(lifted::Regs& ctx);
// BuildList::Insert sub_8219C230: r3 = a build list, r4 = a pointer's
// address, r5 = a position.
void BuildListInsert(lifted::Regs& ctx);
// GarageStage::SetDark sub_8217D0C8: r3 = the stage's holder, r4 = dark.
void GarageStageSetDark(lifted::Regs& ctx);

// BuildScreen::GroupArrows sub_8219D1B0 and BuildArrows sub_8219D4B8 (r3 =
// the scroll, r4 = the count), StandArrows sub_8219D7B0, ShowBuildMark
// sub_8219D440 (r3 = a build), SetPrompts sub_8219DA58 (r3 to r8),
// SetHelp sub_8219DB50 (r3 = a string id), ShowNumber sub_8219D090 (r3 =
// the number), DrawGroupRows sub_8219FA28.
void BuildScreenGroupArrows(lifted::Regs& ctx);
void BuildScreenBuildArrows(lifted::Regs& ctx);
void BuildScreenStandArrows(lifted::Regs& ctx);
void BuildScreenShowBuildMark(lifted::Regs& ctx);
void BuildScreenSetPrompts(lifted::Regs& ctx);
void BuildScreenSetHelp(lifted::Regs& ctx);
void BuildScreenShowNumber(lifted::Regs& ctx);
void BuildScreenDrawGroupRows(lifted::Regs& ctx);

// The transitions (r3 = the step word): HighlightGroups sub_8219C528,
// HighlightGroups2 sub_8219C6D0, FadeOut sub_8219C850, HighlightBuilds
// sub_8219C8F8, HighlightStand sub_8219CA08, HighlightStand2 sub_8219CB18,
// HighlightDetails sub_8219CBD8.
void BuildScreenHighlightGroupsA(lifted::Regs& ctx);
void BuildScreenHighlightGroupsB(lifted::Regs& ctx);
void BuildScreenFadeOut(lifted::Regs& ctx);
void BuildScreenHighlightBuilds(lifted::Regs& ctx);
void BuildScreenHighlightStand(lifted::Regs& ctx);
void BuildScreenHighlightStand2(lifted::Regs& ctx);
void BuildScreenHighlightDetails(lifted::Regs& ctx);

// The builds level: InitBuildList sub_8219BFE8, CopyBuilds sub_821A0128 (r3
// = a group's index), ListBuilds sub_821A0D38 (r3 = a group's index),
// DrawBuildRows sub_8219D3E8, DrawStandRow sub_8219D550 (r3 = a row, r4 = a
// position), ShowGroup sub_8219CF08 (r3 = a group), SetUpBuilds
// sub_821A1008. InitBuildList, CopyBuilds, ListBuilds and DrawBuildRows
// have no adapters (the harness's steps).
void BuildScreenInitBuildList(lifted::Regs& ctx);
void BuildScreenCopyBuilds(lifted::Regs& ctx);
void BuildScreenListBuilds(lifted::Regs& ctx);
void BuildScreenDrawBuildRows(lifted::Regs& ctx);
void BuildScreenDrawStandRow(lifted::Regs& ctx);
void BuildScreenShowGroup(lifted::Regs& ctx);
void BuildScreenSetUpBuilds(lifted::Regs& ctx);

// The groups level: SetUpGroups sub_821A0060; the transitions (r3 = the
// step word) Open sub_821A08D8, BackToGroups sub_821A0A38, ChangeCategory
// sub_8219FC60.
void BuildScreenSetUpGroups(lifted::Regs& ctx);
void BuildScreenOpen(lifted::Regs& ctx);
void BuildScreenBackToGroups(lifted::Regs& ctx);
void BuildScreenChangeCategory(lifted::Regs& ctx);
// The transition OpenBuilds sub_821A1138 (r3 = the step word).
void BuildScreenOpenBuilds(lifted::Regs& ctx);
// The stand: RestoreStand sub_8219F868; the transition BackToStand
// sub_8219FF78 (r3 = the step word).
void BuildScreenRestoreStand(lifted::Regs& ctx);
void BuildScreenBackToStand(lifted::Regs& ctx);
// The details: the transition OpenDetails sub_8219FEC8 (r3 = the step
// word), SetUpDetails sub_8219F8E0 and ShowDetails sub_8219D820 (r3 = a
// build; neither has an adapter: only OpenDetails' plain C++ reaches them).
void BuildScreenOpenDetails(lifted::Regs& ctx);
void BuildScreenSetUpDetails(lifted::Regs& ctx);
void BuildScreenShowDetails(lifted::Regs& ctx);

// BuildScreen::PutOnStand sub_8219D6F0 (r3 the build's index, r4 its +32 &
// 2), SetUpStand sub_8219F780 and OpenStand sub_8219FE10 (r3 the step
// word): GarageStage::ReloadModel's plain C++ where its adapter would run
// it, else its adapter.
void BuildScreenPutOnStand(lifted::Regs& ctx);
void BuildScreenSetUpStand(lifted::Regs& ctx);
void BuildScreenOpenStand(lifted::Regs& ctx);

}  // namespace hooks
