// XACT's waves natively (waves.h, docs/functions/xact-waves.md): the
// waves' set-up and their leaves. The objects still recompiled (the owner's
// vtable, the renderer's slots, the wave bank's when they aren't the native
// functions) are called through the guest call; XACT's lock (the engine's
// critical section at +144) is the game's, entered and left through its
// imports as the original does. Strict floating point (CMakeLists.txt), as
// the original's single-precision steps.

#include "xact/waves.h"

#include "base/log.h"
#include "graphics/video_output.h"
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
double LoadF(uint8_t* base, uint32_t address) { return double(guest::LoadFloat(base, address)); }
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

uint32_t WaveConstruct(uint8_t* base, uint32_t wave, uint32_t engine, uint32_t bank,
                       uint32_t index, uint32_t owner, bool native_heap, uint32_t frame,
                       const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D23F0 XACTWave::XACTWave");
  Store(base, wave + 4, wave);
  Store(base, wave + 8, wave);
  Store(base, wave + 12, kListenerBaseVtable);
  const uint8_t flags = guest::LoadU8(base, wave + 25);
  Store(base, wave + 20, engine);
  Store(base, wave + 0, kWaveVtable);
  Store(base, wave + 36, bank);
  Store(base, wave + 48, owner);
  Store(base, wave + 12, kWaveListenerVtable);
  Store(base, wave + 16, 0);
  guest::StoreU8(base, wave + 24, 0);
  for (const uint32_t at : {28u, 32u, 40u, 44u, 56u, 60u}) {
    Store(base, wave + at, 0);
  }
  guest::StoreU8(base, wave + 25, flags & 0x20);
  guest::StoreU16(base, wave + 52, uint16_t(index));
  // AddListener on the bank's +4 interface.
  const uint32_t iface = bank + 4;
  const uint32_t add = xa::Method(base, iface, 0);
  if (add == kWaveBankAddListener && native_heap) {
    SetRegs(call, Args({iface, wave + 12}));
    WaveBankAddListener(base, iface, wave + 12);
  } else {
    xa::Regs regs = Args({iface, wave + 12});
    regs.sp = frame;
    call(add, 0x825D2490, regs);
  }
  return wave;
}

uint32_t InMemoryWaveConstruct(uint8_t* base, uint32_t wave, uint32_t engine, uint32_t bank,
                               uint32_t index, uint32_t owner, bool native_heap, uint32_t frame,
                               const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D37A8 XACTInMemoryWave::XACTInMemoryWave");
  SetRegs(call, Args({wave, engine, bank, index, owner}));
  WaveConstruct(base, wave, engine, bank, index, owner, native_heap,
                xa::FrameBelow(base, frame, kWaveFrame), call);
  Store(base, wave + 0, kInMemoryWaveVtable);
  Store(base, wave + 12, kWaveListenerVtable);
  for (uint32_t at = 64; at <= 92; at += 4) {
    Store(base, wave + at, 0);
  }
  Store(base, wave + 64, wave);
  Store(base, wave + 80, wave);
  return wave;
}

std::optional<CategoryVolume> TrackGetVolume(uint8_t* base, uint32_t track) {
  ITC_NATIVE_RUNNING("sub_825D0268 XACTTrack::GetVolume");
  const double f31 = LoadF(base, track + 40);
  const double f30 = LoadF(base, track + 56);
  const auto volume = SoundGetVolume(base, Load(base, track + 20));
  if (!volume) {
    return std::nullopt;
  }
  const double f0 = Single(f30 * f31);
  return CategoryVolume{Single(f0 * volume->f1), volume->r3};
}

CategoryVolume TrackGetPitch(uint8_t* base, uint32_t track) {
  ITC_NATIVE_RUNNING("sub_825D02B0 XACTTrack::GetPitch");
  const double f31 = LoadF(base, track + 44);
  const uint32_t sound = Load(base, track + 20);
  const double f30 = LoadF(base, track + 60);
  const double pitch = SoundGetPitch(base, sound);
  const double f0 = Single(f30 + f31);
  return CategoryVolume{Single(f0 + pitch), sound};
}

