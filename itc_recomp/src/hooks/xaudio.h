// XAudio's voices' register steps (src/xaudio/, docs/functions/xaudio.md),
// shared by the adapters (src/hooks/xaudio.cpp, behind --native_xaudio) and
// the harness (without the flag). Each takes the original's registers,
// leaves r3 as the original does, and runs the original (`__imp__`) where
// the plain C++ can't.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"
#include "hooks/guest_call.h"
#include "xaudio/voices.h"

#include <type_traits>

namespace hooks {

// For other subsystems' adapters whose plain C++ calls XAudio's: the guest
// call through `ctx` (the kernel imports at their thunks; the registers not
// given keep what the previous call left), the thread's words (r13, its +256
// and +268), and a body run in a frame of the original's size (its back
// chain, lr the original's; the stack floor saved, cleared and put back),
// whose result goes to r3. `frame_bytes` 0: no frame (a tail call's), the
// body given the stack pointer.
xaudio::GuestCall XAudioCaller(lifted::Regs& ctx);
xaudio::Thread XAudioThreadOf(lifted::Regs& ctx);
template <typename Body>
void XAudioFramed(lifted::Regs& ctx, uint32_t frame_bytes, Body body) {
  const uint64_t lr = ctx.lr;
  uint32_t r3 = 0;
  const uint32_t floor = xaudio::StackFloor();
  xaudio::StackFloor() = 0;
  if (frame_bytes == 0) {
    r3 = body(XAudioCaller(ctx), ctx.r1.u32);
  } else {
    guest_call::InFrame(ctx, frame_bytes, uint32_t(lr),
                        [&](uint32_t frame) { r3 = body(XAudioCaller(ctx), frame); });
  }
  xaudio::StackFloor() = floor;
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}

void XAudioSourceOutputFormat(lifted::Regs& ctx);  // sub_8245DC88
void XAudioSubmixDescribe(lifted::Regs& ctx);      // sub_8245F010
void XAudioDefaultMatrix(lifted::Regs& ctx);       // sub_8245EC40
void XAudioBufferSize(lifted::Regs& ctx);          // sub_8245FA28
void XAudioBufferClear(lifted::Regs& ctx);         // sub_8245F8E8
void XAudioEngineBuffer(lifted::Regs& ctx);        // sub_8245DEB0 (r13's processor)
void XAudioGetType(lifted::Regs& ctx);             // sub_8245E318
void XAudioGetFormat(lifted::Regs& ctx);           // sub_8245E328
void XAudioSetFormatWords(lifted::Regs& ctx);      // sub_82461140
void XAudioAddRef(lifted::Regs& ctx);              // sub_8245F860
void XAudioSetRootVtable(lifted::Regs& ctx);       // sub_8245FB00
void XAudioAddStat(lifted::Regs& ctx);             // sub_8245F640

// Calls into the recompiled objects go through `ctx` (the registers not
// given keep what the previous call left), inside a frame of the original's
// size.
void XAudioFactorySize(lifted::Regs& ctx);         // sub_8245D610
void XAudioFactoryCreate(lifted::Regs& ctx);       // sub_8245D630
void XAudioBufferAddRefThunk(lifted::Regs& ctx);   // sub_8245F6B0
void XAudioBufferReleaseThunk(lifted::Regs& ctx);  // sub_8245F6D0
void XAudioRelease(lifted::Regs& ctx);             // sub_8245F878
void XAudioObjectFree(lifted::Regs& ctx);          // sub_8245AAD8
void XAudioApplyVolume(lifted::Regs& ctx);         // sub_8245DFC0
void XAudioStopAndReset(lifted::Regs& ctx);        // sub_8245DD60
void XAudioSetEffectParameters(lifted::Regs& ctx);  // sub_82461418
void XAudioReleaseOutputs(lifted::Regs& ctx);      // sub_82460A58
void XAudioSetOutputMatrices(lifted::Regs& ctx);   // sub_82460AB8
void XAudioSetFormat(lifted::Regs& ctx);           // sub_82460778
void XAudioSourceDescribe(lifted::Regs& ctx);      // sub_8245E010
void XAudioReformat(lifted::Regs& ctx);            // sub_8245DCF0
void XAudioAlloc(lifted::Regs& ctx);               // sub_8245EF80
void XAudioBufferClearThunk(lifted::Regs& ctx);    // sub_8245FA10
void XAudioBaseSize(lifted::Regs& ctx);            // sub_82461158
void XAudioBufferConstruct(lifted::Regs& ctx);     // sub_8245F750
void XAudioBufferCreate(lifted::Regs& ctx);        // sub_8245FA80
void XAudioVoiceConstruct(lifted::Regs& ctx);      // sub_82461500
void XAudioDefaultAllocator(lifted::Regs& ctx);    // sub_824605E8
void XAudioGetAllocator(lifted::Regs& ctx);        // sub_82460688

void XAudioConstructOutputs(lifted::Regs& ctx);   // sub_82460738
void XAudioOutputsSize(lifted::Regs& ctx);        // sub_824607C8
void XAudioSetOutput(lifted::Regs& ctx);          // sub_82460998
void XAudioSetOutputVoices(lifted::Regs& ctx);    // sub_82460BF0, sub_8245F228
void XAudioPass(lifted::Regs& ctx);               // sub_82461070
void XAudioDestroy(lifted::Regs& ctx);            // sub_82461240
void XAudioCreateObject(lifted::Regs& ctx);       // sub_82461308
void XAudioProcessEffect(lifted::Regs& ctx);      // sub_82461678 (r13's processor)
void XAudioSourceSize(lifted::Regs& ctx);  // sub_8245E0D8
void XAudioCreateEffects(lifted::Regs& ctx);  // sub_82461600
void XAudioInitBase(lifted::Regs& ctx);  // sub_82461D50
void XAudioInitOutputs(lifted::Regs& ctx);  // sub_82460880
void XAudioSourceInit(lifted::Regs& ctx);  // sub_8245E348
void XAudioSubmixInit(lifted::Regs& ctx);  // sub_8245F230
void XAudioProcessEffects(lifted::Regs& ctx);  // sub_82461E88 (r13's processor)
void XAudioMixToOutputs(lifted::Regs& ctx);  // sub_82460CB8 (r13's processor)
void XAudioSubmixPass(lifted::Regs& ctx);  // sub_8245EFA0

// The voice lists' spin lock taken and released by r13's thread (the
// harness's).
void XAudioSpinTake(lifted::Regs& ctx);
void XAudioSpinRelease(lifted::Regs& ctx);
void XAudioSourceGetState(lifted::Regs& ctx);      // sub_8245DDC8
void XAudioPlay(lifted::Regs& ctx);                // sub_82461798
void XAudioActivate(lifted::Regs& ctx);            // sub_82460EF0
void XAudioDeactivate(lifted::Regs& ctx);          // sub_82460D50
void XAudioStartLater(lifted::Regs& ctx);          // sub_8245E7D0
void XAudioWaitForVoice(lifted::Regs& ctx);        // sub_8245AE60
void XAudioCancelPendingStart(lifted::Regs& ctx);  // sub_8245E428
void XAudioStopOrTail(lifted::Regs& ctx);          // sub_824619B0
void XAudioSourceStart(lifted::Regs& ctx);               // sub_8245EBA0
void XAudioSourceStop(lifted::Regs& ctx);                // sub_8245E9F0
void XAudioSourcePass(lifted::Regs& ctx);                // sub_8245DED0
void XAudioSourceConstruct(lifted::Regs& ctx);           // sub_8245E6A8
void XAudioSubmixConstruct(lifted::Regs& ctx);           // sub_8245F298
void XAudioOutputsDestroy(lifted::Regs& ctx);            // sub_82460B50
void XAudioSourceDestroy(lifted::Regs& ctx);             // sub_8245E1B8
void XAudioSourceDeletingDestructor(lifted::Regs& ctx);  // sub_8245E7A0
void XAudioSubmixDestroy(lifted::Regs& ctx);             // sub_8245F108
void XAudioSubmixDeletingDestructor(lifted::Regs& ctx);  // sub_8245F3A0
void XAudioCreateSource(lifted::Regs& ctx);              // sub_8245EAA0
void XAudioCreateSubmix(lifted::Regs& ctx);              // sub_8245F3D0

// The API entry points.
void XAudioApiAddRef(lifted::Regs& ctx);  // sub_82459DB0
void XAudioApiRelease(lifted::Regs& ctx);  // sub_82459E18
void XAudioApiSetEffectParameters(lifted::Regs& ctx);  // sub_82459E80
void XAudioApiSetOutputVoices(lifted::Regs& ctx);  // sub_82459EF0
void XAudioApiSetOutputMatrix(lifted::Regs& ctx);  // sub_82459F48
void XAudioApiGetState(lifted::Regs& ctx);  // sub_82459FA0
void XAudioApiStop(lifted::Regs& ctx);  // sub_8245A1B0
void XAudioApiGetOutputFormat(lifted::Regs& ctx);  // sub_8245A3B8
void XAudioApiSubmitPacket(lifted::Regs& ctx);  // sub_8245A050
void XAudioApiCodec36(lifted::Regs& ctx);  // sub_8245A108
void XAudioApiFlushPackets(lifted::Regs& ctx);  // sub_8245A208
void XAudioApiCodec72(lifted::Regs& ctx);  // sub_8245A270
void XAudioApiSetFormat(lifted::Regs& ctx);  // sub_82459FF8
void XAudioApiReset(lifted::Regs& ctx);  // sub_8245A0B0
void XAudioApiStart(lifted::Regs& ctx);  // sub_8245A168
void XAudioApiSetVolume(lifted::Regs& ctx);  // sub_8245A2C8
void XAudioApiSetFrequencyRatio(lifted::Regs& ctx);  // sub_8245A340
void XAudioApiGetMasteringVoice(lifted::Regs& ctx);  // sub_8245A568
void XAudioApiRegisterCallback(lifted::Regs& ctx);   // sub_8245A5E8
void XAudioApiCommit(lifted::Regs& ctx);             // sub_8245A6E8
void XAudioApiInitialize(lifted::Regs& ctx);         // sub_8245A760 (sub_8245A410)
void XAudioApiCreateSourceVoice(lifted::Regs& ctx);  // sub_8245A768
void XAudioApiCreateSubmixVoice(lifted::Regs& ctx);  // sub_8245A7C8

}  // namespace hooks
