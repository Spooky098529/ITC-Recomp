// XACT's tracks, track instances and events natively: adapters in front of
// src/xact/tracks.h (docs/functions/xact-tracks.md), with the register steps
// the harness shares (hooks/xact_tracks.h).

#include "hooks/xact_tracks.h"

#include "base/flags.h"
#include "hooks/heap.h"
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
              "XACTTrack::SetVolume sub_825D0BF8, UpdateVolume sub_825D0428, CreateRpcs "
              "sub_825D06B8 and StartRpcs sub_825D0800 (without RPCs), XACTTrack sub_825D0CE0, "
              "Update sub_825D0E30, ~XACTTrack sub_825D0110 and its deleting destructor "
              "sub_825D0978; XACTPlayWaveEvent's deleting destructor sub_825D8660, "
              "ApplyVariation sub_825D7558, GetDuration sub_825D8870, Prepare sub_825D86F8; "
              "XACTTrackInstance::PrepareSimple sub_825D53D8 and XACTTrack::PrepareSimple "
              "sub_825D09D0 (allocating through the native layer under --native_heap); "
              "XACTTrackInstance::CancelEvents sub_825D5520 (XACT's lock, the engine's "
              "critical section, entered as the original does), ~XACTTrackInstance sub_825D5838 "
              "and its deleting destructor sub_825D5E58, Stop sub_825D5D18; XACTTrack::Stop "
              "sub_825D01D8; XACTEvent::Create sub_825D0F00 (the play-wave types; the others "
              "run the original); XACTTrackInstance::Prepare sub_825D5A48, XACTTrack::CreateInstance "
              "sub_825D02F8 and Prepare sub_825D0AA0 (a complex sound's track; under --native_heap); "
              "XACTSound::ScheduleEvent sub_825CB0C8; XACTEvent::Schedule sub_825D1918 and Done "
              "sub_825D13F0, XACTPlayWaveEvent::Fire sub_825D8378 and Stop sub_825D83F8 (over "
              "XACT's clock); XACTTrackInstance::Play sub_825D5980 and AudioUpdate sub_825D5EB0, "
              "XACTTrack::AudioUpdate sub_825D0D88 and Play sub_825D01D0; "
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

void XactTrackSetVolume(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t track = ctx.r3.u32;
  const double f1 = ctx.f1.f64;
  bool declined = false;
  Framed(ctx, base, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    const auto r3 = xact::TrackSetVolume(base, track, f1, frame, call);
    declined = !r3;
    return r3.value_or(0);
  });
  if (declined) {
    ctx.r3.u64 = track;
    ctx.f1.f64 = f1;
    __imp__sub_825D0BF8(ctx, base);
  }
}

void XactTrackUpdateVolume(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t track = ctx.r3.u32;
  bool declined = false;
  Framed(ctx, base, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    const auto r3 = xact::TrackUpdateVolume(base, track, frame, call);
    declined = !r3;
    return r3.value_or(0);
  });
  if (declined) {
    ctx.r3.u64 = track;
    __imp__sub_825D0428(ctx, base);
  }
}

void XactTrackCreateRpcs(PPCContext& ctx, uint8_t* base) {
  if (const auto r3 = xact::TrackCreateRpcs(base, ctx.r3.u32)) {
    ctx.r3.u64 = *r3;
    return;
  }
  __imp__sub_825D06B8(ctx, base);
}

void XactTrackStartRpcs(PPCContext& ctx, uint8_t* base) {
  if (const auto r3 = xact::TrackStartRpcs(base, ctx.r3.u32)) {
    ctx.r3.u64 = *r3;
    return;
  }
  __imp__sub_825D0800(ctx, base);
}

void XactTrackConstruct(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t track = ctx.r3.u32, engine = ctx.r4.u32, sound = ctx.r5.u32,
                 record = ctx.r6.u32, index = ctx.r7.u32;
  Framed(ctx, base, 96, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::TrackConstruct(base, track, engine, sound, record, index, frame, call);
  });
}

void XactTrackUpdate(PPCContext& ctx, uint8_t* base) {
  const uint32_t track = ctx.r3.u32;
  Framed(ctx, base, 96, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::TrackUpdate(base, track, frame, call);
  });
}

void XactTrackDestruct(PPCContext& ctx, uint8_t* base) {
  const uint32_t track = ctx.r3.u32;
  Framed(ctx, base, 96, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::TrackDestruct(base, track, frame, call);
  });
}

void XactTrackDeletingDestruct(PPCContext& ctx, uint8_t* base) {
  const uint32_t track = ctx.r3.u32, flags = ctx.r4.u32;
  Framed(ctx, base, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::TrackDeletingDestruct(base, track, flags, frame, call);
  });
}