std::optional<uint32_t> WaveGetStartPosition(uint8_t* base, uint32_t wave, uint32_t entry,
                                             uint32_t ms, uint32_t start, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_825D2170 XACTWave::GetStartPosition");
  uint32_t position = WaveEntryMsToSamples(base, entry, ms);
  const uint32_t duration = WaveEntryGetDuration(base, entry);
  // The count byte only when byte +25's bit 0x20 is clear (with it set the
  // original's beq skips the load: no loops).
  const uint32_t loops =
      (guest::LoadU8(base, wave + 25) & 0x20) == 0 ? guest::LoadU8(base, count) : 0;
  uint32_t loop_start = 0, loop = 0, loop_end = 0;
  if (loops != 0) {
    loop_start = WaveEntryGetLoopStart(base, entry);
    loop = WaveEntryHasLoopRegion(base, entry) != 0 ? WaveEntryGetLoopLength(base, entry)
                                                    : WaveEntryGetDuration(base, entry);
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
    Store(base, start, 0);
    return kPastDuration;
  }
  Store(base, start, position);
  guest::StoreU8(base, count, uint8_t(left));
  return 0;
}

void WaveRequestNext(uint8_t* base, uint32_t wave, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_825D1C30 XACTWave::RequestNext");
  auto* word = reinterpret_cast<uint32_t*>(REX_RAW_ADDR(wave + 32));
  uint32_t seen = __atomic_load_n(word, __ATOMIC_SEQ_CST);
  while (!__atomic_compare_exchange_n(word, &seen, __builtin_bswap32(value), false,
                                      __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {
  }
}

uint32_t EngineRequestCommit(uint8_t* base, uint32_t engine, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D1C78 XACTEngine::RequestCommit");
  call(xa::kRtlEnterCriticalSection, 0x825D1C9C, Args({engine + 144}));
  Store(base, engine + 184, 1);
  return call(xa::kRtlLeaveCriticalSection, 0x825D1CAC, Args({engine + 144}));
}

namespace {

constexpr uint32_t kRequestCommitFrame = 112;

// The wave's owner's cue: [[[[owner + 44] + 16] + 20] + 48].
uint32_t CueOf(uint8_t* base, uint32_t owner) {
  return Load(base, Load(base, Load(base, Load(base, owner + 44) + 16) + 20) + 48);
}

// A guest call from `frame` (the caller's own frame).
uint32_t CallAt(const GuestCall& call, uint32_t function, uint32_t return_address,
                xa::Regs regs, uint32_t frame) {
  regs.sp = frame;
  return call(function, return_address, regs);
}

// XACTEngine::RequestCommit on the wave's engine, in a frame below `frame`.
void Commit(uint8_t* base, uint32_t wave, uint32_t frame, const GuestCall& call) {
  const uint32_t engine = Load(base, wave + 20);
  SetRegs(call, Args({engine}));
  xa::FrameBelow(base, frame, kRequestCommitFrame);
  EngineRequestCommit(base, engine, call);
}

// The voice's Start (slot +16), Stop (+20) and FlushPackets (+24) from
// `frame`: the plain C++ while the slot holds it.
uint32_t VoiceSlotStart(uint8_t* base, uint32_t voice, uint32_t deferred, const Thread& thread,
                        uint32_t frame, uint32_t return_address, const GuestCall& call) {
  const uint32_t method = xa::Method(base, voice, 16);
  if (method == kVoiceStart) {
    SetRegs(call, Args({voice, deferred}));
    return VoiceStart(base, voice, deferred, thread, frame, call);
  }
  return CallAt(call, method, return_address, Args({voice, deferred}), frame);
}
uint32_t VoiceSlotStop(uint8_t* base, uint32_t voice, uint32_t now, uint32_t frame,
                       uint32_t return_address, const GuestCall& call) {
  const uint32_t method = xa::Method(base, voice, 20);
  if (method == kVoiceStop) {
    SetRegs(call, Args({voice, now}));
    return VoiceStop(base, voice, now, frame, call);
  }
  return CallAt(call, method, return_address, Args({voice, now}), frame);
}
uint32_t VoiceSlotFlush(uint8_t* base, uint32_t voice, uint32_t frame, uint32_t return_address,
                        const GuestCall& call) {
  const uint32_t method = xa::Method(base, voice, 24);
  if (method == kVoiceFlushPackets) {
    SetRegs(call, Args({voice}));
    return VoiceFlushPackets(base, voice, frame, call);
  }
  return CallAt(call, method, return_address, Args({voice}), frame);
}

}  // namespace

uint32_t WaveSetVoice(uint8_t* base, uint32_t wave, uint32_t voice, uint32_t frame,
                      const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D1DF8 XACTWave::SetVoice");
  Store(base, wave + 44, voice);
  uint32_t r3 = voice;
  if (voice == 0) {
    return r3;
  }
  const uint32_t owner = Load(base, wave + 48);
  if (owner == 0) {
    return r3;
  }
  const uint32_t cue = CueOf(base, owner);
  if (int32_t(Load(base, cue + 348)) != 0) {
    if (cue + 340 != 0) {
      r3 = CallAt(call, xa::Method(base, voice, 28), 0x825D1E78, Args({voice, cue + 340}), frame);
    }
  } else if (int32_t(Load(base, cue + 316)) != 0) {
    if (cue + 308 != 0) {
      r3 = CallAt(call, xa::Method(base, voice, 32), 0x825D1E78, Args({voice, cue + 308}), frame);
    }
  }
  if (int32_t(Load(base, cue + 72)) == 0) {
    return r3;
  }
  const uint32_t renderer = Load(base, Load(base, wave + 20) + 12);
  const uint32_t matrix = Load(base, cue + 64);
  r3 = CallAt(call, xa::Method(base, renderer, 24), 0x825D1EA0, Args({renderer}), frame);
  const uint32_t now_voice = Load(base, wave + 44);
  const uint32_t channels = guest::LoadU16(base, cue + 68);
  if (now_voice == 0) {
    return r3;
  }
  return CallAt(call, xa::Method(base, now_voice, 48), 0x825D1ED0,
                Args({now_voice, channels, r3 & 0xFFFF, matrix}), frame);
}

uint32_t InMemoryWavePlay(uint8_t* base, uint32_t wave, uint32_t deferred, const Thread& thread,
                          uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D3938 XACTInMemoryWave::Play");
  const uint32_t owner = Load(base, wave + 48);
  if (owner != 0) {
    if ((guest::LoadU8(base, wave + 25) & 0x20) != 0 && guest::LoadU8(base, wave + 24) != 0) {
      CallAt(call, xa::Method(base, owner, 100), 0x825D398C, Args({owner}), frame);
    }
    const uint32_t now_owner = Load(base, wave + 48);
    CallAt(call, xa::Method(base, now_owner, 96), 0x825D39A0, Args({now_owner}), frame);
  }
  if ((guest::LoadU8(base, wave + 25) & 0x80) == 0) {
    const uint32_t voice = Load(base, wave + 44);
    const uint32_t r3 = VoiceSlotStart(base, voice, deferred, thread, frame, 0x825D39C8, call);
    if (int32_t(r3) < 0) {
      return r3;
    }
    if (int32_t(deferred) != 0) {
      Commit(base, wave, frame, call);
    }
    guest::StoreU8(base, wave + 25, guest::LoadU8(base, wave + 25) | 0x2);
  }
  guest::StoreU8(base, wave + 25, guest::LoadU8(base, wave + 25) | 0x40);
  return VoiceSlotFlush(base, Load(base, wave + 44), frame, 0x825D3A0C, call);
}

uint32_t WavePause(uint8_t* base, uint32_t wave, uint32_t pause, const Thread& thread,
                   uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D1ED8 XACTWave::Pause");
  const uint8_t flags = guest::LoadU8(base, wave + 25);
  const int32_t paused = (flags & 0x80) == 0x80 ? 1 : 0;
  if (int32_t(pause) == paused) {
    return 0;
  }
  const uint8_t now = uint8_t((flags & 0x7F) | (pause != 0 ? 0x80 : 0));
  const uint32_t voice = Load(base, wave + 44);
  guest::StoreU8(base, wave + 25, now);
  if (voice == 0) {
    return 0;
  }
  if (int32_t(pause) != 0) {
    return VoiceSlotStop(base, voice, 1, frame, 0x825D1F58, call);
  }
  if ((now & 0x2) == 0) {
    return 0;
  }
  const uint32_t r30 = VoiceSlotStart(base, voice, 1, thread, frame, 0x825D1F80, call);
  if (int32_t(r30) < 0) {
    return r30;
  }
  Commit(base, wave, frame, call);
  if ((guest::LoadU8(base, wave + 25) & 0x40) == 0) {
    return r30;
  }
  return VoiceSlotFlush(base, Load(base, wave + 44), frame, 0x825D1FB8, call);
}

uint32_t WavePrepareNext(uint8_t* base, uint32_t wave, bool native_heap, uint32_t frame,
                         const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D2778 XACTWave::PrepareNext");
  const uint8_t flags = guest::LoadU8(base, wave + 25);
  if ((flags & 0x8) == 0) {
    return wave;
  }
  guest::StoreU8(base, wave + 25, flags & 0xF7);
  if (guest::LoadU8(base, wave + 24) == 0) {
    return wave;
  }
  const uint32_t owner = Load(base, wave + 48);
  uint32_t r3 = CallAt(call, xa::Method(base, owner, 108), 0x825D27D0,
                       Args({owner, frame + 82, frame + 80}), frame);
  if (int32_t(r3) < 0) {
    return r3;
  }
  const uint32_t index = guest::LoadU8(base, frame + 80);
  const uint32_t sound_bank = Load(base, CueOf(base, Load(base, wave + 48)) + 296);
  std::optional<uint32_t> found;
  if (native_heap) {
    SetRegs(call, Args({sound_bank, index}));
    found = SoundBankGetWaveBank(base, sound_bank, index);
  }
  const uint32_t wave_bank =
      found ? *found : CallAt(call, 0x825C7218, 0x825D27F8, Args({sound_bank, index}), frame);
  if (wave_bank == 0) {
    return 0;
  }
  r3 = CallAt(call, xa::Method(base, wave_bank, 40), 0x825D2820,
              Args({wave_bank, guest::LoadU16(base, frame + 82), Load(base, wave + 48),
                    frame + 84}),
              frame);
  if (int32_t(r3) < 0) {
    return r3;
  }
  uint32_t count = guest::LoadU8(base, wave + 24);
  if (count != 255) {
    count = (count + 255) & 0xFF;
  }
  uint32_t next = Load(base, frame + 84);
  r3 = CallAt(call, xa::Method(base, next, 4), 0x825D2868,
              Args({next, count, count != 0 ? 1u : 0u, 0}), frame);
  if (int32_t(r3) >= 0) {
    next = Load(base, frame + 84);
    const uint32_t list = wave_bank + 240;
    Store(base, wave + 56, next);
    Store(base, next + 4, list);
    Store(base, next + 8, Load(base, list + 8));
    Store(base, Load(base, list + 8) + 4, next);
    Store(base, list + 8, next);
    return r3;
  }
  next = Load(base, frame + 84);
  if (next == 0) {
    return 0;
  }
  return CallAt(call, xa::Method(base, next, 0), 0x825D28B8, Args({next, 1}), frame);
}

uint32_t InMemoryWaveUpdate(uint8_t* base, uint32_t wave, bool native_heap, uint32_t frame,
                            const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D3890 XACTInMemoryWave::Update");
  SetRegs(call, Args({wave}));
  uint32_t r3 = WavePrepareNext(base, wave, native_heap, xa::FrameBelow(base, frame, 112), call);
  if ((guest::LoadU8(base, wave + 25) & 0x10) != 0) {
    return CallAt(call, xa::Method(base, wave, 0), 0x825D38D0, Args({wave, 1}), frame);
  }
  const uint32_t voice = Load(base, wave + 44);
  if (voice == 0 || int32_t(Load(base, wave + 40)) != 5) {
    return r3;
  }
  const uint32_t renderer = Load(base, Load(base, wave + 20) + 12);
  CallAt(call, xa::Method(base, renderer, 12), 0x825D3914, Args({renderer, voice}), frame);
  SetRegs(call, Args({wave, 0}));
  return WaveSetVoice(base, wave, 0, xa::FrameBelow(base, frame, 112), call);
}

// --- XACT's clock and notifications ---

namespace {

constexpr uint32_t kGetTimeMs = 0x825BFE28;
constexpr uint32_t kMatchRegistration = 0x825BFCC8;  // never seen in play
constexpr uint32_t kSendNotificationFrame = 112;
constexpr uint32_t kNotifyFrame = 160;

void StoreU8(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU8(base, address, uint8_t(value));
}
void StoreU16(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU16(base, address, uint16_t(value));
}
void StoreU64(uint8_t* base, uint32_t address, uint64_t value) {
  Store(base, address, uint32_t(value >> 32));
  Store(base, address + 4, uint32_t(value));
}
uint64_t LoadU64(uint8_t* base, uint32_t address) {
  return (uint64_t(Load(base, address)) << 32) | Load(base, address + 4);
}
// A word at an unaligned place as the original stores it: its top byte,
// the middle 16 bits, then its low byte.
void StoreSplit(uint8_t* base, uint32_t address, uint32_t value) {
  StoreU8(base, address, value >> 24);
  StoreU16(base, address + 1, value >> 8);
  StoreU8(base, address + 3, value);
}
// The engine's +20 exchanged atomically (the original's lwarx/stwcx. loop).
void ExchangeWord(uint8_t* base, uint32_t address, uint32_t value) {
  auto* word = reinterpret_cast<uint32_t*>(REX_RAW_ADDR(address));
  uint32_t seen = __atomic_load_n(word, __ATOMIC_SEQ_CST);
  while (!__atomic_compare_exchange_n(word, &seen, __builtin_bswap32(value), false,
                                      __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {
  }
}

}  // namespace

uint32_t ClockMs(uint8_t* base, uint32_t return_address, uint32_t frame, const GuestCall& call) {
  if (const auto ms = GetTimeMs(base)) {
    return uint32_t(*ms);
  }
  return CallAt(call, kGetTimeMs, return_address, Args({}), frame);
}

uint32_t EngineSendNotification(uint8_t* base, uint32_t engine, const uint64_t words[4],
                                uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825C1388 XACTEngine::SendNotification");
  const uint32_t notification = frame + 144;
  for (uint32_t i = 0; i < 4; i++) {
    StoreU64(base, notification + 8 * i, words[i]);
  }
  if (Load(base, engine + 16) == 0) {
    return engine;
  }
  const uint32_t now = ClockMs(base, 0x825C13C4, frame, call);
  StoreU8(base, notification + 4, now);
  StoreU8(base, notification + 1, now >> 24);
  StoreU16(base, notification + 2, now >> 8);
  const uint32_t list = engine + 100;
  uint32_t node = Load(base, engine + 104);
  if (node == list) {
    return now;
  }
  bool matched = false;
  uint32_t r3 = now;
  do {
    const uint32_t description = node + 16;
    const uint32_t next = Load(base, node + 4);
    r3 = CallAt(call, kMatchRegistration, 0x825C13FC, Args({engine, notification, description}),
                frame);
    if (r3 != 0) {
      matched = true;
      StoreSplit(base, notification + 5, Load(base, description + 16));
      if ((guest::LoadU8(base, description + 1) & 1) == 0) {
        // Moved to the tail of the list +112.
        const uint32_t done = engine + 112;
        Store(base, Load(base, node + 4) + 8, Load(base, node + 8));
        Store(base, Load(base, node + 8) + 4, Load(base, node + 4));
        Store(base, node + 8, node);
        Store(base, node + 4, done);
        Store(base, node + 8, Load(base, done + 8));
        Store(base, Load(base, done + 8) + 4, node);
        Store(base, done + 8, node);
      }
    }
    node = next;
  } while (node != list);
  if (!matched) {
    return r3;
  }
  ExchangeWord(base, engine + 20, graphics::video_output::CurrentThreadId());
  r3 = CallAt(call, Load(base, engine + 16), 0x825C14E4, Args({notification}), frame);
  ExchangeWord(base, engine + 20, 0xFFFFFFFFu);
  return r3;
}

uint32_t WaveNotify(uint8_t* base, uint32_t wave, uint32_t type, uint32_t frame,
                    const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D2030 XACTWave::Notify");
  const uint32_t bank = Load(base, wave + 36);
  StoreU8(base, frame + 80, 0);
  // memset (sub_825A71D0) of 28 bytes from +81, its r3 the destination.
  for (uint32_t i = 0; i < 28; i++) {
    StoreU8(base, frame + 81 + i, 0);
  }
  const uint32_t cleared = frame + 81;
  if (bank == 0) {
    return cleared;
  }
  const uint32_t owner = Load(base, wave + 48);
  if (owner != 0) {
    const uint32_t cue = CueOf(base, owner);
    const uint32_t sound_bank = Load(base, cue + 296);
    if (sound_bank == 0) {
      return cleared;
    }
    const uint32_t index = guest::LoadU16(base, cue + 304);
    StoreU8(base, frame + 95, index >> 8);
    StoreU8(base, frame + 96, index);
    StoreSplit(base, frame + 97, sound_bank);
    StoreSplit(base, frame + 101, cue);
  } else {
    StoreSplit(base, frame + 97, 0);
    StoreSplit(base, frame + 101, 0);
    StoreU8(base, frame + 95, 0xFF);
    StoreU8(base, frame + 96, 0xFF);
  }
  const uint32_t index = guest::LoadU16(base, wave + 52);
  StoreU8(base, frame + 80, type);
  StoreU8(base, frame + 92, bank);
  StoreU8(base, frame + 93, index >> 8);
  StoreU8(base, frame + 94, index);
  const uint32_t engine = Load(base, wave + 20);
  uint64_t words[4];
  words[0] = LoadU64(base, frame + 80);
  words[2] = LoadU64(base, frame + 96);
  // The last word's top five bytes (+104 to +108; the three after them
  // aren't the notification's).
  words[3] = (uint64_t(Load(base, frame + 104)) << 32) |
             (uint64_t(guest::LoadU8(base, frame + 108)) << 24);
  StoreU8(base, frame + 89, bank >> 24);
  StoreU16(base, frame + 90, bank >> 8);
  words[1] = LoadU64(base, frame + 88);
  SetRegs(call, Args({engine, words[0], words[1], words[2], words[3]}));
  return EngineSendNotification(base, engine, words,
                                xa::FrameBelow(base, frame, kSendNotificationFrame), call);
}

uint32_t WaveSetState(uint8_t* base, uint32_t wave, uint32_t state, uint32_t frame,
                      const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D24D8 XACTWave::SetState");
  uint32_t r3 = wave;
  const int32_t now = int32_t(Load(base, wave + 40));
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
    Store(base, wave + 40, step);
    SetRegs(call, Args({wave, type}));
    r3 = WaveNotify(base, wave, type, xa::FrameBelow(base, frame, kNotifyFrame), call);
  }
  Store(base, wave + 40, state);
  return r3;
}

// --- Releasing and stopping waves ---

namespace {

constexpr uint32_t kReleaseFrame = 112;
constexpr uint32_t kDestructFrame = 96;
constexpr uint32_t kSetStateFrame = 112;
constexpr uint32_t kListBaseVtable = 0x82075344;  // XACT's base vtable

}  // namespace

uint32_t WaveRelease(uint8_t* base, uint32_t wave, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D28D0 XACTWave::Release");
  guest::StoreU8(base, wave + 25, uint8_t(guest::LoadU8(base, wave + 25) | 0x4));
  const uint32_t next_node = Load(base, wave + 4);
  if (next_node != wave) {
    const uint32_t previous = Load(base, wave + 8);
    if (previous != wave) {
      Store(base, next_node + 8, previous);
      Store(base, Load(base, wave + 8) + 4, Load(base, wave + 4));
      Store(base, wave + 4, wave);
      Store(base, wave + 8, wave);
    }
  }
  const uint32_t next = Load(base, wave + 56);
  if (next != 0) {
    CallAt(call, xa::Method(base, next, 0), 0x825D2944, Args({next, 1}), frame);
  }
  const uint32_t owner = Load(base, wave + 48);
  if (owner != 0) {
    // r4 as the original leaves it (the caller's, or 1 after the next wave).
    xa::Regs regs;
    regs.r[regs.count++] = owner;
    CallAt(call, xa::Method(base, owner, 28), 0x825D2960, regs, frame);
  }
  const uint32_t voice = Load(base, wave + 44);
  if (voice != 0) {
    const uint32_t renderer = Load(base, Load(base, wave + 20) + 12);
    CallAt(call, xa::Method(base, renderer, 12), 0x825D2984, Args({renderer, voice}), frame);
  }
  const int32_t state = int32_t(Load(base, wave + 40));
  if (state >= 3 && state < 5) {
    Store(base, wave + 40, 5);
    SetRegs(call, Args({wave, 13}));
    WaveNotify(base, wave, 13, xa::FrameBelow(base, frame, kNotifyFrame), call);
  }
  Store(base, wave + 40, 5);
  const uint32_t listener = Load(base, wave + 36) + 4;
  return CallAt(call, xa::Method(base, listener, 4), 0x825D29CC, Args({listener, wave + 12}),
                frame);
}

uint32_t WaveDestruct(uint8_t* base, uint32_t wave, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D29E8 XACTWave::~XACTWave");
  Store(base, wave + 0, kWaveVtable);
  Store(base, wave + 12, kWaveListenerVtable);
  uint32_t r3 = wave;
  if ((guest::LoadU8(base, wave + 25) & 0x4) == 0) {
    SetRegs(call, Args({wave}));
    r3 = WaveRelease(base, wave, xa::FrameBelow(base, frame, kReleaseFrame), call);
  }
  Store(base, wave + 0, kListBaseVtable);
  return r3;
}

uint32_t InMemoryWaveDeletingDestruct(uint8_t* base, uint32_t wave, uint32_t flags,
                                      uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D3820 XACTInMemoryWave::~XACTInMemoryWave (deleting)");
  Store(base, wave + 0, kInMemoryWaveVtable);
  Store(base, wave + 12, kWaveListenerVtable);
  SetRegs(call, Args({wave}));
  WaveDestruct(base, wave, xa::FrameBelow(base, frame, kDestructFrame), call);
  if ((flags & 1) != 0) {
    SetRegs(call, Args({wave, kInMemoryWaveAttributes}));
    memory::crt::XMemFree(wave, kInMemoryWaveAttributes);
  }
  return wave;
}

uint32_t InMemoryWaveStop(uint8_t* base, uint32_t wave, uint32_t mode, uint32_t exit_loop,
                          uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825D3A28 XACTInMemoryWave::Stop");
  const auto set_state = [&](uint32_t state) {
    SetRegs(call, Args({wave, state}));
    WaveSetState(base, wave, state, xa::FrameBelow(base, frame, kSetStateFrame), call);
  };
  const int32_t state = int32_t(Load(base, wave + 40));
  if (state == 5) {
    return 0;
  }
  if (state == 4 && (int32_t(Load(base, wave + 16)) != 0 || int32_t(mode) != 0)) {
    return 0;
  }
  if (state != 3 && state != 4) {
    set_state(5);
    return 0;
  }
  if (mode == 0) {
    Store(base, wave + 16, 1);
    set_state(4);
    const uint32_t voice = Load(base, wave + 44);
    return VoiceSlotStop(base, voice, 1, frame, 0x825D3B04, call);
  }
  if (mode != 1) {
    return 0;
  }
  set_state(4);
  if (int32_t(exit_loop) == 0) {
    return 0;
  }
  const uint32_t voice = Load(base, wave + 44);
  guest::StoreU8(base, wave + 24, 0);
  return CallAt(call, xa::Method(base, voice, 52), 0x825D3AD0, Args({voice}), frame);
}

}  // namespace xact
