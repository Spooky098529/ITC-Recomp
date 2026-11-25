// XACT's tracks, track instances and events natively (tracks.h,
// docs/functions/xact-tracks.md). The objects' slots run the plain C++ here
// while they hold the native function, else the guest call; the waves' and
// voices' slots go through the guest call (their adapters run their plain
// C++). Strict floating point (CMakeLists.txt), as the original's steps.

#include "xact/tracks.h"

#include "base/log.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "schema/xact_tracks.h"
#include "schema/xact_waves.h"
#include "xact/xact.h"

#include <initializer_list>

namespace xact {

namespace {

namespace xa = xaudio;

using TrackV = schema::XACTTrack;
using TrackRecordV = schema::XACTTrackRecord;
using InstanceV = schema::XACTTrackInstance;
using EventV = schema::XACTEvent;
using PlayWaveV = schema::XACTPlayWaveEvent;
using WaveVariationV = schema::XACTWaveVariationEvent;
using EventRecordV = schema::XACTEventRecord;
using G = schema::XactTracksGlobals;
using WaveV = schema::XACTWave;

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}
uint8_t LoadU8(uint32_t address) { return guest::LoadU8(address); }

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
uint32_t WaveOf(uint32_t event, uint32_t return_address, uint32_t frame,
                const GuestCall& call) {
  const uint32_t method = xa::Method(event, 56);
  if (method == kGetWord60) {
    SetRegs(call, Args({event}));
    return PlayWaveV(event).wave();
  }
  return CallAt(call, method, return_address, Args({event}), frame);
}

uint32_t WaveSetVolumeFrom(uint32_t wave, double volume, uint32_t sp,
                           uint32_t return_address, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D1CC8 XACTWave::SetVolume");
  const uint32_t voice = WaveV(wave).voice().address;
  if (voice == 0) {
    return 0;
  }
  return CallAt(call, xa::Method(voice, 4), return_address, WithF1(Args({voice}), volume),
                sp);
}

}  // namespace

void TrackInstanceConstruct(uint32_t instance, uint32_t engine, uint32_t track) {
  ITC_NATIVE_RUNNING("sub_825D57D0 XACTTrackInstance::XACTTrackInstance");
  InstanceV(instance).set_node_next(instance);
  InstanceV(instance).set_node_prev(instance);
  InstanceV(instance).set_engine(engine);
  InstanceV(instance).set_track(track);
  InstanceV(instance).set_vtable(kTrackInstanceVtable);
  InstanceV(instance).set_events(0);
  InstanceV(instance).set_event_count(0);
  InstanceV(instance).set_start_time(0);
  InstanceV(instance).set_play_wave_event(0);
  InstanceV(instance).set_state(1);
  InstanceV(instance).set_unfired_count(0);
  InstanceV(instance).set_unfinished_count(0);
  InstanceV(instance).set_flags(0);
  const InstanceV inst(instance);
  const uint32_t list = inst.helpers_vtable_address();
  inst.set_helpers_vtable(kListBaseVtable);
  inst.set_helpers_next(list);
  inst.set_helpers_prev(list);
  InstanceV(instance).set_stopped_at_once(0);
}

void TrackInstanceCheckDone(uint32_t instance) {
  ITC_NATIVE_RUNNING("sub_825D55E0 XACTTrackInstance::CheckDone");
  if (InstanceV(instance).unfired_count() != 0 ||
      InstanceV(instance).unfinished_count() != 0) {
    return;
  }
  const InstanceV inst(instance);
  const uint32_t list = inst.helpers_vtable_address();
  if (inst.helpers_next() != list || inst.helpers_prev() != list) {
    return;
  }
  InstanceV(instance).set_state(6);
}

uint32_t TrackInstanceSetVolume(uint32_t instance, double volume, uint32_t sp,
                                const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D5678 XACTTrackInstance::SetVolume");
  const uint32_t event = InstanceV(instance).play_wave_event().address;
  if (event == 0) {
    return 0;
  }
  const uint32_t method = xa::Method(event, 64);
  if (method == kPlayWaveSetVolume) {
    SetRegs(call, Args({event}));
    return PlayWaveSetVolume(event, volume, xa::FrameBelow(sp, kPlayWaveFrame),
                             call);
  }
  return CallAt(call, method, 0, WithF1(Args({event}), volume), sp);
}

uint32_t TrackInstanceIsBusy(uint32_t instance, uint32_t sp,
                             const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D5780 XACTTrackInstance::IsBusy");
  const uint32_t event = InstanceV(instance).play_wave_event().address;
  if (event == 0) {
    return 0;
  }
  const uint32_t method = xa::Method(event, 44);
  if (method == kPlayWaveIsBusy) {
    SetRegs(call, Args({event}));
    return PlayWaveIsBusy(event, xa::FrameBelow(sp, kPlayWaveSmallFrame), call);
  }
  return CallAt(call, method, 0, Args({event}), sp);
}

uint32_t TrackInstancePause(uint32_t instance, uint32_t pause, uint32_t sp,
                            const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D57A8 XACTTrackInstance::Pause");
  const uint32_t event = InstanceV(instance).play_wave_event().address;
  if (event == 0) {
    return 0;
  }
  const uint32_t method = xa::Method(event, 88);
  if (method == kPlayWavePause) {
    SetRegs(call, Args({event}));
    return PlayWavePause(event, pause, xa::FrameBelow(sp, kPlayWaveFrame), call);
  }
  return CallAt(call, method, 0, Args({event}), sp);
}

uint32_t TrackPause(uint32_t track, uint32_t pause, uint32_t sp,
                    const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D05A8 XACTTrack::Pause");
  const uint32_t instance = TrackV(track).instance().address;
  if (instance == 0) {
    return 0;
  }
  SetRegs(call, Args({instance}));
  return TrackInstancePause(instance, pause, sp, call);
}

uint32_t TrackIsBusy(uint32_t track, uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D0590 XACTTrack::IsBusy");
  const uint32_t instance = TrackV(track).instance().address;
  if (instance == 0) {
    return 0;
  }
  SetRegs(call, Args({instance}));
  return TrackInstanceIsBusy(instance, sp, call);
}

void EventConstruct(uint32_t event, uint32_t engine, uint32_t instance,
                    uint32_t record, uint32_t previous, uint32_t uncounted) {
  ITC_NATIVE_RUNNING("sub_825D1760 XACTEvent::XACTEvent");
  EventV(event).set_node_next(event);
  EventV(event).set_node_prev(event);
  EventV(event).set_engine(engine);
  EventV(event).set_record(record);
  EventV(event).set_previous(previous);
  EventV(event).set_instance(instance);
  EventV(event).set_vtable(kEventVtable);
  EventV(event).set_fire_time(0);
  EventV(event).set_done_time(0);
  EventV(event).set_elapsed(0);
  EventV(event).set_end_time(0);
  EventV(event).set_flags(uncounted != 0 ? 0x08 : 0);
}

void EventDestruct(uint32_t event) {
  ITC_NATIVE_RUNNING("sub_825D0EF0 XACTEvent::~XACTEvent");
  EventV(event).set_vtable(kListBaseVtable);
}