void XactPlayWaveDeletingDestruct(PPCContext& ctx, uint8_t* base) {
  const uint32_t event = ctx.r3.u32, flags = ctx.r4.u32;
  Framed(ctx, base, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::PlayWaveDeletingDestruct(base, event, flags, frame, call);
  });
}

void XactPlayWaveApplyVariation(PPCContext& ctx, uint8_t* base) {
  const uint32_t event = ctx.r3.u32;
  Framed(ctx, base, 96, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::PlayWaveApplyVariation(base, event, frame, call);
  });
}

void XactPlayWaveGetDuration(PPCContext& ctx, uint8_t* base) {
  const uint32_t event = ctx.r3.u32;
  const bool native_heap = NativeHeapActive();
  Framed(ctx, base, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::PlayWaveGetDuration(base, event, native_heap, frame, call);
  });
}

void XactPlayWavePrepare(PPCContext& ctx, uint8_t* base) {
  const uint32_t event = ctx.r3.u32, offset = ctx.r4.u32;
  const bool native_heap = NativeHeapActive();
  Framed(ctx, base, 128, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::PlayWavePrepare(base, event, offset, native_heap, frame, call);
  });
}

void XactTrackInstancePrepareSimple(PPCContext& ctx, uint8_t* base) {
  const uint32_t instance = ctx.r3.u32, record = ctx.r4.u32, offset = ctx.r5.u32;
  Framed(ctx, base, 128, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::TrackInstancePrepareSimple(base, instance, record, offset, frame, call);
  });
}

void XactTrackPrepareSimple(PPCContext& ctx, uint8_t* base) {
  const uint32_t track = ctx.r3.u32, record = ctx.r4.u32, offset = ctx.r5.u32;
  Framed(ctx, base, 128, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::TrackPrepareSimple(base, track, record, offset, frame, call);
  });
}

void XactTrackInstanceCancelEvents(PPCContext& ctx, uint8_t* base) {
  const uint32_t instance = ctx.r3.u32;
  Framed(ctx, base, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::TrackInstanceCancelEvents(base, instance, frame, call);
  });
}

void XactTrackInstanceDestruct(PPCContext& ctx, uint8_t* base) {
  const uint32_t instance = ctx.r3.u32;
  Framed(ctx, base, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::TrackInstanceDestruct(base, instance, frame, call);
  });
}

void XactTrackInstanceDeletingDestruct(PPCContext& ctx, uint8_t* base) {
  const uint32_t instance = ctx.r3.u32, flags = ctx.r4.u32;
  Framed(ctx, base, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::TrackInstanceDeletingDestruct(base, instance, flags, frame, call);
  });
}

void XactTrackInstanceStop(PPCContext& ctx, uint8_t* base) {
  const uint32_t instance = ctx.r3.u32, immediate = ctx.r4.u32;
  Framed(ctx, base, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::TrackInstanceStop(base, instance, immediate, frame, call);
  });
}

void XactTrackStop(PPCContext& ctx, uint8_t* base) {
  const uint32_t track = ctx.r3.u32, immediate = ctx.r4.u32;
  Framed(ctx, base, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::TrackStop(base, track, immediate, frame, call);
  });
}

void XactEventCreate(PPCContext& ctx, uint8_t* base) {
  const uint32_t engine = ctx.r3.u32, instance = ctx.r4.u32, record = ctx.r5.u32,
                 previous = ctx.r6.u32, out = ctx.r7.u32;
  bool declined = false;
  Framed(ctx, base, 144, [&](const xaudio::GuestCall& call, uint32_t) {
    const auto r3 = xact::EventCreate(base, engine, instance, record, previous, out, call);
    declined = !r3;
    return r3.value_or(0);
  });
  if (declined) {
    ctx.r3.u64 = engine;
    __imp__sub_825D0F00(ctx, base);
  }
}

void XactTrackInstancePrepare(PPCContext& ctx, uint8_t* base) {
  const uint32_t instance = ctx.r3.u32, key = ctx.r4.u32, offset = ctx.r5.u32;
  Framed(ctx, base, 192, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::TrackInstancePrepare(base, instance, key, offset, frame, call);
  });
}

void XactTrackCreateInstance(PPCContext& ctx, uint8_t* base) {
  const uint32_t track = ctx.r3.u32, key = ctx.r4.u32, out = ctx.r5.u32, offset = ctx.r6.u32;
  Framed(ctx, base, 128, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::TrackCreateInstance(base, track, key, out, offset, frame, call);
  });
}

void XactTrackPrepare(PPCContext& ctx, uint8_t* base) {
  const uint32_t track = ctx.r3.u32, key = ctx.r4.u32, offset = ctx.r5.u32;
  Framed(ctx, base, 128, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::TrackPrepare(base, track, key, offset, frame, call);
  });
}

