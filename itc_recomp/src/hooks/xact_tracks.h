// XACT's tracks', track instances' and events' register steps
// (src/xact/tracks.h, docs/functions/xact-tracks.md), shared by the adapters
// (src/hooks/xact_tracks.cpp, behind --native_xact_tracks) and the harness
// (without the flag). Each takes the original's registers and leaves r3 as
// the original does, in a frame of the original's size.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

void XactTrackInstanceConstruct(lifted::Regs& ctx);  // sub_825D57D0
void XactTrackInstanceCheckDone(lifted::Regs& ctx);  // sub_825D55E0
void XactTrackInstanceSetVolume(lifted::Regs& ctx);  // sub_825D5678
void XactTrackInstanceIsBusy(lifted::Regs& ctx);  // sub_825D5780
void XactTrackInstancePause(lifted::Regs& ctx);  // sub_825D57A8
void XactTrackPause(lifted::Regs& ctx);  // sub_825D05A8
void XactTrackIsBusy(lifted::Regs& ctx);  // sub_825D0590
void XactEventConstruct(lifted::Regs& ctx);  // sub_825D1760
void XactEventDestruct(lifted::Regs& ctx);  // sub_825D0EF0
void XactEventFire(lifted::Regs& ctx);  // sub_825D1358
void XactPlayWaveConstruct(lifted::Regs& ctx);  // sub_825D82E8
void XactPlayWaveSetQueued(lifted::Regs& ctx);  // sub_825D8530
void XactPlayWaveGetLoopCount(lifted::Regs& ctx);  // sub_825D8358
void XactPlayWaveSetWave(lifted::Regs& ctx);  // sub_82483208
void XactWaveVariationSetWave(lifted::Regs& ctx);  // sub_825D7B78
void XactPlayWaveGetVariationRecord(lifted::Regs& ctx);  // sub_825D8920
void XactPlayWaveForgetWave(lifted::Regs& ctx);  // sub_825D8360
void XactPlayWaveGetWaveBank(lifted::Regs& ctx);  // sub_825D84C0
void XactPlayWaveIsPrepared(lifted::Regs& ctx);  // sub_825D76A0
void XactPlayWaveIsPlaying(lifted::Regs& ctx);  // sub_825D7708
void XactPlayWaveIsBusy(lifted::Regs& ctx);  // sub_825D81E8
void XactPlayWavePause(lifted::Regs& ctx);  // sub_825D8268
void XactPlayWaveSetVolume(lifted::Regs& ctx);  // sub_825D7FE0
void XactWaveSetVolume(lifted::Regs& ctx);  // sub_825D1CC8

// Batch 2: the track's volume, set-up, update and destructors, the play-wave
// event's destructor, variation and duration. These run the original
// (`__imp__`) where the plain C++ declines.
void XactTrackSetVolume(lifted::Regs& ctx);       // sub_825D0BF8
void XactTrackUpdateVolume(lifted::Regs& ctx);    // sub_825D0428
void XactTrackCreateRpcs(lifted::Regs& ctx);      // sub_825D06B8
void XactTrackStartRpcs(lifted::Regs& ctx);       // sub_825D0800
void XactTrackConstruct(lifted::Regs& ctx);       // sub_825D0CE0
void XactTrackUpdate(lifted::Regs& ctx);          // sub_825D0E30
void XactTrackDestruct(lifted::Regs& ctx);        // sub_825D0110
void XactTrackDeletingDestruct(lifted::Regs& ctx);     // sub_825D0978
void XactPlayWaveDeletingDestruct(lifted::Regs& ctx);  // sub_825D8660
void XactPlayWaveApplyVariation(lifted::Regs& ctx);    // sub_825D7558
void XactPlayWaveGetDuration(lifted::Regs& ctx);       // sub_825D8870
// Batch 3: preparing.
void XactPlayWavePrepare(lifted::Regs& ctx);            // sub_825D86F8
void XactTrackInstancePrepareSimple(lifted::Regs& ctx);  // sub_825D53D8
void XactTrackPrepareSimple(lifted::Regs& ctx);          // sub_825D09D0
// Batch 4: cancelling, stopping and deleting instances.
void XactTrackInstanceCancelEvents(lifted::Regs& ctx);          // sub_825D5520
void XactTrackInstanceDestruct(lifted::Regs& ctx);              // sub_825D5838
void XactTrackInstanceDeletingDestruct(lifted::Regs& ctx);      // sub_825D5E58
void XactTrackInstanceStop(lifted::Regs& ctx);                  // sub_825D5D18
void XactTrackStop(lifted::Regs& ctx);                          // sub_825D01D8
// Batch 5: making events. Runs the original (`__imp__`) where the plain C++
// declines.
void XactEventCreate(lifted::Regs& ctx);  // sub_825D0F00
// The second migration: preparing a complex sound's track.
void XactTrackInstancePrepare(lifted::Regs& ctx);  // sub_825D5A48
void XactTrackCreateInstance(lifted::Regs& ctx);   // sub_825D02F8
void XactTrackPrepare(lifted::Regs& ctx);          // sub_825D0AA0
void XactSoundScheduleEvent(lifted::Regs& ctx);    // sub_825CB0C8
// The third migration: what waited on XACT's clock.
void XactEventSchedule(lifted::Regs& ctx);  // sub_825D1918
void XactEventDone(lifted::Regs& ctx);      // sub_825D13F0
void XactPlayWaveFire(lifted::Regs& ctx);   // sub_825D8378
void XactPlayWaveStop(lifted::Regs& ctx);   // sub_825D83F8
void XactTrackInstancePlay(lifted::Regs& ctx);         // sub_825D5980
void XactTrackInstanceAudioUpdate(lifted::Regs& ctx);  // sub_825D5EB0
void XactTrackAudioUpdate(lifted::Regs& ctx);          // sub_825D0D88
void XactTrackPlay(lifted::Regs& ctx);                 // sub_825D01D0

}  // namespace hooks
