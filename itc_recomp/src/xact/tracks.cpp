// XACT's tracks, track instances and events natively (tracks.h,
// docs/functions/xact-tracks.md). The objects' slots run the plain C++ here
// while they hold the native function, else the guest call; the waves' and
// voices' slots go through the guest call (their adapters run their plain
// C++). Strict floating point (CMakeLists.txt), as the original's steps.

#include "xact/tracks.h"

#include "base/log.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"

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

// --- The track's volume, set-up, update and destructors; the play-wave
// event's destructor, variation and duration ---

namespace {

constexpr uint32_t kZero = 0x82000A34;       // 0.0f
constexpr uint32_t kOne = 0x82000A30;        // 1.0f
constexpr uint32_t kGainTable = 0x820761F8;  // 256 floats
constexpr uint32_t kTrackSetVolume = 0x825D0BF8;
constexpr uint32_t kTrackSetVolumeFrame = 112;
constexpr uint32_t kTrackDestructFrame = 96;
constexpr uint32_t kGetVariationRecord = 0x825D8920;  // slot +92
constexpr uint32_t kSoundBankGetWaveBank = 0x825C7218;
constexpr uint32_t kWaveBankGetWaveDurations = 0x825CC380;

double LoadF(uint8_t* base, uint32_t address) { return double(guest::LoadFloat(base, address)); }
double Single(double value) { return double(float(value)); }

// The sound's volume times the track's factors, to the instance's SetVolume
// (from `frame`); `track_first` the product's order (SetVolume's f1 x +40,
// UpdateVolume's +40 x f1).
std::optional<uint32_t> ApplyVolume(uint8_t* base, uint32_t track, bool track_first,
                                    uint32_t frame, const GuestCall& call) {
  const double f31 = LoadF(base, track + 40);
  const uint32_t sound = Load(base, track + 20);
  const double f30 = LoadF(base, track + 56);
  const auto volume = SoundGetVolume(base, sound);
  if (!volume) {
    return std::nullopt;
  }
  const double f0 = track_first ? Single(f31 * volume->f1) : Single(volume->f1 * f31);
  const double f1 = Single(f0 * f30);
  const uint32_t instance = Load(base, track + 24);
  SetRegs(call, WithF1(Args({instance}), f1));
  return TrackInstanceSetVolume(base, instance, f1, frame, call);
}

}  // namespace

std::optional<uint32_t> TrackSetVolume(uint8_t* base, uint32_t track, double volume,
                                       uint32_t frame, const GuestCall& call) {
  const double zero = LoadF(base, kZero);
  guest::StoreFloat(base, track + 40, float(volume < zero ? zero : volume));
  if (Load(base, track + 24) == 0) {
    ITC_NATIVE_RUNNING("sub_825D0BF8 XACTTrack::SetVolume");
    return track;
  }
  const auto r3 = ApplyVolume(base, track, false, frame, call);
  if (r3) {
    ITC_NATIVE_RUNNING("sub_825D0BF8 XACTTrack::SetVolume");
  }
  return r3;
}

std::optional<uint32_t> TrackUpdateVolume(uint8_t* base, uint32_t track, uint32_t frame,
                                          const GuestCall& call) {
  if (Load(base, track + 24) == 0) {
    ITC_NATIVE_RUNNING("sub_825D0428 XACTTrack::UpdateVolume");
    return track;
  }
  const auto r3 = ApplyVolume(base, track, true, frame, call);
  if (r3) {
    ITC_NATIVE_RUNNING("sub_825D0428 XACTTrack::UpdateVolume");
  }
  return r3;
}

uint32_t TrackConstruct(uint8_t* base, uint32_t track, uint32_t engine, uint32_t sound,
                        uint32_t record, uint32_t index, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D0CE0 XACTTrack::XACTTrack");
  const float zero = guest::LoadFloat(base, kZero);
  guest::StoreFloat(base, track + 44, zero);
  Store(base, track + 4, track);
  guest::StoreFloat(base, track + 60, zero);
  Store(base, track + 8, track);
  Store(base, track + 12, engine);
  Store(base, track + 16, record);
  Store(base, track + 20, sound);
  Store(base, track + 0, kTrackVtable);
  double f1 = LoadF(base, kOne);
  guest::StoreFloat(base, track + 56, float(f1));
  StoreU8(base, track + 48, uint8_t(index));
  Store(base, track + 24, 0);
  Store(base, track + 28, 0);
  Store(base, track + 36, 0);
  Store(base, track + 52, 0);
  StoreU8(base, track + 32, 0);
  if (record != 0) {
    f1 = LoadF(base, kGainTable + (uint32_t(LoadU8(base, record)) << 2));
  }
  SetRegs(call, WithF1(Args({track}), f1));
  if (!TrackSetVolume(base, track, f1, xa::FrameBelow(base, frame, kTrackSetVolumeFrame),
                      call)) {
    CallAt(call, kTrackSetVolume, 0x825D0D6C, WithF1(Args({track}), f1), frame);
  }
  return track;
}