void XactSoundScheduleEvent(PPCContext& ctx, uint8_t* base) {
  const uint32_t sound = ctx.r3.u32, event = ctx.r4.u32;
  Framed(ctx, base, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::SoundScheduleEvent(base, sound, event, frame, call);
  });
}

void XactEventSchedule(PPCContext& ctx, uint8_t* base) {
  const uint32_t event = ctx.r3.u32;
  const bool native_heap = NativeHeapActive();
  Framed(ctx, base, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::EventSchedule(base, event, native_heap, frame, call);
  });
}

void XactEventDone(PPCContext& ctx, uint8_t* base) {
  const uint32_t event = ctx.r3.u32;
  Framed(ctx, base, 96, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::EventDone(base, event, frame, call);
  });
}

void XactPlayWaveFire(PPCContext& ctx, uint8_t* base) {
  const uint32_t event = ctx.r3.u32;
  uint64_t r3 = 0;
  Framed(ctx, base, 96, [&](const xaudio::GuestCall& call, uint32_t frame) {
    r3 = xact::PlayWaveFire(base, event, frame, call);
    return uint32_t(r3);
  });
  ctx.r3.u64 = r3;  // in full: the time sign-extended, as the original leaves it
}

void XactPlayWaveStop(PPCContext& ctx, uint8_t* base) {
  const uint32_t event = ctx.r3.u32, immediate = ctx.r4.u32;
  Framed(ctx, base, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::PlayWaveStop(base, event, immediate, frame, call);
  });
}

void XactTrackInstancePlay(PPCContext& ctx, uint8_t* base) {
  const uint32_t instance = ctx.r3.u32;
  const bool native_heap = NativeHeapActive();
  Framed(ctx, base, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::TrackInstancePlay(base, instance, native_heap, frame, call);
  });
}

void XactTrackInstanceAudioUpdate(PPCContext& ctx, uint8_t* base) {
  const uint32_t instance = ctx.r3.u32;
  const bool native_heap = NativeHeapActive();
  Framed(ctx, base, 96, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::TrackInstanceAudioUpdate(base, instance, native_heap, frame, call);
  });
}

void XactTrackAudioUpdate(PPCContext& ctx, uint8_t* base) {
  const uint32_t track = ctx.r3.u32;
  const bool native_heap = NativeHeapActive();
  Framed(ctx, base, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::TrackAudioUpdate(base, track, native_heap, frame, call);
  });
}

