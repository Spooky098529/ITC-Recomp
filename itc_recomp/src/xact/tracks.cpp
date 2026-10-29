// XACT's tracks, track instances and events natively (tracks.h,
// docs/functions/xact-tracks.md). The objects' slots run the plain C++ here
// while they hold the native function, else the guest call; the waves' and
// voices' slots go through the guest call (their adapters run their plain
// C++). Strict floating point (CMakeLists.txt), as the original's steps.

#include "xact/tracks.h"

#include "base/log.h"
#include "guest.h"

#include <initializer_list>

namespace xact {

namespace {

namespace xa = xaudio;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint8_t LoadU8(uint8_t* base, uint32_t address) { return guest::LoadU8(base, address); }
void StoreU8(uint8_t* base, uint32_t address, uint8_t value) {
  guest::StoreU8(base, address, value);
}

xa::Regs Args(std::initializer_list<uint64_t> r) {
  xa::Regs regs;
  for (const uint64_t value : r) {
    regs.r[regs.count++] = value;
  }
  return regs;
}
xa::Regs WithF1(xa::Regs regs, double f1) {
  regs.f1_set = true;
  regs.f1 = f1;
  return regs;
}
void SetRegs(const GuestCall& call, const xa::Regs& regs) { call(0, 0, regs); }

// A guest call from `frame` (the caller's own frame, or its stack pointer
// for a tail call); `return_address` 0 keeps the link register (a tail
// call's).
uint32_t CallAt(const GuestCall& call, uint32_t function, uint32_t return_address,
                xa::Regs regs, uint32_t frame) {
  regs.sp = frame;
  return call(function, return_address, regs);
}

constexpr uint32_t kPlayWaveFrame = 112;  // SetVolume's and Pause's frames
constexpr uint32_t kPlayWaveSmallFrame = 96;
constexpr uint32_t kPlayWaveSetVolume = 0x825D7FE0;  // slot +64
constexpr uint32_t kPlayWaveIsBusy = 0x825D81E8;     // slot +44
constexpr uint32_t kPlayWavePause = 0x825D8268;      // slot +88

// The event's wave: its slot +56 (the getter of +60 read here while the slot
// holds it), called from `frame` with `return_address`.
uint32_t WaveOf(uint8_t* base, uint32_t event, uint32_t return_address, uint32_t frame,
                const GuestCall& call) {
  const uint32_t method = xa::Method(base, event, 56);
  if (method == kGetWord60) {
    SetRegs(call, Args({event}));
    return Load(base, event + 60);
  }
  return CallAt(call, method, return_address, Args({event}), frame);
}

uint32_t WaveSetVolumeFrom(uint8_t* base, uint32_t wave, double volume, uint32_t sp,
                           uint32_t return_address, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D1CC8 XACTWave::SetVolume");
  const uint32_t voice = Load(base, wave + 44);
  if (voice == 0) {
    return 0;
  }
  return CallAt(call, xa::Method(base, voice, 4), return_address, WithF1(Args({voice}), volume),
                sp);
}

}  // namespace

void TrackInstanceConstruct(uint8_t* base, uint32_t instance, uint32_t engine, uint32_t track) {
  ITC_NATIVE_RUNNING("sub_825D57D0 XACTTrackInstance::XACTTrackInstance");
  Store(base, instance + 4, instance);
  Store(base, instance + 8, instance);
  Store(base, instance + 12, engine);
  Store(base, instance + 16, track);
  Store(base, instance + 0, kTrackInstanceVtable);
  Store(base, instance + 20, 0);
  StoreU8(base, instance + 24, 0);
  Store(base, instance + 28, 0);
  Store(base, instance + 36, 0);
  Store(base, instance + 40, 1);
  StoreU8(base, instance + 44, 0);
  StoreU8(base, instance + 45, 0);
  StoreU8(base, instance + 32, 0);
  const uint32_t list = instance + 48;
  Store(base, list + 0, kListBaseVtable);
  Store(base, list + 4, list);
  Store(base, list + 8, list);
  Store(base, instance + 60, 0);
}

void TrackInstanceCheckDone(uint8_t* base, uint32_t instance) {
  ITC_NATIVE_RUNNING("sub_825D55E0 XACTTrackInstance::CheckDone");
  if (LoadU8(base, instance + 44) != 0 || LoadU8(base, instance + 45) != 0) {
    return;
  }
  const uint32_t list = instance + 48;
  if (Load(base, list + 4) != list || Load(base, list + 8) != list) {
    return;
  }
  Store(base, instance + 40, 6);
}

uint32_t TrackInstanceSetVolume(uint8_t* base, uint32_t instance, double volume, uint32_t sp,
                                const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D5678 XACTTrackInstance::SetVolume");
  const uint32_t event = Load(base, instance + 36);
  if (event == 0) {
    return 0;
  }
  const uint32_t method = xa::Method(base, event, 64);
  if (method == kPlayWaveSetVolume) {
    SetRegs(call, Args({event}));
    return PlayWaveSetVolume(base, event, volume, xa::FrameBelow(base, sp, kPlayWaveFrame),
                             call);
  }
  return CallAt(call, method, 0, WithF1(Args({event}), volume), sp);
}

uint32_t TrackInstanceIsBusy(uint8_t* base, uint32_t instance, uint32_t sp,
                             const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D5780 XACTTrackInstance::IsBusy");
  const uint32_t event = Load(base, instance + 36);
  if (event == 0) {
    return 0;
  }
  const uint32_t method = xa::Method(base, event, 44);
  if (method == kPlayWaveIsBusy) {
    SetRegs(call, Args({event}));
    return PlayWaveIsBusy(base, event, xa::FrameBelow(base, sp, kPlayWaveSmallFrame), call);
  }
  return CallAt(call, method, 0, Args({event}), sp);
}

uint32_t TrackInstancePause(uint8_t* base, uint32_t instance, uint32_t pause, uint32_t sp,
                            const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D57A8 XACTTrackInstance::Pause");
  const uint32_t event = Load(base, instance + 36);
  if (event == 0) {
    return 0;
  }
  const uint32_t method = xa::Method(base, event, 88);
  if (method == kPlayWavePause) {
    SetRegs(call, Args({event}));
    return PlayWavePause(base, event, pause, xa::FrameBelow(base, sp, kPlayWaveFrame), call);
  }
  return CallAt(call, method, 0, Args({event}), sp);
}

uint32_t TrackPause(uint8_t* base, uint32_t track, uint32_t pause, uint32_t sp,
                    const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D05A8 XACTTrack::Pause");
  const uint32_t instance = Load(base, track + 24);
  if (instance == 0) {
    return 0;
  }
  SetRegs(call, Args({instance}));
  return TrackInstancePause(base, instance, pause, sp, call);
}

uint32_t TrackIsBusy(uint8_t* base, uint32_t track, uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D0590 XACTTrack::IsBusy");
  const uint32_t instance = Load(base, track + 24);
  if (instance == 0) {
    return 0;
  }
  SetRegs(call, Args({instance}));
  return TrackInstanceIsBusy(base, instance, sp, call);
}

void EventConstruct(uint8_t* base, uint32_t event, uint32_t engine, uint32_t instance,
                    uint32_t record, uint32_t previous, uint32_t uncounted) {
  ITC_NATIVE_RUNNING("sub_825D1760 XACTEvent::XACTEvent");
  Store(base, event + 4, event);
  Store(base, event + 8, event);
  Store(base, event + 12, engine);
  Store(base, event + 16, record);
  Store(base, event + 36, previous);
  Store(base, event + 44, instance);
  Store(base, event + 0, kEventVtable);
  Store(base, event + 20, 0);
  Store(base, event + 24, 0);
  Store(base, event + 32, 0);
  Store(base, event + 28, 0);
  StoreU8(base, event + 40, uncounted != 0 ? 0x08 : 0);
}

void EventDestruct(uint8_t* base, uint32_t event) {
  ITC_NATIVE_RUNNING("sub_825D0EF0 XACTEvent::~XACTEvent");
  Store(base, event + 0, kListBaseVtable);
}

uint32_t EventFire(uint8_t* base, uint32_t event, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D1358 XACTEvent::Fire");
  uint8_t flags = LoadU8(base, event + 40);
  if ((flags & 0x80) == 0) {
    flags |= 0x80;
    StoreU8(base, event + 40, flags);
    if ((flags & 0x08) == 0) {
      const uint32_t instance = Load(base, event + 44);
      StoreU8(base, instance + 45, uint8_t(LoadU8(base, instance + 45) + 1));
    }
  }
  const uint32_t method = xa::Method(base, event, 16);
  uint32_t r3;
  if (method == kPlayWaveSetQueued) {
    SetRegs(call, Args({event, 0}));
    PlayWaveSetQueued(base, event, 0);
    r3 = event;
  } else {
    r3 = CallAt(call, method, 0x825D13BC, Args({event, 0}), frame);
  }
  if ((LoadU8(base, event + 40) & 0x08) == 0) {
    const uint32_t instance = Load(base, event + 44);
    StoreU8(base, instance + 44, uint8_t(LoadU8(base, instance + 44) + 255));
  }
  return r3;
}

uint32_t PlayWaveConstruct(uint8_t* base, uint32_t event, uint32_t engine, uint32_t instance,
                           uint32_t record, uint32_t previous, uint32_t variation) {
  ITC_NATIVE_RUNNING("sub_825D82E8 XACTPlayWaveEvent::XACTPlayWaveEvent");
  EventConstruct(base, event, engine, instance, record, previous, 0);
  const uint32_t at = Load(base, event + 16);
  Store(base, event + 0, kPlayWaveEventVtable);
  Store(base, event + 48, 0);
  const uint8_t loops = LoadU8(base, at + 11);
  Store(base, event + 56, variation);
  Store(base, event + 60, 0);
  Store(base, event + 64, 0);
  Store(base, event + 68, 0);
  StoreU8(base, event + 52, loops);
  return event;
}

void PlayWaveSetQueued(uint8_t* base, uint32_t event, uint32_t queued) {
  ITC_NATIVE_RUNNING("sub_825D8530 XACTPlayWaveEvent::SetQueued");
  uint8_t flags = LoadU8(base, event + 40);
  const uint32_t wave = Load(base, event + 60);
  flags = uint8_t((flags & ~0x20) | (queued != 0 ? 0x20 : 0));
  StoreU8(base, event + 40, flags);
  const uint32_t prepared = Load(base, wave + 60);
  flags = uint8_t((flags & ~0x40) | (prepared == 0 ? 0x40 : 0));
  StoreU8(base, event + 40, flags);
}

uint32_t PlayWaveGetLoopCount(uint8_t* base, uint32_t event) {
  ITC_NATIVE_RUNNING("sub_825D8358 XACTPlayWaveEvent::GetLoopCount");
  return LoadU8(base, event + 52);
}

void PlayWaveSetWave(uint8_t* base, uint32_t event, uint32_t wave) {
  ITC_NATIVE_RUNNING("sub_82483208 XACTPlayWaveEvent::SetWave");
  Store(base, event + 60, wave);
}

void WaveVariationSetWave(uint8_t* base, uint32_t event, uint32_t wave) {
  ITC_NATIVE_RUNNING("sub_825D7B78 XACTWaveVariationEvent::SetWave");
  Store(base, event + 72, wave);
}

uint32_t PlayWaveGetVariationRecord(uint8_t* base, uint32_t event) {
  ITC_NATIVE_RUNNING("sub_825D8920 XACTPlayWaveEvent::GetVariationRecord");
  const uint32_t record = Load(base, event + 16);
  const int32_t type = int32_t(Load(base, record) & 0x1F);
  if (type == 4) {
    return record + 12;
  }
  return type == 5 ? record + 16 : 0;
}

uint32_t PlayWaveForgetWave(uint8_t* base, uint32_t event, uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D8360 XACTPlayWaveEvent::ForgetWave");
  const uint32_t method = xa::Method(base, event, 104);
  if (method == kPlayWaveSetWave) {
    SetRegs(call, Args({event, 0}));
    PlayWaveSetWave(base, event, 0);
    return event;
  }
  return CallAt(call, method, 0, Args({event, 0}), sp);
}

uint32_t PlayWaveGetWaveBank(uint8_t* base, uint32_t event, uint32_t frame,
                             const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D84C0 XACTPlayWaveEvent::GetWaveBank");
  if (WaveOf(base, event, 0x825D84E4, frame, call) == 0) {
    return 0;
  }
  return Load(base, WaveOf(base, event, 0x825D8500, frame, call) + 36);
}

uint32_t PlayWaveIsPrepared(uint8_t* base, uint32_t event, uint32_t frame,
                            const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D76A0 XACTPlayWaveEvent::IsPrepared");
  if (WaveOf(base, event, 0x825D76C4, frame, call) == 0) {
    return 0;
  }
  const uint32_t wave = WaveOf(base, event, 0x825D76E0, frame, call);
  return int32_t(Load(base, wave + 40)) >= 2 ? 1 : 0;
}

uint32_t PlayWaveIsPlaying(uint8_t* base, uint32_t event, uint32_t frame,
                           const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D7708 XACTPlayWaveEvent::IsPlaying");
  if (WaveOf(base, event, 0x825D772C, frame, call) == 0) {
    return 0;
  }
  if (int32_t(Load(base, WaveOf(base, event, 0x825D7748, frame, call) + 40)) < 3) {
    return 0;
  }
  return int32_t(Load(base, WaveOf(base, event, 0x825D7768, frame, call) + 40)) > 5 ? 0 : 1;
}

uint32_t PlayWaveIsBusy(uint8_t* base, uint32_t event, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D81E8 XACTPlayWaveEvent::IsBusy");
  if (WaveOf(base, event, 0x825D820C, frame, call) == 0) {
    return 0;
  }
  const uint32_t wave = WaveOf(base, event, 0x825D8228, frame, call);
  return CallAt(call, xa::Method(base, wave, 20), 0x825D8238, Args({wave}), frame);
}

uint32_t PlayWavePause(uint8_t* base, uint32_t event, uint32_t pause, uint32_t frame,
                       const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D8268 XACTPlayWaveEvent::Pause");
  if (WaveOf(base, event, 0x825D8294, frame, call) == 0) {
    return 0;
  }
  const uint32_t wave = WaveOf(base, event, 0x825D82B0, frame, call);
  return CallAt(call, xa::Method(base, wave, 36), 0x825D82C4, Args({wave, pause}), frame);
}

uint32_t PlayWaveSetVolume(uint8_t* base, uint32_t event, double volume, uint32_t frame,
                           const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D7FE0 XACTPlayWaveEvent::SetVolume");
  if (WaveOf(base, event, 0x825D800C, frame, call) == 0) {
    return 0;
  }
  const uint32_t wave = WaveOf(base, event, 0x825D8028, frame, call);
  SetRegs(call, WithF1(Args({wave}), volume));
  return WaveSetVolumeFrom(base, wave, volume, frame, 0x825D8030, call);
}

uint32_t WaveSetVolume(uint8_t* base, uint32_t wave, double volume, uint32_t sp,
                       const GuestCall& call) {
  return WaveSetVolumeFrom(base, wave, volume, sp, 0, call);
}

}  // namespace xact