uint32_t EventFire(uint32_t event, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D1358 XACTEvent::Fire");
  uint8_t flags = EventV(event).flags();
  if ((flags & 0x80) == 0) {
    flags |= 0x80;
    EventV(event).set_flags(flags);
    if ((flags & 0x08) == 0) {
      const uint32_t instance = EventV(event).instance().address;
      InstanceV(instance)
          .set_unfinished_count(uint8_t(InstanceV(instance).unfinished_count() + 1));
    }
  }
  const uint32_t method = xa::Method(event, 16);
  uint32_t r3;
  if (method == kPlayWaveSetQueued) {
    SetRegs(call, Args({event, 0}));
    PlayWaveSetQueued(event, 0);
    r3 = event;
  } else {
    r3 = CallAt(call, method, 0x825D13BC, Args({event, 0}), frame);
  }
  if ((EventV(event).flags() & 0x08) == 0) {
    const uint32_t instance = EventV(event).instance().address;
    InstanceV(instance)
        .set_unfired_count(uint8_t(InstanceV(instance).unfired_count() + 255));
  }
  return r3;
}

uint32_t PlayWaveConstruct(uint32_t event, uint32_t engine, uint32_t instance,
                           uint32_t record, uint32_t previous, uint32_t variation) {
  ITC_NATIVE_RUNNING("sub_825D82E8 XACTPlayWaveEvent::XACTPlayWaveEvent");
  EventConstruct(event, engine, instance, record, previous, 0);
  const uint32_t at = EventV(event).record().address;
  EventV(event).set_vtable(kPlayWaveEventVtable);
  PlayWaveV(event).set_slot36_flag(0);
  const uint8_t loops = EventRecordV(at).loop_count();
  PlayWaveV(event).set_variation(variation);
  PlayWaveV(event).set_wave(0);
  PlayWaveV(event).set_play_time(0);
  PlayWaveV(event).set_stop_time(0);
  PlayWaveV(event).set_loop_count(loops);
  return event;
}

void PlayWaveSetQueued(uint32_t event, uint32_t queued) {
  ITC_NATIVE_RUNNING("sub_825D8530 XACTPlayWaveEvent::SetQueued");
  uint8_t flags = EventV(event).flags();
  const uint32_t wave = PlayWaveV(event).wave();
  flags = uint8_t((flags & ~0x20) | (queued != 0 ? 0x20 : 0));
  EventV(event).set_flags(flags);
  const uint32_t prepared = WaveV(wave).prepared();
  flags = uint8_t((flags & ~0x40) | (prepared == 0 ? 0x40 : 0));
  EventV(event).set_flags(flags);
}

uint32_t PlayWaveGetLoopCount(uint32_t event) {
  ITC_NATIVE_RUNNING("sub_825D8358 XACTPlayWaveEvent::GetLoopCount");
  return PlayWaveV(event).loop_count();
}

void PlayWaveSetWave(uint32_t event, uint32_t wave) {
  ITC_NATIVE_RUNNING("sub_82483208 XACTPlayWaveEvent::SetWave");
  PlayWaveV(event).set_wave(wave);
}

void WaveVariationSetWave(uint32_t event, uint32_t wave) {
  ITC_NATIVE_RUNNING("sub_825D7B78 XACTWaveVariationEvent::SetWave");
  WaveVariationV(event).set_wave(wave);
}

uint32_t PlayWaveGetVariationRecord(uint32_t event) {
  ITC_NATIVE_RUNNING("sub_825D8920 XACTPlayWaveEvent::GetVariationRecord");
  const uint32_t record = EventV(event).record().address;
  const int32_t type = int32_t(EventRecordV(record).word() & 0x1F);
  if (type == 4) {
    return record + 12;
  }
  return type == 5 ? record + 16 : 0;
}

uint32_t PlayWaveForgetWave(uint32_t event, uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D8360 XACTPlayWaveEvent::ForgetWave");
  const uint32_t method = xa::Method(event, 104);
  if (method == kPlayWaveSetWave) {
    SetRegs(call, Args({event, 0}));
    PlayWaveSetWave(event, 0);
    return event;
  }
  return CallAt(call, method, 0, Args({event, 0}), sp);
}

uint32_t PlayWaveGetWaveBank(uint32_t event, uint32_t frame,
                             const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D84C0 XACTPlayWaveEvent::GetWaveBank");
  if (WaveOf(event, 0x825D84E4, frame, call) == 0) {
    return 0;
  }
  return WaveV(WaveOf(event, 0x825D8500, frame, call)).wave_bank();
}

uint32_t PlayWaveIsPrepared(uint32_t event, uint32_t frame,
                            const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D76A0 XACTPlayWaveEvent::IsPrepared");
  if (WaveOf(event, 0x825D76C4, frame, call) == 0) {
    return 0;
  }
  const uint32_t wave = WaveOf(event, 0x825D76E0, frame, call);
  return int32_t(WaveV(wave).state()) >= 2 ? 1 : 0;
}

uint32_t PlayWaveIsPlaying(uint32_t event, uint32_t frame,
                           const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D7708 XACTPlayWaveEvent::IsPlaying");
  if (WaveOf(event, 0x825D772C, frame, call) == 0) {
    return 0;
  }
  if (int32_t(WaveV(WaveOf(event, 0x825D7748, frame, call)).state()) < 3) {
    return 0;
  }
  return int32_t(WaveV(WaveOf(event, 0x825D7768, frame, call)).state()) > 5 ? 0 : 1;
}

uint32_t PlayWaveIsBusy(uint32_t event, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D81E8 XACTPlayWaveEvent::IsBusy");
  if (WaveOf(event, 0x825D820C, frame, call) == 0) {
    return 0;
  }
  const uint32_t wave = WaveOf(event, 0x825D8228, frame, call);
  return CallAt(call, xa::Method(wave, 20), 0x825D8238, Args({wave}), frame);
}

uint32_t PlayWavePause(uint32_t event, uint32_t pause, uint32_t frame,
                       const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D8268 XACTPlayWaveEvent::Pause");
  if (WaveOf(event, 0x825D8294, frame, call) == 0) {
    return 0;
  }
  const uint32_t wave = WaveOf(event, 0x825D82B0, frame, call);
  return CallAt(call, xa::Method(wave, 36), 0x825D82C4, Args({wave, pause}), frame);
}

uint32_t PlayWaveSetVolume(uint32_t event, double volume, uint32_t frame,
                           const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D7FE0 XACTPlayWaveEvent::SetVolume");
  if (WaveOf(event, 0x825D800C, frame, call) == 0) {
    return 0;
  }
  const uint32_t wave = WaveOf(event, 0x825D8028, frame, call);
  SetRegs(call, WithF1(Args({wave}), volume));
  return WaveSetVolumeFrom(wave, volume, frame, 0x825D8030, call);
}

uint32_t WaveSetVolume(uint32_t wave, double volume, uint32_t sp,
                       const GuestCall& call) {
  return WaveSetVolumeFrom(wave, volume, sp, 0, call);
}

// --- The track's volume, set-up, update and destructors; the play-wave
// event's destructor, variation and duration ---