void XactTrackPlay(PPCContext& ctx, uint8_t* base) {
  const uint32_t track = ctx.r3.u32;
  const bool native_heap = NativeHeapActive();
  Framed(ctx, base, 0, [&](const xaudio::GuestCall& call, uint32_t sp) {
    return xact::TrackPlay(base, track, native_heap, sp, call);
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

// The same for a function freeing through the native layer: the original
// unless --native_heap routes the game's heap there.
#define ITC_XACT_TRACKS_HEAP_ADAPTER(address, step)                    \
  extern "C" REX_FUNC(sub_##address) {                                 \
    REX_FUNC_PROLOGUE();                                               \
    if (!ITC_FLAG(native_xact_tracks) || !hooks::NativeHeapActive()) { \
      __imp__sub_##address(ctx, base);                                 \
      return;                                                          \
    }                                                                  \
    hooks::step(ctx, base);                                            \
  }

ITC_XACT_TRACKS_ADAPTER(825D57D0, XactTrackInstanceConstruct)
// No adapter (removed 2026-10-29: only XACTEvent::Done's plain C++ calls it in play) for
// XACTTrackInstance::CheckDone sub_825D55E0.
// No adapter (removed 2026-10-29: only XACTTrack::SetVolume's and UpdateVolume's plain C++ calls it in
// play) for XACTTrackInstance::SetVolume sub_825D5678.
// No adapter (removed 2026-10-29: only XACTTrack::IsBusy's plain C++ calls it in play) for
// XACTTrackInstance::IsBusy sub_825D5780.
// No adapter (removed 2026-10-29: only XACTTrack::Pause's plain C++ calls it in play) for
// XACTTrackInstance::Pause sub_825D57A8.
ITC_XACT_TRACKS_ADAPTER(825D05A8, XactTrackPause)
ITC_XACT_TRACKS_ADAPTER(825D0590, XactTrackIsBusy)
// No adapter (removed 2026-10-29: only XACTPlayWaveEvent::XACTPlayWaveEvent's plain C++ calls
// it in play) for XACTEvent::XACTEvent sub_825D1760.
// No adapter (removed 2026-10-29: only XACTPlayWaveEvent's deleting destructor's plain C++ calls it in
// play) for XACTEvent::~XACTEvent sub_825D0EF0.
// No adapter (removed 2026-10-29: only XACTPlayWaveEvent::Fire's and XACTSound::ScheduleEvent's
// plain C++ call it in play) for XACTEvent::Fire sub_825D1358.
// No adapter (removed 2026-10-29: only XACTEvent::Create's and
// XACTTrackInstance::PrepareSimple's plain C++ calls it in play) for
// XACTPlayWaveEvent::XACTPlayWaveEvent sub_825D82E8.
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
// No adapter (removed 2026-10-29: only XACTTrack::XACTTrack's plain C++ calls it in
// play) for XACTTrack::SetVolume sub_825D0BF8.
ITC_XACT_TRACKS_ADAPTER(825D0428, XactTrackUpdateVolume)
ITC_XACT_TRACKS_ADAPTER(825D06B8, XactTrackCreateRpcs)
ITC_XACT_TRACKS_ADAPTER(825D0800, XactTrackStartRpcs)
ITC_XACT_TRACKS_ADAPTER(825D0CE0, XactTrackConstruct)
ITC_XACT_TRACKS_ADAPTER(825D0E30, XactTrackUpdate)
// No adapter (removed 2026-10-29: only its deleting destructor's plain C++ calls it in
// play) for XACTTrack::~XACTTrack sub_825D0110.
ITC_XACT_TRACKS_HEAP_ADAPTER(825D0978, XactTrackDeletingDestruct)
ITC_XACT_TRACKS_HEAP_ADAPTER(825D8660, XactPlayWaveDeletingDestruct)
ITC_XACT_TRACKS_ADAPTER(825D7558, XactPlayWaveApplyVariation)
ITC_XACT_TRACKS_ADAPTER(825D8870, XactPlayWaveGetDuration)
ITC_XACT_TRACKS_ADAPTER(825D86F8, XactPlayWavePrepare)
// No adapter (removed 2026-10-29: only XACTTrack::PrepareSimple's plain C++ calls it in
// play) for XACTTrackInstance::PrepareSimple sub_825D53D8.
ITC_XACT_TRACKS_HEAP_ADAPTER(825D09D0, XactTrackPrepareSimple)
// No adapter (removed 2026-10-29: only ~XACTTrackInstance's and Stop's plain C++ calls it in
// play) for XACTTrackInstance::CancelEvents sub_825D5520.
// No adapter (removed 2026-10-29: only its deleting destructor's plain C++ calls it in
// play) for XACTTrackInstance::~XACTTrackInstance sub_825D5838.
ITC_XACT_TRACKS_HEAP_ADAPTER(825D5E58, XactTrackInstanceDeletingDestruct)
// No adapter (removed 2026-10-29: only XACTTrack::Stop's plain C++ calls it in
// play) for XACTTrackInstance::Stop sub_825D5D18.
ITC_XACT_TRACKS_ADAPTER(825D01D8, XactTrackStop)
// No adapter (removed 2026-10-29: only XACTTrackInstance::Prepare's plain C++ calls it in
// play) for XACTEvent::Create sub_825D0F00.
// No adapter (removed 2026-10-29: only XACTTrack::CreateInstance's plain C++ calls it in
// play) for XACTTrackInstance::Prepare sub_825D5A48.
// No adapter (removed 2026-10-29: only XACTTrack::Prepare's plain C++ calls it in play;
// sub_825D0B08 never ran) for XACTTrack::CreateInstance sub_825D02F8.
ITC_XACT_TRACKS_HEAP_ADAPTER(825D0AA0, XactTrackPrepare)
// No adapter (removed 2026-10-29: only XACTEvent::Schedule's plain C++ calls it in play) for
// XACTSound::ScheduleEvent sub_825CB0C8.
ITC_XACT_TRACKS_ADAPTER(825D1918, XactEventSchedule)
ITC_XACT_TRACKS_ADAPTER(825D13F0, XactEventDone)
ITC_XACT_TRACKS_ADAPTER(825D8378, XactPlayWaveFire)
ITC_XACT_TRACKS_ADAPTER(825D83F8, XactPlayWaveStop)
// No adapter (removed 2026-10-29: only XACTTrack::Play's and XACTTrackInstance::AudioUpdate's
// plain C++ call it in play) for XACTTrackInstance::Play sub_825D5980.
// No adapter (removed 2026-10-29: only XACTTrack::AudioUpdate's plain C++ calls it in play) for
// XACTTrackInstance::AudioUpdate sub_825D5EB0.
ITC_XACT_TRACKS_ADAPTER(825D0D88, XactTrackAudioUpdate)
ITC_XACT_TRACKS_ADAPTER(825D01D0, XactTrackPlay)
