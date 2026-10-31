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
// The helpers: IsFirstDay sub_8216D3F0 (no arguments), OpponentPrize
// sub_8216C820 and ItemBonus sub_8216C938 (r4 = the item id).
void RaceHudMachineIsFirstDay(PPCContext& ctx, uint8_t* base);
void RaceHudMachineOpponentPrize(PPCContext& ctx, uint8_t* base);
void RaceHudMachineItemBonus(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
