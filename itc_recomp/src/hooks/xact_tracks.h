// XACT's tracks', track instances' and events' register steps
// (src/xact/tracks.h, docs/functions/xact-tracks.md), shared by the adapters
// (src/hooks/xact_tracks.cpp, behind --native_xact_tracks) and the harness
// (without the flag). Each takes the original's registers and leaves r3 as
// the original does, in a frame of the original's size.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

void XactTrackInstanceConstruct(PPCContext& ctx, uint8_t* base);
void XactTrackInstanceConstruct(lifted::Regs& ctx, uint8_t* base);  // sub_825D57D0
void XactTrackInstanceCheckDone(PPCContext& ctx, uint8_t* base);
void XactTrackInstanceCheckDone(lifted::Regs& ctx, uint8_t* base);  // sub_825D55E0
void XactTrackInstanceSetVolume(PPCContext& ctx, uint8_t* base);
void XactTrackInstanceSetVolume(lifted::Regs& ctx, uint8_t* base);  // sub_825D5678
void XactTrackInstanceIsBusy(PPCContext& ctx, uint8_t* base);
void XactTrackInstanceIsBusy(lifted::Regs& ctx, uint8_t* base);  // sub_825D5780
void XactTrackInstancePause(PPCContext& ctx, uint8_t* base);
void XactTrackInstancePause(lifted::Regs& ctx, uint8_t* base);  // sub_825D57A8
void XactTrackPause(PPCContext& ctx, uint8_t* base);
void XactTrackPause(lifted::Regs& ctx, uint8_t* base);  // sub_825D05A8
void XactTrackIsBusy(PPCContext& ctx, uint8_t* base);
void XactTrackIsBusy(lifted::Regs& ctx, uint8_t* base);  // sub_825D0590
void XactEventConstruct(PPCContext& ctx, uint8_t* base);
void XactEventConstruct(lifted::Regs& ctx, uint8_t* base);  // sub_825D1760
void XactEventDestruct(PPCContext& ctx, uint8_t* base);
void XactEventDestruct(lifted::Regs& ctx, uint8_t* base);  // sub_825D0EF0
void XactEventFire(PPCContext& ctx, uint8_t* base);
void XactEventFire(lifted::Regs& ctx, uint8_t* base);  // sub_825D1358
void XactPlayWaveConstruct(PPCContext& ctx, uint8_t* base);
void XactPlayWaveConstruct(lifted::Regs& ctx, uint8_t* base);  // sub_825D82E8
void XactPlayWaveSetQueued(PPCContext& ctx, uint8_t* base);
void XactPlayWaveSetQueued(lifted::Regs& ctx, uint8_t* base);  // sub_825D8530
void XactPlayWaveGetLoopCount(PPCContext& ctx, uint8_t* base);
void XactPlayWaveGetLoopCount(lifted::Regs& ctx, uint8_t* base);  // sub_825D8358
void XactPlayWaveSetWave(PPCContext& ctx, uint8_t* base);
void XactPlayWaveSetWave(lifted::Regs& ctx, uint8_t* base);  // sub_82483208
void XactWaveVariationSetWave(PPCContext& ctx, uint8_t* base);
void XactWaveVariationSetWave(lifted::Regs& ctx, uint8_t* base);  // sub_825D7B78
void XactPlayWaveGetVariationRecord(PPCContext& ctx, uint8_t* base);
void XactPlayWaveGetVariationRecord(lifted::Regs& ctx, uint8_t* base);  // sub_825D8920
void XactPlayWaveForgetWave(PPCContext& ctx, uint8_t* base);
void XactPlayWaveForgetWave(lifted::Regs& ctx, uint8_t* base);  // sub_825D8360
void XactPlayWaveGetWaveBank(PPCContext& ctx, uint8_t* base);
void XactPlayWaveGetWaveBank(lifted::Regs& ctx, uint8_t* base);  // sub_825D84C0
void XactPlayWaveIsPrepared(PPCContext& ctx, uint8_t* base);
void XactPlayWaveIsPrepared(lifted::Regs& ctx, uint8_t* base);  // sub_825D76A0
void XactPlayWaveIsPlaying(PPCContext& ctx, uint8_t* base);
void XactPlayWaveIsPlaying(lifted::Regs& ctx, uint8_t* base);  // sub_825D7708
void XactPlayWaveIsBusy(PPCContext& ctx, uint8_t* base);
void XactPlayWaveIsBusy(lifted::Regs& ctx, uint8_t* base);  // sub_825D81E8
void XactPlayWavePause(PPCContext& ctx, uint8_t* base);
void XactPlayWavePause(lifted::Regs& ctx, uint8_t* base);  // sub_825D8268
void XactPlayWaveSetVolume(PPCContext& ctx, uint8_t* base);
void XactPlayWaveSetVolume(lifted::Regs& ctx, uint8_t* base);  // sub_825D7FE0
void XactWaveSetVolume(PPCContext& ctx, uint8_t* base);
void XactWaveSetVolume(lifted::Regs& ctx, uint8_t* base);  // sub_825D1CC8

