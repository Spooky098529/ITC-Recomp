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

}  // namespace hooks
