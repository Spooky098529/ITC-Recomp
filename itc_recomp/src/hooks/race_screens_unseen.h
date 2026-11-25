// The in-race screens never seen in play: register steps
// (src/race_screens/unseen.h, docs/functions/race-screens-unseen.md),
// shared by the adapters (src/hooks/race_screens_unseen.cpp, behind
// --native_race_screens_unseen) and the harness (without the flag). Each
// leaves r3 as the original does.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// Kind 4's pause menu. Adapters (the race HUD machine's handlers reach
// them from recompiled code): Open sub_82211390 (r3 the screen, r4 the
// player, r5 the kind), Update sub_82211458 (r3, r4 passed to the pages),
// Draw sub_82211590 (r3 the screen).
void RaceScreensUnseenPause4Open(lifted::Regs& ctx);
void RaceScreensUnseenPause4Update(lifted::Regs& ctx);
void RaceScreensUnseenPause4Draw(lifted::Regs& ctx);
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
void RaceScreensUnseenPause4MainPage(lifted::Regs& ctx);
void RaceScreensUnseenPause4VolumePage(lifted::Regs& ctx);
void RaceScreensUnseenPause4ConfirmPage(lifted::Regs& ctx);
void RaceScreensUnseenPause4AnswerPage(lifted::Regs& ctx);
void RaceScreensUnseenPause4StepVolume(lifted::Regs& ctx);
void RaceScreensUnseenPause4LoadVolumes(lifted::Regs& ctx);
void RaceScreensUnseenPause4SaveVolumes(lifted::Regs& ctx);
void RaceScreensUnseenPause4SetUpMain(lifted::Regs& ctx);
void RaceScreensUnseenPause4ShowItems(lifted::Regs& ctx);
void RaceScreensUnseenPause4ColorItems(lifted::Regs& ctx);
void RaceScreensUnseenPause4SetUpVolume(lifted::Regs& ctx);
void RaceScreensUnseenPause4RestoreNodes(lifted::Regs& ctx);
void RaceScreensUnseenPause4ShowVolumes(lifted::Regs& ctx);
void RaceScreensUnseenPause4ShowVolume(lifted::Regs& ctx);
void RaceScreensUnseenPause4ShowVolumesB(lifted::Regs& ctx);
void RaceScreensUnseenPause4SetArrow(lifted::Regs& ctx);
void RaceScreensUnseenPause4RunTen(lifted::Regs& ctx);
void RaceScreensUnseenPause4OpenMain(lifted::Regs& ctx);
void RaceScreensUnseenPause4ShowMain(lifted::Regs& ctx);
void RaceScreensUnseenPause4BackToMain(lifted::Regs& ctx);
void RaceScreensUnseenPause4MoveMain(lifted::Regs& ctx);
void RaceScreensUnseenPause4CloseMain(lifted::Regs& ctx);
void RaceScreensUnseenPause4OpenVolume(lifted::Regs& ctx);
void RaceScreensUnseenPause4ShowVolumeRow(lifted::Regs& ctx);
void RaceScreensUnseenPause4MoveVolume(lifted::Regs& ctx);
void RaceScreensUnseenPause4SetVolume(lifted::Regs& ctx);
void RaceScreensUnseenPause4CloseVolume(lifted::Regs& ctx);

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
void RaceScreensUnseenRaceResultsOpen(lifted::Regs& ctx);
void RaceScreensUnseenRaceResultsUpdate(lifted::Regs& ctx);
void RaceScreensUnseenRaceResultsDraw(lifted::Regs& ctx);
// SplitResults::Draw sub_822200E0 (r3 the screen): an adapter (the race HUD
// machine's recompiled ResultsDraw reaches it).
void RaceScreensUnseenSplitResultsDraw(lifted::Regs& ctx);
void RaceScreensUnseenRaceResultsSetTexts(lifted::Regs& ctx);
void RaceScreensUnseenRaceResultsMainPage(lifted::Regs& ctx);
void RaceScreensUnseenRaceResultsIntro(lifted::Regs& ctx);
void RaceScreensUnseenRaceResultsShow(lifted::Regs& ctx);
void RaceScreensUnseenRaceResultsClose(lifted::Regs& ctx);
void RaceScreensUnseenSplitResultsOpen(lifted::Regs& ctx);
void RaceScreensUnseenSplitResultsUpdate(lifted::Regs& ctx);
void RaceScreensUnseenSplitResultsMainPage(lifted::Regs& ctx);
void RaceScreensUnseenSplitResultsIntro(lifted::Regs& ctx);
void RaceScreensUnseenSplitResultsShow(lifted::Regs& ctx);
void RaceScreensUnseenSplitResultsClose(lifted::Regs& ctx);

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
void RaceScreensUnseenKind6Close(lifted::Regs& ctx);
void RaceScreensUnseenKind6Open(lifted::Regs& ctx);
void RaceScreensUnseenKind6Draw(lifted::Regs& ctx);
void RaceScreensUnseenKind6ShowPanelA(lifted::Regs& ctx);
void RaceScreensUnseenKind6SetUpList(lifted::Regs& ctx);
void RaceScreensUnseenKind6SetUpTexts(lifted::Regs& ctx);
void RaceScreensUnseenKind6ShowCourse(lifted::Regs& ctx);
void RaceScreensUnseenKind6ShowHelp(lifted::Regs& ctx);
void RaceScreensUnseenKind6HidePanels(lifted::Regs& ctx);
void RaceScreensUnseenKind6SetCursorX(lifted::Regs& ctx);
void RaceScreensUnseenKind6ShowList(lifted::Regs& ctx);
void RaceScreensUnseenKind6ShowReplay(lifted::Regs& ctx);
void RaceScreensUnseenKind6Intro(lifted::Regs& ctx);
void RaceScreensUnseenKind6Show(lifted::Regs& ctx);
void RaceScreensUnseenKind6RunToEnd(lifted::Regs& ctx);
void RaceScreensUnseenKind6MoveMain(lifted::Regs& ctx);
void RaceScreensUnseenKind6BackToQuit(lifted::Regs& ctx);
void RaceScreensUnseenKind6OpenList(lifted::Regs& ctx);
void RaceScreensUnseenKind6CloseList(lifted::Regs& ctx);
void RaceScreensUnseenKind6MoveList(lifted::Regs& ctx);
void RaceScreensUnseenKind6OpenQuestion(lifted::Regs& ctx);
void RaceScreensUnseenKind6CloseQuestion(lifted::Regs& ctx);
void RaceScreensUnseenKind6MoveQuestion(lifted::Regs& ctx);
void RaceScreensUnseenKind6OpenMessage(lifted::Regs& ctx);
void RaceScreensUnseenKind6CloseMessage(lifted::Regs& ctx);
void RaceScreensUnseenKind6OpenWaiting(lifted::Regs& ctx);
void RaceScreensUnseenKind6CloseWaiting(lifted::Regs& ctx);


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
void RaceScreensUnseenKind6Update(lifted::Regs& ctx);
void RaceScreensUnseenKind6MainPage(lifted::Regs& ctx);
void RaceScreensUnseenKind6AnswerPage(lifted::Regs& ctx);
void RaceScreensUnseenKind6SlotPage(lifted::Regs& ctx);
void RaceScreensUnseenKind6SavePage(lifted::Regs& ctx);
void RaceScreensUnseenKind6OverwritePage(lifted::Regs& ctx);
void RaceScreensUnseenKind6SavingPage(lifted::Regs& ctx);
void RaceScreensUnseenKind6QuitPage(lifted::Regs& ctx);
void RaceScreensUnseenKind6CheckRecordPage(lifted::Regs& ctx);
void RaceScreensUnseenKind6RankingReadPage(lifted::Regs& ctx);
void RaceScreensUnseenKind6UploadPage(lifted::Regs& ctx);
void RaceScreensUnseenKind6MenuBackPage(lifted::Regs& ctx);
void RaceScreensUnseenKind6UploadResultPage(lifted::Regs& ctx);
void RaceScreensUnseenKind6RankingPage(lifted::Regs& ctx);
void RaceScreensUnseenKind6MessageBackPage(lifted::Regs& ctx);
void RaceScreensUnseenKind6QuestionPage(lifted::Regs& ctx);
void RaceScreensUnseenKind6QuestionAnswerPage(lifted::Regs& ctx);
void RaceScreensUnseenKind6MessagePage(lifted::Regs& ctx);
void RaceScreensUnseenKind6WaitingPage(lifted::Regs& ctx);
void RaceScreensUnseenKind6ShowPanelB(lifted::Regs& ctx);
void RaceScreensUnseenKind6ShowTimeOnly(lifted::Regs& ctx);
void RaceScreensUnseenKind6ShowRanking(lifted::Regs& ctx);
void RaceScreensUnseenKind6RankingTime(lifted::Regs& ctx);
void RaceScreensUnseenKind6Rank(lifted::Regs& ctx);
void RaceScreensUnseenKind6CarLogo(lifted::Regs& ctx);
// The leaderboard read sub_82292CC0 (r3 the session; answered as no Live)
// and its spec's set-up sub_82292648 (no arguments, r3 kept): adapters.
void RaceScreensUnseenKind6LeaderboardRead(lifted::Regs& ctx);
void RaceScreensUnseenKind6ReadSpecSetUp(lifted::Regs& ctx);
// Kind6Menu::SaveReplay sub_8221D678 (a step for the harness),
// RaceDialog::AskSlot sub_82219058 (r3 the slot, r4 the record, r5 the
// replay; an adapter), StoreReplay sub_82218FF8 (r3 the record, r4 the
// replay; a step) and Replay::CopyFrom sub_821A66A8 (r3, r4 the replays; an
// adapter).
void RaceScreensUnseenKind6SaveReplay(lifted::Regs& ctx);
void RaceScreensUnseenAskSlot(lifted::Regs& ctx);
void RaceScreensUnseenStoreReplay(lifted::Regs& ctx);
void RaceScreensUnseenReplayCopyFrom(lifted::Regs& ctx);
void RaceScreensUnseenKind6NetState(lifted::Regs& ctx);

}  // namespace hooks
