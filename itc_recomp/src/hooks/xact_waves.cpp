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
              "destructor sub_825CA448, over XAudio's entry points' plain C++; XACTWave::XACTWave "
              "sub_825D23F0 and XACTInMemoryWave::XACTInMemoryWave sub_825D37A8 (the wave bank's "
              "AddListener's plain C++ while its slot holds it); XACTTrack::GetVolume sub_825D0268 "
              "and GetPitch sub_825D02B0; XACTWave::GetStartPosition sub_825D2170; "
              "XACTWave::RequestNext sub_825D1C30; XACTEngine::RequestCommit sub_825D1C78, the "
              "engine's critical section entered as the original does; XACTWave::SetVoice "
              "sub_825D1DF8, XACTInMemoryWave::Play sub_825D3938, XACTWave::Pause sub_825D1ED8 "
              "and XACTWave::PrepareNext sub_825D2778, XACTInMemoryWave::Update sub_825D3890, XACTWave::Notify sub_825D2030 and SetState "
              "sub_825D24D8, the voice's methods through their plain "
              "C++ while its slots hold them, the owner's and renderer's slots through the "
              "guest call) natively instead "
              "of through the translated code (the same reads and writes; XAudio's locks stay "
              "the game's, taken as the original does; checked by "
              "--run_harnesses=xact_waves)");

ITC_FLAG_DECLARE(bool, native_xact);

namespace hooks {

void XactEngineSendNotification(PPCContext& ctx, uint8_t* base) {
  const uint32_t engine = ctx.r3.u32;
  const uint64_t words[4] = {ctx.r4.u64, ctx.r5.u64, ctx.r6.u64, ctx.r7.u64};
  XAudioFramed(ctx, base, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::EngineSendNotification(base, engine, words, frame, call);
  });
}

void XactWaveNotify(PPCContext& ctx, uint8_t* base) {
  const uint32_t wave = ctx.r3.u32, type = ctx.r4.u32;
  XAudioFramed(ctx, base, 160, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::WaveNotify(base, wave, type, frame, call);
  });
}

void XactWaveSetState(PPCContext& ctx, uint8_t* base) {
  const uint32_t wave = ctx.r3.u32, state = ctx.r4.u32;
  XAudioFramed(ctx, base, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::WaveSetState(base, wave, state, frame, call);
  });
}

void XactVoiceCreate(PPCContext& ctx, uint8_t* base) {
  const uint32_t description = ctx.r3.u32, holder = ctx.r4.u32, level = ctx.r5.u32,
                 reverb = ctx.r6.u32, out = ctx.r7.u32;
  const xaudio::Thread thread = XAudioThreadOf(ctx, base);
  XAudioFramed(ctx, base, 144, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::VoiceCreate(base, description, holder, level, reverb, out, thread, frame, call);
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

void XactWaveConstruct(PPCContext& ctx, uint8_t* base) {
  const uint32_t wave = ctx.r3.u32, engine = ctx.r4.u32, bank = ctx.r5.u32, index = ctx.r6.u32,
                 owner = ctx.r7.u32;
  const bool native_heap = NativeHeapActive();
  XAudioFramed(ctx, base, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::WaveConstruct(base, wave, engine, bank, index, owner, native_heap, frame, call);
  });
}

void XactInMemoryWaveConstruct(PPCContext& ctx, uint8_t* base) {
  const uint32_t wave = ctx.r3.u32, engine = ctx.r4.u32, bank = ctx.r5.u32, index = ctx.r6.u32,
                 owner = ctx.r7.u32;
  const bool native_heap = NativeHeapActive();
  XAudioFramed(ctx, base, 96, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::InMemoryWaveConstruct(base, wave, engine, bank, index, owner, native_heap,
                                       frame, call);
  });
}

void XactTrackGetVolume(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (const auto done = xact::TrackGetVolume(base, ctx.r3.u32)) {
    ctx.f1.f64 = done->f1;
    ctx.r3.u64 = done->r3;
    return;
  }
  __imp__sub_825D0268(ctx, base);
}

void XactTrackGetPitch(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const xact::CategoryVolume done = xact::TrackGetPitch(base, ctx.r3.u32);
  ctx.f1.f64 = done.f1;
  ctx.r3.u64 = done.r3;
}

void XactWaveGetStartPosition(PPCContext& ctx, uint8_t* base) {
  if (const auto done = xact::WaveGetStartPosition(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                   ctx.r6.u32, ctx.r7.u32)) {
    ctx.r3.u64 = *done;
    return;
  }
  __imp__sub_825D2170(ctx, base);
}

void XactWaveRequestNext(PPCContext& ctx, uint8_t* base) {
  xact::WaveRequestNext(base, ctx.r3.u32, ctx.r5.u32);
}

