// XACT's tracks, track instances and events natively: adapters in front of
// src/xact/tracks.h (docs/functions/xact-tracks.md), with the register steps
// the harness shares (hooks/xact_tracks.h).

#include "hooks/xact_tracks.h"
#include "lifted/originals.h"
#include "hooks/native_entry.h"

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
void Framed(lifted::Regs& ctx, uint32_t bytes, Body body) {
  XAudioFramed(ctx, bytes, body);
}

}  // namespace

ITC_REGS_STEP(XactTrackInstanceConstruct) {
  xact::TrackInstanceConstruct(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

ITC_REGS_STEP(XactTrackInstanceCheckDone) {
  xact::TrackInstanceCheckDone(ctx.r3.u32);
}

ITC_REGS_STEP(XactTrackInstanceSetVolume) {
  ctx.fpscr.disableFlushMode();
  const uint32_t instance = ctx.r3.u32;
  const double f1 = ctx.f1.f64;
  Framed(ctx, 0, [&](const xaudio::GuestCall& call, uint32_t sp) {
    return xact::TrackInstanceSetVolume(instance, f1, sp, call);
  });
}

ITC_REGS_STEP(XactTrackInstanceIsBusy) {
  const uint32_t instance = ctx.r3.u32;
  Framed(ctx, 0, [&](const xaudio::GuestCall& call, uint32_t sp) {
    return xact::TrackInstanceIsBusy(instance, sp, call);
  });
}

ITC_REGS_STEP(XactTrackInstancePause) {
  const uint32_t instance = ctx.r3.u32, pause = ctx.r4.u32;
  Framed(ctx, 0, [&](const xaudio::GuestCall& call, uint32_t sp) {
    return xact::TrackInstancePause(instance, pause, sp, call);
  });
}

ITC_REGS_STEP(XactTrackPause) {
  const uint32_t track = ctx.r3.u32, pause = ctx.r4.u32;
  Framed(ctx, 0, [&](const xaudio::GuestCall& call, uint32_t sp) {
    return xact::TrackPause(track, pause, sp, call);
  });
}

ITC_REGS_STEP(XactTrackIsBusy) {
  const uint32_t track = ctx.r3.u32;
  Framed(ctx, 0, [&](const xaudio::GuestCall& call, uint32_t sp) {
    return xact::TrackIsBusy(track, sp, call);
  });
}

ITC_REGS_STEP(XactEventConstruct) {
  xact::EventConstruct(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                       ctx.r8.u32);
}

ITC_REGS_STEP(XactEventDestruct) { xact::EventDestruct(ctx.r3.u32); }

ITC_REGS_STEP(XactEventFire) {
  const uint32_t event = ctx.r3.u32;
  Framed(ctx, 96, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::EventFire(event, frame, call);
  });
}

ITC_REGS_STEP(XactPlayWaveConstruct) {
  ctx.r3.u64 = xact::PlayWaveConstruct(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                       ctx.r7.u32, ctx.r8.u32);
}

ITC_REGS_STEP(XactPlayWaveSetQueued) {
  xact::PlayWaveSetQueued(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(XactPlayWaveGetLoopCount) {
  ctx.r3.u64 = xact::PlayWaveGetLoopCount(ctx.r3.u32);
}

ITC_REGS_STEP(XactPlayWaveSetWave) {
  xact::PlayWaveSetWave(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(XactWaveVariationSetWave) {
  xact::WaveVariationSetWave(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(XactPlayWaveGetVariationRecord) {
  ctx.r3.u64 = xact::PlayWaveGetVariationRecord(ctx.r3.u32);
}

ITC_REGS_STEP(XactPlayWaveForgetWave) {
  const uint32_t event = ctx.r3.u32;
  Framed(ctx, 0, [&](const xaudio::GuestCall& call, uint32_t sp) {
    return xact::PlayWaveForgetWave(event, sp, call);
  });
}

ITC_REGS_STEP(XactPlayWaveGetWaveBank) {
  const uint32_t event = ctx.r3.u32;
  Framed(ctx, 96, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::PlayWaveGetWaveBank(event, frame, call);
  });
}

ITC_REGS_STEP(XactPlayWaveIsPrepared) {
  const uint32_t event = ctx.r3.u32;
  Framed(ctx, 96, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::PlayWaveIsPrepared(event, frame, call);
  });
}

ITC_REGS_STEP(XactPlayWaveIsPlaying) {
  const uint32_t event = ctx.r3.u32;
  Framed(ctx, 96, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::PlayWaveIsPlaying(event, frame, call);
  });
}

ITC_REGS_STEP(XactPlayWaveIsBusy) {
  const uint32_t event = ctx.r3.u32;
  Framed(ctx, 96, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::PlayWaveIsBusy(event, frame, call);
  });
}

ITC_REGS_STEP(XactPlayWavePause) {
  const uint32_t event = ctx.r3.u32, pause = ctx.r4.u32;
  Framed(ctx, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::PlayWavePause(event, pause, frame, call);
  });
}

ITC_REGS_STEP(XactPlayWaveSetVolume) {
  ctx.fpscr.disableFlushMode();
  const uint32_t event = ctx.r3.u32;
  const double f1 = ctx.f1.f64;
  Framed(ctx, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::PlayWaveSetVolume(event, f1, frame, call);
  });
}

ITC_REGS_STEP(XactWaveSetVolume) {
  ctx.fpscr.disableFlushMode();
  const uint32_t wave = ctx.r3.u32;
  const double f1 = ctx.f1.f64;
  Framed(ctx, 0, [&](const xaudio::GuestCall& call, uint32_t sp) {
    return xact::WaveSetVolume(wave, f1, sp, call);
  });
}

ITC_REGS_STEP(XactTrackSetVolume) {
  ctx.fpscr.disableFlushMode();
  const uint32_t track = ctx.r3.u32;
  const double f1 = ctx.f1.f64;
  bool declined = false;
  Framed(ctx, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    const auto r3 = xact::TrackSetVolume(track, f1, frame, call);
    declined = !r3;
    return r3.value_or(0);
  });
  if (declined) {
    ctx.r3.u64 = track;
    ctx.f1.f64 = f1;
    lifted::original::c825D0BF8(ctx);
  }
}

