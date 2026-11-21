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

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint8_t LoadU8(uint8_t* base, uint32_t address) { return guest::LoadU8(base, address); }

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
    return PlayWaveV(base, event).wave();
  }
  return CallAt(call, method, return_address, Args({event}), frame);
}

uint32_t WaveSetVolumeFrom(uint8_t* base, uint32_t wave, double volume, uint32_t sp,
                           uint32_t return_address, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D1CC8 XACTWave::SetVolume");
  const uint32_t voice = WaveV(base, wave).voice().address;
  if (voice == 0) {
    return 0;
  }
  return CallAt(call, xa::Method(base, voice, 4), return_address, WithF1(Args({voice}), volume),
                sp);
}

}  // namespace

void TrackInstanceConstruct(uint8_t* base, uint32_t instance, uint32_t engine, uint32_t track) {
  ITC_NATIVE_RUNNING("sub_825D57D0 XACTTrackInstance::XACTTrackInstance");
  InstanceV(base, instance).set_node_next(instance);
  InstanceV(base, instance).set_node_prev(instance);
  InstanceV(base, instance).set_engine(engine);
  InstanceV(base, instance).set_track(track);
  InstanceV(base, instance).set_vtable(kTrackInstanceVtable);
  InstanceV(base, instance).set_events(0);
  InstanceV(base, instance).set_event_count(0);
  InstanceV(base, instance).set_start_time(0);
  InstanceV(base, instance).set_play_wave_event(0);
  InstanceV(base, instance).set_state(1);
  InstanceV(base, instance).set_unfired_count(0);
  InstanceV(base, instance).set_unfinished_count(0);
  InstanceV(base, instance).set_flags(0);
  const InstanceV inst(base, instance);
  const uint32_t list = inst.helpers_vtable_address();
  inst.set_helpers_vtable(kListBaseVtable);
  inst.set_helpers_next(list);
  inst.set_helpers_prev(list);
  InstanceV(base, instance).set_stopped_at_once(0);
}

void TrackInstanceCheckDone(uint8_t* base, uint32_t instance) {
  ITC_NATIVE_RUNNING("sub_825D55E0 XACTTrackInstance::CheckDone");
  if (InstanceV(base, instance).unfired_count() != 0 ||
      InstanceV(base, instance).unfinished_count() != 0) {
    return;
  }
  const InstanceV inst(base, instance);
  const uint32_t list = inst.helpers_vtable_address();
  if (inst.helpers_next() != list || inst.helpers_prev() != list) {
    return;
  }
  InstanceV(base, instance).set_state(6);
}

uint32_t TrackInstanceSetVolume(uint8_t* base, uint32_t instance, double volume, uint32_t sp,
                                const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D5678 XACTTrackInstance::SetVolume");
  const uint32_t event = InstanceV(base, instance).play_wave_event().address;
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
  const uint32_t event = InstanceV(base, instance).play_wave_event().address;
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
  const uint32_t event = InstanceV(base, instance).play_wave_event().address;
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
  const uint32_t instance = TrackV(base, track).instance().address;
  if (instance == 0) {
    return 0;
  }
  SetRegs(call, Args({instance}));
  return TrackInstancePause(base, instance, pause, sp, call);
}

uint32_t TrackIsBusy(uint8_t* base, uint32_t track, uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D0590 XACTTrack::IsBusy");
  const uint32_t instance = TrackV(base, track).instance().address;
  if (instance == 0) {
    return 0;
  }
  SetRegs(call, Args({instance}));
  return TrackInstanceIsBusy(base, instance, sp, call);
}

void EventConstruct(uint8_t* base, uint32_t event, uint32_t engine, uint32_t instance,
                    uint32_t record, uint32_t previous, uint32_t uncounted) {
  ITC_NATIVE_RUNNING("sub_825D1760 XACTEvent::XACTEvent");
  EventV(base, event).set_node_next(event);
  EventV(base, event).set_node_prev(event);
  EventV(base, event).set_engine(engine);
  EventV(base, event).set_record(record);
  EventV(base, event).set_previous(previous);
  EventV(base, event).set_instance(instance);
  EventV(base, event).set_vtable(kEventVtable);
  EventV(base, event).set_fire_time(0);
  EventV(base, event).set_done_time(0);
  EventV(base, event).set_elapsed(0);
  EventV(base, event).set_end_time(0);
  EventV(base, event).set_flags(uncounted != 0 ? 0x08 : 0);
}

void EventDestruct(uint8_t* base, uint32_t event) {
  ITC_NATIVE_RUNNING("sub_825D0EF0 XACTEvent::~XACTEvent");
  EventV(base, event).set_vtable(kListBaseVtable);
}

