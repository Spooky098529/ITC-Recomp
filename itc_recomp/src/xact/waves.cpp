// XACT's waves natively (waves.h, docs/functions/xact-waves.md): the
// waves' set-up and their leaves. The objects still recompiled (the owner's
// vtable, the renderer's slots, the wave bank's when they aren't the native
// functions) are called through the guest call; XACT's lock (the engine's
// critical section at +144) is the game's, entered and left through its
// imports as the original does. Strict floating point (CMakeLists.txt), as
// the original's single-precision steps.

#include "xact/waves.h"

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

}  // namespace xact
