// XAudio's engine and mixer's register steps (src/xaudio/engine.h,
// docs/functions/xaudio-engine.md), shared by the adapters
// (src/hooks/xaudio_engine.cpp, behind --native_xaudio_engine) and the
// harness (without the flag). Each takes the original's registers and
// leaves r3 as the original does.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

void XAudioFactoryTableSize(PPCContext& ctx, uint8_t* base);
void XAudioFactoryTableSize(lifted::Regs& ctx, uint8_t* base);      // sub_8245D9E8
void XAudioFactorySizeOf(PPCContext& ctx, uint8_t* base);
void XAudioFactorySizeOf(lifted::Regs& ctx, uint8_t* base);         // sub_8245DBD0
void XAudioBufferGetFormat(PPCContext& ctx, uint8_t* base);
void XAudioBufferGetFormat(lifted::Regs& ctx, uint8_t* base);       // sub_8245F6F0
void XAudioBufferGetFormatThunk(PPCContext& ctx, uint8_t* base);
void XAudioBufferGetFormatThunk(lifted::Regs& ctx, uint8_t* base);  // sub_8245F718
void XAudioBufferInterface(PPCContext& ctx, uint8_t* base);
void XAudioBufferInterface(lifted::Regs& ctx, uint8_t* base);       // sub_8245F8D0
void XAudioAllocatorRemaining(PPCContext& ctx, uint8_t* base);
void XAudioAllocatorRemaining(lifted::Regs& ctx, uint8_t* base);    // sub_82460388
void XAudioInterfaceAddRef(PPCContext& ctx, uint8_t* base);
void XAudioInterfaceAddRef(lifted::Regs& ctx, uint8_t* base);       // sub_82460370
void XAudioObjectInfo(PPCContext& ctx, uint8_t* base);
void XAudioObjectInfo(lifted::Regs& ctx, uint8_t* base);            // sub_82461EF8
void XAudioTakeStats(PPCContext& ctx, uint8_t* base);
void XAudioTakeStats(lifted::Regs& ctx, uint8_t* base);             // sub_8245F678
void XAudioFree(PPCContext& ctx, uint8_t* base);
void XAudioFree(lifted::Regs& ctx, uint8_t* base);                  // sub_8245EF90
void XAudioAllocatorAlloc(PPCContext& ctx, uint8_t* base);
void XAudioAllocatorAlloc(lifted::Regs& ctx, uint8_t* base);        // sub_82460288
void XAudioDeleteObject(PPCContext& ctx, uint8_t* base);
void XAudioDeleteObject(lifted::Regs& ctx, uint8_t* base);          // sub_8245AAB8
void XAudioBarrier(PPCContext& ctx, uint8_t* base);
void XAudioBarrier(lifted::Regs& ctx, uint8_t* base);               // sub_8245A940 (r13's processor)

void XAudioEngineConstruct(PPCContext& ctx, uint8_t* base);
void XAudioEngineConstruct(lifted::Regs& ctx, uint8_t* base);       // sub_8245C098
void XAudioFactoryConstruct(PPCContext& ctx, uint8_t* base);
void XAudioFactoryConstruct(lifted::Regs& ctx, uint8_t* base);      // sub_8245DA48
void XAudioFactoryCreateObject(PPCContext& ctx, uint8_t* base);
void XAudioFactoryCreateObject(lifted::Regs& ctx, uint8_t* base);   // sub_8245DC10
void XAudioFactoryCreateSlot(PPCContext& ctx, uint8_t* base);
void XAudioFactoryCreateSlot(lifted::Regs& ctx, uint8_t* base);     // sub_8245D650 (a tail call)
void XAudioFactorySizeSlot(PPCContext& ctx, uint8_t* base);
void XAudioFactorySizeSlot(lifted::Regs& ctx, uint8_t* base);       // sub_8245D6D8 (a tail call)
void XAudioRegisterStats(PPCContext& ctx, uint8_t* base);
void XAudioRegisterStats(lifted::Regs& ctx, uint8_t* base);         // sub_8245F588
void XAudioSetUpProcessors(PPCContext& ctx, uint8_t* base);
void XAudioSetUpProcessors(lifted::Regs& ctx, uint8_t* base);       // sub_8245A828
void XAudioEngineSize(PPCContext& ctx, uint8_t* base);
void XAudioEngineSize(lifted::Regs& ctx, uint8_t* base);            // sub_8245B2C0

