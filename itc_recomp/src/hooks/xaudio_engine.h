// XAudio's engine and mixer's register steps (src/xaudio/engine.h,
// docs/functions/xaudio-engine.md), shared by the adapters
// (src/hooks/xaudio_engine.cpp, behind --native_xaudio_engine) and the
// harness (without the flag). Each takes the original's registers and
// leaves r3 as the original does.

#pragma once

#include "guest.h"

namespace hooks {

void XAudioFactoryTableSize(PPCContext& ctx, uint8_t* base);      // sub_8245D9E8
void XAudioFactorySizeOf(PPCContext& ctx, uint8_t* base);         // sub_8245DBD0
void XAudioBufferGetFormat(PPCContext& ctx, uint8_t* base);       // sub_8245F6F0
void XAudioBufferGetFormatThunk(PPCContext& ctx, uint8_t* base);  // sub_8245F718
void XAudioBufferInterface(PPCContext& ctx, uint8_t* base);       // sub_8245F8D0
void XAudioAllocatorRemaining(PPCContext& ctx, uint8_t* base);    // sub_82460388
void XAudioInterfaceAddRef(PPCContext& ctx, uint8_t* base);       // sub_82460370
void XAudioObjectInfo(PPCContext& ctx, uint8_t* base);            // sub_82461EF8
void XAudioTakeStats(PPCContext& ctx, uint8_t* base);             // sub_8245F678
void XAudioFree(PPCContext& ctx, uint8_t* base);                  // sub_8245EF90
void XAudioAllocatorAlloc(PPCContext& ctx, uint8_t* base);        // sub_82460288
void XAudioDeleteObject(PPCContext& ctx, uint8_t* base);          // sub_8245AAB8
void XAudioBarrier(PPCContext& ctx, uint8_t* base);               // sub_8245A940 (r13's processor)

}  // namespace hooks
