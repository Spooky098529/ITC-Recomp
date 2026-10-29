// XAudio's voices' register steps (src/xaudio/, docs/functions/xaudio.md),
// shared by the adapters (src/hooks/xaudio.cpp, behind --native_xaudio) and
// the harness (without the flag). Each takes the original's registers,
// leaves r3 as the original does, and runs the original (`__imp__`) where
// the plain C++ can't.

#pragma once

#include "guest.h"
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
xaudio::GuestCall XAudioCaller(PPCContext& ctx, uint8_t* base);
xaudio::Thread XAudioThreadOf(PPCContext& ctx, uint8_t* base);
template <typename Body>
void XAudioFramed(PPCContext& ctx, uint8_t* base, uint32_t frame_bytes, Body body) {
  const uint64_t lr = ctx.lr;
  uint32_t r3 = 0;
  const uint32_t floor = xaudio::StackFloor();
  xaudio::StackFloor() = 0;
  if (frame_bytes == 0) {
    r3 = body(XAudioCaller(ctx, base), ctx.r1.u32);
  } else {
    guest_call::InFrame(ctx, base, frame_bytes, uint32_t(lr),
                        [&](uint32_t frame) { r3 = body(XAudioCaller(ctx, base), frame); });
  }
  xaudio::StackFloor() = floor;
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}

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
void XAudioSourceSize(PPCContext& ctx, uint8_t* base);  // sub_8245E0D8
void XAudioCreateEffects(PPCContext& ctx, uint8_t* base);  // sub_82461600
void XAudioInitBase(PPCContext& ctx, uint8_t* base);  // sub_82461D50
void XAudioInitOutputs(PPCContext& ctx, uint8_t* base);  // sub_82460880
void XAudioSourceInit(PPCContext& ctx, uint8_t* base);  // sub_8245E348
void XAudioSubmixInit(PPCContext& ctx, uint8_t* base);  // sub_8245F230
void XAudioProcessEffects(PPCContext& ctx, uint8_t* base);  // sub_82461E88 (r13's processor)
void XAudioMixToOutputs(PPCContext& ctx, uint8_t* base);  // sub_82460CB8 (r13's processor)
void XAudioSubmixPass(PPCContext& ctx, uint8_t* base);  // sub_8245EFA0

// The voice lists' spin lock taken and released by r13's thread (the
// harness's).
void XAudioSpinTake(PPCContext& ctx, uint8_t* base);
void XAudioSpinRelease(PPCContext& ctx, uint8_t* base);
void XAudioSourceGetState(PPCContext& ctx, uint8_t* base);      // sub_8245DDC8
void XAudioPlay(PPCContext& ctx, uint8_t* base);                // sub_82461798
void XAudioActivate(PPCContext& ctx, uint8_t* base);            // sub_82460EF0
void XAudioDeactivate(PPCContext& ctx, uint8_t* base);          // sub_82460D50
void XAudioStartLater(PPCContext& ctx, uint8_t* base);          // sub_8245E7D0
void XAudioWaitForVoice(PPCContext& ctx, uint8_t* base);        // sub_8245AE60
void XAudioCancelPendingStart(PPCContext& ctx, uint8_t* base);  // sub_8245E428
void XAudioStopOrTail(PPCContext& ctx, uint8_t* base);          // sub_824619B0
void XAudioSourceStart(PPCContext& ctx, uint8_t* base);               // sub_8245EBA0
void XAudioSourceStop(PPCContext& ctx, uint8_t* base);                // sub_8245E9F0
void XAudioSourcePass(PPCContext& ctx, uint8_t* base);                // sub_8245DED0
void XAudioSourceConstruct(PPCContext& ctx, uint8_t* base);           // sub_8245E6A8
void XAudioSubmixConstruct(PPCContext& ctx, uint8_t* base);           // sub_8245F298
void XAudioOutputsDestroy(PPCContext& ctx, uint8_t* base);            // sub_82460B50
void XAudioSourceDestroy(PPCContext& ctx, uint8_t* base);             // sub_8245E1B8
void XAudioSourceDeletingDestructor(PPCContext& ctx, uint8_t* base);  // sub_8245E7A0
void XAudioSubmixDestroy(PPCContext& ctx, uint8_t* base);             // sub_8245F108
void XAudioSubmixDeletingDestructor(PPCContext& ctx, uint8_t* base);  // sub_8245F3A0
void XAudioCreateSource(PPCContext& ctx, uint8_t* base);              // sub_8245EAA0
void XAudioCreateSubmix(PPCContext& ctx, uint8_t* base);              // sub_8245F3D0

// The API entry points.
void XAudioApiAddRef(PPCContext& ctx, uint8_t* base);  // sub_82459DB0
void XAudioApiRelease(PPCContext& ctx, uint8_t* base);  // sub_82459E18
void XAudioApiSetEffectParameters(PPCContext& ctx, uint8_t* base);  // sub_82459E80
void XAudioApiSetOutputVoices(PPCContext& ctx, uint8_t* base);  // sub_82459EF0
void XAudioApiSetOutputMatrix(PPCContext& ctx, uint8_t* base);  // sub_82459F48
void XAudioApiGetState(PPCContext& ctx, uint8_t* base);  // sub_82459FA0
void XAudioApiStop(PPCContext& ctx, uint8_t* base);  // sub_8245A1B0
void XAudioApiGetOutputFormat(PPCContext& ctx, uint8_t* base);  // sub_8245A3B8
void XAudioApiSubmitPacket(PPCContext& ctx, uint8_t* base);  // sub_8245A050
void XAudioApiCodec36(PPCContext& ctx, uint8_t* base);  // sub_8245A108
void XAudioApiFlushPackets(PPCContext& ctx, uint8_t* base);  // sub_8245A208
void XAudioApiCodec72(PPCContext& ctx, uint8_t* base);  // sub_8245A270
void XAudioApiSetFormat(PPCContext& ctx, uint8_t* base);  // sub_82459FF8
void XAudioApiReset(PPCContext& ctx, uint8_t* base);  // sub_8245A0B0
void XAudioApiStart(PPCContext& ctx, uint8_t* base);  // sub_8245A168
void XAudioApiSetVolume(PPCContext& ctx, uint8_t* base);  // sub_8245A2C8
void XAudioApiSetFrequencyRatio(PPCContext& ctx, uint8_t* base);  // sub_8245A340
void XAudioApiGetMasteringVoice(PPCContext& ctx, uint8_t* base);  // sub_8245A568
void XAudioApiCreateSourceVoice(PPCContext& ctx, uint8_t* base);  // sub_8245A768
void XAudioApiCreateSubmixVoice(PPCContext& ctx, uint8_t* base);  // sub_8245A7C8

}  // namespace hooks