uint32_t TrackUpdate(uint8_t* base, uint32_t track, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D0E30 XACTTrack::Update");
  const uint32_t event = Load(base, track + 36);
  if (event != 0) {
    CallAt(call, 0x825D0B08, 0x825D0E54, Args({track, event}), frame);
    Store(base, track + 36, 0);
  }
  // The empty sub_82106540 with the instance: nothing to do.
  const uint32_t instance = Load(base, track + 24);
  SetRegs(call, Args({instance}));
  return instance;
}

std::optional<uint32_t> TrackCreateRpcs(uint8_t* base, uint32_t track) {
  const uint32_t sound = Load(base, track + 20);
  if ((LoadU8(base, Load(base, sound + 28)) & 0x4) != 0) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825D06B8 XACTTrack::CreateRpcs");
  return 0;
}

std::optional<uint32_t> TrackStartRpcs(uint8_t* base, uint32_t track) {
  const uint32_t sound = Load(base, track + 20);
  if ((LoadU8(base, Load(base, sound + 28)) & 0x4) != 0) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825D0800 XACTTrack::StartRpcs");
  return sound;
}

uint32_t TrackDestruct(uint8_t* base, uint32_t track, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D0110 XACTTrack::~XACTTrack");
  Store(base, track + 0, kTrackVtable);
  uint32_t r3 = Load(base, track + 28);
  if (r3 != 0) {
    r3 = CallAt(call, xa::Method(base, r3, 0), 0x825D0150, Args({r3, 1}), frame);
  }
  r3 = Load(base, track + 24);
  if (r3 != 0) {
    r3 = CallAt(call, xa::Method(base, r3, 0), 0x825D0170, Args({r3, 1}), frame);
  }
  const uint32_t entries = Load(base, track + 52);
  if (entries != 0) {
    // The RPC entries (never in play): the counted array's vector deleting
    // destructor through the first entry's +4 vtable, or operator delete on
    // the block, as the original.
    if (int32_t(Load(base, entries - 4)) != 0) {
      r3 = CallAt(call, xa::Method(base, entries + 4, 0), 0x825D01A4, Args({entries + 4, 3}),
                  frame);
    } else {
      r3 = CallAt(call, 0x825A70E8, 0x825D01AC, Args({entries - 4}), frame);
    }
  }
  Store(base, track + 0, kListBaseVtable);
  return r3;
}

uint32_t TrackDeletingDestruct(uint8_t* base, uint32_t track, uint32_t flags, uint32_t frame,
                               const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D0978 XACTTrack::~XACTTrack (deleting)");
  SetRegs(call, Args({track, flags}));
  TrackDestruct(base, track, xa::FrameBelow(base, frame, kTrackDestructFrame), call);
  if ((flags & 1) != 0) {
    SetRegs(call, Args({track, kTrackAttributes}));
    memory::crt::XMemFree(track, kTrackAttributes);
  }
  return track;
}

uint32_t PlayWaveDeletingDestruct(uint8_t* base, uint32_t event, uint32_t flags,
                                  uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D8660 XACTPlayWaveEvent::~XACTPlayWaveEvent (deleting)");
  const uint32_t wave = Load(base, event + 60);
  Store(base, event + 0, kPlayWaveEventVtable);
  if (wave != 0) {
    CallAt(call, xa::Method(base, wave, 0), 0x825D86A8, Args({wave, 1}), frame);
  }
  Store(base, event + 0, kWaveEventBaseVtable);
  SetRegs(call, Args({event}));
  EventDestruct(base, event);
  if ((flags & 1) != 0) {
    SetRegs(call, Args({event, kPlayWaveAttributes}));
    memory::crt::XMemFree(event, kPlayWaveAttributes);
  }
  return event;
}

uint32_t PlayWaveApplyVariation(uint8_t* base, uint32_t event, uint32_t frame,
                                const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D7558 XACTPlayWaveEvent::ApplyVariation");
  const uint32_t method = xa::Method(base, event, 92);
  uint32_t record;
  if (method == kGetVariationRecord) {
    SetRegs(call, Args({event}));
    record = PlayWaveGetVariationRecord(base, event);
  } else {
    record = CallAt(call, method, 0x825D757C, Args({event}), frame);
  }
  if (record == 0) {
    return 0;
  }
  return CallAt(call, 0x825D1478, 0x825D7590, Args({event, record}), frame);
}