void XAudioMasteringSize(PPCContext& ctx, uint8_t* base);
void XAudioMasteringSize(lifted::Regs& ctx, uint8_t* base);         // sub_8245FF68
void XAudioRenderDriverConstruct(PPCContext& ctx, uint8_t* base);
void XAudioRenderDriverConstruct(lifted::Regs& ctx, uint8_t* base); // sub_82465938
void XAudioMasteringInit(PPCContext& ctx, uint8_t* base);
void XAudioMasteringInit(lifted::Regs& ctx, uint8_t* base);         // sub_8245FCA0
void XAudioMasteringCreate(PPCContext& ctx, uint8_t* base);
void XAudioMasteringCreate(lifted::Regs& ctx, uint8_t* base);       // sub_824600F0
void XAudioMasteringStart(PPCContext& ctx, uint8_t* base);
void XAudioMasteringStart(lifted::Regs& ctx, uint8_t* base);        // sub_8245FE20 (r13 the thread)
void XAudioMasteringPass(PPCContext& ctx, uint8_t* base);
void XAudioMasteringPass(lifted::Regs& ctx, uint8_t* base);         // sub_8245FDC8
void XAudioMasteringProcess(PPCContext& ctx, uint8_t* base);
void XAudioMasteringProcess(lifted::Regs& ctx, uint8_t* base);      // sub_8245FB80 (r13's processor)

void XAudioStageSwap(PPCContext& ctx, uint8_t* base);
void XAudioStageSwap(lifted::Regs& ctx, uint8_t* base);             // sub_8245B208
void XAudioSwapStageLists(PPCContext& ctx, uint8_t* base);
void XAudioSwapStageLists(lifted::Regs& ctx, uint8_t* base);        // sub_8245B720
void XAudioRunStage(PPCContext& ctx, uint8_t* base);
void XAudioRunStage(lifted::Regs& ctx, uint8_t* base);              // sub_8245B480
void XAudioRunCallbacks(PPCContext& ctx, uint8_t* base);
void XAudioRunCallbacks(lifted::Regs& ctx, uint8_t* base);          // sub_8245ACE0
void XAudioRegisterCallback(PPCContext& ctx, uint8_t* base);
void XAudioRegisterCallback(lifted::Regs& ctx, uint8_t* base);      // sub_8245AB60
void XAudioEngineCommit(PPCContext& ctx, uint8_t* base);
void XAudioEngineCommit(lifted::Regs& ctx, uint8_t* base);          // sub_8245B638
void XAudioStageOf(PPCContext& ctx, uint8_t* base);
void XAudioStageOf(lifted::Regs& ctx, uint8_t* base);               // sub_8245FBF0
void XAudioJoinMix(PPCContext& ctx, uint8_t* base);
void XAudioJoinMix(lifted::Regs& ctx, uint8_t* base);               // sub_82460000

void XAudioAllocatorDestroy(PPCContext& ctx, uint8_t* base);
void XAudioAllocatorDestroy(lifted::Regs& ctx, uint8_t* base);         // sub_824602F8
void XAudioDefaultAllocatorDestroy(PPCContext& ctx, uint8_t* base);
void XAudioDefaultAllocatorDestroy(lifted::Regs& ctx, uint8_t* base);  // sub_824604E8
void XAudioDefaultAllocatorThunk(PPCContext& ctx, uint8_t* base);
void XAudioDefaultAllocatorThunk(lifted::Regs& ctx, uint8_t* base);    // sub_824603A0 (a tail call)
void XAudioInterfaceFree(PPCContext& ctx, uint8_t* base);
void XAudioInterfaceFree(lifted::Regs& ctx, uint8_t* base);            // sub_824603A8

void XAudioMix(PPCContext& ctx, uint8_t* base);
void XAudioMix(lifted::Regs& ctx, uint8_t* base);            // sub_8245BF80 (r13 the thread)
void XAudioMixerCallback(PPCContext& ctx, uint8_t* base);
void XAudioMixerCallback(lifted::Regs& ctx, uint8_t* base);  // sub_8245FBE0 (a tail call)
void XAudioMasterThread(PPCContext& ctx, uint8_t* base);
void XAudioMasterThread(lifted::Regs& ctx, uint8_t* base);   // sub_8245BE08
void XAudioWorkerThread(PPCContext& ctx, uint8_t* base);
void XAudioWorkerThread(lifted::Regs& ctx, uint8_t* base);   // sub_8245BED0
void XAudioEngineInit(PPCContext& ctx, uint8_t* base);
void XAudioEngineInit(lifted::Regs& ctx, uint8_t* base);     // sub_8245C198 (r3 in full)
void XAudioEngineCreate(PPCContext& ctx, uint8_t* base);
void XAudioEngineCreate(lifted::Regs& ctx, uint8_t* base);   // sub_8245C538 (r3 in full)
void XAudioCreateEngine(PPCContext& ctx, uint8_t* base);
void XAudioCreateEngine(lifted::Regs& ctx, uint8_t* base);   // sub_82459D60 (r3 in full)

}  // namespace hooks
