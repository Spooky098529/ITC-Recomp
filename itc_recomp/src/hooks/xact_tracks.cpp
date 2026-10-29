// XACT's tracks, track instances and events natively: adapters in front of
// src/xact/tracks.h (docs/functions/xact-tracks.md), with the register steps
// the harness shares (hooks/xact_tracks.h).

#include "hooks/xact_tracks.h"

#include "base/flags.h"
#include "hooks/xaudio.h"
#include "xact/tracks.h"

ITC_FLAG_BOOL(native_xact_tracks, true, "ITC",
              "Run XACT's tracks, track instances and events (docs/functions/xact-tracks.md: "
              "XACTTrackInstance::XACTTrackInstance sub_825D57D0, CheckDone sub_825D55E0, "
              "SetVolume sub_825D5678, IsBusy sub_825D5780, Pause sub_825D57A8; XACTTrack::Pause "
              "sub_825D05A8 and IsBusy sub_825D0590; XACTEvent::XACTEvent sub_825D1760, "
              "~XACTEvent sub_825D0EF0, Fire sub_825D1358; XACTPlayWaveEvent::XACTPlayWaveEvent "
              "sub_825D82E8, SetQueued sub_825D8530, GetLoopCount sub_825D8358, SetWave "
              "sub_82483208, GetVariationRecord sub_825D8920, ForgetWave sub_825D8360, "
              "GetWaveBank sub_825D84C0, IsPrepared sub_825D76A0, IsPlaying sub_825D7708, IsBusy "
              "sub_825D81E8, Pause sub_825D8268, SetVolume sub_825D7FE0; "
              "XACTWaveVariationEvent::SetWave sub_825D7B78; XACTWave::SetVolume sub_825D1CC8; "
              "the objects' slots through their plain C++ while they hold it, the waves' and "
              "voices' through the guest call) natively instead of through the translated code "
              "(the same reads and writes; checked by --run_harnesses=xact_tracks)");

namespace hooks {

namespace {

// A body with the guest call in a frame of `bytes` (0: the caller's stack
// pointer, a tail call's), its result to r3.
template <typename Body>
void Framed(PPCContext& ctx, uint8_t* base, uint32_t bytes, Body body) {
  XAudioFramed(ctx, base, bytes, body);
}

}  // namespace

void XactTrackInstanceConstruct(PPCContext& ctx, uint8_t* base) {
  xact::TrackInstanceConstruct(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

void XactTrackInstanceCheckDone(PPCContext& ctx, uint8_t* base) {
  xact::TrackInstanceCheckDone(base, ctx.r3.u32);
}

void XactTrackInstanceSetVolume(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t instance = ctx.r3.u32;
  const double f1 = ctx.f1.f64;
  Framed(ctx, base, 0, [&](const xaudio::GuestCall& call, uint32_t sp) {
    return xact::TrackInstanceSetVolume(base, instance, f1, sp, call);
  });
}

void XactTrackInstanceIsBusy(PPCContext& ctx, uint8_t* base) {
  const uint32_t instance = ctx.r3.u32;
  Framed(ctx, base, 0, [&](const xaudio::GuestCall& call, uint32_t sp) {
    return xact::TrackInstanceIsBusy(base, instance, sp, call);
  });
}

void XactTrackInstancePause(PPCContext& ctx, uint8_t* base) {
  const uint32_t instance = ctx.r3.u32, pause = ctx.r4.u32;
  Framed(ctx, base, 0, [&](const xaudio::GuestCall& call, uint32_t sp) {
    return xact::TrackInstancePause(base, instance, pause, sp, call);
  });
}

void XactTrackPause(PPCContext& ctx, uint8_t* base) {
  const uint32_t track = ctx.r3.u32, pause = ctx.r4.u32;
  Framed(ctx, base, 0, [&](const xaudio::GuestCall& call, uint32_t sp) {
    return xact::TrackPause(base, track, pause, sp, call);
  });
}

void XactTrackIsBusy(PPCContext& ctx, uint8_t* base) {
  const uint32_t track = ctx.r3.u32;
  Framed(ctx, base, 0, [&](const xaudio::GuestCall& call, uint32_t sp) {
    return xact::TrackIsBusy(base, track, sp, call);
  });
}

void XactEventConstruct(PPCContext& ctx, uint8_t* base) {
  xact::EventConstruct(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                       ctx.r8.u32);
}

void XactEventDestruct(PPCContext& ctx, uint8_t* base) { xact::EventDestruct(base, ctx.r3.u32); }

void XactEventFire(PPCContext& ctx, uint8_t* base) {
  const uint32_t event = ctx.r3.u32;
  Framed(ctx, base, 96, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::EventFire(base, event, frame, call);
  });
}

void XactPlayWaveConstruct(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = xact::PlayWaveConstruct(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                       ctx.r7.u32, ctx.r8.u32);
}

void XactPlayWaveSetQueued(PPCContext& ctx, uint8_t* base) {
  xact::PlayWaveSetQueued(base, ctx.r3.u32, ctx.r4.u32);
}

void XactPlayWaveGetLoopCount(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = xact::PlayWaveGetLoopCount(base, ctx.r3.u32);
}

void XactPlayWaveSetWave(PPCContext& ctx, uint8_t* base) {
  xact::PlayWaveSetWave(base, ctx.r3.u32, ctx.r4.u32);
}

void XactWaveVariationSetWave(PPCContext& ctx, uint8_t* base) {
  xact::WaveVariationSetWave(base, ctx.r3.u32, ctx.r4.u32);
}

void XactPlayWaveGetVariationRecord(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = xact::PlayWaveGetVariationRecord(base, ctx.r3.u32);
}

void XactPlayWaveForgetWave(PPCContext& ctx, uint8_t* base) {
  const uint32_t event = ctx.r3.u32;
  Framed(ctx, base, 0, [&](const xaudio::GuestCall& call, uint32_t sp) {
    return xact::PlayWaveForgetWave(base, event, sp, call);
  });
}

void XactPlayWaveGetWaveBank(PPCContext& ctx, uint8_t* base) {
  const uint32_t event = ctx.r3.u32;
  Framed(ctx, base, 96, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::PlayWaveGetWaveBank(base, event, frame, call);
  });
}

void XactPlayWaveIsPrepared(PPCContext& ctx, uint8_t* base) {
  const uint32_t event = ctx.r3.u32;
  Framed(ctx, base, 96, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::PlayWaveIsPrepared(base, event, frame, call);
  });
}