uint32_t PlayWaveGetDuration(uint8_t* base, uint32_t event, bool native_heap, uint32_t frame,
                             const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D8870 XACTPlayWaveEvent::GetDuration");
  // The cue: [[[[event + 44] + 16] + 20] + 48] (instance, track, sound).
  const uint32_t cue =
      Load(base, Load(base, Load(base, Load(base, event + 44) + 16) + 20) + 48);
  const uint32_t sound_bank = Load(base, cue + 296);
  const uint32_t index = LoadU8(base, Load(base, event + 16) + 10);
  std::optional<uint32_t> found;
  if (native_heap) {
    SetRegs(call, Args({sound_bank, index}));
    found = SoundBankGetWaveBank(base, sound_bank, index);
  }
  const uint32_t bank = found ? *found
                              : CallAt(call, kSoundBankGetWaveBank, 0x825D88AC,
                                       Args({sound_bank, index}), frame);
  if (bank == 0) {
    return 0;
  }
  const uint32_t record = Load(base, event + 16);
  if (LoadU8(base, record + 11) == 255) {
    return 0x7FFFFFFF;
  }
  const uint32_t wave = guest::LoadU16(base, record + 8);
  const xa::Regs regs = Args({bank, wave, frame + 84, frame + 80});
  uint32_t r3;
  if (const auto durations = WaveBankGetWaveDurations(base, bank, wave)) {
    SetRegs(call, regs);
    Store(base, frame + 84, durations->duration_ms);
    Store(base, frame + 80, durations->loop_length_ms);
    r3 = 0;
  } else {
    r3 = CallAt(call, kWaveBankGetWaveDurations, 0x825D88E0, regs, frame);
  }
  if (int32_t(r3) < 0) {
    return 0;
  }
  const uint32_t loops = LoadU8(base, Load(base, event + 16) + 11);
  return loops * Load(base, frame + 80) + Load(base, frame + 84);
}

// --- Preparing ---

namespace {

constexpr uint32_t kPlayWaveGetLoopCount = 0x825D8358;  // slot +48
constexpr uint32_t kPlayWavePrepare = 0x825D86F8;       // slot +8
constexpr uint32_t kPlayWaveIsPrepared = 0x825D76A0;    // slot +20
constexpr uint32_t kPlayWavePrepareFrame = 128;
constexpr uint32_t kTrackInstancePrepareFrame = 128;
constexpr uint32_t kNoWaveBank = 0x8AC70013;        // XACTENGINE_E_NOWAVEBANK
constexpr uint32_t kWaveBankNotPrepared = 0x8AC70016;
constexpr uint32_t kOutOfMemory = 0x8007000E;       // E_OUTOFMEMORY

// The event's slots +8 (Prepare) and +20 (IsPrepared) from `frame`: the
// plain C++ while they hold it.
uint32_t EventSlotPrepare(uint8_t* base, uint32_t event, uint32_t offset, uint32_t frame,
                          uint32_t return_address, const GuestCall& call) {
  const uint32_t method = xa::Method(base, event, 8);
  if (method == kPlayWavePrepare) {
    SetRegs(call, Args({event, offset}));
    return PlayWavePrepare(base, event, offset, true,
                           xa::FrameBelow(base, frame, kPlayWavePrepareFrame), call);
  }
  return CallAt(call, method, return_address, Args({event, offset}), frame);
}
uint32_t EventSlotIsPrepared(uint8_t* base, uint32_t event, uint32_t frame,
                             uint32_t return_address, const GuestCall& call) {
  const uint32_t method = xa::Method(base, event, 20);
  if (method == kPlayWaveIsPrepared) {
    SetRegs(call, Args({event}));
    return PlayWaveIsPrepared(base, event, xa::FrameBelow(base, frame, kPlayWaveSmallFrame),
                              call);
  }
  return CallAt(call, method, return_address, Args({event}), frame);
}

}  // namespace

