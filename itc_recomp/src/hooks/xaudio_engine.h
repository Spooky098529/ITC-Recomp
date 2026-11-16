// XAudio's engine and mixer's register steps (src/xaudio/engine.h,
// docs/functions/xaudio-engine.md), shared by the adapters
// (src/hooks/xaudio_engine.cpp, behind --native_xaudio_engine) and the
// harness (without the flag). Each takes the original's registers and
// leaves r3 as the original does.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

void XAudioFactoryTableSize(lifted::Regs& ctx, uint8_t* base);      // sub_8245D9E8
void XAudioFactorySizeOf(lifted::Regs& ctx, uint8_t* base);         // sub_8245DBD0
void XAudioBufferGetFormat(lifted::Regs& ctx, uint8_t* base);       // sub_8245F6F0
void XAudioBufferGetFormatThunk(lifted::Regs& ctx, uint8_t* base);  // sub_8245F718
void XAudioBufferInterface(lifted::Regs& ctx, uint8_t* base);       // sub_8245F8D0
void XAudioAllocatorRemaining(lifted::Regs& ctx, uint8_t* base);    // sub_82460388
void XAudioInterfaceAddRef(lifted::Regs& ctx, uint8_t* base);       // sub_82460370
void XAudioObjectInfo(lifted::Regs& ctx, uint8_t* base);            // sub_82461EF8
void XAudioTakeStats(lifted::Regs& ctx, uint8_t* base);             // sub_8245F678
void XAudioFree(lifted::Regs& ctx, uint8_t* base);                  // sub_8245EF90
void XAudioAllocatorAlloc(lifted::Regs& ctx, uint8_t* base);        // sub_82460288
void XAudioDeleteObject(lifted::Regs& ctx, uint8_t* base);          // sub_8245AAB8
void XAudioBarrier(lifted::Regs& ctx, uint8_t* base);               // sub_8245A940 (r13's processor)

void XAudioEngineConstruct(lifted::Regs& ctx, uint8_t* base);       // sub_8245C098
void XAudioFactoryConstruct(lifted::Regs& ctx, uint8_t* base);      // sub_8245DA48
void XAudioFactoryCreateObject(lifted::Regs& ctx, uint8_t* base);   // sub_8245DC10
void XAudioFactoryCreateSlot(lifted::Regs& ctx, uint8_t* base);     // sub_8245D650 (a tail call)
void XAudioFactorySizeSlot(lifted::Regs& ctx, uint8_t* base);       // sub_8245D6D8 (a tail call)
void XAudioRegisterStats(lifted::Regs& ctx, uint8_t* base);         // sub_8245F588
void XAudioSetUpProcessors(lifted::Regs& ctx, uint8_t* base);       // sub_8245A828
void XAudioEngineSize(lifted::Regs& ctx, uint8_t* base);            // sub_8245B2C0

void XAudioMasteringSize(lifted::Regs& ctx, uint8_t* base);         // sub_8245FF68
void XAudioRenderDriverConstruct(lifted::Regs& ctx, uint8_t* base); // sub_82465938
void XAudioMasteringInit(lifted::Regs& ctx, uint8_t* base);         // sub_8245FCA0
void XAudioMasteringCreate(lifted::Regs& ctx, uint8_t* base);       // sub_824600F0
void XAudioMasteringStart(lifted::Regs& ctx, uint8_t* base);        // sub_8245FE20 (r13 the thread)
void XAudioMasteringPass(lifted::Regs& ctx, uint8_t* base);         // sub_8245FDC8
void XAudioMasteringProcess(lifted::Regs& ctx, uint8_t* base);      // sub_8245FB80 (r13's processor)

void XAudioStageSwap(lifted::Regs& ctx, uint8_t* base);             // sub_8245B208
void XAudioSwapStageLists(lifted::Regs& ctx, uint8_t* base);        // sub_8245B720
void XAudioRunStage(lifted::Regs& ctx, uint8_t* base);              // sub_8245B480
void XAudioRunCallbacks(lifted::Regs& ctx, uint8_t* base);          // sub_8245ACE0
void XAudioRegisterCallback(lifted::Regs& ctx, uint8_t* base);      // sub_8245AB60
void XAudioEngineCommit(lifted::Regs& ctx, uint8_t* base);          // sub_8245B638
void XAudioStageOf(lifted::Regs& ctx, uint8_t* base);               // sub_8245FBF0
void XAudioJoinMix(lifted::Regs& ctx, uint8_t* base);               // sub_82460000

void XAudioAllocatorDestroy(lifted::Regs& ctx, uint8_t* base);         // sub_824602F8
void XAudioDefaultAllocatorDestroy(lifted::Regs& ctx, uint8_t* base);  // sub_824604E8
void XAudioDefaultAllocatorThunk(lifted::Regs& ctx, uint8_t* base);    // sub_824603A0 (a tail call)
void XAudioInterfaceFree(lifted::Regs& ctx, uint8_t* base);            // sub_824603A8

void XAudioMix(lifted::Regs& ctx, uint8_t* base);            // sub_8245BF80 (r13 the thread)
void XAudioMixerCallback(lifted::Regs& ctx, uint8_t* base);  // sub_8245FBE0 (a tail call)
void XAudioMasterThread(lifted::Regs& ctx, uint8_t* base);   // sub_8245BE08
void XAudioWorkerThread(lifted::Regs& ctx, uint8_t* base);   // sub_8245BED0
void XAudioEngineInit(lifted::Regs& ctx, uint8_t* base);     // sub_8245C198 (r3 in full)
void XAudioEngineCreate(lifted::Regs& ctx, uint8_t* base);   // sub_8245C538 (r3 in full)
void XAudioCreateEngine(lifted::Regs& ctx, uint8_t* base);   // sub_82459D60 (r3 in full)

}  // namespace hooks