ITC_REGS_STEP(XactTrackUpdateVolume) {
  ctx.fpscr.disableFlushMode();
  const uint32_t track = ctx.r3.u32;
  bool declined = false;
  Framed(ctx, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    const auto r3 = xact::TrackUpdateVolume(track, frame, call);
    declined = !r3;
    return r3.value_or(0);
  });
  if (declined) {
    ctx.r3.u64 = track;
    lifted::original::c825D0428(ctx);
  }
}

ITC_REGS_STEP(XactTrackCreateRpcs) {
  if (const auto r3 = xact::TrackCreateRpcs(ctx.r3.u32)) {
    ctx.r3.u64 = *r3;
    return;
  }
  lifted::original::c825D06B8(ctx);
}

ITC_REGS_STEP(XactTrackStartRpcs) {
  if (const auto r3 = xact::TrackStartRpcs(ctx.r3.u32)) {
    ctx.r3.u64 = *r3;
    return;
  }
  lifted::original::c825D0800(ctx);
}

ITC_REGS_STEP(XactTrackConstruct) {
  ctx.fpscr.disableFlushMode();
  const uint32_t track = ctx.r3.u32, engine = ctx.r4.u32, sound = ctx.r5.u32,
                 record = ctx.r6.u32, index = ctx.r7.u32;
  Framed(ctx, 96, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::TrackConstruct(track, engine, sound, record, index, frame, call);
  });
}

ITC_REGS_STEP(XactTrackUpdate) {
  const uint32_t track = ctx.r3.u32;
  Framed(ctx, 96, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::TrackUpdate(track, frame, call);
  });
}

ITC_REGS_STEP(XactTrackDestruct) {
  const uint32_t track = ctx.r3.u32;
  Framed(ctx, 96, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::TrackDestruct(track, frame, call);
  });
}

ITC_REGS_STEP(XactTrackDeletingDestruct) {
  const uint32_t track = ctx.r3.u32, flags = ctx.r4.u32;
  Framed(ctx, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::TrackDeletingDestruct(track, flags, frame, call);
  });
}

