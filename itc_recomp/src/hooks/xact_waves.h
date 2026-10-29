// XACT's voice class and waves' register steps (src/xact/waves.h,
// docs/functions/xact-waves.md), shared by the adapters
// (src/hooks/xact_waves.cpp, behind --native_xact_waves) and the harness
// (without the flag). Each takes the original's registers and leaves r3 as
// the original does, in a frame of the original's size.

#pragma once

#include "guest.h"

namespace hooks {

void XactVoiceCreate(PPCContext& ctx, uint8_t* base);         // sub_825C9C40
void XactVoiceGetState(PPCContext& ctx, uint8_t* base);       // sub_825C9DB8
void XactVoiceSetVolume(PPCContext& ctx, uint8_t* base);      // sub_825C9BE8
void XactVoiceSetPitch(PPCContext& ctx, uint8_t* base);       // sub_825C9BF0
void XactVoiceSubmitPacket(PPCContext& ctx, uint8_t* base);   // sub_825C9E08
void XactVoiceStart(PPCContext& ctx, uint8_t* base);          // sub_825C9BF8
void XactVoiceStop(PPCContext& ctx, uint8_t* base);           // sub_825C9C10
void XactVoiceFlushPackets(PPCContext& ctx, uint8_t* base);   // sub_825C9C28
void XactVoiceDeletingDestruct(PPCContext& ctx, uint8_t* base);  // sub_825CA448
void XactWaveConstruct(PPCContext& ctx, uint8_t* base);          // sub_825D23F0
void XactInMemoryWaveConstruct(PPCContext& ctx, uint8_t* base);  // sub_825D37A8
void XactTrackGetPitch(PPCContext& ctx, uint8_t* base);          // sub_825D02B0
void XactWaveRequestNext(PPCContext& ctx, uint8_t* base);        // sub_825D1C30
void XactEngineRequestCommit(PPCContext& ctx, uint8_t* base);    // sub_825D1C78
void XactWaveSetVoice(PPCContext& ctx, uint8_t* base);          // sub_825D1DF8
void XactInMemoryWavePlay(PPCContext& ctx, uint8_t* base);      // sub_825D3938
void XactWavePause(PPCContext& ctx, uint8_t* base);             // sub_825D1ED8
void XactWavePrepareNext(PPCContext& ctx, uint8_t* base);       // sub_825D2778
void XactInMemoryWaveUpdate(PPCContext& ctx, uint8_t* base);    // sub_825D3890
// XACT's notifications: XACTEngine::SendNotification (xact.md's row, under
// --native_xact), XACTWave::Notify and SetState.
void XactEngineSendNotification(PPCContext& ctx, uint8_t* base);  // sub_825C1388
void XactWaveNotify(PPCContext& ctx, uint8_t* base);              // sub_825D2030
void XactWaveSetState(PPCContext& ctx, uint8_t* base);            // sub_825D24D8
// These run the original (`__imp__`) where the plain C++ declines.
void XactTrackGetVolume(PPCContext& ctx, uint8_t* base);         // sub_825D0268
void XactWaveGetStartPosition(PPCContext& ctx, uint8_t* base);   // sub_825D2170

}  // namespace hooks