namespace {

constexpr uint32_t kTrackSetVolume = 0x825D0BF8;
constexpr uint32_t kTrackSetVolumeFrame = 112;
constexpr uint32_t kTrackDestructFrame = 96;
constexpr uint32_t kGetVariationRecord = 0x825D8920;  // slot +92
constexpr uint32_t kSoundBankGetWaveBank = 0x825C7218;
constexpr uint32_t kWaveBankGetWaveDurations = 0x825CC380;

double LoadF(uint32_t address) { return double(guest::LoadFloat(address)); }
double Single(double value) { return double(float(value)); }

// The sound's volume times the track's factors, to the instance's SetVolume
// (from `frame`); `track_first` the product's order (SetVolume's f1 x +40,
// UpdateVolume's +40 x f1).
std::optional<uint32_t> ApplyVolume(uint32_t track, bool track_first,
                                    uint32_t frame, const GuestCall& call) {
  const double f31 = double(TrackV(track).volume());
  const uint32_t sound = TrackV(track).sound();
  const double f30 = double(TrackV(track).rpc_volume());
  const auto volume = SoundGetVolume(sound);
  if (!volume) {
    return std::nullopt;
  }
  const double f0 = track_first ? Single(f31 * volume->f1) : Single(volume->f1 * f31);
  const double f1 = Single(f0 * f30);
  const uint32_t instance = TrackV(track).instance().address;
  SetRegs(call, WithF1(Args({instance}), f1));
  return TrackInstanceSetVolume(instance, f1, frame, call);
}

}  // namespace

std::optional<uint32_t> TrackSetVolume(uint32_t track, double volume,
                                       uint32_t frame, const GuestCall& call) {
  const double zero = double(G::zero());
  TrackV(track).set_volume(float(volume < zero ? zero : volume));
  if (TrackV(track).instance().address == 0) {
    ITC_NATIVE_RUNNING("sub_825D0BF8 XACTTrack::SetVolume");
    return track;
  }
  const auto r3 = ApplyVolume(track, false, frame, call);
  if (r3) {
    ITC_NATIVE_RUNNING("sub_825D0BF8 XACTTrack::SetVolume");
  }
  return r3;
}

std::optional<uint32_t> TrackUpdateVolume(uint32_t track, uint32_t frame,
                                          const GuestCall& call) {
  if (TrackV(track).instance().address == 0) {
    ITC_NATIVE_RUNNING("sub_825D0428 XACTTrack::UpdateVolume");
    return track;
  }
  const auto r3 = ApplyVolume(track, true, frame, call);
  if (r3) {
    ITC_NATIVE_RUNNING("sub_825D0428 XACTTrack::UpdateVolume");
  }
  return r3;
}

uint32_t TrackConstruct(uint32_t track, uint32_t engine, uint32_t sound,
                        uint32_t record, uint32_t index, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D0CE0 XACTTrack::XACTTrack");
  const float zero = G::zero();
  TrackV(track).set_pitch(zero);
  TrackV(track).set_node_next(track);
  TrackV(track).set_rpc_pitch(zero);
  TrackV(track).set_node_prev(track);
  TrackV(track).set_engine(engine);
  TrackV(track).set_record(record);
  TrackV(track).set_sound(sound);
  TrackV(track).set_vtable(kTrackVtable);
  double f1 = double(G::one());
  TrackV(track).set_rpc_volume(float(f1));
  TrackV(track).set_index(uint8_t(index));
  TrackV(track).set_instance(0);
  TrackV(track).set_next_instance(0);
  TrackV(track).set_pending_transition(0);
  TrackV(track).set_rpc_entries(0);
  TrackV(track).set_flags(0);
  if (record != 0) {
    f1 =
        LoadF(G::gain_table_address() + (uint32_t(TrackRecordV(record).volume()) << 2));
  }
  SetRegs(call, WithF1(Args({track}), f1));
  if (!TrackSetVolume(track, f1, xa::FrameBelow(frame, kTrackSetVolumeFrame),
                      call)) {
    CallAt(call, kTrackSetVolume, 0x825D0D6C, WithF1(Args({track}), f1), frame);
  }
  return track;
}

uint32_t TrackUpdate(uint32_t track, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D0E30 XACTTrack::Update");
  const uint32_t event = TrackV(track).pending_transition();
  if (event != 0) {
    CallAt(call, 0x825D0B08, 0x825D0E54, Args({track, event}), frame);
    TrackV(track).set_pending_transition(0);
  }
  // The empty sub_82106540 with the instance: nothing to do.
  const uint32_t instance = TrackV(track).instance().address;
  SetRegs(call, Args({instance}));
  return instance;
}

std::optional<uint32_t> TrackCreateRpcs(uint32_t track) {
  const uint32_t sound = TrackV(track).sound();
  if ((LoadU8(Load(sound + 28)) & 0x4) != 0) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825D06B8 XACTTrack::CreateRpcs");
  return 0;
}

std::optional<uint32_t> TrackStartRpcs(uint32_t track) {
  const uint32_t sound = TrackV(track).sound();
  if ((LoadU8(Load(sound + 28)) & 0x4) != 0) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825D0800 XACTTrack::StartRpcs");
  return sound;
}

uint32_t TrackDestruct(uint32_t track, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D0110 XACTTrack::~XACTTrack");
  TrackV(track).set_vtable(kTrackVtable);
  uint32_t r3 = TrackV(track).next_instance().address;
  if (r3 != 0) {
    r3 = CallAt(call, xa::Method(r3, 0), 0x825D0150, Args({r3, 1}), frame);
  }
  r3 = TrackV(track).instance().address;
  if (r3 != 0) {
    r3 = CallAt(call, xa::Method(r3, 0), 0x825D0170, Args({r3, 1}), frame);
  }
  const uint32_t entries = TrackV(track).rpc_entries();
  if (entries != 0) {
    // The RPC entries (never in play): the counted array's vector deleting
    // destructor through the first entry's +4 vtable, or operator delete on
    // the block, as the original.
    if (int32_t(Load(entries - 4)) != 0) {
      r3 = CallAt(call, xa::Method(entries + 4, 0), 0x825D01A4, Args({entries + 4, 3}),
                  frame);
    } else {
      r3 = CallAt(call, 0x825A70E8, 0x825D01AC, Args({entries - 4}), frame);
    }
  }
  TrackV(track).set_vtable(kListBaseVtable);
  return r3;
}

uint32_t TrackDeletingDestruct(uint32_t track, uint32_t flags, uint32_t frame,
                               const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D0978 XACTTrack::~XACTTrack (deleting)");
  SetRegs(call, Args({track, flags}));
  TrackDestruct(track, xa::FrameBelow(frame, kTrackDestructFrame), call);
  if ((flags & 1) != 0) {
    SetRegs(call, Args({track, kTrackAttributes}));
    memory::crt::XMemFree(track, kTrackAttributes);
  }
  return track;
}

uint32_t PlayWaveDeletingDestruct(uint32_t event, uint32_t flags,
                                  uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D8660 XACTPlayWaveEvent::~XACTPlayWaveEvent (deleting)");
  const uint32_t wave = PlayWaveV(event).wave();
  EventV(event).set_vtable(kPlayWaveEventVtable);
  if (wave != 0) {
    CallAt(call, xa::Method(wave, 0), 0x825D86A8, Args({wave, 1}), frame);
  }
  EventV(event).set_vtable(kWaveEventBaseVtable);
  SetRegs(call, Args({event}));
  EventDestruct(event);
  if ((flags & 1) != 0) {
    SetRegs(call, Args({event, kPlayWaveAttributes}));
    memory::crt::XMemFree(event, kPlayWaveAttributes);
  }
  return event;
}

uint32_t PlayWaveApplyVariation(uint32_t event, uint32_t frame,
                                const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D7558 XACTPlayWaveEvent::ApplyVariation");
  const uint32_t method = xa::Method(event, 92);
  uint32_t record;
  if (method == kGetVariationRecord) {
    SetRegs(call, Args({event}));
    record = PlayWaveGetVariationRecord(event);
  } else {
    record = CallAt(call, method, 0x825D757C, Args({event}), frame);
  }
  if (record == 0) {
    return 0;
  }
  return CallAt(call, 0x825D1478, 0x825D7590, Args({event, record}), frame);
}

