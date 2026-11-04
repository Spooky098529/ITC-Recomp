// The race's sounds' register steps (src/race_sounds/,
// docs/functions/race-sounds.md), shared by the adapters
// (src/hooks/race_sounds.cpp, behind --native_race_sounds) and the harness
// (without the flag). Each takes the original's registers (r3 the machine
// for the handlers) and leaves r3 as the original does.

#pragma once

#include "guest.h"

namespace hooks {

void RaceSoundsSetListener(PPCContext& ctx, uint8_t* base);          // sub_821758A0 (r3-r6)
void RaceSoundsFadeMusic(PPCContext& ctx, uint8_t* base);            // sub_82175908
void RaceSoundsRestoreMusicVolume(PPCContext& ctx, uint8_t* base);   // sub_821759C8
void RaceSoundsEndTick(PPCContext& ctx, uint8_t* base);              // sub_82175A40
void RaceSoundsPause(PPCContext& ctx, uint8_t* base);                // sub_82175AE8
void RaceSoundsResume(PPCContext& ctx, uint8_t* base);               // sub_82175B80
void RaceSoundsPauseCars(PPCContext& ctx, uint8_t* base);            // sub_82175C00
void RaceSoundsResumeCars(PPCContext& ctx, uint8_t* base);           // sub_82175C58
void RaceSoundsOnStageCountdown(PPCContext& ctx, uint8_t* base);     // sub_82175D20
void RaceSoundsOnRollingStartDone(PPCContext& ctx, uint8_t* base);   // sub_82175D48
void RaceSoundsOnEffectsBankLoaded(PPCContext& ctx, uint8_t* base);  // sub_82175ED0
void RaceSoundsSetUpCars(PPCContext& ctx, uint8_t* base);            // sub_82176188
void RaceSoundsUpdateCars(PPCContext& ctx, uint8_t* base);           // sub_82175F20
void RaceSoundsPickRivalMusic(PPCContext& ctx, uint8_t* base);       // sub_821764F0
void RaceSoundsPlayRivalMusic(PPCContext& ctx, uint8_t* base);       // sub_821762F8
void RaceSoundsRaceTick(PPCContext& ctx, uint8_t* base);  // sub_821769D0
void RaceSoundsStageTick(PPCContext& ctx, uint8_t* base);  // sub_82176F28
void RaceSoundsRollingStartTick(PPCContext& ctx, uint8_t* base);  // sub_82177130
void RaceSoundsAfterRaceTick(PPCContext& ctx, uint8_t* base);  // sub_82176C10
void RaceSoundsOnCountdown(PPCContext& ctx, uint8_t* base);  // sub_82176968
void RaceSoundsOnRaceEnd(PPCContext& ctx, uint8_t* base);  // sub_82176BA0
void RaceSoundsOnLeaveAfterRace(PPCContext& ctx, uint8_t* base);  // sub_82176E60
void RaceSoundsOnNextStage(PPCContext& ctx, uint8_t* base);  // sub_82176EC8
void RaceSoundsOnRollingStart(PPCContext& ctx, uint8_t* base);  // sub_821770C0
void RaceSoundsPlayStartMusic(PPCContext& ctx, uint8_t* base);  // sub_82175DB0
void RaceSoundsIdleTick(PPCContext& ctx, uint8_t* base);  // sub_821767A8
void RaceSoundsBegin(PPCContext& ctx, uint8_t* base);  // sub_82176618

}  // namespace hooks