uint32_t PlayWavePrepare(uint8_t* base, uint32_t event, uint32_t offset, bool native_heap,
                         uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D86F8 XACTPlayWaveEvent::Prepare");
  const uint32_t record = Load(base, event + 16);
  const uint32_t time = (Load(base, record) >> 5) & 0xFFFFFF;
  uint32_t seek = 0;
  if (int32_t(time) > int32_t(offset)) {
    Store(base, event + 32, offset);
  } else {
    seek = offset - time;
    Store(base, event + 32, time);
  }
  const uint32_t cue =
      Load(base, Load(base, Load(base, Load(base, event + 44) + 16) + 20) + 48);
  const uint32_t sound_bank = Load(base, cue + 296);
  const uint32_t index = LoadU8(base, record + 10);
  std::optional<uint32_t> found;
  if (native_heap) {
    SetRegs(call, Args({sound_bank, index}));
    found = SoundBankGetWaveBank(base, sound_bank, index);
  }
  const uint32_t bank = found ? *found
                              : CallAt(call, kSoundBankGetWaveBank, 0x825D8750,
                                       Args({sound_bank, index}), frame);
  uint32_t r30 = 0;
  if (bank == 0) {
    r30 = kNoWaveBank;
  } else if (int32_t(Load(base, bank + 252)) == 0) {
    r30 = kWaveBankNotPrepared;
  }
  Store(base, frame + 80, 0);
  if (int32_t(r30) < 0) {
    return r30;
  }
  r30 = CallAt(call, xa::Method(base, bank, 40), 0x825D87B0,
               Args({bank, guest::LoadU16(base, Load(base, event + 16) + 8), event, frame + 80}),
               frame);
  if (int32_t(r30) >= 0) {
    const uint32_t count_method = xa::Method(base, event, 48);
    uint32_t loops;
    if (count_method == kPlayWaveGetLoopCount) {
      SetRegs(call, Args({event}));
      loops = PlayWaveGetLoopCount(base, event);
    } else {
      loops = CallAt(call, count_method, 0x825D87D0, Args({event}), frame);
    }
    const uint32_t wave = Load(base, frame + 80);
    r30 = CallAt(call, xa::Method(base, wave, 4), 0x825D87F0, Args({wave, loops, 0, seek}),
                 frame);
    if (int32_t(r30) >= 0) {
      const uint32_t set_method = xa::Method(base, event, 104);
      const uint32_t now_wave = Load(base, frame + 80);
      if (set_method == kPlayWaveSetWave) {
        SetRegs(call, Args({event, now_wave}));
        PlayWaveSetWave(base, event, now_wave);
      } else {
        CallAt(call, set_method, 0x825D8814, Args({event, now_wave}), frame);
      }
      const uint32_t linked = Load(base, frame + 80);
      const uint32_t list = bank + 240;
      Store(base, linked + 4, list);
      Store(base, linked + 8, Load(base, list + 8));
      Store(base, Load(base, list + 8) + 4, linked);
      Store(base, list + 8, linked);
      return r30;
    }
  }
  const uint32_t wave = Load(base, frame + 80);
  if (wave != 0) {
    CallAt(call, xa::Method(base, wave, 0), 0x825D8860, Args({wave, 1}), frame);
  }
  return r30;
}

uint32_t TrackInstancePrepareSimple(uint8_t* base, uint32_t instance, uint32_t record,
                                    uint32_t offset, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D53D8 XACTTrackInstance::PrepareSimple");
  Store(base, instance + 40, 2);
  StoreU8(base, instance + 24, 1);
  SetRegs(call, Args({4, kEventArrayAttributes}));
  const uint32_t array = memory::crt::XMemAlloc(base, 4, kEventArrayAttributes);
  Store(base, instance + 20, array);
  if (array != 0) {
    SetRegs(call, Args({72, kPlayWaveAttributes}));
    uint32_t event = memory::crt::XMemAlloc(base, 72, kPlayWaveAttributes);
    if (event != 0) {
      const uint32_t engine = Load(base, instance + 12);
      SetRegs(call, Args({event, engine, instance, record, 0}));
      PlayWaveConstruct(base, event, engine, instance, record, 0, 0);
    }
    Store(base, Load(base, instance + 20), event);
    event = Load(base, Load(base, instance + 20));
    if (event != 0) {
      Store(base, instance + 36, event);
      const uint32_t r30 = EventSlotPrepare(base, event, offset, frame, 0x825D5490, call);
      if (int32_t(r30) < 0) {
        return r30;
      }
      const uint32_t prepared =
          EventSlotIsPrepared(base, Load(base, instance + 36), frame, 0x825D54B0, call);
      if (int32_t(prepared) != 0) {
        Store(base, instance + 40, 3);
      }
      return r30;
    }
  }
  // Out of memory.
  const uint32_t held = Load(base, instance + 20);
  StoreU8(base, instance + 24, 0);
  if (held == 0) {
    return kOutOfMemory;
  }
  const uint32_t event = Load(base, held);
  if (event != 0) {
    CallAt(call, xa::Method(base, event, 0), 0x825D5500, Args({event, 1}), frame);
  }
  const uint32_t now = Load(base, instance + 20);
  SetRegs(call, Args({now, kEventArrayAttributes}));
  memory::crt::XMemFree(now, kEventArrayAttributes);
  Store(base, instance + 20, 0);
  return kOutOfMemory;
}

uint32_t TrackPrepareSimple(uint8_t* base, uint32_t track, uint32_t record, uint32_t offset,
                            uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D09D0 XACTTrack::PrepareSimple");
  uint32_t r30;
  if (const auto rpcs = TrackCreateRpcs(base, track)) {
    SetRegs(call, Args({track}));
    r30 = *rpcs;
  } else {
    r30 = CallAt(call, 0x825D06B8, 0x825D09EC, Args({track}), frame);
  }
  SetRegs(call, Args({64, kTrackInstanceAttributes}));
  uint32_t instance = memory::crt::XMemAlloc(base, 64, kTrackInstanceAttributes);
  if (instance != 0) {
    const uint32_t engine = Load(base, track + 12);
    SetRegs(call, Args({instance, engine, track, 0xFFFFFFFFFFFFFFFFull}));
    TrackInstanceConstruct(base, instance, engine, track);
  }
  Store(base, track + 24, instance);
  if (instance == 0) {
    r30 = kOutOfMemory;
  } else if (int32_t(r30) >= 0) {
    SetRegs(call, Args({instance, record, offset}));
    r30 = TrackInstancePrepareSimple(base, instance, record, offset,
                                     xa::FrameBelow(base, frame, kTrackInstancePrepareFrame),
                                     call);
    if (int32_t(r30) >= 0) {
      if (const auto started = TrackStartRpcs(base, track)) {
        SetRegs(call, Args({track}));
      } else {
        CallAt(call, 0x825D0800, 0x825D0A90, Args({track}), frame);
      }
      return r30;
    }
  }
  instance = Load(base, track + 24);
  if (instance != 0) {
    CallAt(call, xa::Method(base, instance, 0), 0x825D0A54, Args({instance, 1}), frame);
  }
  Store(base, track + 24, 0);
  return r30;
}

