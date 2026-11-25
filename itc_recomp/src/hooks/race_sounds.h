// The race's sounds' register steps (src/race_sounds/,
// docs/functions/race-sounds.md), shared by the adapters
// (src/hooks/race_sounds.cpp, behind --native_race_sounds) and the harness
// (without the flag). Each takes the original's registers (r3 the machine
// for the handlers) and leaves r3 as the original does.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

void RaceSoundsSetListener(lifted::Regs& ctx);          // sub_821758A0 (r3-r6)
void RaceSoundsFadeMusic(lifted::Regs& ctx);            // sub_82175908
void RaceSoundsRestoreMusicVolume(lifted::Regs& ctx);   // sub_821759C8
void RaceSoundsEndTick(lifted::Regs& ctx);              // sub_82175A40
void RaceSoundsPause(lifted::Regs& ctx);                // sub_82175AE8
void RaceSoundsResume(lifted::Regs& ctx);               // sub_82175B80
void RaceSoundsPauseCars(lifted::Regs& ctx);            // sub_82175C00
void RaceSoundsResumeCars(lifted::Regs& ctx);           // sub_82175C58
void RaceSoundsOnStageCountdown(lifted::Regs& ctx);     // sub_82175D20
void RaceSoundsOnRollingStartDone(lifted::Regs& ctx);   // sub_82175D48
void RaceSoundsOnEffectsBankLoaded(lifted::Regs& ctx);  // sub_82175ED0
void RaceSoundsSetUpCars(lifted::Regs& ctx);            // sub_82176188
void RaceSoundsUpdateCars(lifted::Regs& ctx);           // sub_82175F20
void RaceSoundsPickRivalMusic(lifted::Regs& ctx);       // sub_821764F0
void RaceSoundsPlayRivalMusic(lifted::Regs& ctx);       // sub_821762F8
void RaceSoundsRaceTick(lifted::Regs& ctx);  // sub_821769D0
void RaceSoundsStageTick(lifted::Regs& ctx);  // sub_82176F28
void RaceSoundsRollingStartTick(lifted::Regs& ctx);  // sub_82177130
void RaceSoundsAfterRaceTick(lifted::Regs& ctx);  // sub_82176C10
void RaceSoundsOnCountdown(lifted::Regs& ctx);  // sub_82176968
void RaceSoundsOnRaceEnd(lifted::Regs& ctx);  // sub_82176BA0
void RaceSoundsOnLeaveAfterRace(lifted::Regs& ctx);  // sub_82176E60
void RaceSoundsOnNextStage(lifted::Regs& ctx);  // sub_82176EC8
void RaceSoundsOnRollingStart(lifted::Regs& ctx);  // sub_821770C0
void RaceSoundsPlayStartMusic(lifted::Regs& ctx);  // sub_82175DB0
void RaceSoundsIdleTick(lifted::Regs& ctx);  // sub_821767A8
void RaceSoundsBegin(lifted::Regs& ctx);  // sub_82176618
void RaceSoundsInit(lifted::Regs& ctx);  // sub_82177378
void RaceSoundsOnAbort(lifted::Regs& ctx);  // sub_82175CA0
void RaceSoundsOnAbortDone(lifted::Regs& ctx);  // sub_82175CF8
void RaceSoundsUnload(lifted::Regs& ctx);  // sub_821772D8

}  // namespace hooks