uint32_t PlayWaveGetDuration(uint32_t event, bool native_heap, uint32_t frame,
                             const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D8870 XACTPlayWaveEvent::GetDuration");
  // The cue: [[[[event + 44] + 16] + 20] + 48] (instance, track, sound).
  const uint32_t cue =
      Load(EventV(event).instance().track().sound() + 48);
  const uint32_t sound_bank = Load(cue + 296);
  const uint32_t index = EventV(event).record().wave_bank_index();
  std::optional<uint32_t> found;
  if (native_heap) {
    SetRegs(call, Args({sound_bank, index}));
    found = SoundBankGetWaveBank(sound_bank, index);
  }
  const uint32_t bank = found ? *found
                              : CallAt(call, kSoundBankGetWaveBank, 0x825D88AC,
                                       Args({sound_bank, index}), frame);
  if (bank == 0) {
    return 0;
  }
  const uint32_t record = EventV(event).record().address;
  if (EventRecordV(record).loop_count() == 255) {
    return 0x7FFFFFFF;
  }
  const uint32_t wave = EventRecordV(record).wave_index();
  const xa::Regs regs = Args({bank, wave, frame + 84, frame + 80});
  uint32_t r3;
  if (const auto durations = WaveBankGetWaveDurations(bank, wave)) {
    SetRegs(call, regs);
    Store(frame + 84, durations->duration_ms);
    Store(frame + 80, durations->loop_length_ms);
    r3 = 0;
  } else {
    r3 = CallAt(call, kWaveBankGetWaveDurations, 0x825D88E0, regs, frame);
  }
  if (int32_t(r3) < 0) {
    return 0;
  }
  const uint32_t loops = EventV(event).record().loop_count();
  return loops * Load(frame + 80) + Load(frame + 84);
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
uint32_t EventSlotPrepare(uint32_t event, uint32_t offset, uint32_t frame,
                          uint32_t return_address, const GuestCall& call) {
  const uint32_t method = xa::Method(event, 8);
  if (method == kPlayWavePrepare) {
    SetRegs(call, Args({event, offset}));
    return PlayWavePrepare(event, offset, true,
                           xa::FrameBelow(frame, kPlayWavePrepareFrame), call);
  }
  return CallAt(call, method, return_address, Args({event, offset}), frame);
}
uint32_t EventSlotIsPrepared(uint32_t event, uint32_t frame,
                             uint32_t return_address, const GuestCall& call) {
  const uint32_t method = xa::Method(event, 20);
  if (method == kPlayWaveIsPrepared) {
    SetRegs(call, Args({event}));
    return PlayWaveIsPrepared(event, xa::FrameBelow(frame, kPlayWaveSmallFrame),
                              call);
  }
  return CallAt(call, method, return_address, Args({event}), frame);
}

}  // namespace

uint32_t PlayWavePrepare(uint32_t event, uint32_t offset, bool native_heap,
                         uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D86F8 XACTPlayWaveEvent::Prepare");
  const uint32_t record = EventV(event).record().address;
  const uint32_t time = (EventRecordV(record).word() >> 5) & 0xFFFFFF;
  uint32_t seek = 0;
  if (int32_t(time) > int32_t(offset)) {
    EventV(event).set_elapsed(offset);
  } else {
    seek = offset - time;
    EventV(event).set_elapsed(time);
  }
  const uint32_t cue =
      Load(EventV(event).instance().track().sound() + 48);
  const uint32_t sound_bank = Load(cue + 296);
  const uint32_t index = EventRecordV(record).wave_bank_index();
  std::optional<uint32_t> found;
  if (native_heap) {
    SetRegs(call, Args({sound_bank, index}));
    found = SoundBankGetWaveBank(sound_bank, index);
  }
  const uint32_t bank = found ? *found
                              : CallAt(call, kSoundBankGetWaveBank, 0x825D8750,
                                       Args({sound_bank, index}), frame);
  uint32_t r30 = 0;
  if (bank == 0) {
    r30 = kNoWaveBank;
  } else if (int32_t(Load(bank + 252)) == 0) {
    r30 = kWaveBankNotPrepared;
  }
  Store(frame + 80, 0);
  if (int32_t(r30) < 0) {
    return r30;
  }
  r30 = CallAt(call, xa::Method(bank, 40), 0x825D87B0,
               Args({bank, EventV(event).record().wave_index(), event, frame + 80}),
               frame);
  if (int32_t(r30) >= 0) {
    const uint32_t count_method = xa::Method(event, 48);
    uint32_t loops;
    if (count_method == kPlayWaveGetLoopCount) {
      SetRegs(call, Args({event}));
      loops = PlayWaveGetLoopCount(event);
    } else {
      loops = CallAt(call, count_method, 0x825D87D0, Args({event}), frame);
    }
    const uint32_t wave = Load(frame + 80);
    r30 = CallAt(call, xa::Method(wave, 4), 0x825D87F0, Args({wave, loops, 0, seek}),
                 frame);
    if (int32_t(r30) >= 0) {
      const uint32_t set_method = xa::Method(event, 104);
      const uint32_t now_wave = Load(frame + 80);
      if (set_method == kPlayWaveSetWave) {
        SetRegs(call, Args({event, now_wave}));
        PlayWaveSetWave(event, now_wave);
      } else {
        CallAt(call, set_method, 0x825D8814, Args({event, now_wave}), frame);
      }
      const uint32_t linked = Load(frame + 80);
      const uint32_t list = bank + 240;
      WaveV(linked).set_node_next(list);
      WaveV(linked).set_node_previous(Load(list + 8));
      Store(Load(list + 8) + 4, linked);
      Store(list + 8, linked);
      return r30;
    }
  }
  const uint32_t wave = Load(frame + 80);
  if (wave != 0) {
    CallAt(call, xa::Method(wave, 0), 0x825D8860, Args({wave, 1}), frame);
  }
  return r30;
}

uint32_t TrackInstancePrepareSimple(uint32_t instance, uint32_t record,
                                    uint32_t offset, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D53D8 XACTTrackInstance::PrepareSimple");
  InstanceV(instance).set_state(2);
  InstanceV(instance).set_event_count(1);
  SetRegs(call, Args({4, kEventArrayAttributes}));
  const uint32_t array = memory::crt::XMemAlloc(4, kEventArrayAttributes);
  InstanceV(instance).set_events(array);
  if (array != 0) {
    SetRegs(call, Args({72, kPlayWaveAttributes}));
    uint32_t event = memory::crt::XMemAlloc(72, kPlayWaveAttributes);
    if (event != 0) {
      const uint32_t engine = InstanceV(instance).engine();
      SetRegs(call, Args({event, engine, instance, record, 0}));
      PlayWaveConstruct(event, engine, instance, record, 0, 0);
    }
    Store(InstanceV(instance).events(), event);
    event = Load(InstanceV(instance).events());
    if (event != 0) {
      InstanceV(instance).set_play_wave_event(event);
      const uint32_t r30 = EventSlotPrepare(event, offset, frame, 0x825D5490, call);
      if (int32_t(r30) < 0) {
        return r30;
      }
      const uint32_t prepared = EventSlotIsPrepared(InstanceV(instance).play_wave_event().address, frame, 0x825D54B0, call);
      if (int32_t(prepared) != 0) {
        InstanceV(instance).set_state(3);
      }
      return r30;
    }
  }
  // Out of memory.
  const uint32_t held = InstanceV(instance).events();
  InstanceV(instance).set_event_count(0);
  if (held == 0) {
    return kOutOfMemory;
  }
  const uint32_t event = Load(held);
  if (event != 0) {
    CallAt(call, xa::Method(event, 0), 0x825D5500, Args({event, 1}), frame);
  }
  const uint32_t now = InstanceV(instance).events();
  SetRegs(call, Args({now, kEventArrayAttributes}));
  memory::crt::XMemFree(now, kEventArrayAttributes);
  InstanceV(instance).set_events(0);
  return kOutOfMemory;
}

