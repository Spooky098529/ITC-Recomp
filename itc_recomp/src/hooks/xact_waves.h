// XACT's voice class and waves' register steps (src/xact/waves.h,
// docs/functions/xact-waves.md), shared by the adapters
// (src/hooks/xact_waves.cpp, behind --native_xact_waves) and the harness
// (without the flag). Each takes the original's registers and leaves r3 as
// the original does, in a frame of the original's size.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

#include <optional>

namespace hooks {

void XactVoiceCreate(lifted::Regs& ctx);         // sub_825C9C40
void XactVoiceGetState(lifted::Regs& ctx);       // sub_825C9DB8
void XactVoiceSetVolume(lifted::Regs& ctx);      // sub_825C9BE8
void XactVoiceSetPitch(lifted::Regs& ctx);       // sub_825C9BF0
void XactVoiceSubmitPacket(lifted::Regs& ctx);   // sub_825C9E08
void XactVoiceStart(lifted::Regs& ctx);          // sub_825C9BF8
void XactVoiceStop(lifted::Regs& ctx);           // sub_825C9C10
void XactVoiceFlushPackets(lifted::Regs& ctx);   // sub_825C9C28
void XactVoiceDeletingDestruct(lifted::Regs& ctx);  // sub_825CA448
void XactWaveConstruct(lifted::Regs& ctx);          // sub_825D23F0
void XactInMemoryWaveConstruct(lifted::Regs& ctx);  // sub_825D37A8
void XactTrackGetPitch(lifted::Regs& ctx);          // sub_825D02B0
void XactWaveRequestNext(lifted::Regs& ctx);        // sub_825D1C30
void XactEngineRequestCommit(lifted::Regs& ctx);    // sub_825D1C78
void XactWaveSetVoice(lifted::Regs& ctx);          // sub_825D1DF8
void XactInMemoryWavePlay(lifted::Regs& ctx);      // sub_825D3938
void XactWavePause(lifted::Regs& ctx);             // sub_825D1ED8
void XactWavePrepareNext(lifted::Regs& ctx);       // sub_825D2778
void XactInMemoryWaveUpdate(lifted::Regs& ctx);    // sub_825D3890
// XACT's notifications: XACTEngine::SendNotification (xact.md's row, under
// --native_xact), XACTWave::Notify and SetState.
void XactEngineSendNotification(lifted::Regs& ctx);  // sub_825C1388
void XactWaveNotify(lifted::Regs& ctx);              // sub_825D2030
void XactWaveSetState(lifted::Regs& ctx);            // sub_825D24D8
// Releasing and stopping waves.
void XactWaveRelease(lifted::Regs& ctx);                    // sub_825D28D0
void XactWaveDestruct(lifted::Regs& ctx);                   // sub_825D29E8
void XactInMemoryWaveDeletingDestruct(lifted::Regs& ctx);   // sub_825D3820
void XactInMemoryWaveStop(lifted::Regs& ctx);               // sub_825D3A28
// Preparing a wave and the audio callback's update (r3 in full).
void XactInMemoryWavePrepare(lifted::Regs& ctx);  // sub_825D3B10
void XactWaveAudioUpdate(lifted::Regs& ctx);      // sub_825D2560
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
void XactTrackGetVolume(lifted::Regs& ctx);         // sub_825D0268
void XactWaveGetStartPosition(lifted::Regs& ctx);   // sub_825D2170

}  // namespace hooks
