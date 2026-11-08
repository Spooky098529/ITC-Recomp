// The race HUD machine's register steps (src/race_hud_machine/,
// docs/functions/race-hud-machine.md), shared by the adapters
// (src/hooks/race_hud_machine.cpp, behind --native_race_hud_machine) and the
// harness (without the flag). Each turns flush-to-zero off first and leaves
// r3 (and f1) as the original does; the handlers run the original for kind 4
// (and kind 0 where it draws the holder's +12), never seen in play.

#pragma once

#include "guest.h"

namespace hooks {

// RaceHudMachine::Init sub_8216E6E8: r3 = the machine (race mode +
// 128664); r3 as the last StateMachine::Add leaves it.
void RaceHudMachineInit(PPCContext& ctx, uint8_t* base);
// The handlers: r3 = the machine, r4 = the race mode's dispatcher.
void RaceHudMachinePreRaceUpdate(PPCContext& ctx, uint8_t* base);   // sub_8216C210
void RaceHudMachinePreRaceDraw(PPCContext& ctx, uint8_t* base);     // sub_8216C2A0
void RaceHudMachineCountdownDraw(PPCContext& ctx, uint8_t* base);   // sub_8216C330
void RaceHudMachineStartIntro(PPCContext& ctx, uint8_t* base);      // sub_8216C510
void RaceHudMachineEndIntro(PPCContext& ctx, uint8_t* base);        // sub_8216C520
void RaceHudMachineIntroUpdate(PPCContext& ctx, uint8_t* base);     // sub_8216C530
void RaceHudMachineIntroDraw(PPCContext& ctx, uint8_t* base);       // sub_8216C5B8
void RaceHudMachineCountdownUpdate(PPCContext& ctx, uint8_t* base); // sub_8216CB98
void RaceHudMachineRaceDraw(PPCContext& ctx, uint8_t* base);        // sub_8216CE00
void RaceHudMachineRaceUpdate(PPCContext& ctx, uint8_t* base);      // sub_8216D918
void RaceHudMachineSetUp(PPCContext& ctx, uint8_t* base);           // sub_8216D450
void RaceHudMachineCloseResults(PPCContext& ctx, uint8_t* base);    // sub_8216D198
void RaceHudMachineResultsUpdate(PPCContext& ctx, uint8_t* base);   // sub_8216DFD0
void RaceHudMachinePauseUpdate(PPCContext& ctx, uint8_t* base);     // sub_8216DB70
void RaceHudMachineAfterRaceUpdate(PPCContext& ctx, uint8_t* base); // sub_8216DD88
// CreateScreens sub_8216CA28 (r3 = the machine), ClosePause sub_8216C658,
// ResultsDraw sub_8216D6D0 and AfterRaceDraw sub_8216C6D8.
void RaceHudMachineCreateScreens(PPCContext& ctx, uint8_t* base);   // sub_8216CA28
void RaceHudMachineClosePause(PPCContext& ctx, uint8_t* base);      // sub_8216C658
void RaceHudMachineResultsDraw(PPCContext& ctx, uint8_t* base);     // sub_8216D6D0
void RaceHudMachineAfterRaceDraw(PPCContext& ctx, uint8_t* base);   // sub_8216C6D8
// PauseDraw sub_8216CF08 and RacePauseDraw sub_8216CFE0 (r3 = the machine).
void RaceHudMachinePauseDraw(PPCContext& ctx, uint8_t* base);       // sub_8216CF08
void RaceHudMachineRacePauseDraw(PPCContext& ctx, uint8_t* base);   // sub_8216CFE0
// DestroyScreens sub_8216C208 (r3 = the machine; a tail call with the
// caller's lr).
void RaceHudMachineDestroyScreens(PPCContext& ctx, uint8_t* base);  // sub_8216C208
// StartCountdown sub_8216CC60 (r3 = the machine).
void RaceHudMachineStartCountdown(PPCContext& ctx, uint8_t* base);  // sub_8216CC60
// The helpers: IsFirstDay sub_8216D3F0 (no arguments), OpponentPrize
// sub_8216C820 and ItemBonus sub_8216C938 (r4 = the item id).
void RaceHudMachineIsFirstDay(PPCContext& ctx, uint8_t* base);
void RaceHudMachineOpponentPrize(PPCContext& ctx, uint8_t* base);
void RaceHudMachineItemBonus(PPCContext& ctx, uint8_t* base);
// The pause menu's openers: OpenPause sub_8216D600 and OpenRacePause
// sub_8216DA40 (r3 the machine, tail calls with the caller's lr),
// OpenCareerPause sub_8216D8C8 (no adapter: its only caller,
// OpenRacePause's plain C++, calls it directly; the step for the harness).
void RaceHudMachineOpenPause(PPCContext& ctx, uint8_t* base);
void RaceHudMachineOpenRacePause(PPCContext& ctx, uint8_t* base);
void RaceHudMachineOpenCareerPause(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
