// The sound cues (src/sound_cues/, docs/functions/sound-cues.md) against
// the recompiled functions. The objects (the manager's listener and bank
// slots, bank pairs, Sounds, emitters, bank holders) are built in the
// scratch area, its other bytes left at the fill, and the whole area is
// compared after each case: floats where the function stores floats, bytes
// everywhere else. The originals run with --native_sound_cues off, so
// their callees are the recompiled code too. Cases where the original
// traps (a bank slot index past the vector's end, or no vector) aren't
// run: the native version hands those to the original.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/heap.h"
#include "memory/c_runtime_heap.h"
#include "sound_cues/sound_cues.h"

#include <fmt/format.h>

#include <bit>
#include <iterator>
#include <limits>
#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_sound_cues);

namespace harness {

namespace {

namespace sc = sound_cues;

// The register steps: the cases run the plain C++ through these (not the
// adapters), with the registers each adapter leaves.
#define ITC_NATIVE_STEP(name, body)           \
  void name(PPCContext& ctx, uint8_t* base) { \
    ctx.fpscr.disableFlushMode();             \
    body;                                     \
  }

ITC_NATIVE_STEP(NativeListenerConstruct, sc::ListenerConstruct(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeListenerSetOrientation,
                sc::ListenerSetOrientation(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_STEP(NativeListenerSetPosition,
                sc::ListenerSetPosition(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeSetVector36, sc::SetVector36(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeManagerSetListenerPosition,
                sc::ManagerSetListenerPosition(base, ctx.r3.u32, ctx.r4.u32);
                ctx.r3.u64 = ctx.r3.u32 + 1456)
ITC_NATIVE_STEP(NativeManagerSetListenerOrientation,
                sc::ManagerSetListenerOrientation(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
                ctx.r3.u64 = ctx.r3.u32 + 1456)
ITC_NATIVE_STEP(NativeManagerSetListenerVelocity,
                sc::ManagerSetListenerVelocity(base, ctx.r3.u32, ctx.r4.u32);
                ctx.r3.u64 = ctx.r3.u32 + 1456)
ITC_NATIVE_STEP(NativeManagerGetEngine, ctx.r3.u64 = sc::ManagerGetEngine(ctx.r3.u32))
ITC_NATIVE_STEP(NativeBankPairRelease, sc::BankPairRelease(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeBankPairSoundBank, ctx.r3.u64 = sc::BankPairSoundBank(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeBankPairIsLoading, ctx.r3.u64 = sc::BankPairIsLoading(base, ctx.r3.u32))

#define ITC_NATIVE_SLOT(name, function)                     \
  void name(PPCContext& ctx, uint8_t* base) {               \
    uint32_t result = 0;                                    \
    if (!function(base, ctx.r3.u32, ctx.r4.u32, &result)) { \
      ctx.r3.u64 = 0xDEADDEAD; /* not run: the original traps */ \
      return;                                               \
    }                                                       \
    ctx.r3.u64 = result;                                    \
  }
ITC_NATIVE_SLOT(NativeManagerReleaseBank, sc::ManagerReleaseBank)
ITC_NATIVE_SLOT(NativeManagerIsBankLoading, sc::ManagerIsBankLoading)
ITC_NATIVE_SLOT(NativeManagerIsBankSlotEmpty, sc::ManagerIsBankSlotEmpty)
ITC_NATIVE_SLOT(NativeManagerGetSoundBank, sc::ManagerGetSoundBank)

ITC_NATIVE_STEP(NativeSoundStart, sc::SoundStart(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeSoundSet3D, sc::SoundSet3D(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeSoundSetPosition,
                ctx.r3.u64 = sc::SoundSetPosition(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeSoundIsBank, ctx.r3.u64 = sc::SoundIsBank(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeSoundIsCue, ctx.r3.u64 = sc::SoundIsCue(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeEmitterConstruct, sc::EmitterConstruct(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeSoundBankHolderConstruct, sc::SoundBankHolderConstruct(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeWaveBankHolderConstruct, sc::WaveBankHolderConstruct(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeManagerIsCuePlaying,
                ctx.r3.u64 = sc::ManagerIsCuePlaying(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_STEP(NativeX3DAudioInitialize,
                ctx.r3.u64 = sc::X3DAudioInitialize(base, ctx.r3.u32, ctx.f1.f64, ctx.r5.u32))
ITC_NATIVE_STEP(NativeAudioEngineStart3D, ctx.r3.u64 = sc::AudioEngineStart3D(base, ctx.r3.u32))
void NativeDelegateClone(PPCContext& ctx, uint8_t* base) {
  uint32_t copy = 0;
  if (!sc::DelegateClone(base, ctx.r3.u32, &copy)) {
    ctx.r3.u64 = 0xDEADDEAD;  // operator new failed: the original's path
    return;
  }
  ctx.r3.u64 = copy;
}

// The odd float words mixed into inputs: NaNs of both kinds and signs,
// infinities, denormals, signed zeros.
constexpr uint32_t kOddFloats[] = {0x7FC00000, 0x7F800001, 0xFFC00001, 0xFF800001, 0x7F800000,
                                   0xFF800000, 0x00000001, 0x807FFFFF, 0x00000000, 0x80000000,
                                   0x7F7FFFFF, 0x3F800000};

uint32_t FloatWord(Random& random) {
  switch (random.Below(3)) {
    case 0:
      return kOddFloats[random.Below(std::size(kOddFloats))];
    case 1:
      return uint32_t(random.Next());
    default: {
      const float f = float(int32_t(random.Below(20001)) - 10000) / 7.0f;
      return std::bit_cast<uint32_t>(f);
    }
  }
}

void RandomWords(Context& h, uint32_t address, uint32_t count) {
  for (uint32_t i = 0; i < count; i++) {
    guest::StoreU32(h.base, address + 4 * i, FloatWord(h.random));
  }
}

// Where a case's function stores floats: [offset, end) in the scratch area.
using FloatRanges = std::vector<std::pair<uint32_t, uint32_t>>;

// The scratch area's outputs: the float ranges as floats, the rest as bytes,
// then r3.
void Collect(Context& h, Outputs& out, const FloatRanges& floats, const PPCContext& t) {
  uint32_t at = 0;
  for (const auto& [from, to] : floats) {
    if (from > at) {
      out.Bytes(Read(h.base, h.data + at, from - at));
    }
    out.Floats(h.base, h.data + from, (to - from) / 4);
    at = to;
  }
  out.Bytes(Read(h.base, h.data + at, kScratchBytes - at));
  out.Word(t.r3.u32);
}

// One case through both versions: the area filled, `setup()` writes the
// inputs (the same both times: it's handed a copy of the generator's state),
// `args(t)` sets the registers, the original (flag off) or the native step
// runs, and the area and r3 are compared.
template <typename Setup, typename Args>
void Case(Context& h, Results& results, Setup setup, Args args, GuestFunction recompiled,
          GuestFunction native, const FloatRanges& floats, const std::string& what) {
  Outputs out[2];
  const Random start = h.random;
  Random after;
  for (int v = 0; v < 2; v++) {
    h.random = start;
    Fill(h.base, h.data, kScratchBytes);
    setup();
    PPCContext t = CallContext(h);
    args(t);
    ITC_FLAG(native_sound_cues) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_sound_cues) = true;
    Collect(h, out[v], floats, t);
    after = h.random;
  }
  h.random = after;
  results.Check(out[0], out[1], what);
}

void Listener(Context& h) {
  Results results("sound_cues listener");
  const uint32_t d = h.data;
  for (int i = 0; i < 8; i++) {
    const bool filled = i % 2;
    Case(
        h, results, [&] { if (filled) RandomWords(h, d, 14); },
        [&](PPCContext& t) { t.r3.u64 = d; }, __imp__sub_8259D478, NativeListenerConstruct,
        {{0, 56}}, fmt::format("Listener::Listener {}", i));
  }
  for (int i = 0; i < 40; i++) {
    auto setup = [&] {
      RandomWords(h, d, 14);
      RandomWords(h, d + 512, 6);
    };
    Case(h, results, setup, [&](PPCContext& t) { t.r3.u64 = d; t.r4.u64 = d + 512;
                                                 t.r5.u64 = d + 524; },
         __imp__sub_8259D550, NativeListenerSetOrientation, {{0, 56}},
         fmt::format("Listener::SetOrientation {}", i));
    Case(h, results, setup, [&](PPCContext& t) { t.r3.u64 = d; t.r4.u64 = d + 512; },
         __imp__sub_8259D588, NativeListenerSetPosition, {{0, 56}},
         fmt::format("Listener::SetPosition {}", i));
    Case(h, results, setup, [&](PPCContext& t) { t.r3.u64 = d; t.r4.u64 = d + 516; },
         __imp__sub_8259CC80, NativeSetVector36, {{0, 56}},
         fmt::format("sub_8259CC80 {}", i));
    // The manager's thunks, on its listener at +1456.
    auto manager_setup = [&] {
      RandomWords(h, d + 1456, 14);
      RandomWords(h, d + 512, 6);
    };
    Case(h, results, manager_setup, [&](PPCContext& t) { t.r3.u64 = d; t.r4.u64 = d + 512; },
         __imp__sub_82117E08, NativeManagerSetListenerPosition, {{1456, 1512}},
         fmt::format("AudioManager::SetListenerPosition {}", i));
    Case(h, results, manager_setup, [&](PPCContext& t) { t.r3.u64 = d; t.r4.u64 = d + 512;
                                                         t.r5.u64 = d + 520; },
         __imp__sub_82117E10, NativeManagerSetListenerOrientation, {{1456, 1512}},
         fmt::format("AudioManager::SetListenerOrientation {}", i));
    Case(h, results, manager_setup, [&](PPCContext& t) { t.r3.u64 = d; t.r4.u64 = d + 512; },
         __imp__sub_82117E18, NativeManagerSetListenerVelocity, {{1456, 1512}},
         fmt::format("AudioManager::SetListenerVelocity {}", i));
  }
  for (uint32_t r3 : {0u, 0x40000000u, 0x82971478u, 0xFFFFFFF8u}) {
    Case(h, results, [] {}, [&](PPCContext& t) { t.r3.u64 = r3; }, __imp__sub_82117EA8,
         NativeManagerGetEngine, {}, fmt::format("AudioManager::GetEngine {:08X}", r3));
  }
  results.Report();
}

// A bank pair's loaded bytes and sound bank: one of the four ways, or
// random.
void RandomPair(Context& h, uint32_t pair, int kind) {
  RandomWords(h, pair, 28);
  const uint8_t a = kind < 4 ? uint8_t(kind & 1) : uint8_t(h.random.Next());
  const uint8_t b = kind < 4 ? uint8_t(kind >> 1) : uint8_t(h.random.Next());
  guest::StoreU8(h.base, pair + 108, a);
  guest::StoreU8(h.base, pair + 109, b);
}

void Banks(Context& h) {
  Results results("sound_cues banks");
  const uint32_t d = h.data;
  const uint32_t pair = d + 256;
  for (int i = 0; i < 24; i++) {
    auto setup = [&] { RandomPair(h, pair, i % 6); };
    auto args = [&](PPCContext& t) { t.r3.u64 = pair; };
    Case(h, results, setup, args, __imp__sub_82117760, NativeBankPairRelease, {},
         fmt::format("BankPair::Release {}", i));
    Case(h, results, setup, args, __imp__sub_82117770, NativeBankPairSoundBank, {},
         fmt::format("BankPair::SoundBank {}", i));
    Case(h, results, setup, args, __imp__sub_82117670, NativeBankPairIsLoading, {},
         fmt::format("BankPair::IsLoading {}", i));
  }
  // The manager at the area's start (its bank vector at +1516, +1520), the
  // slots at +2048, up to six pairs from +2304 (128 bytes apart), the index
  // at +3584.
  constexpr uint32_t kSlots = 2048, kPairs = 2304, kIndex = 3584;
  for (int i = 0; i < 120; i++) {
    const uint32_t count = 1 + h.random.Below(6);
    const uint32_t index = h.random.Below(count);
    const int function = i % 4;
    auto setup = [&] {
      guest::StoreU32(h.base, d + 1516, d + kSlots);
      guest::StoreU32(h.base, d + 1520, d + kSlots + 4 * count);
      for (uint32_t s = 0; s < count; s++) {
        // GetSoundBank reads the pair without a null check: always one there.
        const bool empty = function != 3 && h.random.Below(3) == 0;
        guest::StoreU32(h.base, d + kSlots + 4 * s, empty ? 0 : d + kPairs + 128 * s);
        RandomPair(h, d + kPairs + 128 * s, int(h.random.Below(6)));
      }
      guest::StoreU32(h.base, d + kIndex, index);
    };
    static constexpr GuestFunction kRecompiled[] = {__imp__sub_82118190, __imp__sub_82118200,
                                                    __imp__sub_821182E0, __imp__sub_82118400};
    static constexpr GuestFunction kNative[] = {
        NativeManagerReleaseBank, NativeManagerIsBankLoading, NativeManagerIsBankSlotEmpty,
        NativeManagerGetSoundBank};
    static constexpr const char* kNames[] = {"ReleaseBank", "IsBankLoading", "IsBankSlotEmpty",
                                             "GetSoundBank"};
    // GetSoundBank is on the interface at the manager's +8.
    const uint32_t r3 = function == 3 ? d + 8 : d;
    Case(h, results, setup, [&](PPCContext& t) { t.r3.u64 = r3; t.r4.u64 = d + kIndex; },
         kRecompiled[function], kNative[function], {},
         fmt::format("AudioManager::{} {} (index {} of {})", kNames[function], i, index, count));
  }
  for (int i = 0; i < 6; i++) {
    auto setup = [&] { RandomWords(h, d, 8); };
    auto args = [&](PPCContext& t) { t.r3.u64 = d + 4; };
    Case(h, results, setup, args, __imp__sub_8259D3D8, NativeSoundBankHolderConstruct, {},
         fmt::format("SoundBankHolder::SoundBankHolder {}", i));
    Case(h, results, setup, args, __imp__sub_8259D418, NativeWaveBankHolderConstruct, {},
         fmt::format("WaveBankHolder::WaveBankHolder {}", i));
  }
  results.Report();
}

void Sounds(Context& h) {
  Results results("sound_cues sounds");
  const uint32_t d = h.data;
  constexpr uint32_t kEmitter = 1024, kInput = 600;
  for (int i = 0; i < 40; i++) {
    const bool same = h.random.Below(2) == 0;
    auto setup = [&] {
      RandomWords(h, d, 6);
      guest::StoreU32(h.base, d + 4, d + kEmitter);
      RandomWords(h, d + kEmitter, 16);
      RandomWords(h, d + kInput, 4);
      if (same) {
        guest::StoreU32(h.base, d + kInput, guest::LoadU32(h.base, d + 16 + 4 * (i % 2)));
      }
    };
    auto args = [&](PPCContext& t) { t.r3.u64 = d; t.r4.u64 = d + kInput; };
    Case(h, results, setup, args, __imp__sub_821173F0, NativeSoundStart, {},
         fmt::format("Sound::Start {}", i));
    Case(h, results, setup, args, __imp__sub_82117568, NativeSoundSet3D, {},
         fmt::format("Sound::Set3D {}", i));
    Case(h, results, setup, args, __imp__sub_82117560, NativeSoundSetPosition,
         {{kEmitter, kEmitter + 64}}, fmt::format("Sound::SetPosition {}", i));
    Case(h, results, setup, args, __imp__sub_821175B8, NativeSoundIsBank, {},
         fmt::format("Sound::IsBank {}", i));
    Case(h, results, setup, args, __imp__sub_82117578, NativeSoundIsCue, {},
         fmt::format("Sound::IsCue {}", i));
  }
  // The emitter's constructor: where it stores floats.
  FloatRanges floats = {{12, 60}, {64, 68}, {92, 136}, {152, 160}};
  for (uint32_t i = 0; i < 6; i++) {
    floats.push_back({168 + 8 * i, 172 + 8 * i});
  }
  floats.push_back({224, 228});
  for (int i = 0; i < 6; i++) {
    const bool filled = i % 2;
    Case(
        h, results, [&] { if (filled) RandomWords(h, d, 80); },
        [&](PPCContext& t) { t.r3.u64 = d; }, __imp__sub_8259C9B0, NativeEmitterConstruct,
        floats, fmt::format("Emitter::Emitter {}", i));
  }
  results.Report();
}

void CueList(Context& h) {
  Results results("sound_cues cue list");
  const uint32_t d = h.data;
  // The manager at the area's start: the list's sentinel at +2048 (its
  // address at +1532), up to six nodes from +2064 (16 bytes apart), their
  // Sounds from +2400 (24 bytes apart), the bank and cue indexes at +3584
  // and +3588.
  constexpr uint32_t kSentinel = 2048, kNodes = 2064, kSounds = 2400, kIndexes = 3584;
  for (int i = 0; i < 200; i++) {
    const uint32_t count = h.random.Below(7);
    auto setup = [&] {
      RandomWords(h, d + 1528, 2);
      guest::StoreU32(h.base, d + 1532, d + kSentinel);
      uint32_t previous = d + kSentinel;
      for (uint32_t n = 0; n < count; n++) {
        const uint32_t node = d + kNodes + 16 * n;
        guest::StoreU32(h.base, previous, node);
        guest::StoreU32(h.base, node + 4, previous);
        guest::StoreU32(h.base, node + 8, d + kSounds + 24 * n);
        RandomWords(h, d + kSounds + 24 * n, 6);
        guest::StoreU32(h.base, d + kSounds + 24 * n + 16, h.random.Below(3));
        guest::StoreU32(h.base, d + kSounds + 24 * n + 20, h.random.Below(3));
        previous = node;
      }
      guest::StoreU32(h.base, previous, d + kSentinel);
      guest::StoreU32(h.base, d + kIndexes, h.random.Below(3));
      guest::StoreU32(h.base, d + kIndexes + 4, h.random.Below(3));
    };
    Case(h, results, setup,
         [&](PPCContext& t) { t.r3.u64 = d; t.r4.u64 = d + kIndexes; t.r5.u64 = d + kIndexes + 4; },
         __imp__sub_82118B58, NativeManagerIsCuePlaying, {},
         fmt::format("AudioManager::IsCuePlaying {} ({} Sounds)", i, count));
  }
  results.Report();

  // The delegate clone: each version's block differs, so its 16 bytes are
  // compared (and r3 being non-zero), and both blocks freed.
  Results clone("sound_cues delegate clone");
  if (hooks::NativeHeapActive()) {
    for (int i = 0; i < 20; i++) {
      Outputs out[2];
      const Random start = h.random;
      for (int v = 0; v < 2; v++) {
        h.random = start;
        Fill(h.base, h.data, kScratchBytes);
        RandomWords(h, d, 4);
        PPCContext t = CallContext(h);
        t.r3.u64 = d;
        ITC_FLAG(native_sound_cues) = v == 1;
        (v == 0 ? __imp__sub_82118720 : NativeDelegateClone)(t, h.base);
        ITC_FLAG(native_sound_cues) = true;
        const uint32_t block = t.r3.u32;
        out[v].Word(block != 0 && block != 0xDEADDEAD ? 1 : 0);
        if (block != 0 && block != 0xDEADDEAD) {
          out[v].Bytes(Read(h.base, block, 16));
          memory::crt::OperatorDelete(block);
        }
        out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      }
      clone.Check(out[0], out[1], fmt::format("sub_82118720 {}", i));
    }
  }
  clone.Report();
}

void X3DAudio(Context& h) {
  Results results("sound_cues X3DAudio");
  const uint32_t d = h.data;
  const uint32_t masks[] = {0, 1, 3, 4, 7, 8, 9, 15, 0x3F, 0xFF, 0x80000000u, 0xFFFFFFFFu};
  const double speeds[] = {343.5,
                           0.0,
                           -0.0,
                           -1.0,
                           1e-40,
                           1e300,
                           std::numeric_limits<double>::infinity(),
                           -std::numeric_limits<double>::infinity(),
                           std::numeric_limits<double>::quiet_NaN(),
                           std::bit_cast<double>(0xFFF0000000000001ull),
                           std::bit_cast<double>(0x36A0000000000000ull)};
  int n = 0;
  for (uint32_t mask : masks) {
    for (double speed : speeds) {
      Case(h, results, [&] { RandomWords(h, d, 8); },
           [&](PPCContext& t) { t.r3.u64 = mask; t.f1.f64 = speed; t.r5.u64 = d + 4; },
           __imp__sub_82480710, NativeX3DAudioInitialize, {{12, 20}},
           fmt::format("X3DAudioInitialize {:08X} {}", mask, speed));
      n++;
    }
  }
  for (int i = 0; i < 60; i++) {
    const uint32_t mask = uint32_t(h.random.Next());
    const double speed = double(std::bit_cast<float>(FloatWord(h.random)));
    Case(h, results, [&] { RandomWords(h, d, 8); },
         [&](PPCContext& t) { t.r3.u64 = mask; t.f1.f64 = speed; t.r5.u64 = d; },
         __imp__sub_82480710, NativeX3DAudioInitialize, {{8, 16}},
         fmt::format("X3DAudioInitialize random {}", i));
  }
  for (int i = 0; i < 4; i++) {
    Case(h, results, [&] { RandomWords(h, d, 40); },
         [&](PPCContext& t) { t.r3.u64 = d; }, __imp__sub_8259CE10, NativeAudioEngineStart3D,
         {{20, 28}}, fmt::format("AudioEngine::Start3D {}", i));
  }
  results.Report();
}

}  // namespace

void SoundCues(Context& h) {
  Listener(h);
  Banks(h);
  Sounds(h);
  CueList(h);
  X3DAudio(h);
}

}  // namespace harness
