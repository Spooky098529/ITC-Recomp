// XACT's voice class and waves natively: adapters in front of
// src/xact/waves.h (docs/functions/xact-waves.md), with the register steps
// the harness shares (hooks/xact_waves.h).

#include "hooks/xact_waves.h"

#include "base/flags.h"
#include "hooks/heap.h"
#include "hooks/xaudio.h"
#include "xact/waves.h"

ITC_FLAG_BOOL(native_xact_waves, true, "ITC",
              "Run XACT's voice class and waves (docs/functions/xact-waves.md: "
              "XACTVoice::Create sub_825C9C40 with Init sub_825C9B60, GetState sub_825C9DB8, "
              "SetVolume sub_825C9BE8, SetPitch sub_825C9BF0, SubmitPacket sub_825C9E08, Start "
              "sub_825C9BF8, Stop sub_825C9C10, FlushPackets sub_825C9C28 and the deleting "
              "destructor sub_825CA448, over XAudio's entry points' plain C++) natively instead "
              "of through the translated code (the same reads and writes; XAudio's locks stay "
              "the game's, taken as the original does; checked by "
              "--run_harnesses=xact_waves)");

namespace hooks {

void XactVoiceCreate(PPCContext& ctx, uint8_t* base) {
  const uint32_t description = ctx.r3.u32, holder = ctx.r4.u32, level = ctx.r5.u32,
                 reverb = ctx.r6.u32, out = ctx.r7.u32;
  const xaudio::Thread thread = XAudioThreadOf(ctx, base);
  XAudioFramed(ctx, base, 144, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::VoiceCreate(base, description, holder, level, reverb, out, thread, frame, call);
  });
}

void XactVoiceInit(PPCContext& ctx, uint8_t* base) {
  const uint32_t voice = ctx.r3.u32, description = ctx.r4.u32, holder = ctx.r5.u32,
                 level = ctx.r6.u32, reverb = ctx.r7.u32;
  const xaudio::Thread thread = XAudioThreadOf(ctx, base);
  XAudioFramed(ctx, base, 128, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::VoiceInit(base, voice, description, holder, level, reverb, thread, frame, call);
  });
}

void XactVoiceGetState(PPCContext& ctx, uint8_t* base) {
  const uint32_t voice = ctx.r3.u32, out = ctx.r4.u32;
  XAudioFramed(ctx, base, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::VoiceGetState(base, voice, out, frame, call);
  });
}

void XactVoiceSetVolume(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t voice = ctx.r3.u32;
  const double f1 = ctx.f1.f64;
  XAudioFramed(ctx, base, 0, [&](const xaudio::GuestCall& call, uint32_t sp) {
    return xact::VoiceSetVolume(base, voice, f1, sp, call);
  });
}

void XactVoiceSetPitch(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t voice = ctx.r3.u32;
  const double f1 = ctx.f1.f64;
  XAudioFramed(ctx, base, 0, [&](const xaudio::GuestCall& call, uint32_t sp) {
    return xact::VoiceSetPitch(base, voice, f1, sp, call);
  });
}

void XactVoiceSubmitPacket(PPCContext& ctx, uint8_t* base) {
  const uint32_t voice = ctx.r3.u32, packet = ctx.r4.u32, flag = ctx.r5.u32;
  XAudioFramed(ctx, base, 128, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::VoiceSubmitPacket(base, voice, packet, flag, frame, call);
  });
}

void XactVoiceStart(PPCContext& ctx, uint8_t* base) {
  const uint32_t voice = ctx.r3.u32, deferred = ctx.r4.u32;
  const xaudio::Thread thread = XAudioThreadOf(ctx, base);
  XAudioFramed(ctx, base, 0, [&](const xaudio::GuestCall& call, uint32_t sp) {
    return xact::VoiceStart(base, voice, deferred, thread, sp, call);
  });
}

void XactVoiceStop(PPCContext& ctx, uint8_t* base) {
  const uint32_t voice = ctx.r3.u32, now = ctx.r4.u32;
  XAudioFramed(ctx, base, 0, [&](const xaudio::GuestCall& call, uint32_t sp) {
    return xact::VoiceStop(base, voice, now, sp, call);
  });
}

void XactVoiceFlushPackets(PPCContext& ctx, uint8_t* base) {
  const uint32_t voice = ctx.r3.u32;
  XAudioFramed(ctx, base, 0, [&](const xaudio::GuestCall& call, uint32_t sp) {
    return xact::VoiceFlushPackets(base, voice, sp, call);
  });
}

void XactVoiceDeletingDestruct(PPCContext& ctx, uint8_t* base) {
  const uint32_t voice = ctx.r3.u32, flags = ctx.r4.u32;
  XAudioFramed(ctx, base, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::VoiceDeletingDestruct(base, voice, flags, frame, call);
  });
}

}  // namespace hooks

// The flag's fallback, then the register step.
#define ITC_XACT_WAVES_ADAPTER(address, step) \
  extern "C" REX_FUNC(sub_##address) {        \
    REX_FUNC_PROLOGUE();                      \
    if (!ITC_FLAG(native_xact_waves)) {       \
      __imp__sub_##address(ctx, base);        \
      return;                                 \
    }                                         \
    hooks::step(ctx, base);                   \
  }

// The same for a function allocating or freeing through the native layer:
// the original unless --native_heap routes the game's heap there.
#define ITC_XACT_WAVES_HEAP_ADAPTER(address, step)                    \
  extern "C" REX_FUNC(sub_##address) {                                \
    REX_FUNC_PROLOGUE();                                              \
    if (!ITC_FLAG(native_xact_waves) || !hooks::NativeHeapActive()) { \
      __imp__sub_##address(ctx, base);                                \
      return;                                                         \
    }                                                                 \
    hooks::step(ctx, base);                                           \
  }

ITC_XACT_WAVES_HEAP_ADAPTER(825C9C40, XactVoiceCreate)
ITC_XACT_WAVES_HEAP_ADAPTER(825C9B60, XactVoiceInit)
ITC_XACT_WAVES_ADAPTER(825C9DB8, XactVoiceGetState)
ITC_XACT_WAVES_ADAPTER(825C9BE8, XactVoiceSetVolume)
ITC_XACT_WAVES_ADAPTER(825C9BF0, XactVoiceSetPitch)
ITC_XACT_WAVES_ADAPTER(825C9E08, XactVoiceSubmitPacket)
ITC_XACT_WAVES_ADAPTER(825C9BF8, XactVoiceStart)
ITC_XACT_WAVES_ADAPTER(825C9C10, XactVoiceStop)
ITC_XACT_WAVES_ADAPTER(825C9C28, XactVoiceFlushPackets)
ITC_XACT_WAVES_HEAP_ADAPTER(825CA448, XactVoiceDeletingDestruct)