uint32_t TrackPrepareSimple(uint32_t track, uint32_t record, uint32_t offset,
                            uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D09D0 XACTTrack::PrepareSimple");
  uint32_t r30;
  if (const auto rpcs = TrackCreateRpcs(track)) {
    SetRegs(call, Args({track}));
    r30 = *rpcs;
  } else {
    r30 = CallAt(call, 0x825D06B8, 0x825D09EC, Args({track}), frame);
  }
  SetRegs(call, Args({64, kTrackInstanceAttributes}));
  uint32_t instance = memory::crt::XMemAlloc(64, kTrackInstanceAttributes);
  if (instance != 0) {
    const uint32_t engine = TrackV(track).engine();
    SetRegs(call, Args({instance, engine, track, 0xFFFFFFFFFFFFFFFFull}));
    TrackInstanceConstruct(instance, engine, track);
  }
  TrackV(track).set_instance(instance);
  if (instance == 0) {
    r30 = kOutOfMemory;
  } else if (int32_t(r30) >= 0) {
    SetRegs(call, Args({instance, record, offset}));
    r30 = TrackInstancePrepareSimple(instance, record, offset,
                                     xa::FrameBelow(frame, kTrackInstancePrepareFrame),
                                     call);
    if (int32_t(r30) >= 0) {
      if (const auto started = TrackStartRpcs(track)) {
        SetRegs(call, Args({track}));
      } else {
        CallAt(call, 0x825D0800, 0x825D0A90, Args({track}), frame);
      }
      return r30;
    }
  }
  instance = TrackV(track).instance().address;
  if (instance != 0) {
    CallAt(call, xa::Method(instance, 0), 0x825D0A54, Args({instance, 1}), frame);
  }
  TrackV(track).set_instance(0);
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
uint32_t EventSlotIsPlaying(uint32_t event, uint32_t frame,
                            uint32_t return_address, const GuestCall& call) {
  const uint32_t method = xa::Method(event, 24);
  if (method == kPlayWaveIsPlaying) {
    SetRegs(call, Args({event}));
    return PlayWaveIsPlaying(event, xa::FrameBelow(frame, kPlayWaveSmallFrame),
                             call);
  }
  return CallAt(call, method, return_address, Args({event}), frame);
}

// The sound of the instance's track.
uint32_t SoundOf(uint32_t instance) {
  return InstanceV(instance).track().sound();
}

}  // namespace

uint32_t TrackInstanceCancelEvents(uint32_t instance, uint32_t frame,
                                   const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D5520 XACTTrackInstance::CancelEvents");
  uint32_t count = 0;
  CallAt(call, xa::kRtlEnterCriticalSection, 0x825D5540,
         Args({uint64_t(InstanceV(instance).engine() + 144)}), frame);
  for (uint32_t i = 0; i < InstanceV(instance).event_count(); i = (i + 1) & 0xFF) {
    const uint32_t event = Load(InstanceV(instance).events() + 4 * i);
    if ((EventV(event).flags() & 0x20) == 0) {
      continue;
    }
    CallAt(call, kSoundCancel, 0x825D557C, Args({SoundOf(instance), event}), frame);
    count++;
  }
  const InstanceV inst(instance);
  const uint32_t list = inst.helpers_vtable_address();
  while (inst.helpers_next() != list && inst.helpers_prev() != list) {
    const uint32_t helper = Load(InstanceV(instance).helpers_next() + 12);
    CallAt(call, kSoundCancel, 0x825D55C4, Args({SoundOf(instance), helper}), frame);
  }
  CallAt(call, xa::kRtlLeaveCriticalSection, 0x825D55D4,
         Args({uint64_t(InstanceV(instance).engine() + 144)}), frame);
  return count;
}

uint32_t TrackInstanceDestruct(uint32_t instance, uint32_t frame,
                               const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D5838 XACTTrackInstance::~XACTTrackInstance");
  InstanceV(instance).set_vtable(kTrackInstanceVtable);
  uint32_t r3 = instance;
  const uint32_t next = InstanceV(instance).node_next();
  if (next != instance) {
    const uint32_t prev = InstanceV(instance).node_prev();
    if (prev != instance) {
      Store(next + 8, prev);
      Store(InstanceV(instance).node_prev() + 4, InstanceV(instance).node_next());
      InstanceV(instance).set_node_next(instance);
      InstanceV(instance).set_node_prev(instance);
    }
  }
  if (InstanceV(instance).events() != 0) {
    SetRegs(call, Args({instance}));
    r3 = TrackInstanceCancelEvents(instance, xa::FrameBelow(frame, kCancelFrame),
                                   call);
    for (uint32_t i = 0; i < InstanceV(instance).event_count(); i = (i + 1) & 0xFF) {
      r3 = Load(InstanceV(instance).events() + 4 * i);
      if (r3 != 0) {
        r3 = CallAt(call, xa::Method(r3, 0), 0x825D58D8, Args({r3, 1}), frame);
      }
    }
    const uint32_t events = InstanceV(instance).events();
    SetRegs(call, Args({events, kEventArrayAttributes}));
    r3 = memory::crt::XMemFree(events, kEventArrayAttributes);
  }
  const InstanceV inst(instance);
  const uint32_t list = inst.helpers_vtable_address();
  while (inst.helpers_next() != list && inst.helpers_prev() != list) {
    r3 = InstanceV(instance).helpers_next();
    if (r3 != 0) {
      r3 = CallAt(call, xa::Method(r3, 0), 0x825D5938, Args({r3, 1}), frame);
    }
  }
  const int32_t state = int32_t(InstanceV(instance).state());
  if (state == 4 || state == 5) {
    InstanceV(instance).set_state(6);
  }
  inst.set_helpers_vtable(kListBaseVtable);
  InstanceV(instance).set_vtable(kListBaseVtable);
  return r3;
}

uint32_t TrackInstanceDeletingDestruct(uint32_t instance, uint32_t flags,
                                       uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D5E58 XACTTrackInstance::~XACTTrackInstance (deleting)");
  SetRegs(call, Args({instance, flags}));
  TrackInstanceDestruct(instance, xa::FrameBelow(frame, kInstanceDestructFrame),
                        call);
  if ((flags & 1) != 0) {
    SetRegs(call, Args({instance, kTrackInstanceAttributes}));
    memory::crt::XMemFree(instance, kTrackInstanceAttributes);
  }
  return instance;
}

