// The other in-race screens' register steps (src/race_screens/,
// docs/functions/race-screens.md), shared by the adapters
// (src/hooks/race_screens.cpp and, for the functions logged by
// --log_pause_menu and --log_pause_movie, src/debug/pause_menu_log.cpp and
// pause_movie_log.cpp; behind --native_race_screens) and the harness
// (without the flag). Each leaves r3 as the original does.

#pragma once

#include "guest.h"

namespace hooks {

// ScreenPages::Init sub_8220BA98 (r3 the page object, r4 the page), Go
// sub_8220BA68 (r4 the page, r5 the transition, r6 the queued one), Finish
// sub_8220BA28 (r4 the result).
void RaceScreensPagesInit(PPCContext& ctx, uint8_t* base);
void RaceScreensPagesGo(PPCContext& ctx, uint8_t* base);
void RaceScreensPagesFinish(PPCContext& ctx, uint8_t* base);
// MenuList:: (r3 the list): Selection sub_8220BC20, SetSelection
// sub_8220BC30 (r4), SetEnabled sub_8220BC00 (r4 the item, r5), IsEnabled
// sub_8220BC10 (r4), Fit sub_8220BC40 (r4 quiet), Fired sub_8220C020 (r4 the
// action), Update sub_8220C1E0, Accepted sub_8220BD50, Cancelled
// sub_8220BDE0, Pressed152 sub_8220BE70, Pressed166 sub_8220BF90.
void RaceScreensListSelection(PPCContext& ctx, uint8_t* base);
void RaceScreensListSetSelection(PPCContext& ctx, uint8_t* base);
void RaceScreensListSetEnabled(PPCContext& ctx, uint8_t* base);
void RaceScreensListIsEnabled(PPCContext& ctx, uint8_t* base);
void RaceScreensListFit(PPCContext& ctx, uint8_t* base);
void RaceScreensListFired(PPCContext& ctx, uint8_t* base);
void RaceScreensListUpdate(PPCContext& ctx, uint8_t* base);
void RaceScreensListAccepted(PPCContext& ctx, uint8_t* base);
void RaceScreensListCancelled(PPCContext& ctx, uint8_t* base);
void RaceScreensListPressed152(PPCContext& ctx, uint8_t* base);
void RaceScreensListPressed166(PPCContext& ctx, uint8_t* base);
// The makers (no arguments): PauseMenu::Create sub_8220D7E8, PauseMenu4
// sub_82210868, AfterRaceMenu sub_8221B1F0, RaceDialog sub_82217D70,
// CareerResults sub_82219138, RaceResults sub_8221F390, SplitResults
// sub_8221FA30, Kind6Menu sub_8221BDD0. The original where the native heap
// isn't active.
void RaceScreensPauseMenuCreate(PPCContext& ctx, uint8_t* base);
void RaceScreensPauseMenu4Create(PPCContext& ctx, uint8_t* base);
void RaceScreensAfterRaceMenuCreate(PPCContext& ctx, uint8_t* base);
void RaceScreensRaceDialogCreate(PPCContext& ctx, uint8_t* base);
void RaceScreensCareerResultsCreate(PPCContext& ctx, uint8_t* base);
void RaceScreensRaceResultsCreate(PPCContext& ctx, uint8_t* base);
void RaceScreensSplitResultsCreate(PPCContext& ctx, uint8_t* base);
void RaceScreensKind6MenuCreate(PPCContext& ctx, uint8_t* base);
// The teardowns (no arguments): PauseMenu::Destroy sub_8220C4C8,
// PauseMenu4 sub_8220FC98, AfterRaceMenu sub_8221A3E8, RaceDialog
// sub_82217CD0, CareerResults sub_82219170, RaceResults sub_8221F3C8,
// SplitResults sub_8221FA68, Kind6Menu sub_8221BE30; AfterRaceMenu::Close
// sub_8221A498. The original where the native heap isn't active or a block
// isn't the layer's.
void RaceScreensPauseMenuDestroy(PPCContext& ctx, uint8_t* base);
void RaceScreensPauseMenu4Destroy(PPCContext& ctx, uint8_t* base);
void RaceScreensAfterRaceMenuDestroy(PPCContext& ctx, uint8_t* base);
void RaceScreensRaceDialogDestroy(PPCContext& ctx, uint8_t* base);
void RaceScreensCareerResultsDestroy(PPCContext& ctx, uint8_t* base);
void RaceScreensRaceResultsDestroy(PPCContext& ctx, uint8_t* base);
void RaceScreensSplitResultsDestroy(PPCContext& ctx, uint8_t* base);
void RaceScreensKind6MenuDestroy(PPCContext& ctx, uint8_t* base);
void RaceScreensAfterRaceMenuClose(PPCContext& ctx, uint8_t* base);
// The pause menu's data (no arguments): LoadData sub_8220F840, SaveOptions
// sub_8220D6A0, Close sub_8220D890, SaveHudOptions sub_8220D740; the answers:
// PauseMenu::AnswerPage sub_8220C5D0, AfterRaceMenu::AnswerPage sub_8221A720,
// RaceDialog::Answer sub_82217D40.
void RaceScreensPauseMenuLoadData(PPCContext& ctx, uint8_t* base);
void RaceScreensPauseMenuSaveOptions(PPCContext& ctx, uint8_t* base);
void RaceScreensPauseMenuClose(PPCContext& ctx, uint8_t* base);
void RaceScreensPauseMenuSaveHudOptions(PPCContext& ctx, uint8_t* base);
void RaceScreensPauseMenuAnswerPage(PPCContext& ctx, uint8_t* base);
void RaceScreensAfterRaceMenuAnswerPage(PPCContext& ctx, uint8_t* base);
void RaceScreensRaceDialogAnswer(PPCContext& ctx, uint8_t* base);
// The pages' dispatch: PauseMenu::Update sub_8220F638 (r3, r4 passed on),
// CareerResults::Update sub_8221A090 (r3 the screen), AfterRaceMenu::Update
// sub_8221BD20, RaceDialog::Update sub_822185B0; CareerResults::Start
// sub_822191D0; PauseMenu::StepVolume sub_8220CED0 and StepAdjust
// sub_8220D248.
void RaceScreensPauseMenuUpdate(PPCContext& ctx, uint8_t* base);
void RaceScreensCareerResultsUpdate(PPCContext& ctx, uint8_t* base);
void RaceScreensAfterRaceMenuUpdate(PPCContext& ctx, uint8_t* base);
void RaceScreensRaceDialogUpdate(PPCContext& ctx, uint8_t* base);
void RaceScreensCareerResultsStart(PPCContext& ctx, uint8_t* base);
void RaceScreensPauseMenuStepVolume(PPCContext& ctx, uint8_t* base);
void RaceScreensPauseMenuStepAdjust(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
