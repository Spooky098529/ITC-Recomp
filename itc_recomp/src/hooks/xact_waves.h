// XACT's voice class and waves' register steps (src/xact/waves.h,
// docs/functions/xact-waves.md), shared by the adapters
// (src/hooks/xact_waves.cpp, behind --native_xact_waves) and the harness
// (without the flag). Each takes the original's registers and leaves r3 as
// the original does, in a frame of the original's size.

#pragma once

#include "guest.h"

#include <optional>

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
// Releasing and stopping waves.
void XactWaveRelease(PPCContext& ctx, uint8_t* base);                    // sub_825D28D0
void XactWaveDestruct(PPCContext& ctx, uint8_t* base);                   // sub_825D29E8
void XactInMemoryWaveDeletingDestruct(PPCContext& ctx, uint8_t* base);   // sub_825D3820
void XactInMemoryWaveStop(PPCContext& ctx, uint8_t* base);               // sub_825D3A28
// Preparing a wave and the audio callback's update (r3 in full).
void XactInMemoryWavePrepare(PPCContext& ctx, uint8_t* base);  // sub_825D3B10
void XactWaveAudioUpdate(PPCContext& ctx, uint8_t* base);      // sub_825D2560
// The harness's stand-in for XACTRenderer::CreateVoice (sub_825C31D8) on
// this thread: r3 to r10 to `record` (eight words), [r10] = `voice` when `r3`
// isn't an error, r3 sign-extended; nullopt: the original.
struct CreateVoiceStandIn {
  uint32_t record;
  uint32_t voice;
  uint32_t r3;
};
void SetCreateVoiceForHarness(std::optional<CreateVoiceStandIn> stand_in);
// These run the original (`__imp__`) where the plain C++ declines.
void XactTrackGetVolume(PPCContext& ctx, uint8_t* base);         // sub_825D0268
void XactWaveGetStartPosition(PPCContext& ctx, uint8_t* base);   // sub_825D2170

}  // namespace hooks
