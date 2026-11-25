// XACT's waves natively (waves.h, docs/functions/xact-waves.md): the
// waves' set-up and their leaves. The objects still recompiled (the owner's
// vtable, the renderer's slots, the wave bank's when they aren't the native
// functions) are called through the guest call; XACT's lock (the engine's
// critical section at +144) is the game's, entered and left through its
// imports as the original does. Strict floating point (CMakeLists.txt), as
// the original's single-precision steps.

#include "xact/waves.h"

#include "xact/tracks.h"

#include "base/log.h"
#include "graphics/video_output.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "schema/xact_tracks.h"
#include "schema/xact_waves.h"

#include <initializer_list>

namespace xact {

namespace {

namespace xa = xaudio;
using Wave = schema::XACTWave;
using Track = schema::XACTTrack;
using Event = schema::XACTEvent;  // the wave's owner (the play-wave event)

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}
// A single-precision step's result, as the register holds it.
double Single(double value) { return double(float(value)); }

xa::Regs Args(std::initializer_list<uint64_t> r) {
  xa::Regs regs;
  for (const uint64_t value : r) {
    regs.r[regs.count++] = value;
  }
  return regs;
}
void SetRegs(const GuestCall& call, const xa::Regs& regs) { call(0, 0, regs); }

constexpr uint32_t kWaveFrame = 112;  // XACTWave::XACTWave
constexpr uint32_t kPastDuration = 0x8AC70019u;

}  // namespace

uint32_t WaveConstruct(uint32_t wave, uint32_t engine, uint32_t bank,
                       uint32_t index, uint32_t owner, bool native_heap, uint32_t frame,
                       const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D23F0 XACTWave::XACTWave");
  const Wave w(wave);
  w.set_node_next(wave);
  w.set_node_previous(wave);
  w.set_listener_vtable(kListenerBaseVtable);
  const uint8_t flags = w.flags();
  w.set_engine(engine);
  w.set_vtable(kWaveVtable);
  w.set_wave_bank(bank);
  w.set_owner(owner);
  w.set_listener_vtable(kWaveListenerVtable);
  w.set_stopped_at_once(0);
  w.set_loop_count(0);
  w.set_request_owner(0);
  w.set_request_stop(0);
  w.set_state(0);
  w.set_voice(0);
  w.set_next_wave(0);
  w.set_prepared(0);
  w.set_flags(flags & 0x20);
  w.set_wave_index(uint16_t(index));
  // AddListener on the bank's +4 interface.
  const uint32_t iface = bank + 4;
  const uint32_t add = xa::Method(iface, 0);
  if (add == kWaveBankAddListener && native_heap) {
    SetRegs(call, Args({iface, w.listener_vtable_address()}));
    WaveBankAddListener(iface, w.listener_vtable_address());
  } else {
    xa::Regs regs = Args({iface, w.listener_vtable_address()});
    regs.sp = frame;
    call(add, 0x825D2490, regs);
  }
  return wave;
}

uint32_t InMemoryWaveConstruct(uint32_t wave, uint32_t engine, uint32_t bank,
                               uint32_t index, uint32_t owner, bool native_heap, uint32_t frame,
                               const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D37A8 XACTInMemoryWave::XACTInMemoryWave");
  const Wave w(wave);
  SetRegs(call, Args({wave, engine, bank, index, owner}));
  WaveConstruct(wave, engine, bank, index, owner, native_heap,
                xa::FrameBelow(frame, kWaveFrame), call);
  w.set_vtable(kInMemoryWaveVtable);
  w.set_listener_vtable(kWaveListenerVtable);
  w.set_packet_context(0);
  w.set_unknown_68(0);
  w.set_packet_context_looped(0);
  w.set_unknown_76(0);
  w.set_loop_context(0);
  w.set_unknown_84(0);
  w.set_loop_context_looped(0);
  w.set_unknown_92(0);
  w.set_packet_context(wave);
  w.set_loop_context(wave);
  return wave;
}

std::optional<CategoryVolume> TrackGetVolume(uint32_t track) {
  ITC_NATIVE_RUNNING("sub_825D0268 XACTTrack::GetVolume");
  const Track t(track);
  const double f31 = double(t.volume());
  const double f30 = double(t.rpc_volume());
  const auto volume = SoundGetVolume(t.sound());
  if (!volume) {
    return std::nullopt;
  }
  const double f0 = Single(f30 * f31);
  return CategoryVolume{Single(f0 * volume->f1), volume->r3};
}

CategoryVolume TrackGetPitch(uint32_t track) {
  ITC_NATIVE_RUNNING("sub_825D02B0 XACTTrack::GetPitch");
  const Track t(track);
  const double f31 = double(t.pitch());
  const uint32_t sound = t.sound();
  const double f30 = double(t.rpc_pitch());
  const double pitch = SoundGetPitch(sound);
  const double f0 = Single(f30 + f31);
  return CategoryVolume{Single(f0 + pitch), sound};
}