ITC_REGS_STEP(XactPlayWaveDeletingDestruct) {
  const uint32_t event = ctx.r3.u32, flags = ctx.r4.u32;
  Framed(ctx, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::PlayWaveDeletingDestruct(event, flags, frame, call);
  });
}

ITC_REGS_STEP(XactPlayWaveApplyVariation) {
  const uint32_t event = ctx.r3.u32;
  Framed(ctx, 96, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::PlayWaveApplyVariation(event, frame, call);
  });
}

ITC_REGS_STEP(XactPlayWaveGetDuration) {
  const uint32_t event = ctx.r3.u32;
  const bool native_heap = NativeHeapActive();
  Framed(ctx, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::PlayWaveGetDuration(event, native_heap, frame, call);
  });
}

ITC_REGS_STEP(XactPlayWavePrepare) {
  const uint32_t event = ctx.r3.u32, offset = ctx.r4.u32;
  const bool native_heap = NativeHeapActive();
  Framed(ctx, 128, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::PlayWavePrepare(event, offset, native_heap, frame, call);
  });
}

ITC_REGS_STEP(XactTrackInstancePrepareSimple) {
  const uint32_t instance = ctx.r3.u32, record = ctx.r4.u32, offset = ctx.r5.u32;
  Framed(ctx, 128, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::TrackInstancePrepareSimple(instance, record, offset, frame, call);
  });
}

ITC_REGS_STEP(XactTrackPrepareSimple) {
  const uint32_t track = ctx.r3.u32, record = ctx.r4.u32, offset = ctx.r5.u32;
  Framed(ctx, 128, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::TrackPrepareSimple(track, record, offset, frame, call);
  });
}

ITC_REGS_STEP(XactTrackInstanceCancelEvents) {
  const uint32_t instance = ctx.r3.u32;
  Framed(ctx, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::TrackInstanceCancelEvents(instance, frame, call);
  });
}

ITC_REGS_STEP(XactTrackInstanceDestruct) {
  const uint32_t instance = ctx.r3.u32;
  Framed(ctx, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::TrackInstanceDestruct(instance, frame, call);
  });
}

ITC_REGS_STEP(XactTrackInstanceDeletingDestruct) {
  const uint32_t instance = ctx.r3.u32, flags = ctx.r4.u32;
  Framed(ctx, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::TrackInstanceDeletingDestruct(instance, flags, frame, call);
  });
}

ITC_REGS_STEP(XactTrackInstanceStop) {
  const uint32_t instance = ctx.r3.u32, immediate = ctx.r4.u32;
  Framed(ctx, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::TrackInstanceStop(instance, immediate, frame, call);
  });
}

ITC_REGS_STEP(XactTrackStop) {
  const uint32_t track = ctx.r3.u32, immediate = ctx.r4.u32;
  Framed(ctx, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::TrackStop(track, immediate, frame, call);
  });
}

ITC_REGS_STEP(XactEventCreate) {
  const uint32_t engine = ctx.r3.u32, instance = ctx.r4.u32, record = ctx.r5.u32,
                 previous = ctx.r6.u32, out = ctx.r7.u32;
  bool declined = false;
  Framed(ctx, 144, [&](const xaudio::GuestCall& call, uint32_t) {
    const auto r3 = xact::EventCreate(engine, instance, record, previous, out, call);
    declined = !r3;
    return r3.value_or(0);
  });
  if (declined) {
    ctx.r3.u64 = engine;
    lifted::original::c825D0F00(ctx);
  }
}

ITC_REGS_STEP(XactTrackInstancePrepare) {
  const uint32_t instance = ctx.r3.u32, key = ctx.r4.u32, offset = ctx.r5.u32;
  Framed(ctx, 192, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::TrackInstancePrepare(instance, key, offset, frame, call);
  });
}

ITC_REGS_STEP(XactTrackCreateInstance) {
  const uint32_t track = ctx.r3.u32, key = ctx.r4.u32, out = ctx.r5.u32, offset = ctx.r6.u32;
  Framed(ctx, 128, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::TrackCreateInstance(track, key, out, offset, frame, call);
  });
}

ITC_REGS_STEP(XactTrackPrepare) {
  const uint32_t track = ctx.r3.u32, key = ctx.r4.u32, offset = ctx.r5.u32;
  Framed(ctx, 128, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::TrackPrepare(track, key, offset, frame, call);
  });
}