uint32_t EventFire(uint8_t* base, uint32_t event, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D1358 XACTEvent::Fire");
  uint8_t flags = EventV(base, event).flags();
  if ((flags & 0x80) == 0) {
    flags |= 0x80;
    EventV(base, event).set_flags(flags);
    if ((flags & 0x08) == 0) {
      const uint32_t instance = EventV(base, event).instance().address;
      InstanceV(base, instance)
          .set_unfinished_count(uint8_t(InstanceV(base, instance).unfinished_count() + 1));
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
  if ((EventV(base, event).flags() & 0x08) == 0) {
    const uint32_t instance = EventV(base, event).instance().address;
    InstanceV(base, instance)
        .set_unfired_count(uint8_t(InstanceV(base, instance).unfired_count() + 255));
  }
  return r3;
}

uint32_t PlayWaveConstruct(uint8_t* base, uint32_t event, uint32_t engine, uint32_t instance,
                           uint32_t record, uint32_t previous, uint32_t variation) {
  ITC_NATIVE_RUNNING("sub_825D82E8 XACTPlayWaveEvent::XACTPlayWaveEvent");
  EventConstruct(base, event, engine, instance, record, previous, 0);
  const uint32_t at = EventV(base, event).record().address;
  EventV(base, event).set_vtable(kPlayWaveEventVtable);
  PlayWaveV(base, event).set_slot36_flag(0);
  const uint8_t loops = EventRecordV(base, at).loop_count();
  PlayWaveV(base, event).set_variation(variation);
  PlayWaveV(base, event).set_wave(0);
  PlayWaveV(base, event).set_play_time(0);
  PlayWaveV(base, event).set_stop_time(0);
  PlayWaveV(base, event).set_loop_count(loops);
  return event;
}

void PlayWaveSetQueued(uint8_t* base, uint32_t event, uint32_t queued) {
  ITC_NATIVE_RUNNING("sub_825D8530 XACTPlayWaveEvent::SetQueued");
  uint8_t flags = EventV(base, event).flags();
  const uint32_t wave = PlayWaveV(base, event).wave();
  flags = uint8_t((flags & ~0x20) | (queued != 0 ? 0x20 : 0));
  EventV(base, event).set_flags(flags);
  const uint32_t prepared = WaveV(base, wave).prepared();
  flags = uint8_t((flags & ~0x40) | (prepared == 0 ? 0x40 : 0));
  EventV(base, event).set_flags(flags);
}

uint32_t PlayWaveGetLoopCount(uint8_t* base, uint32_t event) {
  ITC_NATIVE_RUNNING("sub_825D8358 XACTPlayWaveEvent::GetLoopCount");
  return PlayWaveV(base, event).loop_count();
}

void PlayWaveSetWave(uint8_t* base, uint32_t event, uint32_t wave) {
  ITC_NATIVE_RUNNING("sub_82483208 XACTPlayWaveEvent::SetWave");
  PlayWaveV(base, event).set_wave(wave);
}

void WaveVariationSetWave(uint8_t* base, uint32_t event, uint32_t wave) {
  ITC_NATIVE_RUNNING("sub_825D7B78 XACTWaveVariationEvent::SetWave");
  WaveVariationV(base, event).set_wave(wave);
}

uint32_t PlayWaveGetVariationRecord(uint8_t* base, uint32_t event) {
  ITC_NATIVE_RUNNING("sub_825D8920 XACTPlayWaveEvent::GetVariationRecord");
  const uint32_t record = EventV(base, event).record().address;
  const int32_t type = int32_t(EventRecordV(base, record).word() & 0x1F);
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
  return WaveV(base, WaveOf(base, event, 0x825D8500, frame, call)).wave_bank();
}

uint32_t PlayWaveIsPrepared(uint8_t* base, uint32_t event, uint32_t frame,
                            const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D76A0 XACTPlayWaveEvent::IsPrepared");
  if (WaveOf(base, event, 0x825D76C4, frame, call) == 0) {
    return 0;
  }
  const uint32_t wave = WaveOf(base, event, 0x825D76E0, frame, call);
  return int32_t(WaveV(base, wave).state()) >= 2 ? 1 : 0;
}

uint32_t PlayWaveIsPlaying(uint8_t* base, uint32_t event, uint32_t frame,
                           const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D7708 XACTPlayWaveEvent::IsPlaying");
  if (WaveOf(base, event, 0x825D772C, frame, call) == 0) {
    return 0;
  }
  if (int32_t(WaveV(base, WaveOf(base, event, 0x825D7748, frame, call)).state()) < 3) {
    return 0;
  }
  return int32_t(WaveV(base, WaveOf(base, event, 0x825D7768, frame, call)).state()) > 5 ? 0 : 1;
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
  const double f31 = double(TrackV(base, track).volume());
  const uint32_t sound = TrackV(base, track).sound();
  const double f30 = double(TrackV(base, track).rpc_volume());
  const auto volume = SoundGetVolume(base, sound);
  if (!volume) {
    return std::nullopt;
  }
  const double f0 = track_first ? Single(f31 * volume->f1) : Single(volume->f1 * f31);
  const double f1 = Single(f0 * f30);
  const uint32_t instance = TrackV(base, track).instance().address;
  SetRegs(call, WithF1(Args({instance}), f1));
  return TrackInstanceSetVolume(base, instance, f1, frame, call);
}

}  // namespace

std::optional<uint32_t> TrackSetVolume(uint8_t* base, uint32_t track, double volume,
                                       uint32_t frame, const GuestCall& call) {
  const double zero = double(G::zero(base));
  TrackV(base, track).set_volume(float(volume < zero ? zero : volume));
  if (TrackV(base, track).instance().address == 0) {
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
  if (TrackV(base, track).instance().address == 0) {
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
  const float zero = G::zero(base);
  TrackV(base, track).set_pitch(zero);
  TrackV(base, track).set_node_next(track);
  TrackV(base, track).set_rpc_pitch(zero);
  TrackV(base, track).set_node_prev(track);
  TrackV(base, track).set_engine(engine);
  TrackV(base, track).set_record(record);
  TrackV(base, track).set_sound(sound);
  TrackV(base, track).set_vtable(kTrackVtable);
  double f1 = double(G::one(base));
  TrackV(base, track).set_rpc_volume(float(f1));
  TrackV(base, track).set_index(uint8_t(index));
  TrackV(base, track).set_instance(0);
  TrackV(base, track).set_next_instance(0);
  TrackV(base, track).set_pending_transition(0);
  TrackV(base, track).set_rpc_entries(0);
  TrackV(base, track).set_flags(0);
  if (record != 0) {
    f1 =
        LoadF(base, G::gain_table_address() + (uint32_t(TrackRecordV(base, record).volume()) << 2));
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
  const uint32_t event = TrackV(base, track).pending_transition();
  if (event != 0) {
    CallAt(call, 0x825D0B08, 0x825D0E54, Args({track, event}), frame);
    TrackV(base, track).set_pending_transition(0);
  }
  // The empty sub_82106540 with the instance: nothing to do.
  const uint32_t instance = TrackV(base, track).instance().address;
  SetRegs(call, Args({instance}));
  return instance;
}

std::optional<uint32_t> TrackCreateRpcs(uint8_t* base, uint32_t track) {
  const uint32_t sound = TrackV(base, track).sound();
  if ((LoadU8(base, Load(base, sound + 28)) & 0x4) != 0) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825D06B8 XACTTrack::CreateRpcs");
  return 0;
}

std::optional<uint32_t> TrackStartRpcs(uint8_t* base, uint32_t track) {
  const uint32_t sound = TrackV(base, track).sound();
  if ((LoadU8(base, Load(base, sound + 28)) & 0x4) != 0) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825D0800 XACTTrack::StartRpcs");
  return sound;
}

uint32_t TrackDestruct(uint8_t* base, uint32_t track, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D0110 XACTTrack::~XACTTrack");
  TrackV(base, track).set_vtable(kTrackVtable);
  uint32_t r3 = TrackV(base, track).next_instance().address;
  if (r3 != 0) {
    r3 = CallAt(call, xa::Method(base, r3, 0), 0x825D0150, Args({r3, 1}), frame);
  }
  r3 = TrackV(base, track).instance().address;
  if (r3 != 0) {
    r3 = CallAt(call, xa::Method(base, r3, 0), 0x825D0170, Args({r3, 1}), frame);
  }
  const uint32_t entries = TrackV(base, track).rpc_entries();
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
  TrackV(base, track).set_vtable(kListBaseVtable);
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
  const uint32_t wave = PlayWaveV(base, event).wave();
  EventV(base, event).set_vtable(kPlayWaveEventVtable);
  if (wave != 0) {
    CallAt(call, xa::Method(base, wave, 0), 0x825D86A8, Args({wave, 1}), frame);
  }
  EventV(base, event).set_vtable(kWaveEventBaseVtable);
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
      Load(base, EventV(base, event).instance().track().sound() + 48);
  const uint32_t sound_bank = Load(base, cue + 296);
  const uint32_t index = EventV(base, event).record().wave_bank_index();
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
  const uint32_t record = EventV(base, event).record().address;
  if (EventRecordV(base, record).loop_count() == 255) {
    return 0x7FFFFFFF;
  }
  const uint32_t wave = EventRecordV(base, record).wave_index();
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
  const uint32_t loops = EventV(base, event).record().loop_count();
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
  const uint32_t record = EventV(base, event).record().address;
  const uint32_t time = (EventRecordV(base, record).word() >> 5) & 0xFFFFFF;
  uint32_t seek = 0;
  if (int32_t(time) > int32_t(offset)) {
    EventV(base, event).set_elapsed(offset);
  } else {
    seek = offset - time;
    EventV(base, event).set_elapsed(time);
  }
  const uint32_t cue =
      Load(base, EventV(base, event).instance().track().sound() + 48);
  const uint32_t sound_bank = Load(base, cue + 296);
  const uint32_t index = EventRecordV(base, record).wave_bank_index();
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
               Args({bank, EventV(base, event).record().wave_index(), event, frame + 80}),
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
      WaveV(base, linked).set_node_next(list);
      WaveV(base, linked).set_node_previous(Load(base, list + 8));
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
  InstanceV(base, instance).set_state(2);
  InstanceV(base, instance).set_event_count(1);
  SetRegs(call, Args({4, kEventArrayAttributes}));
  const uint32_t array = memory::crt::XMemAlloc(base, 4, kEventArrayAttributes);
  InstanceV(base, instance).set_events(array);
  if (array != 0) {
    SetRegs(call, Args({72, kPlayWaveAttributes}));
    uint32_t event = memory::crt::XMemAlloc(base, 72, kPlayWaveAttributes);
    if (event != 0) {
      const uint32_t engine = InstanceV(base, instance).engine();
      SetRegs(call, Args({event, engine, instance, record, 0}));
      PlayWaveConstruct(base, event, engine, instance, record, 0, 0);
    }
    Store(base, InstanceV(base, instance).events(), event);
    event = Load(base, InstanceV(base, instance).events());
    if (event != 0) {
      InstanceV(base, instance).set_play_wave_event(event);
      const uint32_t r30 = EventSlotPrepare(base, event, offset, frame, 0x825D5490, call);
      if (int32_t(r30) < 0) {
        return r30;
      }
      const uint32_t prepared = EventSlotIsPrepared(
          base, InstanceV(base, instance).play_wave_event().address, frame, 0x825D54B0, call);
      if (int32_t(prepared) != 0) {
        InstanceV(base, instance).set_state(3);
      }
      return r30;
    }
  }
  // Out of memory.
  const uint32_t held = InstanceV(base, instance).events();
  InstanceV(base, instance).set_event_count(0);
  if (held == 0) {
    return kOutOfMemory;
  }
  const uint32_t event = Load(base, held);
  if (event != 0) {
    CallAt(call, xa::Method(base, event, 0), 0x825D5500, Args({event, 1}), frame);
  }
  const uint32_t now = InstanceV(base, instance).events();
  SetRegs(call, Args({now, kEventArrayAttributes}));
  memory::crt::XMemFree(now, kEventArrayAttributes);
  InstanceV(base, instance).set_events(0);
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
    const uint32_t engine = TrackV(base, track).engine();
    SetRegs(call, Args({instance, engine, track, 0xFFFFFFFFFFFFFFFFull}));
    TrackInstanceConstruct(base, instance, engine, track);
  }
  TrackV(base, track).set_instance(instance);
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
  instance = TrackV(base, track).instance().address;
  if (instance != 0) {
    CallAt(call, xa::Method(base, instance, 0), 0x825D0A54, Args({instance, 1}), frame);
  }
  TrackV(base, track).set_instance(0);
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
  return InstanceV(base, instance).track().sound();
}

}  // namespace

uint32_t TrackInstanceCancelEvents(uint8_t* base, uint32_t instance, uint32_t frame,
                                   const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D5520 XACTTrackInstance::CancelEvents");
  uint32_t count = 0;
  CallAt(call, xa::kRtlEnterCriticalSection, 0x825D5540,
         Args({uint64_t(InstanceV(base, instance).engine() + 144)}), frame);
  for (uint32_t i = 0; i < InstanceV(base, instance).event_count(); i = (i + 1) & 0xFF) {
    const uint32_t event = Load(base, InstanceV(base, instance).events() + 4 * i);
    if ((EventV(base, event).flags() & 0x20) == 0) {
      continue;
    }
    CallAt(call, kSoundCancel, 0x825D557C, Args({SoundOf(base, instance), event}), frame);
    count++;
  }
  const InstanceV inst(base, instance);
  const uint32_t list = inst.helpers_vtable_address();
  while (inst.helpers_next() != list && inst.helpers_prev() != list) {
    const uint32_t helper = Load(base, InstanceV(base, instance).helpers_next() + 12);
    CallAt(call, kSoundCancel, 0x825D55C4, Args({SoundOf(base, instance), helper}), frame);
  }
  CallAt(call, xa::kRtlLeaveCriticalSection, 0x825D55D4,
         Args({uint64_t(InstanceV(base, instance).engine() + 144)}), frame);
  return count;
}

uint32_t TrackInstanceDestruct(uint8_t* base, uint32_t instance, uint32_t frame,
                               const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D5838 XACTTrackInstance::~XACTTrackInstance");
  InstanceV(base, instance).set_vtable(kTrackInstanceVtable);
  uint32_t r3 = instance;
  const uint32_t next = InstanceV(base, instance).node_next();
  if (next != instance) {
    const uint32_t prev = InstanceV(base, instance).node_prev();
    if (prev != instance) {
      Store(base, next + 8, prev);
      Store(base, InstanceV(base, instance).node_prev() + 4, InstanceV(base, instance).node_next());
      InstanceV(base, instance).set_node_next(instance);
      InstanceV(base, instance).set_node_prev(instance);
    }
  }
  if (InstanceV(base, instance).events() != 0) {
    SetRegs(call, Args({instance}));
    r3 = TrackInstanceCancelEvents(base, instance, xa::FrameBelow(base, frame, kCancelFrame),
                                   call);
    for (uint32_t i = 0; i < InstanceV(base, instance).event_count(); i = (i + 1) & 0xFF) {
      r3 = Load(base, InstanceV(base, instance).events() + 4 * i);
      if (r3 != 0) {
        r3 = CallAt(call, xa::Method(base, r3, 0), 0x825D58D8, Args({r3, 1}), frame);
      }
    }
    const uint32_t events = InstanceV(base, instance).events();
    SetRegs(call, Args({events, kEventArrayAttributes}));
    r3 = memory::crt::XMemFree(events, kEventArrayAttributes);
  }
  const InstanceV inst(base, instance);
  const uint32_t list = inst.helpers_vtable_address();
  while (inst.helpers_next() != list && inst.helpers_prev() != list) {
    r3 = InstanceV(base, instance).helpers_next();
    if (r3 != 0) {
      r3 = CallAt(call, xa::Method(base, r3, 0), 0x825D5938, Args({r3, 1}), frame);
    }
  }
  const int32_t state = int32_t(InstanceV(base, instance).state());
  if (state == 4 || state == 5) {
    InstanceV(base, instance).set_state(6);
  }
  inst.set_helpers_vtable(kListBaseVtable);
  InstanceV(base, instance).set_vtable(kListBaseVtable);
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
  const int32_t state = int32_t(InstanceV(base, instance).state());
  if (state == 6) {
    return 0;
  }
  if (state == 5 &&
      (int32_t(InstanceV(base, instance).stopped_at_once()) != 0 || int32_t(immediate) == 0)) {
    return 0;
  }
  InstanceV(base, instance).set_state(5);
  uint32_t cancelled = 0;
  if (int32_t(immediate) != 0) {
    InstanceV(base, instance).set_stopped_at_once(1);
    SetRegs(call, Args({instance}));
    cancelled = TrackInstanceCancelEvents(base, instance,
                                          xa::FrameBelow(base, frame, kCancelFrame), call);
  }
  const uint32_t event = InstanceV(base, instance).play_wave_event().address;
  if (event != 0 && int32_t(EventSlotIsPlaying(base, event, frame, 0x825D5D98, call)) != 0) {
    const uint32_t now = InstanceV(base, instance).play_wave_event().address;
    CallAt(call, xa::Method(base, now, 60), 0x825D5DB8, Args({now, immediate}), frame);
  }
  if (int32_t(immediate) != 0 && int32_t(cancelled) == 0) {
    InstanceV(base, instance).set_state(6);
  }
  return 0;
}

uint32_t TrackStop(uint8_t* base, uint32_t track, uint32_t immediate, uint32_t frame,
                   const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D01D8 XACTTrack::Stop");
  uint32_t r30 = 0;
  const uint32_t instance = TrackV(base, track).instance().address;
  if (instance != 0) {
    uint32_t now = immediate;
    if (int32_t(now) == 0) {
      const uint32_t event = InstanceV(base, instance).play_wave_event().address;
      if (event == 0 || int32_t(EventSlotIsPlaying(base, event, frame, 0x825D0220, call)) == 0) {
        now = 1;
      }
    }
    const uint32_t stopped = TrackV(base, track).instance().address;
    SetRegs(call, Args({stopped, now}));
    r30 = TrackInstanceStop(base, stopped, now,
                            xa::FrameBelow(base, frame, kInstanceStopFrame), call);
  }
  const uint32_t next = TrackV(base, track).next_instance().address;
  if (next != 0) {
    CallAt(call, 0x825C1548, 0x825D0250, Args({TrackV(base, track).engine(), next}), frame);
    TrackV(base, track).set_next_instance(0);
  }
  return r30;
}

// --- Making events ---

std::optional<uint32_t> EventCreate(uint8_t* base, uint32_t engine, uint32_t instance,
                                    uint32_t record, uint32_t previous, uint32_t out,
                                    const GuestCall& call) {
  const uint32_t type = EventRecordV(base, record).word() & 0x1F;
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
      const uint32_t variation =
          (type == 2 || type == 5) ? EventRecordV(base, record).variation() : 0;
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
  const uint32_t cue = Load(base, InstanceV(base, instance).track().sound() + 48);
  const uint32_t sound_bank = Load(base, cue + 296);
  const uint32_t iterator = frame + 88;
  Store(base, iterator + 0, key);
  Store(base, iterator + 4, 0);
  Store(base, iterator + 8, 0);
  InstanceV(base, instance).set_state(2);
  uint32_t record = SoundBankEvent(base, sound_bank, 24, iterator, frame, 0x825D5AA8, call);
  const uint8_t count = LoadU8(base, iterator + 8);
  InstanceV(base, instance).set_event_count(count);
  SetRegs(call, Args({uint32_t(count) << 2, kEventArrayAttributes}));
  const uint32_t array = memory::crt::XMemAlloc(base, uint32_t(count) << 2, kEventArrayAttributes);
  InstanceV(base, instance).set_events(array);
  uint32_t r29 = array != 0 ? 0 : kOutOfMemory;
  uint8_t kept = 0;
  bool beyond = false;
  while (int32_t(r29) >= 0) {
    if (record == 0) {
      if (beyond && kept == 0) {
        r29 = kSeekBeyondEnd;
        break;
      }
      InstanceV(base, instance).set_event_count(kept);
      const uint32_t event = InstanceV(base, instance).play_wave_event().address;
      if (event != 0) {
        EventSlotIsPrepared(base, event, frame, 0x825D5D04, call);
      }
      InstanceV(base, instance).set_state(3);
      return r29;
    }
    const uint32_t previous =
        (EventRecordV(base, record).word() & 0x20000000) != 0
            ? 0
            : Load(base, InstanceV(base, instance).events() +
                             (uint32_t(EventRecordV(base, record).previous_index()) << 2));
    const uint32_t out = frame + 80;
    Store(base, out, 0);
    const uint32_t engine = InstanceV(base, instance).engine();
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
    const uint32_t word = EventV(base, event).record().word();
    if ((word & 0x1F) == 9 &&
        int32_t(((word >> 5) & 0xFFFFFF) + EventV(base, event).fire_time()) < int32_t(offset)) {
      drop = true;
    } else {
      r29 = EventSlotPrepare(base, event, offset, frame, 0x825D5B94, call);
      if (int32_t(r29) >= 0) {
        const uint32_t prepared = Load(base, out);
        const int32_t type = int32_t(EventV(base, prepared).record().word() & 0x1F);
        if (type >= 1 && type <= 6) {
          InstanceV(base, instance).set_play_wave_event(prepared);
        }
        Store(base, InstanceV(base, instance).events() + (uint32_t(kept) << 2), prepared);
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
    DeleteEvent(base, Load(base, InstanceV(base, instance).events() + (uint32_t(i) << 2)), frame,
                0x825D5C88, call);
  }
  const uint32_t held = InstanceV(base, instance).events();
  InstanceV(base, instance).set_event_count(0);
  if (held != 0) {
    SetRegs(call, Args({held, kEventArrayAttributes}));
    memory::crt::XMemFree(held, kEventArrayAttributes);
    InstanceV(base, instance).set_events(0);
  }
  InstanceV(base, instance).set_play_wave_event(0);
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
  const uint32_t engine = TrackV(base, track).engine();
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
  TrackV(base, track).set_instance(Load(base, out));
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
  const int32_t time = int32_t(EventV(base, event).fire_time());
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
    if (int32_t(EventV(base, node).fire_time()) > time) {
      break;
    }
    node = EventV(base, node).node_next();
  }
  const uint32_t before = Load(base, node + 8);
  EventV(base, event).set_node_prev(before);
  EventV(base, event).set_node_next(Load(base, before + 4));
  Store(base, Load(base, before + 4) + 8, event);
  Store(base, before + 4, event);
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

uint32_t EventSchedule(uint8_t* base, uint32_t event, bool native_heap, uint32_t frame,
                       const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D1918 XACTEvent::Schedule");
  const uint32_t record = EventV(base, event).record().address;
  const uint32_t word = EventRecordV(base, record).word();
  uint32_t fire;
  if ((word & 0x20000000) != 0) {
    const uint32_t start = (EventV(base, event).flags() & 0x08) != 0
                               ? ClockMs(base, 0x825D1950, frame, call)
                               : EventV(base, event).instance().start_time();
    const uint32_t time = RecordTime(EventV(base, event).record().word());
    fire = time - EventV(base, event).elapsed() + start;
  } else {
    const uint32_t previous = EventV(base, event).previous().address;
    uint32_t from;
    if ((word & 0x40000000) != 0) {
      from = EventV(base, previous).fire_time();
    } else if ((EventV(base, previous).flags() & 0x40) != 0) {
      from = EventV(base, previous).done_time();
    } else {
      from = EventV(base, previous).end_time();
    }
    from -= EventV(base, event).elapsed();
    EventV(base, event).set_elapsed(0);
    EventV(base, event).set_fire_time(from);
    const uint32_t time = RecordTime(EventRecordV(base, record).word());
    fire = EventV(base, event).fire_time() + time;
  }
  EventV(base, event).set_elapsed(0);
  EventV(base, event).set_fire_time(fire);
  const uint32_t spread = EventV(base, event).record().random_spread();
  if (spread != 0) {
    const uint32_t extra =
        CallAt(call, kRandom, 0x825D1A20, Args({EventV(base, event).engine(), 0, spread}), frame);
    const uint32_t offset = EventV(base, event).elapsed();
    EventV(base, event).set_elapsed(0);
    EventV(base, event).set_fire_time(fire - offset + extra);
  }
  const uint32_t method = xa::Method(base, event, 12);
  uint32_t duration;
  if (method == kPlayWaveGetDuration) {
    SetRegs(call, Args({event}));
    duration = PlayWaveGetDuration(base, event, native_heap,
                                   xa::FrameBelow(base, frame, kGetDurationFrame), call);
  } else {
    duration = CallAt(call, method, 0x825D1A48, Args({event}), frame);
  }
  EventV(base, event)
      .set_end_time(duration == 0x7FFFFFFF ? 0x7FFFFFFF
                                           : EventV(base, event).fire_time() + duration);
  const uint32_t sound = EventV(base, event).instance().track().sound();
  SetRegs(call, Args({sound, event}));
  return SoundScheduleEvent(base, sound, event, xa::FrameBelow(base, frame, kScheduleEventFrame),
                            call);
}

uint32_t EventDone(uint8_t* base, uint32_t event, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D13F0 XACTEvent::Done");
  const uint8_t flags = EventV(base, event).flags();
  if ((flags & 0x08) != 0) {
    return CallAt(call, kRemoveHelper, 0x825D1420,
                  Args({EventV(base, event).instance().address, event}), frame);
  }
  EventV(base, event).set_flags(uint8_t(flags | 0x40));
  const uint32_t instance = EventV(base, event).instance().address;
  InstanceV(base, instance)
      .set_unfinished_count(uint8_t(InstanceV(base, instance).unfinished_count() + 255));
  const uint32_t now = ClockMs(base, 0x825D1450, frame, call);
  const uint32_t owner = EventV(base, event).instance().address;
  EventV(base, event).set_done_time(now);
  SetRegs(call, Args({owner}));
  TrackInstanceCheckDone(base, owner);
  return owner;
}

uint64_t PlayWaveFire(uint8_t* base, uint32_t event, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D8378 XACTPlayWaveEvent::Fire");
  SetRegs(call, Args({event}));
  EventFire(base, event, xa::FrameBelow(base, frame, kEventFireFrameBelow), call);
  if (WaveOf(base, event, 0x825D83A4, frame, call) != 0) {
    const uint32_t wave = WaveOf(base, event, 0x825D83C0, frame, call);
    CallAt(call, xa::Method(base, wave, 8), 0x825D83DC, Args({wave, 1, 0, 0}), frame);
  }
  // r3 in full, as XACTGetTimeMs leaves it (the counter's low word
  // sign-extended).
  const uint64_t now = ClockMsFull(base, 0x825D83E0, frame, call);
  PlayWaveV(base, event).set_play_time(uint32_t(now));
  return now;
}

uint32_t PlayWaveStop(uint8_t* base, uint32_t event, uint32_t immediate, uint32_t frame,
                      const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D83F8 XACTPlayWaveEvent::Stop");
  if (immediate == 0) {
    PlayWaveV(base, event).set_stop_time(ClockMs(base, 0x825D8418, frame, call));
  }
  if (WaveOf(base, event, 0x825D8434, frame, call) == 0) {
    return 0;
  }
  const uint32_t release = EventV(base, event).record().wave_flags() & 1;
  uint32_t mode = 1;
  if (immediate != 0) {
    mode = 0;
  } else if (release == 0) {
    const uint32_t track = EventV(base, event).instance().track().address;
    if (CallAt(call, kHasReleaseRpc, 0x825D8468, Args({track}), frame) == 0) {
      mode = 0;
    }
  }
  const uint32_t wave = WaveOf(base, event, 0x825D8488, frame, call);
  return CallAt(call, xa::Method(base, wave, 12), 0x825D84A8,
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

uint32_t TrackInstancePlay(uint8_t* base, uint32_t instance, bool native_heap, uint32_t frame,
                           const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D5980 XACTTrackInstance::Play");
  if (int32_t(InstanceV(base, instance).state()) != 3) {
    InstanceV(base, instance).set_flags(uint8_t(InstanceV(base, instance).flags() | 0x40));
    return 0;
  }
  const uint32_t now = ClockMs(base, 0x825D59A8, frame, call);
  InstanceV(base, instance).set_start_time(now);
  InstanceV(base, instance).set_state(4);
  const uint8_t count = InstanceV(base, instance).event_count();
  InstanceV(base, instance).set_unfired_count(count);
  if (count == 0) {
    return 0;
  }
  uint32_t i = 0;
  do {
    const uint32_t event = Load(base, InstanceV(base, instance).events() + 4 * i);
    if ((EventV(base, event).record().word() & 0x20000000) != 0) {
      SetRegs(call, Args({event}));
      EventSchedule(base, event, native_heap, xa::FrameBelow(base, frame, kEventScheduleFrame),
                    call);
    } else {
      const uint32_t sound = InstanceV(base, instance).track().sound();
      CallAt(call, kAddPending, 0x825D5A04, Args({sound, event}), frame);
    }
    i = (i + 1) & 0xFF;
  } while (i < InstanceV(base, instance).event_count());
  return 0;
}

uint32_t TrackInstanceAudioUpdate(uint8_t* base, uint32_t instance, bool native_heap,
                                  uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D5EB0 XACTTrackInstance::AudioUpdate");
  uint32_t r3 = instance;
  if (int32_t(InstanceV(base, instance).state()) == 2) {
    const uint32_t event = InstanceV(base, instance).play_wave_event().address;
    r3 = event;
    if (event != 0) {
      r3 = EventSlotIsPrepared(base, event, frame, 0x825D5EEC, call);
      if (r3 != 0) {
        InstanceV(base, instance).set_state(3);
      }
    }
  }
  const uint8_t flags = InstanceV(base, instance).flags();
  if ((flags & 0x40) != 0 && int32_t(InstanceV(base, instance).state()) == 3) {
    InstanceV(base, instance).set_flags(uint8_t(flags & 0xBF));
    SetRegs(call, Args({instance}));
    r3 = TrackInstancePlay(base, instance, native_heap,
                           xa::FrameBelow(base, frame, kTrackInstancePlayFrame), call);
  }
  return r3;
}

uint32_t TrackAudioUpdate(uint8_t* base, uint32_t track, bool native_heap, uint32_t frame,
                          const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D0D88 XACTTrack::AudioUpdate");
  const uint32_t sound = TrackV(base, track).sound();
  if ((LoadU8(base, Load(base, sound + 28)) & 0x4) != 0) {
    const uint32_t count =
        CallAt(call, kRpcCount, 0x825D0DB8, Args({sound, TrackV(base, track).index()}), frame) &
        0xFF;
    for (uint32_t i = 0; i < count; i = (i + 1) & 0xFF) {
      CallAt(call, kRpcAudioUpdate, 0x825D0DD8, Args({TrackV(base, track).rpc_entries() + 44 * i}),
             frame);
    }
  }
  const uint32_t instance = TrackV(base, track).instance().address;
  SetRegs(call, Args({instance}));
  uint32_t r3 = TrackInstanceAudioUpdate(
      base, instance, native_heap, xa::FrameBelow(base, frame, kTrackInstanceAudioUpdateFrame),
      call);
  const uint8_t flags = TrackV(base, track).flags();
  if ((flags & 0x80) != 0) {
    const uint32_t next = TrackV(base, track).next_instance().address;
    if (next != 0 && int32_t(InstanceV(base, next).state()) == 3) {
      TrackV(base, track).set_flags(uint8_t(flags & 0x7F));
      r3 = CallAt(call, kSwitchInstance, 0x825D0E28, Args({track}), frame);
    }
  }
  return r3;
}

uint32_t TrackPlay(uint8_t* base, uint32_t track, bool native_heap, uint32_t sp,
                   const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D01D0 XACTTrack::Play");
  const uint32_t instance = TrackV(base, track).instance().address;
  SetRegs(call, Args({instance}));
  return TrackInstancePlay(base, instance, native_heap,
                           xa::FrameBelow(base, sp, kTrackInstancePlayFrame), call);
}

}  // namespace xact
