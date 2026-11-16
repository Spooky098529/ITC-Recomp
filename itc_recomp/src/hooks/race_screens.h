// The other in-race screens' register steps (src/race_screens/,
// docs/functions/race-screens.md), shared by the adapters
// (src/hooks/race_screens.cpp and, for the functions logged by
// --log_pause_menu and --log_pause_movie, src/debug/pause_menu_log.cpp and
// pause_movie_log.cpp; behind --native_race_screens) and the harness
// (without the flag). Each leaves r3 as the original does.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// ScreenPages::Init sub_8220BA98 (r3 the page object, r4 the page), Go
// sub_8220BA68 (r4 the page, r5 the transition, r6 the queued one), Finish
// sub_8220BA28 (r4 the result).
void RaceScreensPagesInit(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPagesGo(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPagesFinish(lifted::Regs& ctx, uint8_t* base);
// MenuList:: (r3 the list): Selection sub_8220BC20, SetSelection
// sub_8220BC30 (r4), SetEnabled sub_8220BC00 (r4 the item, r5), IsEnabled
// sub_8220BC10 (r4), Fit sub_8220BC40 (r4 quiet), Fired sub_8220C020 (r4 the
// action), Update sub_8220C1E0, Accepted sub_8220BD50, Cancelled
// sub_8220BDE0, Pressed152 sub_8220BE70, Pressed166 sub_8220BF90.
void RaceScreensListSelection(lifted::Regs& ctx, uint8_t* base);
void RaceScreensListSetSelection(lifted::Regs& ctx, uint8_t* base);
void RaceScreensListSetEnabled(lifted::Regs& ctx, uint8_t* base);
void RaceScreensListIsEnabled(lifted::Regs& ctx, uint8_t* base);
void RaceScreensListFit(lifted::Regs& ctx, uint8_t* base);
void RaceScreensListFired(lifted::Regs& ctx, uint8_t* base);
void RaceScreensListUpdate(lifted::Regs& ctx, uint8_t* base);
void RaceScreensListAccepted(lifted::Regs& ctx, uint8_t* base);
void RaceScreensListCancelled(lifted::Regs& ctx, uint8_t* base);
void RaceScreensListPressed152(lifted::Regs& ctx, uint8_t* base);
void RaceScreensListPressed166(lifted::Regs& ctx, uint8_t* base);
// The makers (no arguments): PauseMenu::Create sub_8220D7E8, PauseMenu4
// sub_82210868, AfterRaceMenu sub_8221B1F0, RaceDialog sub_82217D70,
// CareerResults sub_82219138, RaceResults sub_8221F390, SplitResults
// sub_8221FA30, Kind6Menu sub_8221BDD0. The original where the native heap
// isn't active.
void RaceScreensPauseMenuCreate(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenu4Create(lifted::Regs& ctx, uint8_t* base);
void RaceScreensAfterRaceMenuCreate(lifted::Regs& ctx, uint8_t* base);
void RaceScreensRaceDialogCreate(lifted::Regs& ctx, uint8_t* base);
void RaceScreensCareerResultsCreate(lifted::Regs& ctx, uint8_t* base);
void RaceScreensRaceResultsCreate(lifted::Regs& ctx, uint8_t* base);
void RaceScreensSplitResultsCreate(lifted::Regs& ctx, uint8_t* base);
void RaceScreensKind6MenuCreate(lifted::Regs& ctx, uint8_t* base);
// The teardowns (no arguments): PauseMenu::Destroy sub_8220C4C8,
// PauseMenu4 sub_8220FC98, AfterRaceMenu sub_8221A3E8, RaceDialog
// sub_82217CD0, CareerResults sub_82219170, RaceResults sub_8221F3C8,
// SplitResults sub_8221FA68, Kind6Menu sub_8221BE30; AfterRaceMenu::Close
// sub_8221A498. The original where the native heap isn't active or a block
// isn't the layer's.
void RaceScreensPauseMenuDestroy(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenu4Destroy(lifted::Regs& ctx, uint8_t* base);
void RaceScreensAfterRaceMenuDestroy(lifted::Regs& ctx, uint8_t* base);
void RaceScreensRaceDialogDestroy(lifted::Regs& ctx, uint8_t* base);
void RaceScreensCareerResultsDestroy(lifted::Regs& ctx, uint8_t* base);
void RaceScreensRaceResultsDestroy(lifted::Regs& ctx, uint8_t* base);
void RaceScreensSplitResultsDestroy(lifted::Regs& ctx, uint8_t* base);
void RaceScreensKind6MenuDestroy(lifted::Regs& ctx, uint8_t* base);
void RaceScreensAfterRaceMenuClose(lifted::Regs& ctx, uint8_t* base);
// The pause menu's data (no arguments): LoadData sub_8220F840, SaveOptions
// sub_8220D6A0, Close sub_8220D890, SaveHudOptions sub_8220D740; the answers:
// PauseMenu::AnswerPage sub_8220C5D0, AfterRaceMenu::AnswerPage sub_8221A720,
// RaceDialog::Answer sub_82217D40.
void RaceScreensPauseMenuLoadData(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuSaveOptions(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuClose(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuSaveHudOptions(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuAnswerPage(lifted::Regs& ctx, uint8_t* base);
void RaceScreensAfterRaceMenuAnswerPage(lifted::Regs& ctx, uint8_t* base);
void RaceScreensRaceDialogAnswer(lifted::Regs& ctx, uint8_t* base);
// The pages' dispatch: PauseMenu::Update sub_8220F638 (r3, r4 passed on),
// CareerResults::Update sub_8221A090 (r3 the screen), AfterRaceMenu::Update
// sub_8221BD20, RaceDialog::Update sub_822185B0; CareerResults::Start
// sub_822191D0; PauseMenu::StepVolume sub_8220CED0 and StepAdjust
// sub_8220D248.
void RaceScreensPauseMenuUpdate(lifted::Regs& ctx, uint8_t* base);
void RaceScreensCareerResultsUpdate(lifted::Regs& ctx, uint8_t* base);
void RaceScreensAfterRaceMenuUpdate(lifted::Regs& ctx, uint8_t* base);
void RaceScreensRaceDialogUpdate(lifted::Regs& ctx, uint8_t* base);
// RaceDialog::SetText sub_82218838: r3 = the string id, r4 to r10 the
// varargs.
void RaceScreensRaceDialogSetText(lifted::Regs& ctx, uint8_t* base);
// RaceDialog::Open sub_82218B30: r3 = the mode, r4 = the player.
void RaceScreensRaceDialogOpen(lifted::Regs& ctx, uint8_t* base);
void RaceScreensCareerResultsStart(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuStepVolume(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuStepAdjust(lifted::Regs& ctx, uint8_t* base);
// The pause menu's transitions (r3 the step word): RunTen sub_8220C5E0,
// BackToMain sub_8220C668, CloseMain sub_8220C700, MoveMain sub_8220C7B8,
// ShowOptionsRow sub_8220C868, ToggleOptions sub_8220C930, ShowVolumeRow
// sub_8220CA28, ShowAdjustRow sub_8220CAB0; ConfirmPage sub_8220C578 (r4
// passed on); ShowItems sub_8220CB38, SetArrowA sub_8220CBF8 (r3 the value),
// ShowOptions sub_8220CC60.
void RaceScreensPauseMenuRunTen(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuBackToMain(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuCloseMain(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuMoveMain(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuShowOptionsRow(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuToggleOptions(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuShowVolumeRow(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuShowAdjustRow(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuConfirmPage(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuShowItems(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuSetArrowA(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuShowOptions(lifted::Regs& ctx, uint8_t* base);
// ShowVolumes sub_8220D0B8, ShowSlider sub_8220D130 (r3 the slider, r4 the
// value), RestoreNodes sub_8220D5D0 (r3 the group, r4 the child), the
// transitions MoveOptions sub_8220DD98, MoveAdjust sub_8220E1F8 and ShowMain
// sub_8220EE28 (r3 the step word), ColorItems sub_8220E700, ShowAdjust
// sub_8220E888, Open sub_8220FA88 (r3 the screen, r4 the kind, r5 the flag).
void RaceScreensPauseMenuShowVolumes(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuShowSlider(lifted::Regs& ctx, uint8_t* base);
// PauseMenu::ShowVolume sub_8220D040 (never seen in play): its wrap in
// src/debug/pause_movie_log.cpp runs this with --native_race_screens.
void RaceScreensPauseMenuShowVolume(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuRestoreNodes(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuMoveOptions(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuMoveAdjust(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuShowMain(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuColorItems(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuShowAdjust(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuOpen(lifted::Regs& ctx, uint8_t* base);
// The options page: ToggleOption sub_8220CDA0, OptionsPage sub_8220DB98,
// SetUpOptions sub_8220E530, SetAdjust sub_8220F1B8 (r3 the step word).
void RaceScreensPauseMenuToggleOption(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuOptionsPage(lifted::Regs& ctx, uint8_t* base);
// The main pages: PauseMenu::MainPage sub_8220D898, AfterRaceMenu::MainPage
// sub_8221A4A8 (r3 as the original leaves it: 0).
void RaceScreensPauseMenuMainPage(lifted::Regs& ctx, uint8_t* base);
void RaceScreensAfterRaceMenuMainPage(lifted::Regs& ctx, uint8_t* base);
// PauseMenu::AdjustPage sub_8220EBD8, ShowCareer sub_8220D418, SetUpMain
// sub_8220E418, ShowVolumesB sub_8220E810, ShowAdjustB sub_8220E908 (no
// arguments; r3 as the original leaves it).
void RaceScreensPauseMenuAdjustPage(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuShowCareer(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuSetUpMain(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuShowVolumesB(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuShowAdjustB(lifted::Regs& ctx, uint8_t* base);
// PauseMenu::SetUpVolume sub_8220F3C0 and SetUpAdjust sub_8220F500 (no
// arguments); the transitions OpenMain sub_8220ED90, CloseOptions
// sub_8220EFA8, CloseVolume sub_8220F0A0 and CloseAdjust sub_8220F2A8 (r3
// the step word).
void RaceScreensPauseMenuSetUpVolume(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuSetUpAdjust(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuOpenMain(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuCloseOptions(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuCloseVolume(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuCloseAdjust(lifted::Regs& ctx, uint8_t* base);
// The transitions OpenVolume sub_8220F6D0 and OpenAdjust sub_8220F788 (r3
// the step word); VolumePage sub_8220E988 (no arguments).
void RaceScreensPauseMenuOpenVolume(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuOpenAdjust(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuVolumePage(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuSetUpOptions(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuSetAdjust(lifted::Regs& ctx, uint8_t* base);
// The menu after the race: ConfirmPage sub_8221A6C8 (r4 passed on), the
// transitions Intro sub_8221A730, Show sub_8221A7A8, CloseMain sub_8221A950,
// Move sub_8221AA20 (r3 the step word), ShowItems sub_8221AC00, SetUpItems
// sub_8221B5C8, ColorItems sub_8221B768 (r3 the screen), Start
// sub_8221B860, Open sub_8221BA68 (r3 the screen, r4 the kind, r5 the flag).
void RaceScreensAfterRaceMenuConfirmPage(lifted::Regs& ctx, uint8_t* base);
void RaceScreensAfterRaceMenuIntro(lifted::Regs& ctx, uint8_t* base);
void RaceScreensAfterRaceMenuShow(lifted::Regs& ctx, uint8_t* base);
void RaceScreensAfterRaceMenuCloseMain(lifted::Regs& ctx, uint8_t* base);
void RaceScreensAfterRaceMenuMove(lifted::Regs& ctx, uint8_t* base);
void RaceScreensAfterRaceMenuShowItems(lifted::Regs& ctx, uint8_t* base);
void RaceScreensAfterRaceMenuSetUpItems(lifted::Regs& ctx, uint8_t* base);
void RaceScreensAfterRaceMenuColorItems(lifted::Regs& ctx, uint8_t* base);
void RaceScreensAfterRaceMenuStart(lifted::Regs& ctx, uint8_t* base);
void RaceScreensAfterRaceMenuOpen(lifted::Regs& ctx, uint8_t* base);
// The confirm dialog: Question sub_82217F40, the transitions OpenAnim
// sub_822186A8 and Move sub_82218788 (r3 the step word), Draw sub_82218F30.
// RaceDialog::ShowSaving sub_82218AB0 (an adapter: the recompiled
// AfterRaceMenu::ListPage reaches it), StartSave sub_822189B8 and SetNote
// sub_82218918 (r3 the id, r4 to r10 the varargs; no adapters).
void RaceScreensRaceDialogShowSaving(lifted::Regs& ctx, uint8_t* base);
void RaceScreensRaceDialogStartSave(lifted::Regs& ctx, uint8_t* base);
void RaceScreensRaceDialogSetNote(lifted::Regs& ctx, uint8_t* base);
void RaceScreensRaceDialogQuestion(lifted::Regs& ctx, uint8_t* base);
void RaceScreensRaceDialogOpenAnim(lifted::Regs& ctx, uint8_t* base);
void RaceScreensRaceDialogMove(lifted::Regs& ctx, uint8_t* base);
// ShowMessage sub_82218C10 (transition 0, r3 the step word; the original for
// the messages 0 to 11).
void RaceScreensRaceDialogShowMessage(lifted::Regs& ctx, uint8_t* base);
void RaceScreensRaceDialogDraw(lifted::Regs& ctx, uint8_t* base);
// PauseMenu::OpenOptions sub_8220EEF0 (r3 the step word); the draws (r3 the
// screen): PauseMenu::Draw sub_8220FB10, AfterRaceMenu::Draw sub_8221BB48,
// CareerResults::Draw sub_8221A2F8; the career's results' transitions Intro
// sub_82219210, Show sub_822192B0, Close1 sub_822193D8 (r3 the step word)
// and Page1 sub_82219A78.
void RaceScreensPauseMenuOpenOptions(lifted::Regs& ctx, uint8_t* base);
void RaceScreensPauseMenuDraw(lifted::Regs& ctx, uint8_t* base);
void RaceScreensAfterRaceMenuDraw(lifted::Regs& ctx, uint8_t* base);
void RaceScreensCareerResultsDraw(lifted::Regs& ctx, uint8_t* base);
void RaceScreensCareerResultsIntro(lifted::Regs& ctx, uint8_t* base);
void RaceScreensCareerResultsShow(lifted::Regs& ctx, uint8_t* base);
void RaceScreensCareerResultsClose1(lifted::Regs& ctx, uint8_t* base);
void RaceScreensCareerResultsPage1(lifted::Regs& ctx, uint8_t* base);
// CareerResults::SetTexts sub_822197A8 (r3 the screen) and Open
// sub_8221A1F0 (r3 the screen, r4 the 168-byte block).
void RaceScreensCareerResultsSetTexts(lifted::Regs& ctx, uint8_t* base);
void RaceScreensCareerResultsOpen(lifted::Regs& ctx, uint8_t* base);

}  // namespace hooks
