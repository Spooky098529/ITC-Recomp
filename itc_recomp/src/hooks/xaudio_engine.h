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

void XAudioEngineConstruct(PPCContext& ctx, uint8_t* base);       // sub_8245C098
void XAudioFactoryConstruct(PPCContext& ctx, uint8_t* base);      // sub_8245DA48
void XAudioFactoryCreateObject(PPCContext& ctx, uint8_t* base);   // sub_8245DC10
void XAudioFactoryCreateSlot(PPCContext& ctx, uint8_t* base);     // sub_8245D650 (a tail call)
void XAudioFactorySizeSlot(PPCContext& ctx, uint8_t* base);       // sub_8245D6D8 (a tail call)
void XAudioRegisterStats(PPCContext& ctx, uint8_t* base);         // sub_8245F588
void XAudioSetUpProcessors(PPCContext& ctx, uint8_t* base);       // sub_8245A828
void XAudioEngineSize(PPCContext& ctx, uint8_t* base);            // sub_8245B2C0

void XAudioMasteringSize(PPCContext& ctx, uint8_t* base);         // sub_8245FF68
void XAudioRenderDriverConstruct(PPCContext& ctx, uint8_t* base); // sub_82465938
void XAudioMasteringInit(PPCContext& ctx, uint8_t* base);         // sub_8245FCA0
void XAudioMasteringCreate(PPCContext& ctx, uint8_t* base);       // sub_824600F0
void XAudioMasteringStart(PPCContext& ctx, uint8_t* base);        // sub_8245FE20 (r13 the thread)
void XAudioMasteringPass(PPCContext& ctx, uint8_t* base);         // sub_8245FDC8
void XAudioMasteringProcess(PPCContext& ctx, uint8_t* base);      // sub_8245FB80 (r13's processor)

void XAudioStageSwap(PPCContext& ctx, uint8_t* base);             // sub_8245B208
void XAudioSwapStageLists(PPCContext& ctx, uint8_t* base);        // sub_8245B720
void XAudioRunStage(PPCContext& ctx, uint8_t* base);              // sub_8245B480
void XAudioRunCallbacks(PPCContext& ctx, uint8_t* base);          // sub_8245ACE0
void XAudioRegisterCallback(PPCContext& ctx, uint8_t* base);      // sub_8245AB60
void XAudioEngineCommit(PPCContext& ctx, uint8_t* base);          // sub_8245B638
void XAudioStageOf(PPCContext& ctx, uint8_t* base);               // sub_8245FBF0
void XAudioJoinMix(PPCContext& ctx, uint8_t* base);               // sub_82460000

}  // namespace hooks