// --- Cancelling, stopping and deleting instances ---

namespace {

constexpr uint32_t kCancelFrame = 112;
constexpr uint32_t kInstanceDestructFrame = 112;
constexpr uint32_t kInstanceStopFrame = 112;
constexpr uint32_t kSoundCancel = 0x825CA6F0;
constexpr uint32_t kPlayWaveIsPlaying = 0x825D7708;  // slot +24

// The play-wave event's IsPlaying (slot +24) from `frame`.
uint32_t EventSlotIsPlaying(uint8_t* base, uint32_t event, uint32_t frame,
                            uint32_t return_address, const GuestCall& call) {
  const uint32_t method = xa::Method(base, event, 24);
  if (method == kPlayWaveIsPlaying) {
    SetRegs(call, Args({event}));
    return PlayWaveIsPlaying(base, event, xa::FrameBelow(base, frame, kPlayWaveSmallFrame),
                             call);
  }
  return CallAt(call, method, return_address, Args({event}), frame);
}

// The sound of the instance's track.
uint32_t SoundOf(uint8_t* base, uint32_t instance) {
  return Load(base, Load(base, instance + 16) + 20);
}

}  // namespace

uint32_t TrackInstanceCancelEvents(uint8_t* base, uint32_t instance, uint32_t frame,
                                   const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D5520 XACTTrackInstance::CancelEvents");
  uint32_t count = 0;
  CallAt(call, xa::kRtlEnterCriticalSection, 0x825D5540,
         Args({uint64_t(Load(base, instance + 12) + 144)}), frame);
  for (uint32_t i = 0; i < LoadU8(base, instance + 24); i = (i + 1) & 0xFF) {
    const uint32_t event = Load(base, Load(base, instance + 20) + 4 * i);
    if ((LoadU8(base, event + 40) & 0x20) == 0) {
      continue;
    }
    CallAt(call, kSoundCancel, 0x825D557C, Args({SoundOf(base, instance), event}), frame);
    count++;
  }
  const uint32_t list = instance + 48;
  while (Load(base, list + 4) != list && Load(base, list + 8) != list) {
    const uint32_t helper = Load(base, Load(base, instance + 52) + 12);
    CallAt(call, kSoundCancel, 0x825D55C4, Args({SoundOf(base, instance), helper}), frame);
  }
  CallAt(call, xa::kRtlLeaveCriticalSection, 0x825D55D4,
         Args({uint64_t(Load(base, instance + 12) + 144)}), frame);
  return count;
}

uint32_t TrackInstanceDestruct(uint8_t* base, uint32_t instance, uint32_t frame,
                               const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D5838 XACTTrackInstance::~XACTTrackInstance");
  Store(base, instance + 0, kTrackInstanceVtable);
  uint32_t r3 = instance;
  const uint32_t next = Load(base, instance + 4);
  if (next != instance) {
    const uint32_t prev = Load(base, instance + 8);
    if (prev != instance) {
      Store(base, next + 8, prev);
      Store(base, Load(base, instance + 8) + 4, Load(base, instance + 4));
      Store(base, instance + 4, instance);
      Store(base, instance + 8, instance);
    }
  }
  if (Load(base, instance + 20) != 0) {
    SetRegs(call, Args({instance}));
    r3 = TrackInstanceCancelEvents(base, instance, xa::FrameBelow(base, frame, kCancelFrame),
                                   call);
    for (uint32_t i = 0; i < LoadU8(base, instance + 24); i = (i + 1) & 0xFF) {
      r3 = Load(base, Load(base, instance + 20) + 4 * i);
      if (r3 != 0) {
        r3 = CallAt(call, xa::Method(base, r3, 0), 0x825D58D8, Args({r3, 1}), frame);
      }
    }
    const uint32_t events = Load(base, instance + 20);
    SetRegs(call, Args({events, kEventArrayAttributes}));
    r3 = memory::crt::XMemFree(events, kEventArrayAttributes);
  }
  const uint32_t list = instance + 48;
  while (Load(base, list + 4) != list && Load(base, list + 8) != list) {
    r3 = Load(base, instance + 52);
    if (r3 != 0) {
      r3 = CallAt(call, xa::Method(base, r3, 0), 0x825D5938, Args({r3, 1}), frame);
    }
  }
  const int32_t state = int32_t(Load(base, instance + 40));
  if (state == 4 || state == 5) {
    Store(base, instance + 40, 6);
  }
  Store(base, list + 0, kListBaseVtable);
  Store(base, instance + 0, kListBaseVtable);
  return r3;
}

