// The in-race screens never seen in play: register steps
// (src/race_screens/unseen.h, docs/functions/race-screens-unseen.md),
// shared by the adapters (src/hooks/race_screens_unseen.cpp, behind
// --native_race_screens_unseen) and the harness (without the flag). Each
// leaves r3 as the original does.

#pragma once

#include "guest.h"

namespace hooks {

// Kind 4's pause menu. Adapters (the race HUD machine's handlers reach
// them from recompiled code): Open sub_82211390 (r3 the screen, r4 the
// player, r5 the kind), Update sub_82211458 (r3, r4 passed to the pages),
// Draw sub_82211590 (r3 the screen).
void RaceScreensUnseenPause4Open(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenPause4Update(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenPause4Draw(PPCContext& ctx, uint8_t* base);
// Without adapters (only the native Open, Update and Draw reach them): the
// pages MainPage sub_8220FD48, VolumePage sub_82210E58, ConfirmPage
// sub_8220FF30 (r4), AnswerPage sub_82210910; the helpers StepVolume
// sub_82210488, LoadVolumes sub_822107B8, SaveVolumes sub_82210820,
// SetUpMain sub_82210C00, ShowItems sub_82210360, ColorItems sub_82210CD0,
// SetUpVolume sub_82211250, RestoreNodes sub_822106E8 (r3, r4),
// ShowVolumes sub_82210670, ShowVolume sub_822105F8 (r3 the node, r4 the
// volume), ShowVolumesB sub_82210DE0, SetArrow sub_82210420 (r3); the
// transitions (r3 the step word) RunTen sub_8220FF88, OpenMain
// sub_82211098, ShowMain sub_82210010, BackToMain sub_822100D8, MoveMain
// sub_82210228, CloseMain sub_82210170, OpenVolume sub_822114D8,
// ShowVolumeRow sub_822102D8, MoveVolume sub_82210940, SetVolume
// sub_82210B08, CloseVolume sub_82211138.
void RaceScreensUnseenPause4MainPage(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenPause4VolumePage(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenPause4ConfirmPage(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenPause4AnswerPage(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenPause4StepVolume(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenPause4LoadVolumes(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenPause4SaveVolumes(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenPause4SetUpMain(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenPause4ShowItems(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenPause4ColorItems(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenPause4SetUpVolume(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenPause4RestoreNodes(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenPause4ShowVolumes(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenPause4ShowVolume(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenPause4ShowVolumesB(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenPause4SetArrow(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenPause4RunTen(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenPause4OpenMain(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenPause4ShowMain(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenPause4BackToMain(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenPause4MoveMain(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenPause4CloseMain(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenPause4OpenVolume(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenPause4ShowVolumeRow(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenPause4MoveVolume(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenPause4SetVolume(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenPause4CloseVolume(PPCContext& ctx, uint8_t* base);

// The results at +1028 and +1032. Adapters (the race HUD machine's
// handlers reach them from recompiled code): RaceResults::Open sub_8221F990
// (r3 the screen, r4 the block), Update sub_8221F930 (r3, r4 passed on),
// Draw sub_8221F7D0 (r3 the screen); SplitResults::Open sub_82220050 (r3
// the screen, r4 the outcome, r5 the mode), Update sub_8221FFF0 and Draw
// sub_822200E0. Without adapters (only the native Open, Update and Draw
// reach them): SetTexts sub_8221F628 (r3 the screen), the MainPages
// sub_8221F870 and sub_8221FE70, the transitions (r3 the step word)
// RaceResults' Intro sub_8221F428, Show sub_8221F4B0 and Close
// sub_8221F580, and SplitResults' Intro sub_8221FAC8, Show sub_8221FBE8 and
// Close sub_8221FD98.
void RaceScreensUnseenRaceResultsOpen(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenRaceResultsUpdate(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenRaceResultsDraw(PPCContext& ctx, uint8_t* base);
// SplitResults::Draw sub_822200E0 (r3 the screen): an adapter (the race HUD
// machine's recompiled ResultsDraw reaches it).
void RaceScreensUnseenSplitResultsDraw(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenRaceResultsSetTexts(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenRaceResultsMainPage(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenRaceResultsIntro(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenRaceResultsShow(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenRaceResultsClose(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenSplitResultsOpen(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenSplitResultsUpdate(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenSplitResultsMainPage(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenSplitResultsIntro(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenSplitResultsShow(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenSplitResultsClose(PPCContext& ctx, uint8_t* base);

// Kind 6's screen after the race. Adapters (recompiled code reaches them):
// Close sub_8221C168 (the race HUD machine's CloseResults fallback), Open
// sub_8221EC48 (r3 the screen, r4 the block; ShowKind6Results), Draw
// sub_8221EF60 (r3 the screen; AfterRaceDraw's fallback). Without adapters
// (only native code reaches them): ShowPanelA sub_8221D050, SetUpList
// sub_8221DF70, SetUpTexts sub_8221E038 (r3 the screen), ShowCourse
// sub_8221D1A8, ShowHelp sub_8221D130, HidePanels sub_8221CFC0, SetCursorX
// sub_8221D7B0 (r3), ShowList sub_8221D4E8, ShowReplay sub_8221CBD8 (r3 the
// record), and the transitions (r3 the step word) Intro sub_8221C438, Show
// sub_8221C550, RunToEnd sub_8221C5D8, MoveMain sub_8221DB48, BackToQuit
// sub_8221DC88, OpenList sub_8221EB98, CloseList sub_8221C628, MoveList
// sub_8221DD78, OpenQuestion sub_8221C6C8, CloseQuestion sub_8221C7D8,
// MoveQuestion sub_8221C888, OpenMessage sub_8221CAB8, CloseMessage
// sub_8221CB50, OpenWaiting sub_8221C920, CloseWaiting sub_8221CA20.
void RaceScreensUnseenKind6Close(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6Open(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6Draw(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6ShowPanelA(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6SetUpList(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6SetUpTexts(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6ShowCourse(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6ShowHelp(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6HidePanels(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6SetCursorX(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6ShowList(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6ShowReplay(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6Intro(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6Show(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6RunToEnd(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6MoveMain(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6BackToQuit(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6OpenList(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6CloseList(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6MoveList(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6OpenQuestion(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6CloseQuestion(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6MoveQuestion(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6OpenMessage(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6CloseMessage(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6OpenWaiting(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6CloseWaiting(PPCContext& ctx, uint8_t* base);


// Kind 6's update: an adapter for Update sub_8221F1E8 (r3 the screen, r4
// passed to the dialog pages; the race HUD machine's AfterRaceUpdate
// fallback reaches it). Without adapters (only the native Update reaches
// them): the pages (MainPage sub_8221E788, AnswerPage sub_8221C1E0,
// SlotPage sub_8221DA60, SavePage sub_8221C238, OverwritePage sub_8221E908,
// SavingPage sub_8221C2B8, QuitPage sub_8221C308 (the four with r4),
// CheckRecordPage sub_8221E510, RankingReadPage sub_8221C008, UploadPage
// sub_8221E5E8, MenuBackPage sub_8221D898, UploadResultPage sub_8221D908,
// RankingPage sub_8221BE90, MessageBackPage sub_8221D9D8, QuestionPage
// sub_8221E980, QuestionAnswerPage sub_8221C360, MessagePage sub_8221EAE8,
// WaitingPage sub_8221C3F0), ShowPanelB sub_8221CEA0, ShowTimeOnly
// sub_8221D3D0, ShowRanking sub_8221E1A0, RankingTime sub_8221D808 (r3 the
// row, r4 to r6 the out words), and the network manager's sub_822914A8
// (Rank), sub_8225FFC8 (CarLogo, r3 the car) and sub_822927B0 (NetState, r3
// the state).
void RaceScreensUnseenKind6Update(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6MainPage(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6AnswerPage(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6SlotPage(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6SavePage(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6OverwritePage(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6SavingPage(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6QuitPage(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6CheckRecordPage(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6RankingReadPage(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6UploadPage(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6MenuBackPage(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6UploadResultPage(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6RankingPage(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6MessageBackPage(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6QuestionPage(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6QuestionAnswerPage(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6MessagePage(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6WaitingPage(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6ShowPanelB(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6ShowTimeOnly(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6ShowRanking(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6RankingTime(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6Rank(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6CarLogo(PPCContext& ctx, uint8_t* base);
// The leaderboard read sub_82292CC0 (r3 the session; answered as no Live)
// and its spec's set-up sub_82292648 (no arguments, r3 kept): adapters.
void RaceScreensUnseenKind6LeaderboardRead(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6ReadSpecSetUp(PPCContext& ctx, uint8_t* base);
// Kind6Menu::SaveReplay sub_8221D678 (a step for the harness),
// RaceDialog::AskSlot sub_82219058 (r3 the slot, r4 the record, r5 the
// replay; an adapter), StoreReplay sub_82218FF8 (r3 the record, r4 the
// replay; a step) and Replay::CopyFrom sub_821A66A8 (r3, r4 the replays; an
// adapter).
void RaceScreensUnseenKind6SaveReplay(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenAskSlot(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenStoreReplay(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenReplayCopyFrom(PPCContext& ctx, uint8_t* base);
void RaceScreensUnseenKind6NetState(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