uint32_t TrackInstanceStop(uint32_t instance, uint32_t immediate, uint32_t frame,
                           const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D5D18 XACTTrackInstance::Stop");
  const int32_t state = int32_t(InstanceV(instance).state());
  if (state == 6) {
    return 0;
  }
  if (state == 5 &&
      (int32_t(InstanceV(instance).stopped_at_once()) != 0 || int32_t(immediate) == 0)) {
    return 0;
  }
  InstanceV(instance).set_state(5);
  uint32_t cancelled = 0;
  if (int32_t(immediate) != 0) {
    InstanceV(instance).set_stopped_at_once(1);
    SetRegs(call, Args({instance}));
    cancelled = TrackInstanceCancelEvents(instance,
                                          xa::FrameBelow(frame, kCancelFrame), call);
  }
  const uint32_t event = InstanceV(instance).play_wave_event().address;
  if (event != 0 && int32_t(EventSlotIsPlaying(event, frame, 0x825D5D98, call)) != 0) {
    const uint32_t now = InstanceV(instance).play_wave_event().address;
    CallAt(call, xa::Method(now, 60), 0x825D5DB8, Args({now, immediate}), frame);
  }
  if (int32_t(immediate) != 0 && int32_t(cancelled) == 0) {
    InstanceV(instance).set_state(6);
  }
  return 0;
}

uint32_t TrackStop(uint32_t track, uint32_t immediate, uint32_t frame,
                   const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D01D8 XACTTrack::Stop");
  uint32_t r30 = 0;
  const uint32_t instance = TrackV(track).instance().address;
  if (instance != 0) {
    uint32_t now = immediate;
    if (int32_t(now) == 0) {
      const uint32_t event = InstanceV(instance).play_wave_event().address;
      if (event == 0 || int32_t(EventSlotIsPlaying(event, frame, 0x825D0220, call)) == 0) {
        now = 1;
      }
    }
    const uint32_t stopped = TrackV(track).instance().address;
    SetRegs(call, Args({stopped, now}));
    r30 = TrackInstanceStop(stopped, now,
                            xa::FrameBelow(frame, kInstanceStopFrame), call);
  }
  const uint32_t next = TrackV(track).next_instance().address;
  if (next != 0) {
    CallAt(call, 0x825C1548, 0x825D0250, Args({TrackV(track).engine(), next}), frame);
    TrackV(track).set_next_instance(0);
  }
  return r30;
}

// --- Making events ---

std::optional<uint32_t> EventCreate(uint32_t engine, uint32_t instance,
                                    uint32_t record, uint32_t previous, uint32_t out,
                                    const GuestCall& call) {
  const uint32_t type = EventRecordV(record).word() & 0x1F;
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
    uint32_t event = memory::crt::XMemAlloc(72, kPlayWaveAttributes);
    if (event != 0) {
      const uint32_t variation =
          (type == 2 || type == 5) ? EventRecordV(record).variation() : 0;
      SetRegs(call, Args({event, engine, instance, record, previous}));
      event = PlayWaveConstruct(event, engine, instance, record, previous, variation);
    }
    Store(out, event);
  }
  return Load(out) == 0 ? kOutOfMemory : 0;
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
uint32_t SoundBankEvent(uint32_t bank, uint32_t slot, uint32_t iterator,
                        uint32_t frame, uint32_t return_address, const GuestCall& call) {
  const uint32_t method = xa::Method(bank, slot);
  if (method == (slot == 24 ? kSoundBankFirstEvent : kSoundBankNextEvent)) {
    const auto record = slot == 24 ? SoundBankFirstEvent(bank, iterator)
                                   : SoundBankNextEvent(bank, iterator);
    if (record) {
      SetRegs(call, Args({bank, iterator}));
      return *record;
    }
  }
  return CallAt(call, method, return_address, Args({bank, iterator}), frame);
}

// An event deleted through its slot +0 with 1, when it's there.
void DeleteEvent(uint32_t event, uint32_t frame, uint32_t return_address,
                 const GuestCall& call) {
  if (event != 0) {
    CallAt(call, xa::Method(event, 0), return_address, Args({event, 1}), frame);
  }
}

}  // namespace

uint32_t TrackInstancePrepare(uint32_t instance, uint32_t key, uint32_t offset,
                              uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D5A48 XACTTrackInstance::Prepare");
  const uint32_t cue = Load(InstanceV(instance).track().sound() + 48);
  const uint32_t sound_bank = Load(cue + 296);
  const uint32_t iterator = frame + 88;
  Store(iterator + 0, key);
  Store(iterator + 4, 0);
  Store(iterator + 8, 0);
  InstanceV(instance).set_state(2);
  uint32_t record = SoundBankEvent(sound_bank, 24, iterator, frame, 0x825D5AA8, call);
  const uint8_t count = LoadU8(iterator + 8);
  InstanceV(instance).set_event_count(count);
  SetRegs(call, Args({uint32_t(count) << 2, kEventArrayAttributes}));
  const uint32_t array = memory::crt::XMemAlloc(uint32_t(count) << 2, kEventArrayAttributes);
  InstanceV(instance).set_events(array);
  uint32_t r29 = array != 0 ? 0 : kOutOfMemory;
  uint8_t kept = 0;
  bool beyond = false;
  while (int32_t(r29) >= 0) {
    if (record == 0) {
      if (beyond && kept == 0) {
        r29 = kSeekBeyondEnd;
        break;
      }
      InstanceV(instance).set_event_count(kept);
      const uint32_t event = InstanceV(instance).play_wave_event().address;
      if (event != 0) {
        EventSlotIsPrepared(event, frame, 0x825D5D04, call);
      }
      InstanceV(instance).set_state(3);
      return r29;
    }
    const uint32_t previous =
        (EventRecordV(record).word() & 0x20000000) != 0
            ? 0
            : Load(InstanceV(instance).events() +
                             (uint32_t(EventRecordV(record).previous_index()) << 2));
    const uint32_t out = frame + 80;
    Store(out, 0);
    const uint32_t engine = InstanceV(instance).engine();
    if (const auto made = EventCreate(engine, instance, record, previous, out, call)) {
      SetRegs(call, Args({engine, instance, record, previous, out}));
      r29 = *made;
    } else {
      r29 = CallAt(call, kEventCreate, 0x825D5B3C,
                   Args({engine, instance, record, previous, out}), frame);
    }
    if (int32_t(r29) < 0) {
      DeleteEvent(Load(out), frame, 0x825D5C44, call);
      break;
    }
    bool drop = false;
    const uint32_t event = Load(out);
    const uint32_t word = EventV(event).record().word();
    if ((word & 0x1F) == 9 &&
        int32_t(((word >> 5) & 0xFFFFFF) + EventV(event).fire_time()) < int32_t(offset)) {
      drop = true;
    } else {
      r29 = EventSlotPrepare(event, offset, frame, 0x825D5B94, call);
      if (int32_t(r29) >= 0) {
        const uint32_t prepared = Load(out);
        const int32_t type = int32_t(EventV(prepared).record().word() & 0x1F);
        if (type >= 1 && type <= 6) {
          InstanceV(instance).set_play_wave_event(prepared);
        }
        Store(InstanceV(instance).events() + (uint32_t(kept) << 2), prepared);
        kept = uint8_t(kept + 1);
      }
      if (r29 == kSeekBeyondEnd) {
        beyond = true;
        drop = true;
        r29 = 0;
      }
    }
    if (int32_t(r29) < 0) {
      DeleteEvent(Load(out), frame, 0x825D5C44, call);
      break;
    }
    record = SoundBankEvent(sound_bank, 28, iterator, frame, 0x825D5C18, call);
    if (drop) {
      DeleteEvent(Load(out), frame, 0x825D5C44, call);
    }
  }
  // A failure: every kept event deleted, last first, and the array freed.
  for (uint8_t i = kept; i != 0;) {
    i = uint8_t(i - 1);
    DeleteEvent(Load(InstanceV(instance).events() + (uint32_t(i) << 2)), frame,
                0x825D5C88, call);
  }
  const uint32_t held = InstanceV(instance).events();
  InstanceV(instance).set_event_count(0);
  if (held != 0) {
    SetRegs(call, Args({held, kEventArrayAttributes}));
    memory::crt::XMemFree(held, kEventArrayAttributes);
    InstanceV(instance).set_events(0);
  }
  InstanceV(instance).set_play_wave_event(0);
  return r29;
}