uint32_t TrackInstanceDeletingDestruct(uint8_t* base, uint32_t instance, uint32_t flags,
                                       uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D5E58 XACTTrackInstance::~XACTTrackInstance (deleting)");
  SetRegs(call, Args({instance, flags}));
  TrackInstanceDestruct(base, instance, xa::FrameBelow(base, frame, kInstanceDestructFrame),
                        call);
  if ((flags & 1) != 0) {
    SetRegs(call, Args({instance, kTrackInstanceAttributes}));
    memory::crt::XMemFree(instance, kTrackInstanceAttributes);
  }
  return instance;
}

uint32_t TrackInstanceStop(uint8_t* base, uint32_t instance, uint32_t immediate, uint32_t frame,
                           const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D5D18 XACTTrackInstance::Stop");
  const int32_t state = int32_t(Load(base, instance + 40));
  if (state == 6) {
    return 0;
  }
  if (state == 5 && (int32_t(Load(base, instance + 60)) != 0 || int32_t(immediate) == 0)) {
    return 0;
  }
  Store(base, instance + 40, 5);
  uint32_t cancelled = 0;
  if (int32_t(immediate) != 0) {
    Store(base, instance + 60, 1);
    SetRegs(call, Args({instance}));
    cancelled = TrackInstanceCancelEvents(base, instance,
                                          xa::FrameBelow(base, frame, kCancelFrame), call);
  }
  const uint32_t event = Load(base, instance + 36);
  if (event != 0 && int32_t(EventSlotIsPlaying(base, event, frame, 0x825D5D98, call)) != 0) {
    const uint32_t now = Load(base, instance + 36);
    CallAt(call, xa::Method(base, now, 60), 0x825D5DB8, Args({now, immediate}), frame);
  }
  if (int32_t(immediate) != 0 && int32_t(cancelled) == 0) {
    Store(base, instance + 40, 6);
  }
  return 0;
}

uint32_t TrackStop(uint8_t* base, uint32_t track, uint32_t immediate, uint32_t frame,
                   const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D01D8 XACTTrack::Stop");
  uint32_t r30 = 0;
  const uint32_t instance = Load(base, track + 24);
  if (instance != 0) {
    uint32_t now = immediate;
    if (int32_t(now) == 0) {
      const uint32_t event = Load(base, instance + 36);
      if (event == 0 || int32_t(EventSlotIsPlaying(base, event, frame, 0x825D0220, call)) == 0) {
        now = 1;
      }
    }
    const uint32_t stopped = Load(base, track + 24);
    SetRegs(call, Args({stopped, now}));
    r30 = TrackInstanceStop(base, stopped, now,
                            xa::FrameBelow(base, frame, kInstanceStopFrame), call);
  }
  const uint32_t next = Load(base, track + 28);
  if (next != 0) {
    CallAt(call, 0x825C1548, 0x825D0250, Args({Load(base, track + 12), next}), frame);
    Store(base, track + 28, 0);
  }
  return r30;
}

// --- Making events ---

std::optional<uint32_t> EventCreate(uint8_t* base, uint32_t engine, uint32_t instance,
                                    uint32_t record, uint32_t previous, uint32_t out,
                                    const GuestCall& call) {
  const uint32_t type = Load(base, record) & 0x1F;
  if (type > 20) {
    ITC_NATIVE_RUNNING("sub_825D0F00 XACTEvent::Create");
    return 0x8AC70011u;  // XACTENGINE_E_INVALIDEVENTTYPE
  }
  const bool play_wave = type == 1 || type == 2 || type == 4 || type == 5;
  if (!play_wave && type != 20) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825D0F00 XACTEvent::Create");
  if (play_wave) {
    SetRegs(call, Args({72, kPlayWaveAttributes}));
    uint32_t event = memory::crt::XMemAlloc(base, 72, kPlayWaveAttributes);
    if (event != 0) {
      const uint32_t variation = (type == 2 || type == 5) ? Load(base, record + 12) : 0;
      SetRegs(call, Args({event, engine, instance, record, previous}));
      event = PlayWaveConstruct(base, event, engine, instance, record, previous, variation);
    }
    Store(base, out, event);
  }
  return Load(base, out) == 0 ? kOutOfMemory : 0;
}

// --- Preparing a complex sound's track ---

