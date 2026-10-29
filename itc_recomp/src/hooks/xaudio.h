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

}  // namespace hooks