uint32_t TrackCreateInstance(uint32_t track, uint32_t key, uint32_t out,
                             uint32_t offset, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D02F8 XACTTrack::CreateInstance");
  SetRegs(call, Args({64, kTrackInstanceAttributes}));
  const uint32_t instance = memory::crt::XMemAlloc(64, kTrackInstanceAttributes);
  if (instance == 0) {
    return kOutOfMemory;
  }
  const uint32_t engine = TrackV(track).engine();
  SetRegs(call, Args({instance, engine, track, 0xFFFFFFFFFFFFFFFFull}));
  TrackInstanceConstruct(instance, engine, track);
  SetRegs(call, Args({instance, key, offset}));
  const uint32_t r30 = TrackInstancePrepare(instance, key, offset, xa::FrameBelow(frame, kTrackInstancePrepareFull), call);
  if (int32_t(r30) >= 0) {
    Store(out, instance);
    return r30;
  }
  CallAt(call, xa::Method(instance, 0), 0x825D0370, Args({instance, 1}), frame);
  return r30;
}

uint32_t TrackPrepare(uint32_t track, uint32_t key, uint32_t offset,
                      uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D0AA0 XACTTrack::Prepare");
  uint32_t r3;
  if (const auto rpcs = TrackCreateRpcs(track)) {
    SetRegs(call, Args({track}));
    r3 = *rpcs;
  } else {
    r3 = CallAt(call, 0x825D06B8, 0x825D0ABC, Args({track}), frame);
  }
  const uint32_t out = frame + 80;
  Store(out, 0);
  if (int32_t(r3) < 0) {
    return r3;
  }
  SetRegs(call, Args({track, key, out, offset}));
  const uint32_t r30 = TrackCreateInstance(track, key, out, offset,
                                           xa::FrameBelow(frame, kCreateInstanceFrame),
                                           call);
  if (int32_t(r30) < 0) {
    return r30;
  }
  TrackV(track).set_instance(Load(out));
  if (TrackStartRpcs(track)) {
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

uint32_t SoundScheduleEvent(uint32_t sound, uint32_t event, uint32_t frame,
                            const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825CB0C8 XACTSound::ScheduleEvent");
  const uint32_t queued = xa::Method(event, 16);
  uint32_t r3;
  if (queued == kPlayWaveSetQueued) {
    SetRegs(call, Args({event, 1}));
    PlayWaveSetQueued(event, 1);
    r3 = event;
  } else {
    r3 = CallAt(call, queued, 0x825CB0FC, Args({event, 1}), frame);
  }
  const int32_t time = int32_t(EventV(event).fire_time());
  if (time <= int32_t(Load(Load(sound + 32) + 24))) {
    const uint32_t fire = xa::Method(event, 4);
    if (fire == kEventFire) {
      SetRegs(call, Args({event}));
      return EventFire(event, xa::FrameBelow(frame, kEventFireFrame), call);
    }
    return CallAt(call, fire, 0x825CB124, Args({event}), frame);
  }
  const uint32_t list = sound + 56;
  uint32_t node = Load(sound + 60);
  while (node != list) {
    if (int32_t(EventV(node).fire_time()) > time) {
      break;
    }
    node = EventV(node).node_next();
  }
  const uint32_t before = Load(node + 8);
  EventV(event).set_node_prev(before);
  EventV(event).set_node_next(Load(before + 4));
  Store(Load(before + 4) + 8, event);
  Store(before + 4, event);
  return r3;
}

// --- XACT's clock: scheduling, finishing, firing and stopping events ---

namespace {

constexpr uint32_t kRandom = 0x825BF650;             // XACT's random 0 to r5
constexpr uint32_t kRemoveHelper = 0x825D5E08;       // never seen in play
constexpr uint32_t kHasReleaseRpc = 0x825D05C0;      // never seen in play
constexpr uint32_t kPlayWaveGetDuration = 0x825D8870;  // slot +12
constexpr uint32_t kScheduleEventFrame = 112;
constexpr uint32_t kGetDurationFrame = 112;
constexpr uint32_t kEventFireFrameBelow = 96;

// The record's time: bits 5-28 of its word +0 (the original's test for a
// sign bit above them can never pass).
uint32_t RecordTime(uint32_t word) { return (word >> 5) & 0xFFFFFF; }

}  // namespace

uint32_t EventSchedule(uint32_t event, bool native_heap, uint32_t frame,
                       const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D1918 XACTEvent::Schedule");
  const uint32_t record = EventV(event).record().address;
  const uint32_t word = EventRecordV(record).word();
  uint32_t fire;
  if ((word & 0x20000000) != 0) {
    const uint32_t start = (EventV(event).flags() & 0x08) != 0
                               ? ClockMs(0x825D1950, frame, call)
                               : EventV(event).instance().start_time();
    const uint32_t time = RecordTime(EventV(event).record().word());
    fire = time - EventV(event).elapsed() + start;
  } else {
    const uint32_t previous = EventV(event).previous().address;
    uint32_t from;
    if ((word & 0x40000000) != 0) {
      from = EventV(previous).fire_time();
    } else if ((EventV(previous).flags() & 0x40) != 0) {
      from = EventV(previous).done_time();
    } else {
      from = EventV(previous).end_time();
    }
    from -= EventV(event).elapsed();
    EventV(event).set_elapsed(0);
    EventV(event).set_fire_time(from);
    const uint32_t time = RecordTime(EventRecordV(record).word());
    fire = EventV(event).fire_time() + time;
  }
  EventV(event).set_elapsed(0);
  EventV(event).set_fire_time(fire);
  const uint32_t spread = EventV(event).record().random_spread();
  if (spread != 0) {
    const uint32_t extra =
        CallAt(call, kRandom, 0x825D1A20, Args({EventV(event).engine(), 0, spread}), frame);
    const uint32_t offset = EventV(event).elapsed();
    EventV(event).set_elapsed(0);
    EventV(event).set_fire_time(fire - offset + extra);
  }
  const uint32_t method = xa::Method(event, 12);
  uint32_t duration;
  if (method == kPlayWaveGetDuration) {
    SetRegs(call, Args({event}));
    duration = PlayWaveGetDuration(event, native_heap,
                                   xa::FrameBelow(frame, kGetDurationFrame), call);
  } else {
    duration = CallAt(call, method, 0x825D1A48, Args({event}), frame);
  }
  EventV(event)
      .set_end_time(duration == 0x7FFFFFFF ? 0x7FFFFFFF
                                           : EventV(event).fire_time() + duration);
  const uint32_t sound = EventV(event).instance().track().sound();
  SetRegs(call, Args({sound, event}));
  return SoundScheduleEvent(sound, event, xa::FrameBelow(frame, kScheduleEventFrame),
                            call);
}

uint32_t EventDone(uint32_t event, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D13F0 XACTEvent::Done");
  const uint8_t flags = EventV(event).flags();
  if ((flags & 0x08) != 0) {
    return CallAt(call, kRemoveHelper, 0x825D1420,
                  Args({EventV(event).instance().address, event}), frame);
  }
  EventV(event).set_flags(uint8_t(flags | 0x40));
  const uint32_t instance = EventV(event).instance().address;
  InstanceV(instance)
      .set_unfinished_count(uint8_t(InstanceV(instance).unfinished_count() + 255));
  const uint32_t now = ClockMs(0x825D1450, frame, call);
  const uint32_t owner = EventV(event).instance().address;
  EventV(event).set_done_time(now);
  SetRegs(call, Args({owner}));
  TrackInstanceCheckDone(owner);
  return owner;
}

uint64_t PlayWaveFire(uint32_t event, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D8378 XACTPlayWaveEvent::Fire");
  SetRegs(call, Args({event}));
  EventFire(event, xa::FrameBelow(frame, kEventFireFrameBelow), call);
  if (WaveOf(event, 0x825D83A4, frame, call) != 0) {
    const uint32_t wave = WaveOf(event, 0x825D83C0, frame, call);
    CallAt(call, xa::Method(wave, 8), 0x825D83DC, Args({wave, 1, 0, 0}), frame);
  }
  // r3 in full, as XACTGetTimeMs leaves it (the counter's low word
  // sign-extended).
  const uint64_t now = ClockMsFull(0x825D83E0, frame, call);
  PlayWaveV(event).set_play_time(uint32_t(now));
  return now;
}

uint32_t PlayWaveStop(uint32_t event, uint32_t immediate, uint32_t frame,
                      const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D83F8 XACTPlayWaveEvent::Stop");
  if (immediate == 0) {
    PlayWaveV(event).set_stop_time(ClockMs(0x825D8418, frame, call));
  }
  if (WaveOf(event, 0x825D8434, frame, call) == 0) {
    return 0;
  }
  const uint32_t release = EventV(event).record().wave_flags() & 1;
  uint32_t mode = 1;
  if (immediate != 0) {
    mode = 0;
  } else if (release == 0) {
    const uint32_t track = EventV(event).instance().track().address;
    if (CallAt(call, kHasReleaseRpc, 0x825D8468, Args({track}), frame) == 0) {
      mode = 0;
    }
  }
  const uint32_t wave = WaveOf(event, 0x825D8488, frame, call);
  return CallAt(call, xa::Method(wave, 12), 0x825D84A8,
                Args({wave, mode, release, 0, 0}), frame);
}

// --- Playing and the audio callback's updates ---

namespace {

constexpr uint32_t kAddPending = 0x825CB188;       // the sound's pending list; never seen in play
constexpr uint32_t kRpcCount = 0x825CB348;         // never seen in play
constexpr uint32_t kRpcAudioUpdate = 0x825C9558;   // never seen in play
constexpr uint32_t kSwitchInstance = 0x825D0B70;   // never seen in play
constexpr uint32_t kEventScheduleFrame = 112;
constexpr uint32_t kTrackInstancePlayFrame = 112;
constexpr uint32_t kTrackInstanceAudioUpdateFrame = 96;

}  // namespace

uint32_t TrackInstancePlay(uint32_t instance, bool native_heap, uint32_t frame,
                           const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D5980 XACTTrackInstance::Play");
  if (int32_t(InstanceV(instance).state()) != 3) {
    InstanceV(instance).set_flags(uint8_t(InstanceV(instance).flags() | 0x40));
    return 0;
  }
  const uint32_t now = ClockMs(0x825D59A8, frame, call);
  InstanceV(instance).set_start_time(now);
  InstanceV(instance).set_state(4);
  const uint8_t count = InstanceV(instance).event_count();
  InstanceV(instance).set_unfired_count(count);
  if (count == 0) {
    return 0;
  }
  uint32_t i = 0;
  do {
    const uint32_t event = Load(InstanceV(instance).events() + 4 * i);
    if ((EventV(event).record().word() & 0x20000000) != 0) {
      SetRegs(call, Args({event}));
      EventSchedule(event, native_heap, xa::FrameBelow(frame, kEventScheduleFrame),
                    call);
    } else {
      const uint32_t sound = InstanceV(instance).track().sound();
      CallAt(call, kAddPending, 0x825D5A04, Args({sound, event}), frame);
    }
    i = (i + 1) & 0xFF;
  } while (i < InstanceV(instance).event_count());
  return 0;
}

uint32_t TrackInstanceAudioUpdate(uint32_t instance, bool native_heap,
                                  uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D5EB0 XACTTrackInstance::AudioUpdate");
  uint32_t r3 = instance;
  if (int32_t(InstanceV(instance).state()) == 2) {
    const uint32_t event = InstanceV(instance).play_wave_event().address;
    r3 = event;
    if (event != 0) {
      r3 = EventSlotIsPrepared(event, frame, 0x825D5EEC, call);
      if (r3 != 0) {
        InstanceV(instance).set_state(3);
      }
    }
  }
  const uint8_t flags = InstanceV(instance).flags();
  if ((flags & 0x40) != 0 && int32_t(InstanceV(instance).state()) == 3) {
    InstanceV(instance).set_flags(uint8_t(flags & 0xBF));
    SetRegs(call, Args({instance}));
    r3 = TrackInstancePlay(instance, native_heap,
                           xa::FrameBelow(frame, kTrackInstancePlayFrame), call);
  }
  return r3;
}

uint32_t TrackAudioUpdate(uint32_t track, bool native_heap, uint32_t frame,
                          const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D0D88 XACTTrack::AudioUpdate");
  const uint32_t sound = TrackV(track).sound();
  if ((LoadU8(Load(sound + 28)) & 0x4) != 0) {
    const uint32_t count =
        CallAt(call, kRpcCount, 0x825D0DB8, Args({sound, TrackV(track).index()}), frame) &
        0xFF;
    for (uint32_t i = 0; i < count; i = (i + 1) & 0xFF) {
      CallAt(call, kRpcAudioUpdate, 0x825D0DD8, Args({TrackV(track).rpc_entries() + 44 * i}),
             frame);
    }
  }
  const uint32_t instance = TrackV(track).instance().address;
  SetRegs(call, Args({instance}));
  uint32_t r3 = TrackInstanceAudioUpdate(instance, native_heap, xa::FrameBelow(frame, kTrackInstanceAudioUpdateFrame),
      call);
  const uint8_t flags = TrackV(track).flags();
  if ((flags & 0x80) != 0) {
    const uint32_t next = TrackV(track).next_instance().address;
    if (next != 0 && int32_t(InstanceV(next).state()) == 3) {
      TrackV(track).set_flags(uint8_t(flags & 0x7F));
      r3 = CallAt(call, kSwitchInstance, 0x825D0E28, Args({track}), frame);
    }
  }
  return r3;
}

uint32_t TrackPlay(uint32_t track, bool native_heap, uint32_t sp,
                   const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D01D0 XACTTrack::Play");
  const uint32_t instance = TrackV(track).instance().address;
  SetRegs(call, Args({instance}));
  return TrackInstancePlay(instance, native_heap,
                           xa::FrameBelow(sp, kTrackInstancePlayFrame), call);
}

}  // namespace xact