namespace {

constexpr uint32_t kSoundBankFirstEvent = 0x825C7F30;  // the sound bank's slot +24
constexpr uint32_t kSoundBankNextEvent = 0x825C7F98;   // slot +28
constexpr uint32_t kEventCreate = 0x825D0F00;
constexpr uint32_t kSeekBeyondEnd = 0x8AC70019;  // XACTENGINE_E_SEEKTIMEBEYONDCUEEND
constexpr uint32_t kTrackInstancePrepareFull = 192;
constexpr uint32_t kCreateInstanceFrame = 128;

// The sound bank's slot +24 (FirstEvent) or +28 (NextEvent) on `iterator`,
// from `frame`: the plain C++ while the slot holds it and it answers.
uint32_t SoundBankEvent(uint8_t* base, uint32_t bank, uint32_t slot, uint32_t iterator,
                        uint32_t frame, uint32_t return_address, const GuestCall& call) {
  const uint32_t method = xa::Method(base, bank, slot);
  if (method == (slot == 24 ? kSoundBankFirstEvent : kSoundBankNextEvent)) {
    const auto record = slot == 24 ? SoundBankFirstEvent(base, bank, iterator)
                                   : SoundBankNextEvent(base, bank, iterator);
    if (record) {
      SetRegs(call, Args({bank, iterator}));
      return *record;
    }
  }
  return CallAt(call, method, return_address, Args({bank, iterator}), frame);
}

// An event deleted through its slot +0 with 1, when it's there.
void DeleteEvent(uint8_t* base, uint32_t event, uint32_t frame, uint32_t return_address,
                 const GuestCall& call) {
  if (event != 0) {
    CallAt(call, xa::Method(base, event, 0), return_address, Args({event, 1}), frame);
  }
}

}  // namespace

uint32_t TrackInstancePrepare(uint8_t* base, uint32_t instance, uint32_t key, uint32_t offset,
                              uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D5A48 XACTTrackInstance::Prepare");
  const uint32_t cue = Load(base, Load(base, Load(base, instance + 16) + 20) + 48);
  const uint32_t sound_bank = Load(base, cue + 296);
  const uint32_t iterator = frame + 88;
  Store(base, iterator + 0, key);
  Store(base, iterator + 4, 0);
  Store(base, iterator + 8, 0);
  Store(base, instance + 40, 2);
  uint32_t record = SoundBankEvent(base, sound_bank, 24, iterator, frame, 0x825D5AA8, call);
  const uint8_t count = LoadU8(base, iterator + 8);
  StoreU8(base, instance + 24, count);
  SetRegs(call, Args({uint32_t(count) << 2, kEventArrayAttributes}));
  const uint32_t array = memory::crt::XMemAlloc(base, uint32_t(count) << 2, kEventArrayAttributes);
  Store(base, instance + 20, array);
  uint32_t r29 = array != 0 ? 0 : kOutOfMemory;
  uint8_t kept = 0;
  bool beyond = false;
  while (int32_t(r29) >= 0) {
    if (record == 0) {
      if (beyond && kept == 0) {
        r29 = kSeekBeyondEnd;
        break;
      }
      StoreU8(base, instance + 24, kept);
      const uint32_t event = Load(base, instance + 36);
      if (event != 0) {
        EventSlotIsPrepared(base, event, frame, 0x825D5D04, call);
      }
      Store(base, instance + 40, 3);
      return r29;
    }
    const uint32_t previous =
        (Load(base, record) & 0x20000000) != 0
            ? 0
            : Load(base, Load(base, instance + 20) + (uint32_t(LoadU8(base, record + 6)) << 2));
    const uint32_t out = frame + 80;
    Store(base, out, 0);
    const uint32_t engine = Load(base, instance + 12);
    if (const auto made = EventCreate(base, engine, instance, record, previous, out, call)) {
      SetRegs(call, Args({engine, instance, record, previous, out}));
      r29 = *made;
    } else {
      r29 = CallAt(call, kEventCreate, 0x825D5B3C,
                   Args({engine, instance, record, previous, out}), frame);
    }
    if (int32_t(r29) < 0) {
      DeleteEvent(base, Load(base, out), frame, 0x825D5C44, call);
      break;
    }
    bool drop = false;
    const uint32_t event = Load(base, out);
    const uint32_t word = Load(base, Load(base, event + 16));
    if ((word & 0x1F) == 9 &&
        int32_t(((word >> 5) & 0xFFFFFF) + Load(base, event + 20)) < int32_t(offset)) {
      drop = true;
    } else {
      r29 = EventSlotPrepare(base, event, offset, frame, 0x825D5B94, call);
      if (int32_t(r29) >= 0) {
        const uint32_t prepared = Load(base, out);
        const int32_t type = int32_t(Load(base, Load(base, prepared + 16)) & 0x1F);
        if (type >= 1 && type <= 6) {
          Store(base, instance + 36, prepared);
        }
        Store(base, Load(base, instance + 20) + (uint32_t(kept) << 2), prepared);
        kept = uint8_t(kept + 1);
      }
      if (r29 == kSeekBeyondEnd) {
        beyond = true;
        drop = true;
        r29 = 0;
      }
    }
    if (int32_t(r29) < 0) {
      DeleteEvent(base, Load(base, out), frame, 0x825D5C44, call);
      break;
    }
    record = SoundBankEvent(base, sound_bank, 28, iterator, frame, 0x825D5C18, call);
    if (drop) {
      DeleteEvent(base, Load(base, out), frame, 0x825D5C44, call);
    }
  }
  // A failure: every kept event deleted, last first, and the array freed.
  for (uint8_t i = kept; i != 0;) {
    i = uint8_t(i - 1);
    DeleteEvent(base, Load(base, Load(base, instance + 20) + (uint32_t(i) << 2)), frame,
                0x825D5C88, call);
  }
  const uint32_t held = Load(base, instance + 20);
  StoreU8(base, instance + 24, 0);
  if (held != 0) {
    SetRegs(call, Args({held, kEventArrayAttributes}));
    memory::crt::XMemFree(held, kEventArrayAttributes);
    Store(base, instance + 20, 0);
  }
  Store(base, instance + 36, 0);
  return r29;
}

