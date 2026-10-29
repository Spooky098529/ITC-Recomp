// XACT's voice class and waves (src/xact/waves.h,
// docs/functions/xact-waves.md) against the recompiled functions. The game's
// live voices and waves aren't touched: each case builds its objects in a
// block from the allocation layer, filled with the same random words for
// both versions, and runs the original (with --native_xact_waves off, so its
// callees are the originals too) or the plain C++ through the register steps
// (hooks/xact_waves.h). The block, r3 and the frame words the functions
// leave are compared with harness::Outputs: exactly, except the fields the
// code loads and stores as floats.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/heap.h"
#include "helpers/clock.h"
#include "hooks/xact_waves.h"
#include "hooks/xaudio.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "xact/waves.h"
#include "xaudio/voices.h"
#include "xact/xact.h"

#include <fmt/format.h>

#include <bit>
#include <initializer_list>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_xact_waves);
ITC_FLAG_DECLARE(bool, native_xact);

namespace harness {

namespace {

constexpr uint32_t kBlockBytes = 0x2000;

struct Block {
  Block() : address(memory::Alloc(kBlockBytes, 0)) {}
  ~Block() { memory::Free(address); }
  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;
  uint32_t address;
};

void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }

float Uniform(Random& random, float low, float high) {
  return low + (high - low) * float(random.Below(1000001)) / 1000000.0f;
}
// Now and then a special float (zeros, infinities, NaNs quiet and
// signaling, denormals, the extremes).
uint32_t Special(Random& random) {
  static constexpr uint32_t kSpecial[] = {0x00000000, 0x80000000, 0x7F800000, 0xFF800000,
                                          0x7FC00000, 0xFFC00000, 0x7F800001, 0xFFA00001,
                                          0x00000001, 0x807FFFFF, 0x7F7FFFFF, 0xFF7FFFFF};
  return kSpecial[random.Below(sizeof(kSpecial) / sizeof(kSpecial[0]))];
}
double AnyFloat(Random& random) {
  switch (random.Below(4)) {
    case 0:
      return double(std::bit_cast<float>(Special(random)));
    case 1:
      return double(Uniform(random, 0.0f, 2.0f));
    default:
      return double(int32_t(random.Next())) / 65536.0;
  }
}

// The block's bytes with the float fields (offsets in the block) left out
// as bytes and added as floats.
void BlockOutputs(Outputs& out, uint8_t* base, uint32_t block,
                  const std::vector<uint32_t>& floats) {
  std::vector<uint8_t> bytes = Read(base, block, kBlockBytes);
  for (const uint32_t offset : floats) {
    for (uint32_t i = 0; i < 4; i++) {
      bytes[offset + i] = 0;
    }
  }
  out.Bytes(bytes);
  for (const uint32_t offset : floats) {
    out.Float(guest::LoadU32(base, block + offset));
  }
}

// --- The voice over fake XAudio voices ---
//
// Ten fake XAudio voices at +0x800 + 0x100 k, each with its vtable at
// +0x1C00 (64 slots, each one of three leaf recorders: XAudioVoice::AddRef
// sub_8245F860, the count at +4 up and in r3; sub_820CE9F8, r4 and r5 to
// +444 and +448; sub_820B6950, f1 to +112 as a float), its codec (+76)
// another fake voice and its flags' pending bit (+61 bit 6) clear, so
// XAudio's entry points stay on the fake objects (as the car sounds'
// harness does). The count at +4 is sometimes near 0x9FFFFFFF, so AddRef's
// r3 is a failure. XACT's voice at +0x100 holds one of them at +4 (or 0).
// Compared: the block (the fake voices' +112 and +140 as floats), r3 and the
// frame's +80 to +112 below the stack pointer (GetState's state byte,
// SubmitPacket's packet).
constexpr uint32_t kVoices = 0x800;
constexpr uint32_t kVoiceBytes = 0x100;
constexpr uint32_t kVoiceCount = 10;
constexpr uint32_t kVoiceVtable = 0x1C00;
constexpr uint32_t kVoiceRecorders[] = {0x8245F860, 0x820CE9F8, 0x820B6950};
constexpr uint32_t kXactVoice = 0x100;

uint32_t FakeVoice(uint32_t block, uint32_t k) { return block + kVoices + kVoiceBytes * k; }

void FakeVoices(uint8_t* base, uint32_t block, Random& random) {
  for (uint32_t slot = 0; slot < 0x100; slot += 4) {
    Store(base, block + kVoiceVtable + slot, kVoiceRecorders[random.Below(3)]);
  }
  for (uint32_t k = 0; k < kVoiceCount; k++) {
    const uint32_t voice = FakeVoice(block, k);
    Store(base, voice, block + kVoiceVtable);
    Store(base, voice + 4, random.Below(4) == 0 ? 0x9FFFFFF0u + random.Below(32) : random.Below(3));
    Store(base, voice + 76, FakeVoice(block, random.Below(kVoiceCount)));
    guest::StoreU8(base, voice + 61, guest::LoadU8(base, voice + 61) & 0xBF);
  }
}

// XACT's voice at +0x100: the vtable, a fake XAudio voice at +4 (0 one time
// in `empty`), the rest random.
void FakeXactVoice(uint8_t* base, uint32_t block, Random& random, uint32_t empty) {
  const uint32_t voice = block + kXactVoice;
  Store(base, voice, xact::kVoiceVtable);
  Store(base, voice + 4,
        empty != 0 && random.Below(empty) == 0 ? 0 : FakeVoice(block, random.Below(kVoiceCount)));
}

// `r3_of(r3)` gives the r3 compared (the destructor's freed voice as found).
// The bytes below the stack pointer compared (from sp - 48): GetState's and
// SubmitPacket's frame words; none for the others, whose frames (and the
// tail-called entry points') hold only saved registers.
uint32_t stack_compared = 0;

template <typename Setup, typename R3>
void VoiceCase(Context& h, Results& results, uint32_t block, GuestFunction recompiled,
               GuestFunction native, Setup setup, R3 r3_of, const std::string& what) {
  const Random start = h.random;
  Random random = start;
  Outputs out[2];
  std::vector<uint32_t> floats;
  for (uint32_t k = 0; k < kVoiceCount; k++) {
    floats.push_back(kVoices + kVoiceBytes * k + 112);
    floats.push_back(kVoices + kVoiceBytes * k + 140);
  }
  for (int v = 0; v < 2; v++) {
    random = start;
    for (uint32_t i = 0; i < kBlockBytes; i += 4) {
      Store(h.base, block + i, uint32_t(random.Next()));
    }
    PPCContext t = CallContext(h);
    for (uint32_t i = 4; i <= 0x400; i += 4) {
      Store(h.base, t.r1.u32 - i, uint32_t(random.Next()));
    }
    FakeVoices(h.base, block, random);
    setup(random, t);
    ITC_FLAG(native_xact_waves) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_xact_waves) = true;
    BlockOutputs(out[v], h.base, block, floats);
    out[v].Word(r3_of(t.r3.u32));
    out[v].Bytes(Read(h.base, t.r1.u32 - 48, stack_compared));
  }
  h.random = random;
  results.Check(out[0], out[1], what);
}

template <typename Setup>
void VoiceCase(Context& h, Results& results, uint32_t block, GuestFunction recompiled,
               GuestFunction native, Setup setup, const std::string& what) {
  VoiceCase(h, results, block, recompiled, native, setup, [](uint32_t r3) { return r3; }, what);
}

