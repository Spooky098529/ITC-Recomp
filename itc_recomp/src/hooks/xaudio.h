// XAudio's voices' register steps (src/xaudio/, docs/functions/xaudio.md),
// shared by the adapters (src/hooks/xaudio.cpp, behind --native_xaudio) and
// the harness (without the flag). Each takes the original's registers,
// leaves r3 as the original does, and runs the original (`__imp__`) where
// the plain C++ can't.

#pragma once

#include "guest.h"

namespace hooks {

void XAudioSourceOutputFormat(PPCContext& ctx, uint8_t* base);  // sub_8245DC88
void XAudioSubmixDescribe(PPCContext& ctx, uint8_t* base);      // sub_8245F010
void XAudioDefaultMatrix(PPCContext& ctx, uint8_t* base);       // sub_8245EC40
void XAudioBufferSize(PPCContext& ctx, uint8_t* base);          // sub_8245FA28
void XAudioBufferClear(PPCContext& ctx, uint8_t* base);         // sub_8245F8E8
void XAudioEngineBuffer(PPCContext& ctx, uint8_t* base);        // sub_8245DEB0 (r13's processor)
void XAudioGetType(PPCContext& ctx, uint8_t* base);             // sub_8245E318
void XAudioGetFormat(PPCContext& ctx, uint8_t* base);           // sub_8245E328
void XAudioSetFormatWords(PPCContext& ctx, uint8_t* base);      // sub_82461140
void XAudioAddRef(PPCContext& ctx, uint8_t* base);              // sub_8245F860
void XAudioSetRootVtable(PPCContext& ctx, uint8_t* base);       // sub_8245FB00
void XAudioAddStat(PPCContext& ctx, uint8_t* base);             // sub_8245F640

// Calls into the recompiled objects go through `ctx` (the registers not
// given keep what the previous call left), inside a frame of the original's
// size.
void XAudioFactorySize(PPCContext& ctx, uint8_t* base);         // sub_8245D610
void XAudioFactoryCreate(PPCContext& ctx, uint8_t* base);       // sub_8245D630
void XAudioBufferAddRefThunk(PPCContext& ctx, uint8_t* base);   // sub_8245F6B0
void XAudioBufferReleaseThunk(PPCContext& ctx, uint8_t* base);  // sub_8245F6D0
void XAudioRelease(PPCContext& ctx, uint8_t* base);             // sub_8245F878
void XAudioObjectFree(PPCContext& ctx, uint8_t* base);          // sub_8245AAD8
void XAudioApplyVolume(PPCContext& ctx, uint8_t* base);         // sub_8245DFC0
void XAudioStopAndReset(PPCContext& ctx, uint8_t* base);        // sub_8245DD60
void XAudioSetEffectParameters(PPCContext& ctx, uint8_t* base);  // sub_82461418
void XAudioReleaseOutputs(PPCContext& ctx, uint8_t* base);      // sub_82460A58
void XAudioSetOutputMatrices(PPCContext& ctx, uint8_t* base);   // sub_82460AB8
void XAudioSetFormat(PPCContext& ctx, uint8_t* base);           // sub_82460778
void XAudioSourceDescribe(PPCContext& ctx, uint8_t* base);      // sub_8245E010
void XAudioReformat(PPCContext& ctx, uint8_t* base);            // sub_8245DCF0
void XAudioAlloc(PPCContext& ctx, uint8_t* base);               // sub_8245EF80
void XAudioBufferClearThunk(PPCContext& ctx, uint8_t* base);    // sub_8245FA10
void XAudioBaseSize(PPCContext& ctx, uint8_t* base);            // sub_82461158
void XAudioBufferConstruct(PPCContext& ctx, uint8_t* base);     // sub_8245F750
void XAudioBufferCreate(PPCContext& ctx, uint8_t* base);        // sub_8245FA80
void XAudioVoiceConstruct(PPCContext& ctx, uint8_t* base);      // sub_82461500
void XAudioDefaultAllocator(PPCContext& ctx, uint8_t* base);    // sub_824605E8
void XAudioGetAllocator(PPCContext& ctx, uint8_t* base);        // sub_82460688

void XAudioConstructOutputs(PPCContext& ctx, uint8_t* base);   // sub_82460738
void XAudioOutputsSize(PPCContext& ctx, uint8_t* base);        // sub_824607C8
void XAudioSetOutput(PPCContext& ctx, uint8_t* base);          // sub_82460998
void XAudioSetOutputVoices(PPCContext& ctx, uint8_t* base);    // sub_82460BF0, sub_8245F228
void XAudioPass(PPCContext& ctx, uint8_t* base);               // sub_82461070
void XAudioDestroy(PPCContext& ctx, uint8_t* base);            // sub_82461240
void XAudioCreateObject(PPCContext& ctx, uint8_t* base);       // sub_82461308
void XAudioProcessEffect(PPCContext& ctx, uint8_t* base);      // sub_82461678 (r13's processor)

}  // namespace hooks