std::optional<uint32_t> WaveGetStartPosition(uint32_t wave, uint32_t entry,
                                             uint32_t ms, uint32_t start, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_825D2170 XACTWave::GetStartPosition");
  const Wave w(wave);
  uint32_t position = WaveEntryMsToSamples(entry, ms);
  const uint32_t duration = WaveEntryGetDuration(entry);
  // The count byte only when byte +25's bit 0x20 is clear (with it set the
  // original's beq skips the load: no loops).
  const uint32_t loops =
      (w.flags() & 0x20) == 0 ? guest::LoadU8(count) : 0;
  uint32_t loop_start = 0, loop = 0, loop_end = 0;
  if (loops != 0) {
    loop_start = WaveEntryGetLoopStart(entry);
    loop = WaveEntryHasLoopRegion(entry) != 0 ? WaveEntryGetLoopLength(entry)
                                                    : WaveEntryGetDuration(entry);
    loop_end = loop + loop_start;
  }
  uint32_t left = loops;
  bool past = false;
  if (loops == 0) {
    past = position > duration;
  } else if (loops == 255) {
    if (position > loop_start) {
      if (loop == 0) {
        return std::nullopt;  // the original's divide check traps
      }
      position = (position - loop_start) % loop + loop_start;
    }
  } else {
    const uint32_t end = loops * loop + loop_start;
    if (position > loop_start && position < end) {
      // Inside the loops (the loop length isn't 0 here): one pass, less the
      // passes skipped.
      const uint32_t passes = (position - loop_start) / loop;
      position = (position - loop_start) - passes * loop + loop_start;
      left = (loops - (passes & 0xFF)) & 0xFF;
    } else if (position > end && position < duration) {
      // Past the loops: after them, none left.
      position = loop_end - end + position;
      left = 0;
    } else {
      past = position > duration;
    }
  }
  if (past) {
    Store(start, 0);
    return kPastDuration;
  }
  Store(start, position);
  guest::StoreU8(count, uint8_t(left));
  return 0;
}