void VoiceMethods(Context& h, uint32_t block) {
  const uint32_t voice = block + kXactVoice;
  {
    Results results("XACTVoice::GetState (sub_825C9DB8)");
    stack_compared = 32;
    for (int c = 0; c < 64; c++) {
      VoiceCase(h, results, block, __imp__sub_825C9DB8, hooks::XactVoiceGetState,
                [&](Random& random, PPCContext& t) {
                  FakeXactVoice(h.base, block, random, 0);
                  t.r3.u64 = voice;
                  t.r4.u64 = block + 0x300;
                },
                fmt::format("case {}", c));
    }
    stack_compared = 0;
    results.Report();
  }
  {
    Results results("XACTVoice::SetVolume (sub_825C9BE8)");
    for (int c = 0; c < 64; c++) {
      VoiceCase(h, results, block, __imp__sub_825C9BE8, hooks::XactVoiceSetVolume,
                [&](Random& random, PPCContext& t) {
                  FakeXactVoice(h.base, block, random, 0);
                  t.r3.u64 = voice;
                  t.f1.f64 = AnyFloat(random);
                },
                fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("XACTVoice::SetPitch (sub_825C9BF0)");
    for (int c = 0; c < 64; c++) {
      VoiceCase(h, results, block, __imp__sub_825C9BF0, hooks::XactVoiceSetPitch,
                [&](Random& random, PPCContext& t) {
                  FakeXactVoice(h.base, block, random, 0);
                  t.r3.u64 = voice;
                  t.f1.f64 = AnyFloat(random);
                },
                fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // XACT's 40-byte packet at +0x200: random words, the loop count 255 one
    // time in three.
    Results results("XACTVoice::SubmitPacket (sub_825C9E08)");
    stack_compared = 32;
    for (int c = 0; c < 64; c++) {
      VoiceCase(h, results, block, __imp__sub_825C9E08, hooks::XactVoiceSubmitPacket,
                [&](Random& random, PPCContext& t) {
                  FakeXactVoice(h.base, block, random, 0);
                  if (random.Below(3) == 0) {
                    Store(h.base, block + 0x208, 255);
                  } else if (random.Below(2) == 0) {
                    Store(h.base, block + 0x208, random.Below(256));
                  }
                  t.r3.u64 = voice;
                  t.r4.u64 = block + 0x200;
                  t.r5.u64 = random.Below(2) == 0 ? 0 : random.Next();
                },
                fmt::format("case {}", c));
    }
    stack_compared = 0;
    results.Report();
  }
  {
    // Deferred starts on playing voices only (+61 bit 0 set): StartLater
    // then answers E_UNEXPECTED without touching the engine's pending list.
    Results results("XACTVoice::Start (sub_825C9BF8)");
    for (int c = 0; c < 64; c++) {
      VoiceCase(h, results, block, __imp__sub_825C9BF8, hooks::XactVoiceStart,
                [&](Random& random, PPCContext& t) {
                  FakeXactVoice(h.base, block, random, 0);
                  t.r3.u64 = voice;
                  t.r4.u64 = random.Below(2) == 0 ? 0 : random.Next();
                  if (t.r4.u32 != 0) {
                    const uint32_t source = Load(h.base, voice + 4);
                    guest::StoreU8(h.base, source + 61, guest::LoadU8(h.base, source + 61) | 1);
                  }
                },
                fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("XACTVoice::Stop (sub_825C9C10)");
    for (int c = 0; c < 64; c++) {
      VoiceCase(h, results, block, __imp__sub_825C9C10, hooks::XactVoiceStop,
                [&](Random& random, PPCContext& t) {
                  FakeXactVoice(h.base, block, random, 0);
                  t.r3.u64 = voice;
                  t.r4.u64 = random.Below(2) == 0 ? 0 : random.Next();
                },
                fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("XACTVoice::FlushPackets (sub_825C9C28)");
    for (int c = 0; c < 32; c++) {
      VoiceCase(h, results, block, __imp__sub_825C9C28, hooks::XactVoiceFlushPackets,
                [&](Random& random, PPCContext& t) {
                  FakeXactVoice(h.base, block, random, 0);
                  t.r3.u64 = voice;
                },
                fmt::format("case {}", c));
    }
    results.Report();
  }
  if (!hooks::NativeHeapActive()) {
    ITC_LOG_INFO("harness: xact_waves: no native heap: the destructor's cases skipped");
    return;
  }
  {
    // Without bit 0 the voice in the block; with it a voice from XMemAlloc
    // (the block's voice copied in) freed by the call, compared as found.
    Results results("XACTVoice::~XACTVoice, deleting (sub_825CA448)");
    for (int c = 0; c < 64; c++) {
      uint32_t made = 0;
      VoiceCase(h, results, block, __imp__sub_825CA448, hooks::XactVoiceDeletingDestruct,
                [&](Random& random, PPCContext& t) {
                  FakeXactVoice(h.base, block, random, 3);
                  t.r4.u64 = random.Below(2) == 0 ? random.Next() & ~1ull : random.Next() | 1;
                  t.r3.u64 = voice;
                  made = 0;
                  if ((t.r4.u32 & 1) != 0) {
                    made = memory::crt::XMemAlloc(h.base, 24, xact::kVoiceAttributes);
                    for (uint32_t i = 0; i < 24; i += 4) {
                      Store(h.base, made + i, Load(h.base, voice + i));
                    }
                    t.r3.u64 = made;
                  }
                },
                [&](uint32_t r3) { return made != 0 ? (r3 == made ? 1u : 2u) : r3; },
                fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- Making a voice over the live engine ---
//
// XACTVoice::Create on a description copied from a live source voice (as the
// XAudio harness's XAudioCreateSourceVoice cases), the API lock entered and
// the spin lock held so the new XAudio voice can be taken back out of the
// engine's source list; then compared: r3, XACT's voice (its +4 as the
// offset into the XAudio voice's allocator block), that block with pointers
// into it as offsets, and the block; then both freed. The mastering voice's
// count, which default outputs AddRef, is put back. Without a reverb (as in
// play: the reverb submix is never made).
void Normalized(Outputs& out, uint8_t* base, uint32_t address, uint32_t bytes,
                uint32_t allocator, uint32_t allocator_bytes) {
  std::vector<uint8_t> words = Read(base, address, bytes);
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    const uint32_t word = guest::LoadU32(base, address + i);
    if (word >= allocator && word < allocator + allocator_bytes) {
      const uint32_t offset = 0xA110C000u ^ (word - allocator);
      for (uint32_t b = 0; b < 4; b++) {
        words[i + b] = uint8_t(offset >> (24 - 8 * b));
      }
    }
  }
  out.Bytes(words);
}

void ApiLock(uint8_t* base, const PPCContext& t, bool enter) {
  PPCContext c = t;
  c.r3.u64 = xaudio::kApiLock;
  if (enter) {
    __imp__RtlEnterCriticalSection(c, base);
  } else {
    __imp__RtlLeaveCriticalSection(c, base);
  }
}

uint32_t made_voices = 0;

void Creates(Context& h, uint32_t block) {
  const uint32_t engine = Load(h.base, xaudio::kEngine);
  if (engine == 0 || Load(h.base, engine + 64) == 0 || !hooks::NativeHeapActive()) {
    ITC_LOG_INFO("harness: xact_waves: no engine or native heap: XACTVoice::Create's cases skipped");
    return;
  }
  const uint32_t source_offset = Load(h.base, engine + 88);
  Results results("XACTVoice::Create (sub_825C9C40)");
  for (int c = 0; c < 16; c++) {
    const uint32_t node = Load(h.base, engine + 80);
    const uint32_t live = node != engine + 80 && node != 0 ? node - source_offset : 0;
    if (live == 0) {
      break;
    }
    const Random start = h.random;
    Random random = start;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      random = start;
      for (uint32_t i = 0; i < kBlockBytes; i += 4) {
        Store(h.base, block + i, uint32_t(random.Next()));
      }
      PPCContext t = CallContext(h);
      for (uint32_t i = 4; i <= 0x400; i += 4) {
        Store(h.base, t.r1.u32 - i, uint32_t(random.Next()));
      }
      const uint32_t description = block + 0x1E80;
      for (uint32_t k = 0; k < 92; k += 4) {
        Store(h.base, description + k, 0);
      }
      for (uint32_t k = 0; k < 56; k += 4) {
        Store(h.base, description + k, Load(h.base, live + 84 + k));
      }
      guest::StoreU8(h.base, description + 57, uint8_t(random.Below(3)));
      guest::StoreFloat(h.base, description + 60, 1.0f);
      const uint32_t out_word = block + 0x1FF4;
      Store(h.base, out_word, 0);
      t.r3.u64 = description;
      t.r4.u64 = block + 0x1F00;  // the reverb's holder (not read without one)
      t.r5.u64 = block + 0x1F04;  // the send level (likewise)
      t.r6.u64 = 0;
      t.r7.u64 = out_word;
      const uint32_t mastering = Load(h.base, engine + 64);
      const uint32_t count = Load(h.base, mastering + 4);
      const uint32_t active = Load(h.base, mastering + 40);
      ApiLock(h.base, t, true);
      PPCContext lock = t;
      hooks::XAudioSpinTake(lock, h.base);
      ITC_FLAG(native_xact_waves) = v == 1;
      (v == 0 ? __imp__sub_825C9C40 : hooks::XactVoiceCreate)(t, h.base);
      ITC_FLAG(native_xact_waves) = true;
      out[v].Word(t.r3.u32);
      const uint32_t xact_voice = Load(h.base, out_word);
      out[v].Word(xact_voice != 0);
      const uint32_t xaudio_voice = xact_voice != 0 ? Load(h.base, xact_voice + 4) : 0;
      out[v].Word(xaudio_voice != 0);
      uint32_t allocator = 0, bytes = 0;
      if (xaudio_voice != 0) {
        // Out of the engine's source list again.
        const uint32_t list = engine + 80, at = xaudio_voice + source_offset;
        const bool linked = Load(h.base, at) == list && Load(h.base, list + 4) == at;
        out[v].Word(linked);
        if (linked) {
          const uint32_t prev = Load(h.base, at + 4);
          Store(h.base, prev, list);
          Store(h.base, list + 4, prev);
          Store(h.base, at, 0);
          Store(h.base, at + 4, 0);
        }
        allocator = Load(h.base, xaudio_voice + 8);
        bytes = Load(h.base, allocator + 16) + 28;
        out[v].Word(xaudio_voice - allocator);
        out[v].Word(bytes);
      }
      hooks::XAudioSpinRelease(lock, h.base);
      ApiLock(h.base, t, false);
      Store(h.base, mastering + 4, count);
      Store(h.base, mastering + 40, active);
      if (xact_voice != 0) {
        Normalized(out[v], h.base, xact_voice, 24, allocator, bytes);
        out[v].Float(Load(h.base, xact_voice + 16));
        Store(h.base, out_word, 0);
        memory::crt::XMemFree(xact_voice, xact::kVoiceAttributes);
      }
      Normalized(out[v], h.base, block, kBlockBytes, allocator, bytes);
      if (allocator != 0) {
        Normalized(out[v], h.base, allocator, bytes, allocator, bytes);
        memory::crt::XMemFree(allocator, xaudio::kVoiceAttributes);
        made_voices += v;
      }
    }
    h.random = random;
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
  ITC_LOG_INFO("harness: xact_waves: XACTVoice::Create made {} voices natively", made_voices);
}


// --- The waves ---
//
// One case over the block (random words, the same for both versions), the
// inputs from `setup(random, t)`; compared: the block (exactly but the
// offsets in `floats`), r3, and what `after(out, t)` adds.
template <typename Setup, typename After>
void WaveCase(Context& h, Results& results, uint32_t block, GuestFunction recompiled,
              GuestFunction native, Setup setup, After after,
              const std::vector<uint32_t>& floats, const std::string& what) {
  const Random start = h.random;
  Random random = start;
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    random = start;
    for (uint32_t i = 0; i < kBlockBytes; i += 4) {
      Store(h.base, block + i, uint32_t(random.Next()));
    }
    PPCContext t = CallContext(h);
    for (uint32_t i = 4; i <= 0x400; i += 4) {
      Store(h.base, t.r1.u32 - i, uint32_t(random.Next()));
    }
    setup(random, t);
    ITC_FLAG(native_xact_waves) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_xact_waves) = true;
    BlockOutputs(out[v], h.base, block, floats);
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));  // r3's upper word too
    after(out[v], t);
  }
  h.random = random;
  results.Check(out[0], out[1], what);
}

constexpr uint32_t kWave = 0x400;   // 96 bytes
constexpr uint32_t kBank = 0x600;   // its +4 interface's vtable at +0x700
constexpr uint32_t kOwnerBlock = 0x1000;

void Waves(Context& h, uint32_t block) {
  const auto none = [](Outputs&, PPCContext&) {};
  // The constructors: the bank's +4 interface's slot +0 a recorder
  // (sub_820CE9F8, its r4 and r5 to +448 and +452 of the bank) or the real
  // AddListener (sub_825C8690) over a node pool at the bank's +24
  // (constructed, its 16 inline nodes free, so nothing is allocated).
  for (const bool in_memory : {false, true}) {
    Results results(in_memory ? "XACTInMemoryWave::XACTInMemoryWave (sub_825D37A8)"
                              : "XACTWave::XACTWave (sub_825D23F0)");
    for (int c = 0; c < 64; c++) {
      WaveCase(h, results, block, in_memory ? __imp__sub_825D37A8 : __imp__sub_825D23F0,
               in_memory ? hooks::XactInMemoryWaveConstruct : hooks::XactWaveConstruct,
               [&](Random& random, PPCContext& t) {
                 const uint32_t bank = block + kBank;
                 Store(h.base, bank + 4, block + 0x700);
                 const bool real = random.Below(2) == 0 && hooks::NativeHeapActive();
                 Store(h.base, block + 0x700, real ? xact::kWaveBankAddListener : 0x820CE9F8);
                 if (real) {
                   xact::NodePoolConstruct(h.base, bank + 24);
                 }
                 t.r3.u64 = block + kWave;
                 t.r4.u64 = random.Next();
                 t.r5.u64 = bank;
                 t.r6.u64 = random.Next();
                 t.r7.u64 = random.Next();
               },
               none, {}, fmt::format("case {}", c));
    }
    results.Report();
  }
  // The track getters: the track at +0x1000 (its sound at +0x1100, the
  // sound's record at +0x1200, engine at +0x1300 without global settings,
  // so the category's volume is 1.0, cue at +0x1400); the floats random
  // with specials now and then. Compared: f1 and r3 (the block isn't
  // written).
  const auto track_floats = [&](Random& random, uint32_t track) {
    const uint32_t sound = block + 0x1100;
    for (const uint32_t at : {track + 40, track + 44, track + 56, track + 60, sound + 8,
                              sound + 88, sound + 92, sound + 112, sound + 120,
                              block + 0x1400 + 28}) {
      guest::StoreFloat(h.base, at, float(AnyFloat(random)));
    }
    Store(h.base, track + 20, sound);
    Store(h.base, sound + 28, block + 0x1200);
    Store(h.base, sound + 32, block + 0x1300);
    Store(h.base, sound + 48, block + 0x1400);
    Store(h.base, block + 0x1300 + 124, 0);
  };
  const auto f1 = [](Outputs& out, PPCContext& t) { out.Double(t.f1.u64); };
  {
    Results results("XACTTrack::GetVolume (sub_825D0268)");
    for (int c = 0; c < 64; c++) {
      WaveCase(h, results, block, __imp__sub_825D0268, hooks::XactTrackGetVolume,
               [&](Random& random, PPCContext& t) {
                 track_floats(random, block + kOwnerBlock);
                 t.r3.u64 = block + kOwnerBlock;
               },
               f1, {}, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("XACTTrack::GetPitch (sub_825D02B0)");
    for (int c = 0; c < 64; c++) {
      WaveCase(h, results, block, __imp__sub_825D02B0, hooks::XactTrackGetPitch,
               [&](Random& random, PPCContext& t) {
                 track_floats(random, block + kOwnerBlock);
                 t.r3.u64 = block + kOwnerBlock;
               },
               f1, {}, fmt::format("case {}", c));
    }
    results.Report();
  }
  // GetStartPosition: an entry accessor at +0x800 with a rate (+16 bits
  // 5-22), a duration (+12 >> 4), a loop start (+28) and length (+32, 0 one
  // time in four: no loop region) of the game's sizes; the wave's byte +25
  // random (bit 0x20: looping), the count byte 0, 255 or small; offsets in
  // and past the loops. Never the original's trap (the count 255 with no
  // loop length and an offset past the loop start).
  {
    Results results("XACTWave::GetStartPosition (sub_825D2170)");
    for (int c = 0; c < 400; c++) {
      WaveCase(h, results, block, __imp__sub_825D2170, hooks::XactWaveGetStartPosition,
               [&](Random& random, PPCContext& t) {
                 const uint32_t entry = block + 0x800;
                 static constexpr uint32_t kRates[] = {48000, 44100, 22050, 1, 0x3FFFF};
                 const uint32_t rate = kRates[random.Below(5)];
                 Store(h.base, entry + 16,
                       (Load(h.base, entry + 16) & ~(0x3FFFFu << 5)) | (rate << 5));
                 const uint32_t duration = random.Below(4) == 0 ? random.Below(16)
                                                                : random.Below(2000000);
                 Store(h.base, entry + 12, (duration << 4) | (Load(h.base, entry + 12) & 15));
                 const uint32_t loop_start = random.Below(3) == 0 ? 0 : random.Below(duration + 1);
                 uint32_t loop_length = random.Below(4) == 0 ? 0
                                                              : 1 + random.Below(duration + 1);
                 Store(h.base, entry + 28, loop_start);
                 Store(h.base, entry + 32, loop_length);
                 uint8_t count = uint8_t(random.Below(3) == 0 ? 255 : random.Below(3) == 0
                                                                          ? 0
                                                                          : random.Below(6));
                 guest::StoreU8(h.base, block + 0x900, count);
                 const uint32_t loop = loop_length != 0 ? loop_length : duration;
                 if (count == 255 && loop == 0) {
                   guest::StoreU8(h.base, block + 0x900, 3);
                 }
                 const uint32_t ms = random.Below(5) == 0 ? uint32_t(random.Next())
                                                          : random.Below(200000);
                 t.r3.u64 = block + kWave;
                 t.r4.u64 = entry;
                 t.r5.u64 = ms;
                 t.r6.u64 = block + 0x904;
                 t.r7.u64 = block + 0x900;
               },
               none, {}, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("XACTWave::RequestNext (sub_825D1C30)");
    for (int c = 0; c < 32; c++) {
      WaveCase(h, results, block, __imp__sub_825D1C30, hooks::XactWaveRequestNext,
               [&](Random& random, PPCContext& t) {
                 t.r3.u64 = block + kWave;
                 t.r4.u64 = random.Next();
                 t.r5.u64 = random.Below(2) == 0 ? random.Below(2) : random.Next();
               },
               none, {}, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // The engine at +0x1800 with a critical section at +144 (initialized for
    // each version), held by this thread already one time in two (the wave's
    // Play under XACTCue::Play's lock), then left.
    Results results("XACTEngine::RequestCommit (sub_825D1C78)");
    for (int c = 0; c < 16; c++) {
      bool held = false;
      WaveCase(h, results, block, __imp__sub_825D1C78, hooks::XactEngineRequestCommit,
               [&](Random& random, PPCContext& t) {
                 const uint32_t engine = block + 0x1800;
                 PPCContext c2 = t;
                 c2.r3.u64 = engine + 144;
                 __imp__RtlInitializeCriticalSection(c2, h.base);
                 held = random.Below(2) == 0;
                 if (held) {
                   c2.r3.u64 = engine + 144;
                   __imp__RtlEnterCriticalSection(c2, h.base);
                 }
                 t.r3.u64 = engine;
               },
               [&](Outputs& out, PPCContext& t) {
                 // The lock as left (before the case lets go of it).
                 out.Bytes(Read(h.base, block + 0x1800 + 144, 28));
                 if (held) {
                   PPCContext c2 = t;
                   c2.r3.u64 = block + 0x1800 + 144;
                   __imp__RtlLeaveCriticalSection(c2, h.base);
                 }
               },
               {}, fmt::format("case {}", c));
    }
    results.Report();
  }
}


// --- The waves' calls over fake objects ---
//
// The owner (+0x1300, its cue at +0x1400 through +0x1340, +0x1380 and
// +0x13C0), the renderer (+0x1600, the engine's +12), the engine (+0x1800,
// its critical section at +144 initialized for each version), a sound bank
// (+0x1680, its cached wave banks at +0x1700), a wave bank (+0x1900, its
// wave list at +240 empty), a new wave (+0x1A00) and a voice (+0x1A40)
// share one vtable (+0x1D00) of XAudioVoice::AddRef recorders (the count at
// +4 up and in r3), each count random with a failure (near 0x9FFFFFFF) one
// time in four. The wave at +0x400 holds the engine, the owner (or none)
// and a voice: XACT's (+0x100, over the fake XAudio voices: its methods'
// plain C++), the fake one, or none.
constexpr uint32_t kOwner = 0x1300, kCue = 0x1400, kRenderer = 0x1600, kSoundBank = 0x1680,
                   kEngine = 0x1800, kWaveBank = 0x1900, kNewWave = 0x1A00, kFakeVoice = 0x1A40,
                   kFakeVtable = 0x1D00;

uint32_t Count(Random& random) {
  return random.Below(4) == 0 ? 0x9FFFFFF0u + random.Below(32) : random.Below(3);
}

void FakeObject(uint8_t* base, uint32_t block, uint32_t object, Random& random) {
  Store(base, object, block + kFakeVtable);
  Store(base, object + 4, Count(random));
}

void CallWorld(Context& h, uint32_t block, Random& random, PPCContext& t, bool playing) {
  uint8_t* base = h.base;
  FakeVoices(base, block, random);
  if (playing) {
    // XACT's voice's deferred starts on playing XAudio voices only.
    for (uint32_t k = 0; k < kVoiceCount; k++) {
      guest::StoreU8(base, FakeVoice(block, k) + 61, guest::LoadU8(base, FakeVoice(block, k) + 61) | 1);
    }
  }
  FakeXactVoice(base, block, random, 0);
  for (uint32_t slot = 0; slot < 0x100; slot += 4) {
    Store(base, block + kFakeVtable + slot, 0x8245F860);
  }
  for (const uint32_t at : {kOwner, kOwner + 0x40, kOwner + 0x80, kOwner + 0xC0, kRenderer,
                            kSoundBank, kWaveBank, kNewWave, kFakeVoice}) {
    FakeObject(base, block, block + at, random);
  }
  Store(base, block + kOwner + 44, block + kOwner + 0x40);
  Store(base, block + kOwner + 0x40 + 16, block + kOwner + 0x80);
  Store(base, block + kOwner + 0x80 + 20, block + kOwner + 0xC0);
  Store(base, block + kOwner + 0xC0 + 48, block + kCue);
  Store(base, block + kCue + 296, block + kSoundBank);
  Store(base, block + kSoundBank + 244, block + 0x1700);
  for (uint32_t i = 0; i < 4; i++) {
    Store(base, block + 0x1700 + 4 * i, block + kWaveBank);
  }
  Store(base, block + kWaveBank + 240, block + kWaveBank + 240);
  Store(base, block + kWaveBank + 244, block + kWaveBank + 240);
  Store(base, block + kWaveBank + 248, block + kWaveBank + 240);
  Store(base, block + kEngine + 12, block + kRenderer);
  PPCContext c = t;
  c.r3.u64 = block + kEngine + 144;
  __imp__RtlInitializeCriticalSection(c, base);
  const uint32_t wave = block + kWave;
  Store(base, wave + 20, block + kEngine);
  Store(base, wave + 48, random.Below(4) == 0 ? 0 : block + kOwner);
  const uint32_t pick = random.Below(4);
  Store(base, wave + 44, pick == 0 ? 0 : pick == 1 ? block + kFakeVoice : block + kXactVoice);
  t.r3.u64 = wave;
}

void WaveCalls(Context& h, uint32_t block) {
  std::vector<uint32_t> floats;
  for (uint32_t k = 0; k < kVoiceCount; k++) {
    floats.push_back(kVoices + kVoiceBytes * k + 112);
    floats.push_back(kVoices + kVoiceBytes * k + 140);
  }
  const auto none = [](Outputs&, PPCContext&) {};
  {
    // The cue's three flags each set or clear; the voice the fake one or
    // none (XACT's methods at +28, +32 and +48 aren't native).
    Results results("XACTWave::SetVoice (sub_825D1DF8)");
    for (int c = 0; c < 96; c++) {
      WaveCase(h, results, block, __imp__sub_825D1DF8, hooks::XactWaveSetVoice,
               [&](Random& random, PPCContext& t) {
                 CallWorld(h, block, random, t, false);
                 for (const uint32_t at : {348u, 316u, 72u}) {
                   Store(h.base, block + kCue + at, random.Below(2) == 0 ? 0 : uint32_t(random.Next()));
                 }
                 t.r4.u64 = random.Below(4) == 0 ? 0 : block + kFakeVoice;
               },
               none, floats, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("XACTInMemoryWave::Play (sub_825D3938)");
    for (int c = 0; c < 128; c++) {
      WaveCase(h, results, block, __imp__sub_825D3938, hooks::XactInMemoryWavePlay,
               [&](Random& random, PPCContext& t) {
                 const uint32_t deferred = random.Below(2) == 0 ? 0 : uint32_t(random.Next());
                 CallWorld(h, block, random, t, deferred != 0);
                 // Play reads its voice without a check (as in play, where
                 // Prepare made one): never none here.
                 if (Load(h.base, block + kWave + 44) == 0) {
                   Store(h.base, block + kWave + 44, block + kXactVoice);
                 }
                 t.r4.u64 = deferred;
               },
               none, floats, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("XACTWave::Pause (sub_825D1ED8)");
    for (int c = 0; c < 128; c++) {
      WaveCase(h, results, block, __imp__sub_825D1ED8, hooks::XactWavePause,
               [&](Random& random, PPCContext& t) {
                 CallWorld(h, block, random, t, true);
                 const uint32_t r = random.Below(4);
                 t.r4.u64 = r == 0 ? 0 : r == 1 ? 1 : random.Next();
               },
               none, floats, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // The owner's bank byte (the frame's +80, small) and the new wave (the
    // frame's +84, the fake one: the original reads it unchecked once
    // CreateWave succeeds) from the stack the case fills, as the owner's and
    // the bank's recorders leave them.
    Results results("XACTWave::PrepareNext (sub_825D2778)");
    for (int c = 0; c < 128; c++) {
      WaveCase(h, results, block, __imp__sub_825D2778, hooks::XactWavePrepareNext,
               [&](Random& random, PPCContext& t) {
                 CallWorld(h, block, random, t, false);
                 if (Load(h.base, block + kWave + 48) == 0) {
                   Store(h.base, block + kWave + 48, block + kOwner);
                 }
                 const uint32_t frame = t.r1.u32 - 112;
                 guest::StoreU8(h.base, frame + 80, uint8_t(random.Below(4)));
                 Store(h.base, frame + 84, block + kNewWave);
                 uint8_t flags = guest::LoadU8(h.base, block + kWave + 25);
                 if (random.Below(4) != 0) {
                   flags |= 0x8;
                 }
                 guest::StoreU8(h.base, block + kWave + 25, flags);
                 static constexpr uint8_t kCounts[] = {0, 1, 2, 5, 255};
                 guest::StoreU8(h.base, block + kWave + 24, kCounts[random.Below(5)]);
               },
               [&](Outputs& out, PPCContext& t) {
                 out.Bytes(Read(h.base, t.r1.u32 - 112 + 80, 8));
               },
               floats, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // The wave's own vtable the fake one (its deleting destructor a
    // recorder), finished (0x10) or stopped (state 5) now and then; the
    // stack words PrepareNext reads (its frame 112 bytes below Update's 96)
    // set as for PrepareNext.
    Results results("XACTInMemoryWave::Update (sub_825D3890)");
    for (int c = 0; c < 128; c++) {
      WaveCase(h, results, block, __imp__sub_825D3890, hooks::XactInMemoryWaveUpdate,
               [&](Random& random, PPCContext& t) {
                 CallWorld(h, block, random, t, false);
                 const uint32_t wave = block + kWave;
                 Store(h.base, wave, block + kFakeVtable);
                 if (Load(h.base, wave + 48) == 0) {
                   Store(h.base, wave + 48, block + kOwner);
                 }
                 const uint32_t frame = t.r1.u32 - 96 - 112;
                 guest::StoreU8(h.base, frame + 80, uint8_t(random.Below(4)));
                 Store(h.base, frame + 84, block + kNewWave);
                 uint8_t flags = guest::LoadU8(h.base, wave + 25) & ~0x18;
                 if (random.Below(4) == 0) {
                   flags |= 0x8;
                 }
                 if (random.Below(4) == 0) {
                   flags |= 0x10;
                 }
                 guest::StoreU8(h.base, wave + 25, flags);
                 guest::StoreU8(h.base, wave + 24, uint8_t(random.Below(3)));
                 Store(h.base, wave + 40, random.Below(2) == 0 ? 5 : random.Below(6));
                 // SetVoice with 0 reads nothing more; the owner's cue flags
                 // as SetVoice's cases.
               },
               none, floats, fmt::format("case {}", c));
    }
    results.Report();
  }
}

// XACT's notifications: XACTEngine::SendNotification (xact.md's, under
// --native_xact) with the engine's callback (+16) none or a recorder
// (XAudioVoice::AddRef, the count at the notification's +4 up) and 0 to 2
// registrations (+100; nodes at +0x1B00 and +0x1B60, their descriptions'
// types the notification's one time in two, persistent or not; matched by
// the original sub_825BFCC8, a leaf), the list +112 empty; XACTWave::Notify
// over CallWorld's wave (its bank none one time in four, the owner's cue's
// sound bank none one time in four); XACTWave::SetState from every state to
// every state. Both versions read the same time (the clocks' stand-ins, on
// this thread only). Compared besides the block: the notifications in the
// frames (the one built in Notify's frame, +80 to +111, and the one
// SendNotification stores in its caller's parameter area, its frame +144).
uint32_t Pick(Random& random, std::initializer_list<uint32_t> choices) {
  return *(choices.begin() + random.Below(uint32_t(choices.size())));
}

void Notifications(Context& h, uint32_t block) {
  uint8_t* base = h.base;
  std::vector<uint32_t> floats;
  for (uint32_t k = 0; k < kVoiceCount; k++) {
    floats.push_back(kVoices + kVoiceBytes * k + 112);
    floats.push_back(kVoices + kVoiceBytes * k + 140);
  }
  if (Load(base, 0x8266CDF0) != 0 && Load(base, 0x82971480) == 0 &&
      Load(base, 0x82971484) == 0) {
    ITC_LOG_ERROR("harness: xact_waves: XACT's frequency isn't set yet; notifications skipped");
    return;
  }
  const uint32_t engine = block + kEngine;
  const auto engine_world = [&](Random& random, uint8_t type) {
    // One time in two past 2^31 ms, so the counter's low word (XACTGetTimeMs's
    // r3, sign-extended) is negative.
    const uint32_t ms = random.Below(2) == 0 ? random.Below(100000)
                                             : 0x80000000u + random.Below(0x7FFFFFFF);
    helpers::SetClocksForHarness(uint64_t(ms) * 50000 + random.Below(50000), ms);
    Store(base, engine + 16, random.Below(3) == 0 ? 0 : 0x8245F860u);
    Store(base, engine + 20, uint32_t(random.Next()));
    const uint32_t list = engine + 100, done = engine + 112;
    Store(base, done + 4, done);
    Store(base, done + 8, done);
    uint32_t last = list;
    const uint32_t count = random.Below(3);
    for (uint32_t i = 0; i < count; i++) {
      const uint32_t node = block + 0x1B00 + 0x60 * i;
      const uint32_t description = node + 16;
      guest::StoreU8(base, description, random.Below(2) == 0 ? type : uint8_t(random.Below(16)));
      guest::StoreU8(base, description + 1, uint8_t(random.Next()));
      // The matcher's ids: 0 (any) one time in two.
      for (const uint32_t at : {2u, 6u, 10u}) {
        if (random.Below(2) == 0) {
          Store(base, description + at, 0);
        }
      }
      Store(base, last + 4, node);
      Store(base, node + 8, last);
      last = node;
    }
    Store(base, last + 4, list);
    Store(base, list + 8, last);
  };
  {
    Results results("XACTEngine::SendNotification (sub_825C1388)");
    for (int c = 0; c < 160; c++) {
      const Random start = h.random;
      Random random = start;
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        random = start;
        for (uint32_t i = 0; i < kBlockBytes; i += 4) {
          Store(base, block + i, uint32_t(random.Next()));
        }
        PPCContext t = CallContext(h);
        const uint8_t type = uint8_t(random.Below(16));
        engine_world(random, type);
        // r3's upper word one time in four not 0 (returned as given without a
        // callback, and passed to the matcher).
        t.r3.u64 = engine | (random.Below(4) == 0 ? random.Next() << 32 : 0);
        t.r4.u64 = (uint64_t(type) << 56) | (random.Next() & 0x00FFFFFFFFFFFFFFull);
        t.r5.u64 = random.Next();
        t.r6.u64 = random.Next();
        t.r7.u64 = random.Next();
        ITC_FLAG(native_xact) = v == 1;
        (v == 0 ? __imp__sub_825C1388 : hooks::XactEngineSendNotification)(t, base);
        ITC_FLAG(native_xact) = true;
        BlockOutputs(out[v], base, block, floats);
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));  // r3 in full
        out[v].Bytes(Read(base, t.r1.u32 + 32, 32));
      }
      h.random = random;
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  const auto frames = [&](uint32_t notify_frame) {
    return [&, notify_frame](Outputs& out, PPCContext& t) {
      out.Bytes(Read(base, t.r1.u32 - notify_frame + 80, 32));
      out.Bytes(Read(base, t.r1.u32 - notify_frame - 112 + 144, 32));
    };
  };
  const auto wave_world = [&](Random& random, PPCContext& t, uint8_t type) {
    CallWorld(h, block, random, t, false);
    engine_world(random, type);
    const uint32_t wave = block + kWave;
    Store(base, wave + 36, random.Below(4) == 0 ? 0 : block + kWaveBank);
    if (random.Below(4) == 0) {
      Store(base, block + kCue + 296, 0);
    }
  };
  {
    Results results("XACTWave::Notify (sub_825D2030)");
    for (int c = 0; c < 160; c++) {
      WaveCase(h, results, block, __imp__sub_825D2030, hooks::XactWaveNotify,
               [&](Random& random, PPCContext& t) {
                 const uint8_t type = uint8_t(Pick(random, {12, 13, random.Below(16)}));
                 wave_world(random, t, type);
                 t.r4.u64 = random.Below(4) == 0 ? uint32_t(random.Next()) : type;
               },
               frames(160), floats, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("XACTWave::SetState (sub_825D24D8)");
    for (int c = 0; c < 160; c++) {
      WaveCase(h, results, block, __imp__sub_825D24D8, hooks::XactWaveSetState,
               [&](Random& random, PPCContext& t) {
                 wave_world(random, t, uint8_t(Pick(random, {12, 13})));
                 Store(base, block + kWave + 40,
                       random.Below(8) == 0 ? uint32_t(random.Next()) : random.Below(7));
                 t.r4.u64 = random.Below(8) == 0 ? uint32_t(random.Next()) : random.Below(7);
                 // r3's upper word one time in four not 0 (returned as given
                 // without a notification).
                 if (random.Below(4) == 0) {
                   t.r3.u64 |= random.Next() << 32;
                 }
               },
               frames(112 + 160), floats, fmt::format("case {}", c));
    }
    results.Report();
  }
  helpers::SetClocksForHarness(std::nullopt, std::nullopt);
}

// Releasing and stopping waves over CallWorld's wave: its bank (never none:
// Release reads it unchecked) with its +4 interface's vtable the fake one
// (RemoveListener an AddRef recorder), the next wave (+56) none or the fake
// one, the wave linked into the bank's list (+240) or to itself, released
// (+25 0x4) or not, every state; Stop's voice never none (read unchecked),
// every mode, exit-the-loop set or not. Both versions read the same time.
// Compared besides the block: the notifications in Notify's frame and its
// SendNotification's (as Notifications' cases), at their depth below each.
void Releases(Context& h, uint32_t block) {
  uint8_t* base = h.base;
  std::vector<uint32_t> floats;
  for (uint32_t k = 0; k < kVoiceCount; k++) {
    floats.push_back(kVoices + kVoiceBytes * k + 112);
    floats.push_back(kVoices + kVoiceBytes * k + 140);
  }
  if (Load(base, 0x8266CDF0) != 0 && Load(base, 0x82971480) == 0 &&
      Load(base, 0x82971484) == 0) {
    ITC_LOG_ERROR("harness: xact_waves: XACT's frequency isn't set yet; releases skipped");
    return;
  }
  const uint32_t wave = block + kWave, bank = block + kWaveBank;
  const auto world = [&](Random& random, PPCContext& t) {
    CallWorld(h, block, random, t, false);
    const uint32_t ms = random.Below(100000);
    helpers::SetClocksForHarness(uint64_t(ms) * 50000 + random.Below(50000), ms);
    Store(base, block + kEngine + 16, random.Below(2) == 0 ? 0 : 0x8245F860u);
    const uint32_t list = block + kEngine + 100;
    Store(base, list + 4, list);
    Store(base, list + 8, list);
    Store(base, wave + 36, bank);
    Store(base, bank + 4, block + kFakeVtable);
    Store(base, wave + 56, random.Below(2) == 0 ? 0 : block + kNewWave);
    const uint32_t waves = bank + 240;
    if (random.Below(2) == 0) {
      Store(base, wave + 4, wave);
      Store(base, wave + 8, wave);
    } else {
      Store(base, waves + 4, wave);
      Store(base, waves + 8, wave);
      Store(base, wave + 4, waves);
      Store(base, wave + 8, waves);
    }
    uint8_t flags = guest::LoadU8(base, wave + 25) & ~0x4;
    if (random.Below(4) == 0) {
      flags |= 0x4;
    }
    guest::StoreU8(base, wave + 25, flags);
    Store(base, wave + 40, random.Below(8) == 0 ? uint32_t(random.Next()) : random.Below(7));
  };
  const auto frames = [&](uint32_t notify_frame) {
    return [&, notify_frame](Outputs& out, PPCContext& t) {
      out.Bytes(Read(base, t.r1.u32 - notify_frame + 80, 32));
      out.Bytes(Read(base, t.r1.u32 - notify_frame - 112 + 144, 32));
    };
  };
  struct Function {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
    uint32_t notify_frame;  // Notify's frame below the stack pointer
  };
  const Function functions[] = {
      {"XACTWave::Release (sub_825D28D0)", __imp__sub_825D28D0, hooks::XactWaveRelease,
       112 + 160},
      {"XACTWave::~XACTWave (sub_825D29E8)", __imp__sub_825D29E8, hooks::XactWaveDestruct,
       96 + 112 + 160},
      {"XACTInMemoryWave::~XACTInMemoryWave, deleting, kept (sub_825D3820)", __imp__sub_825D3820,
       hooks::XactInMemoryWaveDeletingDestruct, 112 + 96 + 112 + 160},
  };
  for (const Function& f : functions) {
    Results results(f.name);
    for (int c = 0; c < 128; c++) {
      WaveCase(h, results, block, f.recompiled, f.native,
               [&](Random& random, PPCContext& t) {
                 world(random, t);
                 t.r3.u64 = wave;
                 t.r4.u64 = uint32_t(random.Next()) & ~1u;  // kept: bit 0 clear
               },
               frames(f.notify_frame), floats, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // Freed: the wave from the allocation layer (XMemAlloc with its
    // attributes) for each version, a copy of the block's, linked to
    // itself; compared: the block and whether r3 is the wave.
    Results results("XACTInMemoryWave::~XACTInMemoryWave, deleting, freed (sub_825D3820)");
    if (hooks::NativeHeapActive()) {
      for (int c = 0; c < 48; c++) {
        WaveCase(h, results, block, __imp__sub_825D3820, hooks::XactInMemoryWaveDeletingDestruct,
                 [&](Random& random, PPCContext& t) {
                   world(random, t);
                   const uint32_t object =
                       memory::crt::XMemAlloc(base, 96, xact::kInMemoryWaveAttributes);
                   for (uint32_t i = 0; i < 96; i += 4) {
                     Store(base, object + i, Load(base, wave + i));
                   }
                   Store(base, object + 4, object);
                   Store(base, object + 8, object);
                   t.r3.u64 = object;
                   t.r4.u64 = 1 | (uint32_t(random.Next()) & ~1u);
                   t.r31.u64 = object;  // kept for the comparison below
                 },
                 [&](Outputs& out, PPCContext& t) {
                   out.Word(t.r3.u32 == t.r31.u32 ? 1 : 0);
                 },
                 floats, fmt::format("case {}", c));
      }
    }
    results.Report();
  }
  {
    Results results("XACTInMemoryWave::Stop (sub_825D3A28)");
    for (int c = 0; c < 192; c++) {
      WaveCase(h, results, block, __imp__sub_825D3A28, hooks::XactInMemoryWaveStop,
               [&](Random& random, PPCContext& t) {
                 world(random, t);
                 if (Load(base, wave + 44) == 0) {
                   Store(base, wave + 44, block + kXactVoice);
                 }
                 Store(base, wave + 16, random.Below(2) == 0 ? 0 : uint32_t(random.Next()));
                 t.r3.u64 = wave;
                 t.r4.u64 = Pick(random, {0, 0, 1, 1, 2, uint32_t(random.Next())});
                 t.r5.u64 = Pick(random, {0, 1, uint32_t(random.Next())});
               },
               frames(112 + 112 + 160), floats, fmt::format("case {}", c));
    }
    results.Report();
  }
  helpers::SetClocksForHarness(std::nullopt, std::nullopt);
}

// XACTInMemoryWave::Prepare over CallWorld's wave and a wave bank whose
// slots +16, +20 and +60 are the real GetEntry, GetSeekTable and
// GetWaveRegion (its vtable at +0x1E00, the rest AddRef recorders), with
// four 24-byte entries at +0x1F00 (PCM mostly, XMA one time in three, the
// other tags now and then; the game's rates, durations and loop regions, a
// seek table of one to four blocks); the owner's sound's record at
// +0x1E80 (its reverb flag 0x10 one time in two), its engine for the volume
// at +0x1200 without settings, the wave's engine's settings (+124) a fake
// object at +0x1560 (its +8 the reverb). The renderer's CreateVoice
// (sub_825C31D8) is the harness's stand-in (hooks::SetCreateVoiceForHarness:
// its registers to +0x1FC0, XACT's voice or the fake one handed back, an
// error one time in six). Counts 0, small, or 255 (never the
// GetStartPosition trap); offsets in and past the loops. Compared besides
// the block: Prepare's frame from +80 to +200 (the start, the reverb, the
// voice, the format, the entry's accessor, the packet) and the count byte at
// the caller's +31, and r3 in full.
//
// XACTWave::AudioUpdate (and Finish under it) over Releases' world: the
// request words (+28, +32) 0, 1 or random, every state, the flags (0x4,
// 0x10, 0x80) now and then, the owner's event flags without 0x08 (Done's
// helper list, never seen in play); compared besides the block: the voice's
// state word (the frame's +80), the notifications at Notify's depth (past
// the saved registers), r3 in full.
void Prepares(Context& h, uint32_t block) {
  uint8_t* base = h.base;
  std::vector<uint32_t> floats;
  for (uint32_t k = 0; k < kVoiceCount; k++) {
    floats.push_back(kVoices + kVoiceBytes * k + 112);
    floats.push_back(kVoices + kVoiceBytes * k + 140);
  }
  if (Load(base, 0x8266CDF0) != 0 && Load(base, 0x82971480) == 0 &&
      Load(base, 0x82971484) == 0) {
    ITC_LOG_ERROR("harness: xact_waves: XACT's frequency isn't set yet; prepares skipped");
    return;
  }
  const uint32_t wave = block + kWave, bank = block + kWaveBank;
  const auto clocks = [&](Random& random) {
    const uint32_t ms = random.Below(100000);
    helpers::SetClocksForHarness(uint64_t(ms) * 50000 + random.Below(50000), ms);
  };
  const auto engine_lists = [&](Random& random) {
    Store(base, block + kEngine + 16, random.Below(2) == 0 ? 0 : 0x8245F860u);
    const uint32_t list = block + kEngine + 100;
    Store(base, list + 4, list);
    Store(base, list + 8, list);
  };
  constexpr uint32_t kBankVtable = 0x1E00, kRecord = 0x1E80, kSoundEngine = 0x1200,
                     kEntries = 0x1F00, kHeader = 0x1F60, kSegment = 0x1F70,
                     kStandIn = 0x1FC0, kSettings = 0x1560;
  const auto prepare_world = [&](Random& random, PPCContext& t) {
    CallWorld(h, block, random, t, false);
    clocks(random);
    engine_lists(random);
    Store(base, wave + 36, bank);
    // The bank's vtable and data.
    for (uint32_t slot = 0; slot < 0x40; slot += 4) {
      Store(base, block + kBankVtable + slot, 0x8245F860);
    }
    Store(base, block + kBankVtable + 16, 0x825C8398);
    Store(base, block + kBankVtable + 20, 0x825C8580);
    Store(base, block + kBankVtable + 60, 0x825C8440);
    Store(base, bank, block + kBankVtable);
    Store(base, bank + 272, block + kEntries);
    Store(base, bank + 284, block + kHeader);
    Store(base, block + kHeader + 4, 4);
    Store(base, bank + 288, block + kSegment);
    const uint32_t blocks = 1 + random.Below(4);
    for (uint32_t i = 0; i < 4; i++) {
      Store(base, block + kSegment + 4 * i, 0);
    }
    const uint32_t table = block + kSegment + 16;
    Store(base, table, blocks);
    uint32_t total = 0;
    for (uint32_t i = 0; i < blocks; i++) {
      total += 1 + random.Below(400000);
      Store(base, table + 4 + 4 * i, total);
    }
    const uint32_t index = random.Below(4);
    guest::StoreU16(base, wave + 52, uint16_t(index));
    const uint32_t entry = block + kEntries + 24 * index;
    static constexpr uint32_t kRates[] = {48000, 44100, 22050, 32000};
    const uint32_t pick = random.Below(10);
    const uint32_t tag = pick < 6 ? 0 : pick < 9 ? 1 : 2 + random.Below(2);
    const uint32_t channels = 1 + random.Below(6);
    Store(base, entry + 4,
          (random.Below(2) << 31) | (kRates[random.Below(4)] << 5) | (channels << 2) | tag);
    const uint32_t duration =
        random.Below(4) == 0 ? random.Below(16) : random.Below(2000000);
    Store(base, entry + 0, (duration << 4) | random.Below(16));
    const uint32_t loop_start = random.Below(3) == 0 ? 0 : random.Below(duration + 1);
    const uint32_t loop_length = random.Below(4) == 0 ? 0 : 1 + random.Below(duration + 1);
    Store(base, entry + 16, loop_start);
    Store(base, entry + 20, loop_length);
    // The owner's sound, its record and the volumes' floats.
    const uint32_t sound = block + kOwner + 0xC0, track = block + kOwner + 0x80;
    Store(base, sound + 28, block + kRecord);
    guest::StoreU8(base, block + kRecord, uint8_t(random.Next()));
    Store(base, sound + 32, block + kSoundEngine);
    Store(base, block + kSoundEngine + 124, 0);
    for (const uint32_t at : {track + 40, track + 44, track + 56, track + 60, sound + 8,
                              sound + 88, sound + 92, sound + 112, sound + 116, sound + 120,
                              block + kCue + 28}) {
      guest::StoreFloat(base, at, float(AnyFloat(random)));
    }
    FakeObject(base, block, block + kSettings, random);
    Store(base, block + kEngine + 124, block + kSettings);
    // CreateVoice's stand-in, the renderer's slot +4 (its own vtable at
    // +0x1E40, the rest AddRef recorders).
    for (uint32_t slot = 0; slot < 0x40; slot += 4) {
      Store(base, block + 0x1E40 + slot, 0x8245F860);
    }
    Store(base, block + 0x1E40 + 4, 0x825C31D8);
    Store(base, block + kRenderer, block + 0x1E40);
    const uint32_t voice = random.Below(2) == 0 ? block + kXactVoice : block + kFakeVoice;
    if (voice == block + kXactVoice) {
      for (const uint32_t at : {348u, 316u, 72u}) {
        Store(base, block + kCue + at, 0);
      }
    }
    hooks::SetCreateVoiceForHarness(hooks::CreateVoiceStandIn{
        block + kStandIn, voice, random.Below(6) == 0 ? 0x8007000Eu : 0});
    // The count, whether it loops, the offset.
    uint32_t count = Pick(random, {0, 0, 1, 2, 5, 255});
    const uint32_t loop = loop_length != 0 ? loop_length : duration;
    if (count == 255 && loop == 0) {
      count = 3;
    }
    t.r3.u64 = wave;
    t.r4.u64 = (random.Next() & ~0xFFull) | count;
    t.r5.u64 = Pick(random, {0, 1, uint32_t(random.Next())});
    t.r6.u64 = random.Below(5) == 0 ? uint32_t(random.Next()) : random.Below(60000);
  };
  {
    Results results("XACTInMemoryWave::Prepare (sub_825D3B10)");
    for (int c = 0; c < 256; c++) {
      WaveCase(h, results, block, __imp__sub_825D3B10, hooks::XactInMemoryWavePrepare,
               prepare_world,
               [&](Outputs& out, PPCContext& t) {
                 out.Bytes(Read(base, t.r1.u32 - 320 + 80, 120));
                 out.Bytes(Read(base, t.r1.u32 + 31, 1));
               },
               floats, fmt::format("case {}", c));
    }
    results.Report();
  }
  hooks::SetCreateVoiceForHarness(std::nullopt);
  {
    Results results("XACTWave::AudioUpdate (sub_825D2560)");
    for (int c = 0; c < 256; c++) {
      WaveCase(h, results, block, __imp__sub_825D2560, hooks::XactWaveAudioUpdate,
               [&](Random& random, PPCContext& t) {
                 CallWorld(h, block, random, t, false);
                 clocks(random);
                 engine_lists(random);
                 Store(base, wave + 36, bank);
                 // A next wave only with an owner (the hand-off reads the
                 // owner unchecked; PrepareNext makes one only for an owner).
                 Store(base, wave + 56,
                       random.Below(2) == 0 || Load(base, wave + 48) == 0 ? 0
                                                                         : block + kNewWave);
                 for (const uint32_t at : {28u, 32u}) {
                   Store(base, wave + at, Pick(random, {0, 0, 1, 1, uint32_t(random.Next())}));
                 }
                 uint8_t flags = guest::LoadU8(base, wave + 25) & ~0x94;
                 for (const uint8_t bit : {0x4, 0x10, 0x80}) {
                   if (random.Below(bit == 0x80 ? 3 : 8) == 0) {
                     flags |= bit;
                   }
                 }
                 guest::StoreU8(base, wave + 25, flags);
                 Store(base, wave + 40,
                       random.Below(8) == 0 ? uint32_t(random.Next()) : random.Below(7));
                 const uint32_t event = block + kOwner;
                 guest::StoreU8(base, event + 40, guest::LoadU8(base, event + 40) & ~0x08);
                 if (random.Below(4) == 0) {
                   Store(base, block + kCue + 296, 0);
                 }
                 t.r3.u64 = wave;
               },
               [&](Outputs& out, PPCContext& t) {
                 out.Bytes(Read(base, t.r1.u32 - 128 + 80, 4));
                 out.Bytes(Read(base, t.r1.u32 - 128 - 160 + 80, 32));
                 // SendNotification's copy past its first 12 bytes: without a
                 // notification those hold the saved r31 and lr of XAudio's
                 // GetState under the voice's (the original's register saves,
                 // which plain C++ doesn't make); its time and context there
                 // are SendNotification's own cases'.
                 out.Bytes(Read(base, t.r1.u32 - 128 - 160 - 112 + 144 + 12, 20));
               },
               floats, fmt::format("case {}", c));
    }
    results.Report();
  }
  helpers::SetClocksForHarness(std::nullopt, std::nullopt);
}

}  // namespace

void XactWaves(Context& h) {
  Block block;
  if (!block.address) {
    ITC_LOG_ERROR("harness: xact_waves: no block from the allocation layer");
    return;
  }
  VoiceMethods(h, block.address);
  Creates(h, block.address);
  Waves(h, block.address);
  WaveCalls(h, block.address);
  Notifications(h, block.address);
  Releases(h, block.address);
  Prepares(h, block.address);
}

}  // namespace harness