void XactPlayWaveIsPlaying(PPCContext& ctx, uint8_t* base) {
  const uint32_t event = ctx.r3.u32;
  Framed(ctx, base, 96, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::PlayWaveIsPlaying(base, event, frame, call);
  });
}

void XactPlayWaveIsBusy(PPCContext& ctx, uint8_t* base) {
  const uint32_t event = ctx.r3.u32;
  Framed(ctx, base, 96, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::PlayWaveIsBusy(base, event, frame, call);
  });
}

void XactPlayWavePause(PPCContext& ctx, uint8_t* base) {
  const uint32_t event = ctx.r3.u32, pause = ctx.r4.u32;
  Framed(ctx, base, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::PlayWavePause(base, event, pause, frame, call);
  });
}

void XactPlayWaveSetVolume(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t event = ctx.r3.u32;
  const double f1 = ctx.f1.f64;
  Framed(ctx, base, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::PlayWaveSetVolume(base, event, f1, frame, call);
  });
}

void XactWaveSetVolume(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t wave = ctx.r3.u32;
  const double f1 = ctx.f1.f64;
  Framed(ctx, base, 0, [&](const xaudio::GuestCall& call, uint32_t sp) {
    return xact::WaveSetVolume(base, wave, f1, sp, call);
  });
}

}  // namespace hooks

// The flag's fallback, then the register step.
#define ITC_XACT_TRACKS_ADAPTER(address, step) \
  extern "C" REX_FUNC(sub_##address) {         \
    REX_FUNC_PROLOGUE();                       \
    if (!ITC_FLAG(native_xact_tracks)) {       \
      __imp__sub_##address(ctx, base);         \
      return;                                  \
    }                                          \
    hooks::step(ctx, base);                    \
  }

ITC_XACT_TRACKS_ADAPTER(825D57D0, XactTrackInstanceConstruct)
ITC_XACT_TRACKS_ADAPTER(825D55E0, XactTrackInstanceCheckDone)
ITC_XACT_TRACKS_ADAPTER(825D5678, XactTrackInstanceSetVolume)
// No adapter (removed 2026-10-29: only XACTTrack::IsBusy's plain C++ calls it in play) for
// XACTTrackInstance::IsBusy sub_825D5780.
// No adapter (removed 2026-10-29: only XACTTrack::Pause's plain C++ calls it in play) for
// XACTTrackInstance::Pause sub_825D57A8.
ITC_XACT_TRACKS_ADAPTER(825D05A8, XactTrackPause)
ITC_XACT_TRACKS_ADAPTER(825D0590, XactTrackIsBusy)
// No adapter (removed 2026-10-29: only XACTPlayWaveEvent::XACTPlayWaveEvent's plain C++ calls
// it in play) for XACTEvent::XACTEvent sub_825D1760.
ITC_XACT_TRACKS_ADAPTER(825D0EF0, XactEventDestruct)
ITC_XACT_TRACKS_ADAPTER(825D1358, XactEventFire)
ITC_XACT_TRACKS_ADAPTER(825D82E8, XactPlayWaveConstruct)
ITC_XACT_TRACKS_ADAPTER(825D8530, XactPlayWaveSetQueued)
ITC_XACT_TRACKS_ADAPTER(825D8358, XactPlayWaveGetLoopCount)
ITC_XACT_TRACKS_ADAPTER(82483208, XactPlayWaveSetWave)
ITC_XACT_TRACKS_ADAPTER(825D7B78, XactWaveVariationSetWave)
ITC_XACT_TRACKS_ADAPTER(825D8920, XactPlayWaveGetVariationRecord)
ITC_XACT_TRACKS_ADAPTER(825D8360, XactPlayWaveForgetWave)
ITC_XACT_TRACKS_ADAPTER(825D84C0, XactPlayWaveGetWaveBank)
ITC_XACT_TRACKS_ADAPTER(825D76A0, XactPlayWaveIsPrepared)
ITC_XACT_TRACKS_ADAPTER(825D7708, XactPlayWaveIsPlaying)
ITC_XACT_TRACKS_ADAPTER(825D81E8, XactPlayWaveIsBusy)
ITC_XACT_TRACKS_ADAPTER(825D8268, XactPlayWavePause)
ITC_XACT_TRACKS_ADAPTER(825D7FE0, XactPlayWaveSetVolume)
// No adapter (removed 2026-10-29: only XACTPlayWaveEvent::SetVolume's plain C++ calls it in
// play) for XACTWave::SetVolume sub_825D1CC8.