void XactEngineRequestCommit(PPCContext& ctx, uint8_t* base) {
  const uint32_t engine = ctx.r3.u32;
  XAudioFramed(ctx, base, 112, [&](const xaudio::GuestCall& call, uint32_t) {
    return xact::EngineRequestCommit(base, engine, call);
  });
}

void XactWaveSetVoice(PPCContext& ctx, uint8_t* base) {
  const uint32_t wave = ctx.r3.u32, voice = ctx.r4.u32;
  XAudioFramed(ctx, base, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::WaveSetVoice(base, wave, voice, frame, call);
  });
}

void XactInMemoryWavePlay(PPCContext& ctx, uint8_t* base) {
  const uint32_t wave = ctx.r3.u32, deferred = ctx.r4.u32;
  const xaudio::Thread thread = XAudioThreadOf(ctx, base);
  XAudioFramed(ctx, base, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::InMemoryWavePlay(base, wave, deferred, thread, frame, call);
  });
}

void XactWavePause(PPCContext& ctx, uint8_t* base) {
  const uint32_t wave = ctx.r3.u32, pause = ctx.r4.u32;
  const xaudio::Thread thread = XAudioThreadOf(ctx, base);
  XAudioFramed(ctx, base, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::WavePause(base, wave, pause, thread, frame, call);
  });
}

void XactWavePrepareNext(PPCContext& ctx, uint8_t* base) {
  const uint32_t wave = ctx.r3.u32;
  const bool native_heap = NativeHeapActive();
  XAudioFramed(ctx, base, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::WavePrepareNext(base, wave, native_heap, frame, call);
  });
}

void XactInMemoryWaveUpdate(PPCContext& ctx, uint8_t* base) {
  const uint32_t wave = ctx.r3.u32;
  const bool native_heap = NativeHeapActive();
  XAudioFramed(ctx, base, 96, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::InMemoryWaveUpdate(base, wave, native_heap, frame, call);
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
// No adapter (removed 2026-10-29: only XACTVoice::Create's plain C++ calls it in play) for
// XACTVoice::Init sub_825C9B60.
ITC_XACT_WAVES_ADAPTER(825C9DB8, XactVoiceGetState)
ITC_XACT_WAVES_ADAPTER(825C9BE8, XactVoiceSetVolume)
ITC_XACT_WAVES_ADAPTER(825C9BF0, XactVoiceSetPitch)
ITC_XACT_WAVES_ADAPTER(825C9E08, XactVoiceSubmitPacket)
ITC_XACT_WAVES_ADAPTER(825C9BF8, XactVoiceStart)
ITC_XACT_WAVES_ADAPTER(825C9C10, XactVoiceStop)
ITC_XACT_WAVES_ADAPTER(825C9C28, XactVoiceFlushPackets)
ITC_XACT_WAVES_HEAP_ADAPTER(825CA448, XactVoiceDeletingDestruct)
// No adapter (removed 2026-10-29: only XACTInMemoryWave::XACTInMemoryWave's plain C++ calls it
// in play) for XACTWave::XACTWave sub_825D23F0.
ITC_XACT_WAVES_ADAPTER(825D37A8, XactInMemoryWaveConstruct)
ITC_XACT_WAVES_ADAPTER(825D0268, XactTrackGetVolume)
ITC_XACT_WAVES_ADAPTER(825D02B0, XactTrackGetPitch)
ITC_XACT_WAVES_ADAPTER(825D2170, XactWaveGetStartPosition)
ITC_XACT_WAVES_ADAPTER(825D1C30, XactWaveRequestNext)
// No adapter (removed 2026-10-29: only XACTInMemoryWave::Play's and XACTWave::Pause's plain C++
// calls it in play) for XACTEngine::RequestCommit sub_825D1C78.
ITC_XACT_WAVES_ADAPTER(825D1DF8, XactWaveSetVoice)
ITC_XACT_WAVES_ADAPTER(825D3938, XactInMemoryWavePlay)
ITC_XACT_WAVES_ADAPTER(825D1ED8, XactWavePause)
// No adapter (removed 2026-10-29: only XACTInMemoryWave::Update's plain C++ calls it in play) for
// XACTWave::PrepareNext sub_825D2778.
ITC_XACT_WAVES_ADAPTER(825D3890, XactInMemoryWaveUpdate)
ITC_XACT_WAVES_ADAPTER(825D2030, XactWaveNotify)
ITC_XACT_WAVES_ADAPTER(825D24D8, XactWaveSetState)

// XACTEngine::SendNotification is xact.md's: under --native_xact.
extern "C" REX_FUNC(sub_825C1388) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_xact)) {
    __imp__sub_825C1388(ctx, base);
    return;
  }
  hooks::XactEngineSendNotification(ctx, base);
}