void WaveRequestNext(uint32_t wave, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_825D1C30 XACTWave::RequestNext");
  const Wave w(wave);
  auto* word = reinterpret_cast<uint32_t*>(guest::Pointer(w.request_stop_address()));
  uint32_t seen = __atomic_load_n(word, __ATOMIC_SEQ_CST);
  while (!__atomic_compare_exchange_n(word, &seen, __builtin_bswap32(value), false,
                                      __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {
  }
}

uint32_t EngineRequestCommit(uint32_t engine, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D1C78 XACTEngine::RequestCommit");
  call(xa::kRtlEnterCriticalSection, 0x825D1C9C, Args({engine + 144}));
  Store(engine + 184, 1);
  return call(xa::kRtlLeaveCriticalSection, 0x825D1CAC, Args({engine + 144}));
}

namespace {

constexpr uint32_t kRequestCommitFrame = 112;

// The wave's owner's track: [[owner + 44] + 16] (the instance's track).
Track TrackOf(uint32_t owner) {
  return Event(owner).instance().track();
}

// The wave's owner's cue: the track's sound's +48 (the sound isn't in the
// schema yet).
uint32_t CueOf(uint32_t owner) {
  return Load(TrackOf(owner).sound() + 48);
}

// A guest call from `frame` (the caller's own frame).
uint32_t CallAt(const GuestCall& call, uint32_t function, uint32_t return_address,
                xa::Regs regs, uint32_t frame) {
  regs.sp = frame;
  return call(function, return_address, regs);
}

// XACTEngine::RequestCommit on the wave's engine, in a frame below `frame`.
void Commit(uint32_t wave, uint32_t frame, const GuestCall& call) {
  const Wave w(wave);
  const uint32_t engine = w.engine();
  SetRegs(call, Args({engine}));
  xa::FrameBelow(frame, kRequestCommitFrame);
  EngineRequestCommit(engine, call);
}

// The voice's Start (slot +16), Stop (+20) and FlushPackets (+24) from
// `frame`: the plain C++ while the slot holds it.
uint32_t VoiceSlotStart(uint32_t voice, uint32_t deferred, const Thread& thread,
                        uint32_t frame, uint32_t return_address, const GuestCall& call) {
  const uint32_t method = xa::Method(voice, 16);
  if (method == kVoiceStart) {
    SetRegs(call, Args({voice, deferred}));
    return VoiceStart(voice, deferred, thread, frame, call);
  }
  return CallAt(call, method, return_address, Args({voice, deferred}), frame);
}
uint32_t VoiceSlotStop(uint32_t voice, uint32_t now, uint32_t frame,
                       uint32_t return_address, const GuestCall& call) {
  const uint32_t method = xa::Method(voice, 20);
  if (method == kVoiceStop) {
    SetRegs(call, Args({voice, now}));
    return VoiceStop(voice, now, frame, call);
  }
  return CallAt(call, method, return_address, Args({voice, now}), frame);
}
uint32_t VoiceSlotFlush(uint32_t voice, uint32_t frame, uint32_t return_address,
                        const GuestCall& call) {
  const uint32_t method = xa::Method(voice, 24);
  if (method == kVoiceFlushPackets) {
    SetRegs(call, Args({voice}));
    return VoiceFlushPackets(voice, frame, call);
  }
  return CallAt(call, method, return_address, Args({voice}), frame);
}

}  // namespace

uint32_t WaveSetVoice(uint32_t wave, uint32_t voice, uint32_t frame,
                      const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D1DF8 XACTWave::SetVoice");
  const Wave w(wave);
  w.set_voice(voice);
  uint32_t r3 = voice;
  if (voice == 0) {
    return r3;
  }
  const uint32_t owner = w.owner();
  if (owner == 0) {
    return r3;
  }
  const uint32_t cue = CueOf(owner);
  if (int32_t(Load(cue + 348)) != 0) {
    if (cue + 340 != 0) {
      r3 = CallAt(call, xa::Method(voice, 28), 0x825D1E78, Args({voice, cue + 340}), frame);
    }
  } else if (int32_t(Load(cue + 316)) != 0) {
    if (cue + 308 != 0) {
      r3 = CallAt(call, xa::Method(voice, 32), 0x825D1E78, Args({voice, cue + 308}), frame);
    }
  }
  if (int32_t(Load(cue + 72)) == 0) {
    return r3;
  }
  const uint32_t renderer = Load(w.engine() + 12);
  const uint32_t matrix = Load(cue + 64);
  r3 = CallAt(call, xa::Method(renderer, 24), 0x825D1EA0, Args({renderer}), frame);
  const uint32_t now_voice = w.voice().address;
  const uint32_t channels = guest::LoadU16(cue + 68);
  if (now_voice == 0) {
    return r3;
  }
  return CallAt(call, xa::Method(now_voice, 48), 0x825D1ED0,
                Args({now_voice, channels, r3 & 0xFFFF, matrix}), frame);
}

uint32_t InMemoryWavePlay(uint32_t wave, uint32_t deferred, const Thread& thread,
                          uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D3938 XACTInMemoryWave::Play");
  const Wave w(wave);
  const uint32_t owner = w.owner();
  if (owner != 0) {
    if ((w.flags() & 0x20) != 0 && w.loop_count() != 0) {
      CallAt(call, xa::Method(owner, 100), 0x825D398C, Args({owner}), frame);
    }
    const uint32_t now_owner = w.owner();
    CallAt(call, xa::Method(now_owner, 96), 0x825D39A0, Args({now_owner}), frame);
  }
  if ((w.flags() & 0x80) == 0) {
    const uint32_t voice = w.voice().address;
    const uint32_t r3 = VoiceSlotStart(voice, deferred, thread, frame, 0x825D39C8, call);
    if (int32_t(r3) < 0) {
      return r3;
    }
    if (int32_t(deferred) != 0) {
      Commit(wave, frame, call);
    }
    w.set_flags(w.flags() | 0x2);
  }
  w.set_flags(w.flags() | 0x40);
  return VoiceSlotFlush(w.voice().address, frame, 0x825D3A0C, call);
}

uint32_t WavePause(uint32_t wave, uint32_t pause, const Thread& thread,
                   uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D1ED8 XACTWave::Pause");
  const Wave w(wave);
  const uint8_t flags = w.flags();
  const int32_t paused = (flags & 0x80) == 0x80 ? 1 : 0;
  if (int32_t(pause) == paused) {
    return 0;
  }
  const uint8_t now = uint8_t((flags & 0x7F) | (pause != 0 ? 0x80 : 0));
  const uint32_t voice = w.voice().address;
  w.set_flags(now);
  if (voice == 0) {
    return 0;
  }
  if (int32_t(pause) != 0) {
    return VoiceSlotStop(voice, 1, frame, 0x825D1F58, call);
  }
  if ((now & 0x2) == 0) {
    return 0;
  }
  const uint32_t r30 = VoiceSlotStart(voice, 1, thread, frame, 0x825D1F80, call);
  if (int32_t(r30) < 0) {
    return r30;
  }
  Commit(wave, frame, call);
  if ((w.flags() & 0x40) == 0) {
    return r30;
  }
  return VoiceSlotFlush(w.voice().address, frame, 0x825D1FB8, call);
}

uint32_t WavePrepareNext(uint32_t wave, bool native_heap, uint32_t frame,
                         const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D2778 XACTWave::PrepareNext");
  const Wave w(wave);
  const uint8_t flags = w.flags();
  if ((flags & 0x8) == 0) {
    return wave;
  }
  w.set_flags(flags & 0xF7);
  if (w.loop_count() == 0) {
    return wave;
  }
  const uint32_t owner = w.owner();
  uint32_t r3 = CallAt(call, xa::Method(owner, 108), 0x825D27D0,
                       Args({owner, frame + 82, frame + 80}), frame);
  if (int32_t(r3) < 0) {
    return r3;
  }
  const uint32_t index = guest::LoadU8(frame + 80);
  const uint32_t sound_bank = Load(CueOf(w.owner()) + 296);
  std::optional<uint32_t> found;
  if (native_heap) {
    SetRegs(call, Args({sound_bank, index}));
    found = SoundBankGetWaveBank(sound_bank, index);
  }
  const uint32_t wave_bank =
      found ? *found : CallAt(call, 0x825C7218, 0x825D27F8, Args({sound_bank, index}), frame);
  if (wave_bank == 0) {
    return 0;
  }
  r3 = CallAt(call, xa::Method(wave_bank, 40), 0x825D2820,
              Args({wave_bank, guest::LoadU16(frame + 82), w.owner(),
                    frame + 84}),
              frame);
  if (int32_t(r3) < 0) {
    return r3;
  }
  uint32_t count = w.loop_count();
  if (count != 255) {
    count = (count + 255) & 0xFF;
  }
  uint32_t next = Load(frame + 84);
  r3 = CallAt(call, xa::Method(next, 4), 0x825D2868,
              Args({next, count, count != 0 ? 1u : 0u, 0}), frame);
  if (int32_t(r3) >= 0) {
    next = Load(frame + 84);
    const uint32_t list = wave_bank + 240;
    w.set_next_wave(next);
    const Wave next_wave(next);
    next_wave.set_node_next(list);
    next_wave.set_node_previous(Load(list + 8));
    Store(Load(list + 8) + 4, next);
    Store(list + 8, next);
    return r3;
  }
  next = Load(frame + 84);
  if (next == 0) {
    return 0;
  }
  return CallAt(call, xa::Method(next, 0), 0x825D28B8, Args({next, 1}), frame);
}

uint32_t InMemoryWaveUpdate(uint32_t wave, bool native_heap, uint32_t frame,
                            const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D3890 XACTInMemoryWave::Update");
  const Wave w(wave);
  SetRegs(call, Args({wave}));
  uint32_t r3 = WavePrepareNext(wave, native_heap, xa::FrameBelow(frame, 112), call);
  if ((w.flags() & 0x10) != 0) {
    return CallAt(call, xa::Method(wave, 0), 0x825D38D0, Args({wave, 1}), frame);
  }
  const uint32_t voice = w.voice().address;
  if (voice == 0 || int32_t(w.state()) != 5) {
    return r3;
  }
  const uint32_t renderer = Load(w.engine() + 12);
  CallAt(call, xa::Method(renderer, 12), 0x825D3914, Args({renderer, voice}), frame);
  SetRegs(call, Args({wave, 0}));
  return WaveSetVoice(wave, 0, xa::FrameBelow(frame, 112), call);
}

// --- XACT's clock and notifications ---

namespace {

constexpr uint32_t kGetTimeMs = 0x825BFE28;
constexpr uint32_t kMatchRegistration = 0x825BFCC8;  // never seen in play
constexpr uint32_t kSendNotificationFrame = 112;
constexpr uint32_t kNotifyFrame = 160;

void StoreU8(uint32_t address, uint32_t value) {
  guest::StoreU8(address, uint8_t(value));
}
void StoreU16(uint32_t address, uint32_t value) {
  guest::StoreU16(address, uint16_t(value));
}
void StoreU64(uint32_t address, uint64_t value) {
  Store(address, uint32_t(value >> 32));
  Store(address + 4, uint32_t(value));
}
uint64_t LoadU64(uint32_t address) {
  return (uint64_t(Load(address)) << 32) | Load(address + 4);
}
// A word at an unaligned place as the original stores it: its top byte,
// the middle 16 bits, then its low byte.
void StoreSplit(uint32_t address, uint32_t value) {
  StoreU8(address, value >> 24);
  StoreU16(address + 1, value >> 8);
  StoreU8(address + 3, value);
}
// The engine's +20 exchanged atomically (the original's lwarx/stwcx. loop).
void ExchangeWord(uint32_t address, uint32_t value) {
  auto* word = reinterpret_cast<uint32_t*>(guest::Pointer(address));
  uint32_t seen = __atomic_load_n(word, __ATOMIC_SEQ_CST);
  while (!__atomic_compare_exchange_n(word, &seen, __builtin_bswap32(value), false,
                                      __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {
  }
}

}  // namespace

uint32_t ClockMs(uint32_t return_address, uint32_t frame, const GuestCall& call) {
  if (const auto ms = GetTimeMs()) {
    return uint32_t(*ms);
  }
  return CallAt(call, kGetTimeMs, return_address, Args({}), frame);
}

uint64_t ClockMsFull(uint32_t return_address, uint32_t frame,
                     const GuestCall& call) {
  if (const auto ms = GetTimeMs()) {
    return *ms;
  }
  CallAt(call, kGetTimeMs, return_address, Args({}), frame);
  return xa::LastCallR3();
}

uint64_t EngineSendNotification(uint64_t engine_r3, const uint64_t words[4],
                                uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825C1388 XACTEngine::SendNotification");
  const uint32_t engine = uint32_t(engine_r3);
  const uint32_t notification = frame + 144;
  for (uint32_t i = 0; i < 4; i++) {
    StoreU64(notification + 8 * i, words[i]);
  }
  if (Load(engine + 16) == 0) {
    return engine_r3;  // r3 as the caller left it
  }
  // r3 in full: XACTGetTimeMs's (the counter's low word sign-extended).
  const uint64_t now_r3 = ClockMsFull(0x825C13C4, frame, call);
  const uint32_t now = uint32_t(now_r3);
  StoreU8(notification + 4, now);
  StoreU8(notification + 1, now >> 24);
  StoreU16(notification + 2, now >> 8);
  const uint32_t list = engine + 100;
  uint32_t node = Load(engine + 104);
  if (node == list) {
    return now_r3;
  }
  bool matched = false;
  uint64_t r3 = now_r3;
  do {
    const uint32_t description = node + 16;
    const uint32_t next = Load(node + 4);
    // r3 the engine as the caller left it (the original's mr r3,r31).
    CallAt(call, kMatchRegistration, 0x825C13FC, Args({engine_r3, notification, description}),
           frame);
    r3 = xa::LastCallR3();
    if (int32_t(r3) != 0) {
      matched = true;
      StoreSplit(notification + 5, Load(description + 16));
      if ((guest::LoadU8(description + 1) & 1) == 0) {
        // Moved to the tail of the list +112.
        const uint32_t done = engine + 112;
        Store(Load(node + 4) + 8, Load(node + 8));
        Store(Load(node + 8) + 4, Load(node + 4));
        Store(node + 8, node);
        Store(node + 4, done);
        Store(node + 8, Load(done + 8));
        Store(Load(done + 8) + 4, node);
        Store(done + 8, node);
      }
    }
    node = next;
  } while (node != list);
  if (!matched) {
    return r3;
  }
  ExchangeWord(engine + 20, graphics::video_output::CurrentThreadId());
  CallAt(call, Load(engine + 16), 0x825C14E4, Args({notification}), frame);
  r3 = xa::LastCallR3();
  ExchangeWord(engine + 20, 0xFFFFFFFFu);
  return r3;
}

uint64_t WaveNotify(uint32_t wave, uint32_t type, uint32_t frame,
                    const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D2030 XACTWave::Notify");
  const Wave w(wave);
  const uint32_t bank = w.wave_bank();
  StoreU8(frame + 80, 0);
  // memset (sub_825A71D0) of 28 bytes from +81, its r3 the destination.
  for (uint32_t i = 0; i < 28; i++) {
    StoreU8(frame + 81 + i, 0);
  }
  const uint32_t cleared = frame + 81;
  if (bank == 0) {
    return cleared;
  }
  const uint32_t owner = w.owner();
  if (owner != 0) {
    const uint32_t cue = CueOf(owner);
    const uint32_t sound_bank = Load(cue + 296);
    if (sound_bank == 0) {
      return cleared;
    }
    const uint32_t index = guest::LoadU16(cue + 304);
    StoreU8(frame + 95, index >> 8);
    StoreU8(frame + 96, index);
    StoreSplit(frame + 97, sound_bank);
    StoreSplit(frame + 101, cue);
  } else {
    StoreSplit(frame + 97, 0);
    StoreSplit(frame + 101, 0);
    StoreU8(frame + 95, 0xFF);
    StoreU8(frame + 96, 0xFF);
  }
  const uint32_t index = w.wave_index();
  StoreU8(frame + 80, type);
  StoreU8(frame + 92, bank);
  StoreU8(frame + 93, index >> 8);
  StoreU8(frame + 94, index);
  const uint32_t engine = w.engine();
  uint64_t words[4];
  words[0] = LoadU64(frame + 80);
  words[2] = LoadU64(frame + 96);
  // The last word's top five bytes (+104 to +108; the three after them
  // aren't the notification's).
  words[3] = (uint64_t(Load(frame + 104)) << 32) |
             (uint64_t(guest::LoadU8(frame + 108)) << 24);
  StoreU8(frame + 89, bank >> 24);
  StoreU16(frame + 90, bank >> 8);
  words[1] = LoadU64(frame + 88);
  SetRegs(call, Args({engine, words[0], words[1], words[2], words[3]}));
  return EngineSendNotification(engine, words,
                                xa::FrameBelow(frame, kSendNotificationFrame), call);
}

uint64_t WaveSetState(uint64_t wave_r3, uint32_t state, uint32_t frame,
                      const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D24D8 XACTWave::SetState");
  const uint32_t wave = uint32_t(wave_r3);
  const Wave w(wave);
  uint64_t r3 = wave_r3;  // as the caller left it
  const int32_t now = int32_t(w.state());
  uint32_t step = 0, type = 0;
  if (int32_t(state) == 3) {
    if (now < 3) {
      step = 3;
      type = 12;
    }
  } else if (int32_t(state) == 5 && now >= 3 && now < 5) {
    step = 5;
    type = 13;
  }
  if (step != 0) {
    w.set_state(step);
    SetRegs(call, Args({wave, type}));
    r3 = WaveNotify(wave, type, xa::FrameBelow(frame, kNotifyFrame), call);
  }
  w.set_state(state);
  return r3;
}

// --- Releasing and stopping waves ---

namespace {

constexpr uint32_t kReleaseFrame = 112;
constexpr uint32_t kDestructFrame = 96;
constexpr uint32_t kSetStateFrame = 112;

}  // namespace

uint32_t WaveRelease(uint32_t wave, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D28D0 XACTWave::Release");
  const Wave w(wave);
  w.set_flags(uint8_t(w.flags() | 0x4));
  const uint32_t next_node = w.node_next();
  if (next_node != wave) {
    const uint32_t previous = w.node_previous();
    if (previous != wave) {
      Store(next_node + 8, previous);
      Store(w.node_previous() + 4, w.node_next());
      w.set_node_next(wave);
      w.set_node_previous(wave);
    }
  }
  const uint32_t next = w.next_wave().address;
  if (next != 0) {
    CallAt(call, xa::Method(next, 0), 0x825D2944, Args({next, 1}), frame);
  }
  const uint32_t owner = w.owner();
  if (owner != 0) {
    // r4 as the original leaves it (the caller's, or 1 after the next wave).
    xa::Regs regs;
    regs.r[regs.count++] = owner;
    CallAt(call, xa::Method(owner, 28), 0x825D2960, regs, frame);
  }
  const uint32_t voice = w.voice().address;
  if (voice != 0) {
    const uint32_t renderer = Load(w.engine() + 12);
    CallAt(call, xa::Method(renderer, 12), 0x825D2984, Args({renderer, voice}), frame);
  }
  const int32_t state = int32_t(w.state());
  if (state >= 3 && state < 5) {
    w.set_state(5);
    SetRegs(call, Args({wave, 13}));
    WaveNotify(wave, 13, xa::FrameBelow(frame, kNotifyFrame), call);
  }
  w.set_state(5);
  const uint32_t listener = w.wave_bank() + 4;
  return CallAt(call, xa::Method(listener, 4), 0x825D29CC,
                Args({listener, w.listener_vtable_address()}), frame);
}

uint32_t WaveDestruct(uint32_t wave, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D29E8 XACTWave::~XACTWave");
  const Wave w(wave);
  w.set_vtable(kWaveVtable);
  w.set_listener_vtable(kWaveListenerVtable);
  uint32_t r3 = wave;
  if ((w.flags() & 0x4) == 0) {
    SetRegs(call, Args({wave}));
    r3 = WaveRelease(wave, xa::FrameBelow(frame, kReleaseFrame), call);
  }
  w.set_vtable(kListBaseVtable);
  return r3;
}

uint32_t InMemoryWaveDeletingDestruct(uint32_t wave, uint32_t flags,
                                      uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D3820 XACTInMemoryWave::~XACTInMemoryWave (deleting)");
  const Wave w(wave);
  w.set_vtable(kInMemoryWaveVtable);
  w.set_listener_vtable(kWaveListenerVtable);
  SetRegs(call, Args({wave}));
  WaveDestruct(wave, xa::FrameBelow(frame, kDestructFrame), call);
  if ((flags & 1) != 0) {
    SetRegs(call, Args({wave, kInMemoryWaveAttributes}));
    memory::crt::XMemFree(wave, kInMemoryWaveAttributes);
  }
  return wave;
}

uint32_t InMemoryWaveStop(uint32_t wave, uint32_t mode, uint32_t exit_loop,
                          uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D3A28 XACTInMemoryWave::Stop");
  const Wave w(wave);
  const auto set_state = [&](uint32_t state) {
    SetRegs(call, Args({wave, state}));
    WaveSetState(wave, state, xa::FrameBelow(frame, kSetStateFrame), call);
  };
  const int32_t state = int32_t(w.state());
  if (state == 5) {
    return 0;
  }
  if (state == 4 && (int32_t(w.stopped_at_once()) != 0 || int32_t(mode) != 0)) {
    return 0;
  }
  if (state != 3 && state != 4) {
    set_state(5);
    return 0;
  }
  if (mode == 0) {
    w.set_stopped_at_once(1);
    set_state(4);
    const uint32_t voice = w.voice().address;
    return VoiceSlotStop(voice, 1, frame, 0x825D3B04, call);
  }
  if (mode != 1) {
    return 0;
  }
  set_state(4);
  if (int32_t(exit_loop) == 0) {
    return 0;
  }
  const uint32_t voice = w.voice().address;
  w.set_loop_count(0);
  return CallAt(call, xa::Method(voice, 52), 0x825D3AD0, Args({voice}), frame);
}

}  // namespace xact

// --- Preparing a wave and the audio callback's update ---

namespace xact {

namespace {

constexpr uint32_t kSetStateFrameBelow = 112;
constexpr uint32_t kSetVoiceFrame = 112;
constexpr uint32_t kDoneFrame = 96;
constexpr uint32_t kFinishFrame = 96;
constexpr uint32_t kGetCategoryFlag = 0x825C0708;
constexpr uint32_t kWaveEntrySet = 0x825CED98;
constexpr uint32_t kGetStartPosition = 0x825D2170;
constexpr uint32_t kSamplesToBytes = 0x825CF080;
constexpr uint32_t kGetWaveFormat = 0x825CC320;
constexpr uint32_t kTrackGetVolume = 0x825D0268;

// A guest call from `frame`, r3 in full as the callee leaves it.
uint64_t CallFull(const GuestCall& call, uint32_t function, uint32_t return_address,
                  xa::Regs regs, uint32_t frame) {
  regs.sp = frame;
  call(function, return_address, regs);
  return xa::LastCallR3();
}

// A float moved by lfs and stfs: its bits, as the recompiled code moves
// them (a signaling NaN kept).
void MoveFloat(uint32_t to, uint32_t from) { Store(to, Load(from)); }

void SetStateBelow(uint32_t wave, uint32_t state, uint32_t frame,
                   const GuestCall& call) {
  SetRegs(call, Args({wave, state}));
  WaveSetState(wave, state, xa::FrameBelow(frame, kSetStateFrameBelow), call);
}

}  // namespace

uint64_t InMemoryWavePrepare(uint32_t wave, uint32_t count, uint32_t loops,
                             uint32_t ms, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D3B10 XACTInMemoryWave::Prepare");
  const Wave w(wave);
  const uint32_t count_byte = frame + 351;  // the caller's parameter area
  guest::StoreU8(count_byte, uint8_t(count));
  w.set_prepared(0);
  SetStateBelow(wave, 1, frame, call);
  w.set_loop_count(guest::LoadU8(count_byte));
  const uint32_t owner = w.owner();
  w.set_flags(uint8_t((w.flags() & ~0x20) | (loops != 0 ? 0x20 : 0)));
  uint32_t category_flag = 0;  // r21
  if (owner != 0) {
    const uint32_t record =
        Load(TrackOf(owner).sound() + 28);
    const uint32_t engine = w.engine();
    const uint32_t index = guest::LoadU16(record + 1);
    SetRegs(call, Args({engine, index}));
    const auto flag = EngineGetCategoryFlag(engine, index);
    category_flag =
        flag ? *flag : CallAt(call, kGetCategoryFlag, 0x825D3B88, Args({engine, index}), frame);
  }
  // The entry's accessor at the frame's +112.
  const uint32_t entry = frame + 112;
  SetRegs(call, Args({entry}));
  WaveEntryConstruct(entry);
  {
    const uint32_t bank = w.wave_bank(), index = w.wave_index();
    SetRegs(call, Args({entry, index, bank}));
    if (!WaveEntrySet(entry, index, bank)) {
      CallAt(call, kWaveEntrySet, 0x825D3BA4, Args({entry, index, bank}), frame);
    }
  }
  const uint32_t start = frame + 80;
  Store(start, 0);
  SetRegs(call, Args({wave, entry, ms, start, count_byte}));
  uint64_t r3;
  if (const auto done = WaveGetStartPosition(wave, entry, ms, start, count_byte)) {
    r3 = uint64_t(int64_t(int32_t(*done)));  // li or lis: sign-extended
  } else {
    r3 = CallFull(call, kGetStartPosition, 0x825D3BC0,
                  Args({wave, entry, ms, start, count_byte}), frame);
  }
  if (int32_t(r3) < 0) {
    return r3;
  }
  const uint32_t position = Load(start);
  SetRegs(call, Args({entry, position, 0}));
  const auto bytes = WaveEntrySamplesToBytes(entry, position, 0);
  const uint32_t offset =  // r26
      bytes ? *bytes : CallAt(call, kSamplesToBytes, 0x825D3BD8, Args({entry, position, 0}), frame);
  // The 40-byte packet at the frame's +160, zeroed, then the entry's region
  // (the bank's slot +60: data, length, loop start, loop length, ...).
  const uint32_t packet = frame + 160;
  for (uint32_t at = 0; at < 40; at += 4) {
    Store(packet + at, 0);
  }
  {
    const uint32_t bank = w.wave_bank();
    const uint32_t index = w.wave_index();
    CallAt(call, xa::Method(bank, 60), 0x825D3C18, Args({bank, index, packet}), frame);
  }
  const uint32_t loop_start = Load(packet + 12);   // r23
  const uint32_t loop_length = Load(packet + 16);  // r20
  const uint32_t data = Load(packet + 0);          // r22
  const uint32_t length = Load(packet + 4);        // r25
  const uint32_t loop_end = loop_length + loop_start;    // r24
  const uint32_t in_loop =                               // r27
      guest::LoadU8(count_byte) != 0 && offset > loop_start && offset < loop_end ? 1 : 0;
  Store(frame + 92, 0);
  uint32_t reverb = 0;  // r28
  Store(frame + 88, 0);
  MoveFloat(frame + 84, schema::XactWavesGlobals::one_address());
  if (const uint32_t now_owner = w.owner(); now_owner != 0) {
    const uint32_t sound = TrackOf(now_owner).sound();
    const uint32_t record = Load(sound + 28);
    if ((guest::LoadU8(record) & 0x10) != 0) {
      reverb = 1;
      Store(frame + 88, Load(Load(w.engine() + 124) + 8));
      MoveFloat(frame + 84, sound + 116);
    }
  }
  const uint32_t renderer = Load(w.engine() + 12);  // r30
  {
    const uint32_t bank = w.wave_bank(), index = w.wave_index();
    SetRegs(call, Args({bank, index, frame + 96}));
    if (!WaveBankGetWaveFormat(bank, index, frame + 96)) {
      CallAt(call, kGetWaveFormat, 0x825D3CD4, Args({bank, index, frame + 96}), frame);
    }
  }
  r3 = CallFull(call, xa::Method(renderer, 4), 0x825D3D14,
                Args({renderer, frame + 96, (in_loop != 0 ? 1u : 0u) + 1, category_flag,
                      frame + 88, frame + 84, reverb, frame + 92}),
                frame);
  if (int32_t(r3) < 0) {
    return r3;
  }
  {
    const uint32_t voice = Load(frame + 92);
    SetRegs(call, Args({wave, voice}));
    WaveSetVoice(wave, voice, xa::FrameBelow(frame, kSetVoiceFrame), call);
  }
  SetRegs(call, Args({entry}));
  const uint32_t tag = WaveEntryGetFormatTag(entry);
  uint32_t flag = 0;     // r5
  uint32_t context = 0;  // r11, the packets' context at the packet's +36
  bool common = true;    // the store of +72 = 1
  if (int32_t(tag) == 1) {
    // XMA: one packet with the loop count and the start sample.
    Store(packet + 8, guest::LoadU8(count_byte));
    uint32_t sample = Load(start);
    if (sample > 128) {
      sample -= sample & 0x7F;
    }
    Store(packet + 28, sample);
    flag = 1;
  } else if (in_loop != 0) {
    // PCM from inside the loop region: the rest of this pass, then the loop
    // region with the count less one.
    const uint32_t rest = loop_end - length - offset + Load(packet + 4);
    const uint32_t first = Load(packet + 0) + offset;
    Store(packet + 8, 0);
    Store(packet + 12, 0);
    w.set_packet_context_looped(0);
    Store(packet + 16, 0);
    Store(packet + 36, w.packet_context_address());
    Store(packet + 0, first);
    Store(packet + 4, rest);
    const uint32_t voice = w.voice().address;
    r3 = CallFull(call, xa::Method(voice, 12), 0x825D3DBC, Args({voice, packet, 0}), frame);
    if (int32_t(r3) < 0) {
      return r3;
    }
    Store(packet + 0, loop_start + data);
    Store(packet + 4, length - loop_start);
    uint32_t left = guest::LoadU8(count_byte);
    if (left != 255) {
      left -= 1;
    }
    Store(packet + 8, left);
    Store(packet + 12, 0);
    Store(packet + 16, loop_length);
    w.set_loop_context(w.packet_context());
    w.set_unknown_84(w.unknown_68());
    w.set_loop_context_looped(w.packet_context_looped());
    w.set_unknown_92(w.unknown_76());
    w.set_loop_context_looped(1);
    context = w.loop_context_address();
    common = false;
  } else {
    // PCM from the start, with the loop region and count.
    Store(packet + 0, Load(packet + 0) + offset);
    Store(packet + 4, Load(packet + 4) - offset);
    if (offset > loop_start) {
      Store(packet + 8, 0);
      Store(packet + 12, 0);
      Store(packet + 16, 0);
    } else {
      const uint32_t left = guest::LoadU8(count_byte);
      Store(packet + 12, loop_start);
      Store(packet + 8, left);
      if (left == 0) {
        Store(packet + 12, 0);
        Store(packet + 16, 0);
      } else {
        Store(packet + 16, loop_length);
      }
    }
  }
  if (common) {
    context = w.packet_context_address();
    w.set_packet_context_looped(1);
  }
  const uint32_t voice = w.voice().address;
  Store(packet + 36, context);
  const uint64_t submitted =  // r30
      CallFull(call, xa::Method(voice, 12), 0x825D3E98, Args({voice, packet, flag}), frame);
  if (int32_t(submitted) < 0) {
    return submitted;
  }
  w.set_prepared(1);
  SetStateBelow(wave, 2, frame, call);
  if (w.owner() == 0 || w.voice().address == 0) {
    return submitted;
  }
  {
    const uint32_t track = TrackOf(w.owner()).address;
    SetRegs(call, Args({track}));
    const auto volume = TrackGetVolume(track);
    if (!volume) {
      CallAt(call, kTrackGetVolume, 0x825D3ED8, Args({track}), frame);
    }
    const uint32_t now_voice = w.voice().address;
    xa::Regs regs = Args({now_voice});
    if (volume) {
      regs.f1_set = true;
      regs.f1 = volume->f1;
    }
    CallAt(call, xa::Method(now_voice, 4), 0x825D3EEC, regs, frame);
  }
  {
    const uint32_t track = TrackOf(w.owner()).address;
    SetRegs(call, Args({track}));
    const CategoryVolume pitch = TrackGetPitch(track);
    const uint32_t now_voice = w.voice().address;
    xa::Regs regs = Args({now_voice});
    regs.f1_set = true;
    regs.f1 = pitch.f1;
    CallAt(call, xa::Method(now_voice, 8), 0x825D3F10, regs, frame);
  }
  return submitted;
}

uint64_t WaveFinish(uint32_t wave, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D1FD8 XACTWave::Finish");
  const Wave w(wave);
  const uint32_t owner = w.owner();
  w.set_flags(uint8_t(w.flags() | 0x10));
  if (owner == 0) {
    return 0;
  }
  const uint64_t r3 = CallFull(call, xa::Method(owner, 28), 0x825D2014, Args({owner}), frame);
  w.set_owner(0);
  return r3;
}

namespace {

// A request word taken atomically (the original's lwarx/stwcx. loop): 1
// becomes 0, anything else is written back as it was. Returns the word seen.
uint32_t TakeRequest(uint32_t address) {
  auto* word = reinterpret_cast<uint32_t*>(guest::Pointer(address));
  uint32_t seen = __atomic_load_n(word, __ATOMIC_SEQ_CST);
  while (true) {
    const uint32_t value = __builtin_bswap32(seen);
    const uint32_t next = value == 1 ? 0 : value;
    if (__atomic_compare_exchange_n(word, &seen, __builtin_bswap32(next), false, __ATOMIC_SEQ_CST,
                                    __ATOMIC_SEQ_CST)) {
      return value;
    }
  }
}

}  // namespace

uint64_t WaveAudioUpdate(uint32_t wave, uint64_t r3_in, uint32_t frame,
                         const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D2560 XACTWave::AudioUpdate");
  const Wave w(wave);
  const uint8_t flags = w.flags();
  if ((flags & 0x4) != 0 || (flags & 0x10) != 0 || int32_t(w.state()) >= 5) {
    return r3_in;
  }
  const auto notify = [&](uint32_t type) -> uint64_t {
    SetRegs(call, Args({wave, type}));
    return WaveNotify(wave, type, xa::FrameBelow(frame, kNotifyFrame), call);
  };
  const auto finish = [&]() -> uint64_t {
    SetRegs(call, Args({wave}));
    return WaveFinish(wave, xa::FrameBelow(frame, kFinishFrame), call);
  };
  // The owner's done (XACTEvent::Done), or without an owner Finish.
  const auto done = [&](uint32_t owner) -> uint64_t {
    if (owner == 0) {
      return finish();
    }
    SetRegs(call, Args({owner}));
    return EventDone(owner, xa::FrameBelow(frame, kDoneFrame), call);
  };
  if (TakeRequest(w.request_stop_address()) != 0) {
    // Stop now and hand the next wave to the owner.
    const int32_t state = int32_t(w.state());
    if (state >= 3 && state < 5) {
      w.set_state(5);
      notify(13);
    }
    w.set_state(5);
    const uint32_t next = w.next_wave().address;
    const uint32_t owner = w.owner();
    if (next == 0) {
      return done(owner);
    }
    w.set_next_wave(0);
    CallAt(call, xa::Method(owner, 128), 0x825D2630, Args({owner, next}), frame);
    w.set_owner(0);
    CallAt(call, xa::Method(next, 8), 0x825D2654, Args({next, 1, 0, 0}), frame);
    return finish();
  }
  uint64_t r3 = 0;
  if (TakeRequest(w.request_owner_address()) != 0) {
    const uint32_t owner = w.owner();
    if (owner != 0) {
      CallAt(call, xa::Method(owner, 32), 0x825D26BC, Args({owner}), frame);
    }
  }
  const uint32_t voice = w.voice().address;
  if (voice == 0) {
    return 0;
  }
  r3 = CallFull(call, xa::Method(voice, 0), 0x825D26DC, Args({voice, frame + 80}), frame);
  if (int32_t(r3) < 0) {
    return r3;
  }
  const int32_t state = int32_t(w.state());
  const uint32_t stopped = Load(frame + 80);
  if (state < 3) {
    if (int32_t(stopped) != 0) {
      return r3;
    }
    w.set_state(3);
    r3 = notify(12);
    w.set_state(3);
    return r3;
  }
  if (int32_t(stopped) == 0 || (w.flags() & 0x80) != 0) {
    return r3;
  }
  if (state < 5) {
    w.set_state(5);
    notify(13);
  }
  const uint32_t owner = w.owner();
  w.set_state(5);
  return done(owner);
}

}  // namespace xact