ITC_REGS_STEP(XactSoundScheduleEvent) {
  const uint32_t sound = ctx.r3.u32, event = ctx.r4.u32;
  Framed(ctx, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::SoundScheduleEvent(sound, event, frame, call);
  });
}

ITC_REGS_STEP(XactEventSchedule) {
  const uint32_t event = ctx.r3.u32;
  const bool native_heap = NativeHeapActive();
  Framed(ctx, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::EventSchedule(event, native_heap, frame, call);
  });
}

ITC_REGS_STEP(XactEventDone) {
  const uint32_t event = ctx.r3.u32;
  Framed(ctx, 96, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::EventDone(event, frame, call);
  });
}

ITC_REGS_STEP(XactPlayWaveFire) {
  const uint32_t event = ctx.r3.u32;
  uint64_t r3 = 0;
  Framed(ctx, 96, [&](const xaudio::GuestCall& call, uint32_t frame) {
    r3 = xact::PlayWaveFire(event, frame, call);
    return uint32_t(r3);
  });
  ctx.r3.u64 = r3;  // in full: the time sign-extended, as the original leaves it
}

ITC_REGS_STEP(XactPlayWaveStop) {
  const uint32_t event = ctx.r3.u32, immediate = ctx.r4.u32;
  Framed(ctx, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::PlayWaveStop(event, immediate, frame, call);
  });
}

ITC_REGS_STEP(XactTrackInstancePlay) {
  const uint32_t instance = ctx.r3.u32;
  const bool native_heap = NativeHeapActive();
  Framed(ctx, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::TrackInstancePlay(instance, native_heap, frame, call);
  });
}

ITC_REGS_STEP(XactTrackInstanceAudioUpdate) {
  const uint32_t instance = ctx.r3.u32;
  const bool native_heap = NativeHeapActive();
  Framed(ctx, 96, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::TrackInstanceAudioUpdate(instance, native_heap, frame, call);
  });
}

ITC_REGS_STEP(XactTrackAudioUpdate) {
  const uint32_t track = ctx.r3.u32;
  const bool native_heap = NativeHeapActive();
  Framed(ctx, 112, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::TrackAudioUpdate(track, native_heap, frame, call);
  });
}

ITC_REGS_STEP(XactTrackPlay) {
  const uint32_t track = ctx.r3.u32;
  const bool native_heap = NativeHeapActive();
  Framed(ctx, 0, [&](const xaudio::GuestCall& call, uint32_t sp) {
    return xact::TrackPlay(track, native_heap, sp, call);
  });
}

}  // namespace hooks

// The flag's fallback, then the register step.
#define ITC_XACT_TRACKS_ADAPTER(address, step) \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_xact_tracks)) { \
    hooks::step(ctx);                    \
  }

// The same for a function freeing through the native layer: the original
// unless --native_heap routes the game's heap there.
#define ITC_XACT_TRACKS_HEAP_ADAPTER(address, step)                    \
  ITC_NATIVE_ENTRY_ALWAYS(address) {                                 \
    \
    if (!ITC_FLAG(native_xact_tracks) || !hooks::NativeHeapActive()) { \
      lifted::original::c##address(ctx);                    \
      return;                                                          \
    }                                                                  \
    hooks::step(ctx);                                            \
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
// No adapter (removed 2026-10-29: only XACTWave::AudioUpdate's plain C++ calls it in play) for
// XACTEvent::Done sub_825D13F0.
ITC_XACT_TRACKS_ADAPTER(825D8378, XactPlayWaveFire)
ITC_XACT_TRACKS_ADAPTER(825D83F8, XactPlayWaveStop)
// No adapter (removed 2026-10-29: only XACTTrack::Play's and XACTTrackInstance::AudioUpdate's
// plain C++ call it in play) for XACTTrackInstance::Play sub_825D5980.
// No adapter (removed 2026-10-29: only XACTTrack::AudioUpdate's plain C++ calls it in play) for
// XACTTrackInstance::AudioUpdate sub_825D5EB0.
ITC_XACT_TRACKS_ADAPTER(825D0D88, XactTrackAudioUpdate)
ITC_XACT_TRACKS_ADAPTER(825D01D0, XactTrackPlay)