uint32_t TrackCreateInstance(uint8_t* base, uint32_t track, uint32_t key, uint32_t out,
                             uint32_t offset, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D02F8 XACTTrack::CreateInstance");
  SetRegs(call, Args({64, kTrackInstanceAttributes}));
  const uint32_t instance = memory::crt::XMemAlloc(base, 64, kTrackInstanceAttributes);
  if (instance == 0) {
    return kOutOfMemory;
  }
  const uint32_t engine = Load(base, track + 12);
  SetRegs(call, Args({instance, engine, track, 0xFFFFFFFFFFFFFFFFull}));
  TrackInstanceConstruct(base, instance, engine, track);
  SetRegs(call, Args({instance, key, offset}));
  const uint32_t r30 = TrackInstancePrepare(
      base, instance, key, offset, xa::FrameBelow(base, frame, kTrackInstancePrepareFull), call);
  if (int32_t(r30) >= 0) {
    Store(base, out, instance);
    return r30;
  }
  CallAt(call, xa::Method(base, instance, 0), 0x825D0370, Args({instance, 1}), frame);
  return r30;
}

uint32_t TrackPrepare(uint8_t* base, uint32_t track, uint32_t key, uint32_t offset,
                      uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D0AA0 XACTTrack::Prepare");
  uint32_t r3;
  if (const auto rpcs = TrackCreateRpcs(base, track)) {
    SetRegs(call, Args({track}));
    r3 = *rpcs;
  } else {
    r3 = CallAt(call, 0x825D06B8, 0x825D0ABC, Args({track}), frame);
  }
  const uint32_t out = frame + 80;
  Store(base, out, 0);
  if (int32_t(r3) < 0) {
    return r3;
  }
  SetRegs(call, Args({track, key, out, offset}));
  const uint32_t r30 = TrackCreateInstance(base, track, key, out, offset,
                                           xa::FrameBelow(base, frame, kCreateInstanceFrame),
                                           call);
  if (int32_t(r30) < 0) {
    return r30;
  }
  Store(base, track + 24, Load(base, out));
  if (TrackStartRpcs(base, track)) {
    SetRegs(call, Args({track}));
  } else {
    CallAt(call, 0x825D0800, 0x825D0AFC, Args({track}), frame);
  }
  return r30;
}

// --- Scheduling ---

namespace {

constexpr uint32_t kEventFire = 0x825D1358;  // the base's slot +4
constexpr uint32_t kEventFireFrame = 96;

}  // namespace

uint32_t SoundScheduleEvent(uint8_t* base, uint32_t sound, uint32_t event, uint32_t frame,
                            const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825CB0C8 XACTSound::ScheduleEvent");
  const uint32_t queued = xa::Method(base, event, 16);
  uint32_t r3;
  if (queued == kPlayWaveSetQueued) {
    SetRegs(call, Args({event, 1}));
    PlayWaveSetQueued(base, event, 1);
    r3 = event;
  } else {
    r3 = CallAt(call, queued, 0x825CB0FC, Args({event, 1}), frame);
  }
  const int32_t time = int32_t(Load(base, event + 20));
  if (time <= int32_t(Load(base, Load(base, sound + 32) + 24))) {
    const uint32_t fire = xa::Method(base, event, 4);
    if (fire == kEventFire) {
      SetRegs(call, Args({event}));
      return EventFire(base, event, xa::FrameBelow(base, frame, kEventFireFrame), call);
    }
    return CallAt(call, fire, 0x825CB124, Args({event}), frame);
  }
  const uint32_t list = sound + 56;
  uint32_t node = Load(base, sound + 60);
  while (node != list) {
    if (int32_t(Load(base, node + 20)) > time) {
      break;
    }
    node = Load(base, node + 4);
  }
  const uint32_t before = Load(base, node + 8);
  Store(base, event + 8, before);
  Store(base, event + 4, Load(base, before + 4));
  Store(base, Load(base, before + 4) + 8, event);
  Store(base, before + 4, event);
  return r3;
}

}  // namespace xact