// Batch 2: the track's volume, set-up, update and destructors, the play-wave
// event's destructor, variation and duration. These run the original
// (`__imp__`) where the plain C++ declines.
void XactTrackSetVolume(PPCContext& ctx, uint8_t* base);
void XactTrackSetVolume(lifted::Regs& ctx, uint8_t* base);       // sub_825D0BF8
void XactTrackUpdateVolume(PPCContext& ctx, uint8_t* base);
void XactTrackUpdateVolume(lifted::Regs& ctx, uint8_t* base);    // sub_825D0428
void XactTrackCreateRpcs(PPCContext& ctx, uint8_t* base);
void XactTrackCreateRpcs(lifted::Regs& ctx, uint8_t* base);      // sub_825D06B8
void XactTrackStartRpcs(PPCContext& ctx, uint8_t* base);
void XactTrackStartRpcs(lifted::Regs& ctx, uint8_t* base);       // sub_825D0800
void XactTrackConstruct(PPCContext& ctx, uint8_t* base);
void XactTrackConstruct(lifted::Regs& ctx, uint8_t* base);       // sub_825D0CE0
void XactTrackUpdate(PPCContext& ctx, uint8_t* base);
void XactTrackUpdate(lifted::Regs& ctx, uint8_t* base);          // sub_825D0E30
void XactTrackDestruct(PPCContext& ctx, uint8_t* base);
void XactTrackDestruct(lifted::Regs& ctx, uint8_t* base);        // sub_825D0110
void XactTrackDeletingDestruct(PPCContext& ctx, uint8_t* base);
void XactTrackDeletingDestruct(lifted::Regs& ctx, uint8_t* base);     // sub_825D0978
void XactPlayWaveDeletingDestruct(PPCContext& ctx, uint8_t* base);
void XactPlayWaveDeletingDestruct(lifted::Regs& ctx, uint8_t* base);  // sub_825D8660
void XactPlayWaveApplyVariation(PPCContext& ctx, uint8_t* base);
void XactPlayWaveApplyVariation(lifted::Regs& ctx, uint8_t* base);    // sub_825D7558
void XactPlayWaveGetDuration(PPCContext& ctx, uint8_t* base);
void XactPlayWaveGetDuration(lifted::Regs& ctx, uint8_t* base);       // sub_825D8870
// Batch 3: preparing.
void XactPlayWavePrepare(PPCContext& ctx, uint8_t* base);
void XactPlayWavePrepare(lifted::Regs& ctx, uint8_t* base);            // sub_825D86F8
void XactTrackInstancePrepareSimple(PPCContext& ctx, uint8_t* base);
void XactTrackInstancePrepareSimple(lifted::Regs& ctx, uint8_t* base);  // sub_825D53D8
void XactTrackPrepareSimple(PPCContext& ctx, uint8_t* base);
void XactTrackPrepareSimple(lifted::Regs& ctx, uint8_t* base);          // sub_825D09D0
// Batch 4: cancelling, stopping and deleting instances.
void XactTrackInstanceCancelEvents(PPCContext& ctx, uint8_t* base);
void XactTrackInstanceCancelEvents(lifted::Regs& ctx, uint8_t* base);          // sub_825D5520
void XactTrackInstanceDestruct(PPCContext& ctx, uint8_t* base);
void XactTrackInstanceDestruct(lifted::Regs& ctx, uint8_t* base);              // sub_825D5838
void XactTrackInstanceDeletingDestruct(PPCContext& ctx, uint8_t* base);
void XactTrackInstanceDeletingDestruct(lifted::Regs& ctx, uint8_t* base);      // sub_825D5E58
void XactTrackInstanceStop(PPCContext& ctx, uint8_t* base);
void XactTrackInstanceStop(lifted::Regs& ctx, uint8_t* base);                  // sub_825D5D18
void XactTrackStop(PPCContext& ctx, uint8_t* base);
void XactTrackStop(lifted::Regs& ctx, uint8_t* base);                          // sub_825D01D8
// Batch 5: making events. Runs the original (`__imp__`) where the plain C++
// declines.
void XactEventCreate(PPCContext& ctx, uint8_t* base);
void XactEventCreate(lifted::Regs& ctx, uint8_t* base);  // sub_825D0F00
// The second migration: preparing a complex sound's track.
void XactTrackInstancePrepare(PPCContext& ctx, uint8_t* base);
void XactTrackInstancePrepare(lifted::Regs& ctx, uint8_t* base);  // sub_825D5A48
void XactTrackCreateInstance(PPCContext& ctx, uint8_t* base);
void XactTrackCreateInstance(lifted::Regs& ctx, uint8_t* base);   // sub_825D02F8
void XactTrackPrepare(PPCContext& ctx, uint8_t* base);
void XactTrackPrepare(lifted::Regs& ctx, uint8_t* base);          // sub_825D0AA0
void XactSoundScheduleEvent(PPCContext& ctx, uint8_t* base);
void XactSoundScheduleEvent(lifted::Regs& ctx, uint8_t* base);    // sub_825CB0C8
// The third migration: what waited on XACT's clock.
void XactEventSchedule(PPCContext& ctx, uint8_t* base);
void XactEventSchedule(lifted::Regs& ctx, uint8_t* base);  // sub_825D1918
void XactEventDone(PPCContext& ctx, uint8_t* base);
void XactEventDone(lifted::Regs& ctx, uint8_t* base);      // sub_825D13F0
void XactPlayWaveFire(PPCContext& ctx, uint8_t* base);
void XactPlayWaveFire(lifted::Regs& ctx, uint8_t* base);   // sub_825D8378
void XactPlayWaveStop(PPCContext& ctx, uint8_t* base);
void XactPlayWaveStop(lifted::Regs& ctx, uint8_t* base);   // sub_825D83F8
void XactTrackInstancePlay(PPCContext& ctx, uint8_t* base);
void XactTrackInstancePlay(lifted::Regs& ctx, uint8_t* base);         // sub_825D5980
void XactTrackInstanceAudioUpdate(PPCContext& ctx, uint8_t* base);
void XactTrackInstanceAudioUpdate(lifted::Regs& ctx, uint8_t* base);  // sub_825D5EB0
void XactTrackAudioUpdate(PPCContext& ctx, uint8_t* base);
void XactTrackAudioUpdate(lifted::Regs& ctx, uint8_t* base);          // sub_825D0D88
void XactTrackPlay(PPCContext& ctx, uint8_t* base);
void XactTrackPlay(lifted::Regs& ctx, uint8_t* base);                 // sub_825D01D0

}  // namespace hooks
