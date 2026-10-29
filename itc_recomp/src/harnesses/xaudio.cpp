// XAudio's voices (src/xaudio/, docs/functions/xaudio.md) against the
// recompiled functions. The game's live voices aren't touched: each case
// builds its objects in a block from the allocation layer, filled with the
// same random words for both versions, and runs the original or the plain
// C++ through the register steps (hooks/xaudio.h). The block and r3 are
// compared with harness::Outputs, exactly: none of these functions loads or
// stores a float (the buffers' samples are cleared as bytes).

#include "harness.h"

#include "base/log.h"
#include "hooks/heap.h"
#include "hooks/xaudio.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "xaudio/voices.h"

#include <fmt/format.h>

#include <string>
#include <utility>
#include <vector>

namespace harness {

namespace {

namespace xa = xaudio;

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

// One case: the block filled with random words, then `setup(random, t)`
// (the same random state for both versions) writes the inputs and sets the
// registers; the block and r3 are compared.
template <typename Setup>
void Case(Context& h, Results& results, uint32_t block, GuestFunction recompiled,
          GuestFunction native, Setup setup, const std::string& what) {
  const Random start = h.random;
  Random random = start;
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    random = start;
    for (uint32_t i = 0; i < kBlockBytes; i += 4) {
      Store(h.base, block + i, uint32_t(random.Next()));
    }
    PPCContext t = CallContext(h);
    setup(random, t);
    (v == 0 ? recompiled : native)(t, h.base);
    out[v].Bytes(Read(h.base, block, kBlockBytes));
    out[v].Word(t.r3.u32);
  }
  h.random = random;
  results.Check(out[0], out[1], what);
}

// A rate as the voices hold them: 48,000 and its divisors most of the time,
// any value below 48,000 otherwise (never 0 or above 48,000: the original
// traps there, and the native version hands those to it).
uint32_t Rate(Random& random) {
  static constexpr uint32_t kRates[] = {48000, 24000, 16000, 12000, 8000, 44100, 22050, 32000};
  if (random.Below(4) != 0) {
    return kRates[random.Below(8)];
  }
  return 1 + random.Below(48000);
}

void Formats(Context& h, uint32_t block) {
  {
    Results results("XAudio::SourceOutputFormat (sub_8245DC88)");
    for (int i = 0; i < 64; i++) {
      Case(h, results, block, __imp__sub_8245DC88, hooks::XAudioSourceOutputFormat,
           [&](Random& random, PPCContext& t) {
             // Type 4 (XMA) half the time, with up to 8 streams.
             if (random.Below(2) == 0) {
               guest::StoreU8(h.base, block, 4);
               guest::StoreU8(h.base, block + 4, uint8_t(random.Below(i < 60 ? 9 : 256)));
             }
             t.r3.u64 = block;
             t.r4.u64 = block + 0x1000 + 4 * random.Below(16);
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("XAudioSubmixVoice::Describe (sub_8245F010)");
    for (int i = 0; i < 48; i++) {
      Case(h, results, block, __imp__sub_8245F010, hooks::XAudioSubmixDescribe,
           [&](Random& random, PPCContext& t) {
             // The count and channel bytes 0 now and then (their defaults).
             if (random.Below(3) == 0) {
               guest::StoreU8(h.base, block + 8, 0);
             }
             if (random.Below(3) == 0) {
               guest::StoreU8(h.base, block + 9, 0);
             }
             t.r3.u64 = block;
             t.r4.u64 = block + 0x1000;
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("XAudio::DefaultMatrix (sub_8245EC40)");
    int n = 0;
    // Every pair of 0-8 channels, and random words.
    for (uint32_t in = 0; in <= 8; in++) {
      for (uint32_t to = 0; to <= 8; to++) {
        Case(h, results, block, __imp__sub_8245EC40, hooks::XAudioDefaultMatrix,
             [&](Random& random, PPCContext& t) {
               t.r3.u64 = (random.Next() & ~0xFFull) | in;
               t.r4.u64 = (random.Next() & ~0xFFull) | to;
               t.r5.u64 = block + 0x100;
             },
             fmt::format("{} to {}", in, to));
        n++;
      }
    }
    for (int i = 0; i < 16; i++) {
      Case(h, results, block, __imp__sub_8245EC40, hooks::XAudioDefaultMatrix,
           [&](Random& random, PPCContext& t) {
             t.r3.u64 = random.Next();
             t.r4.u64 = random.Next();
             t.r5.u64 = block + 0x100;
           },
           fmt::format("random {}", i));
    }
    results.Report();
  }
}

void Buffers(Context& h, uint32_t block) {
  {
    Results results("XAudioBuffer::Size (sub_8245FA28)");
    for (int i = 0; i < 48; i++) {
      Case(h, results, block, __imp__sub_8245FA28, hooks::XAudioBufferSize,
           [&](Random& random, PPCContext& t) {
             // Type 0 most of the time (the only one with samples).
             guest::StoreU8(h.base, block, random.Below(4) != 0 ? 0 : uint8_t(random.Next()));
             guest::StoreU8(h.base, block + 5, uint8_t(random.Below(7)));
             Store(h.base, block + 8, Rate(random));
             t.r3.u64 = block;
             t.r4.u64 = block + 0x100;
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("XAudioBuffer::Clear (sub_8245F8E8)");
    // The buffer at +0 (its samples at +0x400), a format at +0x100.
    for (int i = 0; i < 96; i++) {
      Case(h, results, block, __imp__sub_8245F8E8, hooks::XAudioBufferClear,
           [&](Random& random, PPCContext& t) {
             // Capacity: 256 frames at 48 kHz of up to 6 channels fit the
             // block from +0x400 only for the smaller ones: keep the
             // buffer's channels and rates within it.
             guest::StoreU8(h.base, block + 13, uint8_t(1 + random.Below(6)));
             Store(h.base, block + 16, Rate(random));
             Store(h.base, block + 24, Rate(random));
             guest::StoreU8(h.base, block + 21, uint8_t(random.Below(7)));
             Store(h.base, block + 28, block + 0x400);
             const bool own = random.Below(3) == 0;
             guest::StoreU8(h.base, block + 0x101, uint8_t(random.Below(8)));
             Store(h.base, block + 0x104, Rate(random));
             // Cleared bytes stay within the block: at most 6 x 256 x 4
             // bytes past +0x400, which the block has.
             t.r3.u64 = block;
             t.r4.u64 = own ? 0 : block + 0x100;
             t.r5.u64 = random.Next();
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
}

void Accessors(Context& h, uint32_t block) {
  {
    Results results("XAudioVoice::EngineBuffer (sub_8245DEB0)");
    // The live engine's buffers, read only; r13 a stand-in PCR in the block.
    if (guest::LoadU32(h.base, xa::kEngine) != 0) {
      for (int i = 0; i < 12; i++) {
        Case(h, results, block, __imp__sub_8245DEB0, hooks::XAudioEngineBuffer,
             [&](Random&, PPCContext& t) {
               guest::StoreU8(h.base, block + 0x200 + xa::kPcrProcessor, uint8_t(i % 6));
               t.r13.u64 = block + 0x200;
             },
             fmt::format("processor {}", i % 6));
      }
    }
    results.Report();
  }
  struct Simple {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
  };
  const Simple simple[] = {
      {"XAudioVoice::GetType (sub_8245E318)", __imp__sub_8245E318, hooks::XAudioGetType},
      {"XAudioVoice::GetFormat (sub_8245E328)", __imp__sub_8245E328, hooks::XAudioGetFormat},
      {"XAudioVoice::SetFormatWords (sub_82461140)", __imp__sub_82461140,
       hooks::XAudioSetFormatWords},
      {"XAudioVoice::AddRef (sub_8245F860)", __imp__sub_8245F860, hooks::XAudioAddRef},
      {"sub_8245FB00 (the root vtable)", __imp__sub_8245FB00, hooks::XAudioSetRootVtable},
  };
  for (const Simple& s : simple) {
    Results results(s.name);
    for (int i = 0; i < 24; i++) {
      Case(h, results, block, s.recompiled, s.native,
           [&](Random& random, PPCContext& t) {
             t.r3.u64 = block + 4 * random.Below(64);
             t.r4.u64 = block + 0x1000 + 4 * random.Below(64);
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("XAudio::AddStat (sub_8245F640)");
    // The counter's index chosen so it lands in the block (the live
    // counters at 0x82954F24 aren't touched).
    for (int i = 0; i < 24; i++) {
      Case(h, results, block, __imp__sub_8245F640, hooks::XAudioAddStat,
           [&](Random& random, PPCContext& t) {
             const uint32_t counter = block + 4 * random.Below(64);
             t.r3.u64 = (uint32_t(counter - xa::kStats) >> 2) | (random.Next() & 0xC0000000ull);
             t.r4.u64 = random.Next();
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
}

// --- Calls into the recompiled objects ---
//
// Objects every 0x200 bytes (14 of them), each with the vtable at +0x1C00,
// whose slots are recompiled leaves that leave a trace: AddRef (+4 up, the
// count in r3), sub_820CE9F8 (r4 and r5 to +444 and +448, r3 left) and
// RigidBody::SetFloat112 (f1 to +112). The voice is object 0: its
// allocator (+8) and codec (+76) other objects or 0, its effects (+36) at
// +0x1E00 (8 bytes each), its outputs (+72) at +0x1E40 (12 bytes each: the
// target, the matrix object), the matrices' list at +0x1F00 (entries at
// +0x1F10).
constexpr uint32_t kObjectBytes = 0x200;
constexpr uint32_t kObjects = 14;
constexpr uint32_t kVtable = 0x1C00;
constexpr uint32_t kEffects = 0x1E00;
constexpr uint32_t kOutputs = 0x1E40;
constexpr uint32_t kMatrices = 0x1F00;
constexpr uint32_t kRecorders[] = {0x8245F860, 0x820CE9F8, 0x820B6950};

uint32_t Object(uint32_t block, uint32_t k) { return block + kObjectBytes * k; }
uint32_t AnyObject(Random& random, uint32_t block) {
  return Object(block, 1 + random.Below(kObjects - 1));
}

void World(uint8_t* base, uint32_t block, Random& random) {
  for (uint32_t slot = 0; slot < 0x100; slot += 4) {
    Store(base, block + kVtable + slot, kRecorders[random.Below(3)]);
  }
  for (uint32_t k = 0; k < kObjects; k++) {
    const uint32_t object = Object(block, k);
    Store(base, object, block + kVtable);
    // Small reference counts, so releases reach 0 now and then.
    Store(base, object + 4, random.Below(3));
  }
  const uint32_t voice = Object(block, 0);
  Store(base, voice + 8, random.Below(4) == 0 ? 0 : AnyObject(random, block));
  Store(base, voice + 76, random.Below(4) == 0 ? 0 : AnyObject(random, block));
  Store(base, voice + 36, block + kEffects);
  guest::StoreU8(base, voice + 60, uint8_t(random.Below(5)));
  for (uint32_t i = 0; i < 8; i++) {
    Store(base, block + kEffects + 8 * i, AnyObject(random, block));
  }
  Store(base, voice + 72, block + kOutputs);
  guest::StoreU8(base, voice + 69, uint8_t(random.Below(5)));
  for (uint32_t i = 0; i < 4; i++) {
    Store(base, block + kOutputs + 12 * i, AnyObject(random, block));
    Store(base, block + kOutputs + 12 * i + 4, AnyObject(random, block));
  }
  guest::StoreU8(base, block + kMatrices, uint8_t(random.Below(5)));
  Store(base, block + kMatrices + 4, block + kMatrices + 0x10);
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU8(base, block + kMatrices + 0x10 + 8 * i, uint8_t(random.Below(4)));
  }
  // The volume: a float in [-2, 2); the category 0 to 3.
  guest::StoreFloat(base, voice + 140, float(int32_t(random.Below(4001)) - 2000) / 1000.0f);
  guest::StoreU8(base, voice + 144, uint8_t(random.Below(4)));
}

void Calls(Context& h, uint32_t block) {
  struct Function {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
    bool interface;  // r3 an object's interface (+8), else the voice
    bool needs_codec;
  };
  const Function functions[] = {
      {"XAudioFactory::Size (sub_8245D610)", __imp__sub_8245D610, hooks::XAudioFactorySize, true,
       false},
      {"XAudioFactory::Create (sub_8245D630)", __imp__sub_8245D630, hooks::XAudioFactoryCreate,
       true, false},
      {"XAudioBuffer::AddRefThunk (sub_8245F6B0)", __imp__sub_8245F6B0,
       hooks::XAudioBufferAddRefThunk, true, false},
      {"XAudioBuffer::ReleaseThunk (sub_8245F6D0)", __imp__sub_8245F6D0,
       hooks::XAudioBufferReleaseThunk, true, false},
      {"XAudioVoice::Release (sub_8245F878)", __imp__sub_8245F878, hooks::XAudioRelease, false,
       false},
      {"XAudioObject::Free (sub_8245AAD8)", __imp__sub_8245AAD8, hooks::XAudioObjectFree, false,
       false},
      {"XAudioSourceVoice::ApplyVolume (sub_8245DFC0)", __imp__sub_8245DFC0,
       hooks::XAudioApplyVolume, false, true},
      {"XAudioSourceVoice::StopAndReset (sub_8245DD60)", __imp__sub_8245DD60,
       hooks::XAudioStopAndReset, false, false},
      {"XAudioVoice::SetEffectParameters (sub_82461418)", __imp__sub_82461418,
       hooks::XAudioSetEffectParameters, false, false},
      {"XAudioVoice::ReleaseOutputs (sub_82460A58)", __imp__sub_82460A58,
       hooks::XAudioReleaseOutputs, false, false},
      {"XAudioVoice::SetOutputMatrices (sub_82460AB8)", __imp__sub_82460AB8,
       hooks::XAudioSetOutputMatrices, false, false},
      {"XAudioVoice::SetFormat (sub_82460778)", __imp__sub_82460778, hooks::XAudioSetFormat,
       false, false},
  };
  for (const Function& f : functions) {
    Results results(f.name);
    for (int i = 0; i < 48; i++) {
      Case(h, results, block, f.recompiled, f.native,
           [&](Random& random, PPCContext& t) {
             World(h.base, block, random);
             if (f.needs_codec) {
               Store(h.base, block + 76, AnyObject(random, block));
             }
             t.r3.u64 = f.interface ? Object(block, random.Below(kObjects)) + 8 : block;
             // Index, matrices, format and parameters: SetEffectParameters
             // takes an index (sometimes past the chain), the others a
             // pointer into the block; r5-r7 random words.
             t.r4.u64 = f.recompiled == __imp__sub_82461418 ? random.Below(6)
                        : f.recompiled == __imp__sub_82460AB8
                            ? block + kMatrices
                            : block + kMatrices + 0x80;
             t.r5.u64 = random.Next();
             t.r6.u64 = random.Next();
             t.r7.u64 = random.Next();
             t.f1.u64 = random.Next();
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
}

void Descriptions(Context& h, uint32_t block) {
  const bool engine = guest::LoadU32(h.base, xa::kEngine) != 0;
  {
    Results results("XAudioSourceVoice::Describe (sub_8245E010)");
    for (int i = 0; engine && i < 48; i++) {
      Case(h, results, block, __imp__sub_8245E010, hooks::XAudioSourceDescribe,
           [&](Random& random, PPCContext& t) {
             if (random.Below(2) == 0) {
               guest::StoreU8(h.base, block, 4);
               guest::StoreU8(h.base, block + 4, uint8_t(random.Below(9)));
             }
             if (random.Below(3) == 0) {
               guest::StoreU8(h.base, block + 57, 0);
             }
             if (random.Below(3) == 0) {
               guest::StoreU8(h.base, block + 58, 0);
             }
             t.r3.u64 = block;
             t.r4.u64 = block + 0x1000;
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("XAudioSourceVoice::Reformat (sub_8245DCF0)");
    for (int i = 0; i < 48; i++) {
      Case(h, results, block, __imp__sub_8245DCF0, hooks::XAudioReformat,
           [&](Random& random, PPCContext& t) {
             World(h.base, block, random);
             Store(h.base, block + 76, AnyObject(random, block));
             t.r3.u64 = block;
             t.r4.u64 = block + 0x1F80;
             t.r5.u64 = random.Below(2) == 0 ? 0 : block + kOutputs;
             t.r6.u64 = random.Next();
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("XAudioBuffer::ClearThunk (sub_8245FA10)");
    for (int i = 0; i < 48; i++) {
      Case(h, results, block, __imp__sub_8245FA10, hooks::XAudioBufferClearThunk,
           [&](Random& random, PPCContext& t) {
             guest::StoreU8(h.base, block + 13, uint8_t(1 + random.Below(6)));
             Store(h.base, block + 16, Rate(random));
             Store(h.base, block + 24, Rate(random));
             guest::StoreU8(h.base, block + 21, uint8_t(random.Below(7)));
             Store(h.base, block + 28, block + 0x400);
             guest::StoreU8(h.base, block + 0x101, uint8_t(random.Below(8)));
             Store(h.base, block + 0x104, Rate(random));
             t.r3.u64 = block + 8;
             t.r4.u64 = random.Below(3) == 0 ? 0 : block + 0x100;
             t.r5.u64 = random.Next();
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // Without effects: the effects' sizes come from the live engine's
    // factory, whose methods take other arguments by id (the matrix
    // object's, id 1, reads r5 as its parameters), so its path is left to
    // the game (the car sounds' submix has a reverb).
    Results results("XAudioVoice::BaseSize (sub_82461158)");
    for (int i = 0; engine && i < 48; i++) {
      Case(h, results, block, __imp__sub_82461158, hooks::XAudioBaseSize,
           [&](Random& random, PPCContext& t) {
             const uint32_t description = block + 0x1A00;
             if (random.Below(2) == 0) {
               Store(h.base, description + 12, 0);
             }
             guest::StoreU8(h.base, description + 1, uint8_t(random.Below(7)));
             Store(h.base, description + 8, random.Below(4) == 0 ? 0 : block + 0x1B00);
             guest::StoreU8(h.base, block + 0x1B00, 0);
             t.r3.u64 = description;
             t.r4.u64 = block + 0x1C00;
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // Two allocations can't match: whether each gave a block, then freed.
    Results results("XAudio::Alloc (sub_8245EF80)");
    for (int i = 0; hooks::NativeHeapActive() && i < 8; i++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        PPCContext t = CallContext(h);
        t.r4.u64 = 64 + 64 * i;
        t.r5.u64 = 0x61820006;
        (v == 0 ? __imp__sub_8245EF80 : hooks::XAudioAlloc)(t, h.base);
        out[v].Word(t.r3.u32 != 0);
        if (t.r3.u32 != 0) {
          memory::crt::XMemFree(t.r3.u32, 0x61820006);
        }
      }
      results.Check(out[0], out[1], fmt::format("{} bytes", 64 + 64 * i));
    }
    results.Report();
  }
}

// A default allocator's header (28 bytes) with its next free byte (+24) as
// an offset, then freed.
void AllocatorOutputs(Outputs& out, uint8_t* base, uint32_t allocator) {
  out.Word(allocator != 0);
  if (allocator == 0) {
    return;
  }
  std::vector<uint8_t> header = Read(base, allocator, 24);
  out.Bytes(header);
  out.Word(guest::LoadU32(base, allocator + 24) - allocator);
  memory::crt::XMemFree(allocator, 0x61820006);
}

void Objects(Context& h, uint32_t block) {
  {
    Results results("XAudioBuffer::XAudioBuffer (sub_8245F750)");
    for (int i = 0; i < 48; i++) {
      Case(h, results, block, __imp__sub_8245F750, hooks::XAudioBufferConstruct,
           [&](Random& random, PPCContext& t) {
             World(h.base, block, random);
             const uint32_t format = block + 0x1F80;
             guest::StoreU8(h.base, format, random.Below(4) != 0 ? 0 : uint8_t(random.Next()));
             guest::StoreU8(h.base, format + 5, uint8_t(random.Below(7)));
             Store(h.base, format + 8, Rate(random));
             t.r3.u64 = block + 0x1A00;
             t.r4.u64 = format;
             t.r5.u64 = AnyObject(random, block);
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // The allocator's slot +20 is AddRef, its count set so it hands back
    // object 5 (or 0, for E_OUTOFMEMORY, now and then); the buffer's own
    // slot +20 is the game's.
    Results results("XAudioBuffer::Create (sub_8245FA80)");
    for (int i = 0; i < 48; i++) {
      Case(h, results, block, __imp__sub_8245FA80, hooks::XAudioBufferCreate,
           [&](Random& random, PPCContext& t) {
             World(h.base, block, random);
             Store(h.base, block + kVtable + 20, 0x8245F860);
             const uint32_t allocator = Object(block, 1);
             Store(h.base, allocator + 4, random.Below(8) == 0 ? 0xFFFFFFFF : Object(block, 5) - 1);
             const uint32_t format = block + 0x1F80;
             guest::StoreU8(h.base, format, random.Below(4) != 0 ? 0 : uint8_t(random.Next()));
             guest::StoreU8(h.base, format + 5, uint8_t(random.Below(7)));
             Store(h.base, format + 8, Rate(random));
             t.r3.u64 = format;
             t.r4.u64 = allocator;
             t.r5.u64 = block + 0x1F00;
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("XAudioVoice::Construct (sub_82461500)");
    for (int i = 0; i < 48; i++) {
      Case(h, results, block, __imp__sub_82461500, hooks::XAudioVoiceConstruct,
           [&](Random& random, PPCContext& t) {
             World(h.base, block, random);
             t.r3.u64 = block;
             t.r4.u64 = random.Below(4) == 0 ? 0 : AnyObject(random, block);
             t.r5.u64 = random.Next();
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
  if (!hooks::NativeHeapActive()) {
    return;
  }
  {
    // Real blocks: the header compared with its next free byte as an
    // offset, then freed.
    Results results("XAudio::DefaultAllocator (sub_824605E8)");
    for (int i = 0; i < 16; i++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Store(h.base, block, 0);
        PPCContext t = CallContext(h);
        t.r3.u64 = 0x61820006;
        t.r4.u64 = 32 + 96 * i;
        t.r5.u64 = block;
        (v == 0 ? __imp__sub_824605E8 : hooks::XAudioDefaultAllocator)(t, h.base);
        out[v].Word(t.r3.u32);
        AllocatorOutputs(out[v], h.base, guest::LoadU32(h.base, block));
      }
      results.Check(out[0], out[1], fmt::format("{} bytes", 32 + 96 * i));
    }
    results.Report();
  }
  {
    Results results("XAudio::GetAllocator (sub_82460688)");
    for (int i = 0; i < 48; i++) {
      const bool fresh = i % 3 == 0;  // no allocator yet: a real default one
      const Random start = h.random;
      Random random = start;
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        random = start;
        for (uint32_t k = 0; k < kBlockBytes; k += 4) {
          Store(h.base, block + k, uint32_t(random.Next()));
        }
        World(h.base, block, random);
        const uint32_t inout = block + 0x1F00;
        Store(h.base, inout, fresh ? 0 : AnyObject(random, block));
        PPCContext t = CallContext(h);
        // A fresh allocator's attributes as the voices' (the block freed
        // with them); random words for an existing one's.
        t.r3.u64 = fresh ? 0x61820006 : random.Next();
        t.r4.u64 = fresh ? 32 + random.Below(1024) : random.Next();
        t.r5.u64 = inout;
        (v == 0 ? __imp__sub_82460688 : hooks::XAudioGetAllocator)(t, h.base);
        out[v].Word(t.r3.u32);
        if (fresh) {
          AllocatorOutputs(out[v], h.base, guest::LoadU32(h.base, inout));
          Store(h.base, inout, 0);
        }
        out[v].Bytes(Read(h.base, block, kBlockBytes));
      }
      h.random = random;
      results.Check(out[0], out[1], fmt::format("case {}{}", i, fresh ? " (fresh)" : ""));
    }
    results.Report();
  }
}

// --- The voices with outputs ---

// As Case, with the stack below r1 (where the frames go) filled with the
// same random words for both versions: the originals read locals their
// callees may not write (SetOutput's format, ProcessEffect's leftover).
template <typename Setup, typename After>
void FramedCase(Context& h, Results& results, uint32_t block, GuestFunction recompiled,
                GuestFunction native, Setup setup, After after, const std::string& what) {
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
    (v == 0 ? recompiled : native)(t, h.base);
    after(out[v]);  // before the block's read: it may put back what differs by version
    out[v].Bytes(Read(h.base, block, kBlockBytes));
    out[v].Word(t.r3.u32);
  }
  h.random = random;
  results.Check(out[0], out[1], what);
}

template <typename Setup>
void FramedCase(Context& h, Results& results, uint32_t block, GuestFunction recompiled,
                GuestFunction native, Setup setup, const std::string& what) {
  FramedCase(h, results, block, recompiled, native, setup, [](Outputs&) {}, what);
}

// The engine's mastering voice (+64), which the default output entry
// AddRefs: its count put back after each version.
uint32_t Mastering(uint8_t* base) {
  return guest::LoadU32(base, guest::LoadU32(base, xa::kEngine) + 64);
}

void VoiceOutputs(Context& h, uint32_t block) {
  const bool engine = guest::LoadU32(h.base, xa::kEngine) != 0;
  {
    Results results("XAudioVoice::ConstructOutputs (sub_82460738)");
    for (int i = 0; i < 48; i++) {
      Case(h, results, block, __imp__sub_82460738, hooks::XAudioConstructOutputs,
           [&](Random& random, PPCContext& t) {
             World(h.base, block, random);
             t.r3.u64 = block;
             t.r4.u64 = random.Below(4) == 0 ? 0 : AnyObject(random, block);
             t.r5.u64 = random.Next();
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // Without effects, as BaseSize's cases; the matrix objects' size from
    // the live factory (its description only read).
    Results results("XAudioVoice::OutputsSize (sub_824607C8)");
    for (int i = 0; engine && i < 48; i++) {
      FramedCase(h, results, block, __imp__sub_824607C8, hooks::XAudioOutputsSize,
                 [&](Random& random, PPCContext& t) {
                   const uint32_t description = block + 0x1A00;
                   if (random.Below(2) == 0) {
                     Store(h.base, description + 12, 0);
                   }
                   guest::StoreU8(h.base, description + 1, uint8_t(random.Below(7)));
                   Store(h.base, description + 8, random.Below(4) == 0 ? 0 : block + 0x1B00);
                   guest::StoreU8(h.base, block + 0x1B00, 0);
                   guest::StoreU8(h.base, description + 24, uint8_t(random.Below(5)));
                   guest::StoreU8(h.base, description + 25, uint8_t(1 + random.Below(6)));
                   t.r3.u64 = description;
                   t.r4.u64 = block + 0x1C00;
                 },
                 fmt::format("case {}", i));
    }
    results.Report();
  }
  // The output entries at +0x1F80 (8 bytes each: a target or 0, a matrix
  // or 0); the default entry and target 0 reach the live mastering voice.
  const auto entries = [&](Random& random, uint32_t count) {
    for (uint32_t k = 0; k < count; k++) {
      const uint32_t entry = block + 0x1F80 + 8 * k;
      Store(h.base, entry, random.Below(8) == 0 ? 0 : AnyObject(random, block));
      Store(h.base, entry + 4, random.Below(2) == 0 ? 0 : uint32_t(random.Next()));
    }
  };
  {
    Results results("XAudioVoice::SetOutput (sub_82460998)");
    for (int i = 0; engine && i < 48; i++) {
      uint32_t mastering = 0;
      FramedCase(h, results, block, __imp__sub_82460998, hooks::XAudioSetOutput,
                 [&](Random& random, PPCContext& t) {
                   World(h.base, block, random);
                   entries(random, 1);
                   t.r3.u64 = block;
                   t.r4.u64 = random.Below(8) == 0 ? 0 : block + 0x1F80;
                   t.r5.u64 = block + kOutputs + 12 * random.Below(4);
                   mastering = Mastering(h.base);
                   Store(h.base, block + 0x1FF0, guest::LoadU32(h.base, mastering + 4));
                 },
                 [&](Outputs&) {
                   // Put back the count AddRef raised on the mastering voice.
                   const uint32_t count = guest::LoadU32(h.base, block + 0x1FF0);
                   if (guest::LoadU32(h.base, mastering + 4) != count) {
                     Store(h.base, mastering + 4, count);
                   }
                 },
                 fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("XAudioVoice::SetOutputVoices (sub_82460BF0)");
    for (int i = 0; engine && i < 64; i++) {
      uint32_t mastering = 0;
      const bool jump = i % 4 == 3;  // sub_8245F228, the submix's slot +36
      FramedCase(h, results, block, jump ? __imp__sub_8245F228 : __imp__sub_82460BF0,
                 hooks::XAudioSetOutputVoices,
                 [&](Random& random, PPCContext& t) {
                   World(h.base, block, random);
                   const uint32_t list = block + 0x1F60;
                   guest::StoreU8(h.base, list, uint8_t(random.Below(5)));
                   Store(h.base, list + 4, block + 0x1F80);
                   entries(random, 4);
                   guest::StoreU8(h.base, block + 68, uint8_t(random.Below(3)));
                   t.r3.u64 = block;
                   t.r4.u64 = random.Below(4) == 0 ? 0 : list;
                   mastering = Mastering(h.base);
                   Store(h.base, block + 0x1FF0, guest::LoadU32(h.base, mastering + 4));
                 },
                 [&](Outputs&) {
                   const uint32_t count = guest::LoadU32(h.base, block + 0x1FF0);
                   if (guest::LoadU32(h.base, mastering + 4) != count) {
                     Store(h.base, mastering + 4, count);
                   }
                 },
                 fmt::format("case {}{}", i, jump ? " (sub_8245F228)" : ""));
    }
    results.Report();
  }
  {
    Results results("XAudioVoice::Pass (sub_82461070)");
    for (int i = 0; i < 48; i++) {
      FramedCase(h, results, block, __imp__sub_82461070, hooks::XAudioPass,
                 [&](Random& random, PPCContext& t) {
                   World(h.base, block, random);
                   Store(h.base, block + 44, random.Below(2) == 0 ? 0 : 0x8245F860);
                   guest::StoreU16(h.base, block + 64, uint16_t(random.Below(3)));
                   t.r3.u64 = block;
                 },
                 fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("XAudioVoice::Destroy (sub_82461240)");
    for (int i = 0; i < 48; i++) {
      Case(h, results, block, __imp__sub_82461240, hooks::XAudioDestroy,
           [&](Random& random, PPCContext& t) {
             World(h.base, block, random);
             Store(h.base, block + 32, random.Below(3) == 0 ? 0 : AnyObject(random, block) + 8);
             if (random.Below(4) == 0) {
               Store(h.base, block + 8, 0);
             }
             // Some effects empty.
             for (uint32_t k = 0; k < 8; k++) {
               if (random.Below(3) == 0) {
                 Store(h.base, block + kEffects + 8 * k, 0);
               }
             }
             t.r3.u64 = block;
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("XAudioVoice::ProcessEffect (sub_82461678)");
    const uint32_t engine_address = guest::LoadU32(h.base, xa::kEngine);
    for (int i = 0; engine && i < 64; i++) {
      FramedCase(h, results, block, __imp__sub_82461678, hooks::XAudioProcessEffect,
                 [&](Random& random, PPCContext& t) {
                   World(h.base, block, random);
                   const uint32_t entry = block + 0x1F80;
                   Store(h.base, entry, AnyObject(random, block));
                   const uint32_t inout = block + 0x1F90;
                   const uint32_t processor = guest::LoadU8(h.base, t.r13.u32 + xa::kPcrProcessor);
                   const uint32_t pick = random.Below(4);
                   Store(h.base, inout,
                         pick == 0   ? 0
                         : pick == 3 ? uint32_t(random.Next())
                                     : guest::LoadU32(h.base, engine_address + 12 + 8 * processor +
                                                                  4 * (pick - 1)));
                   t.r3.u64 = block;
                   t.r4.u64 = entry;
                   t.r5.u64 = inout;
                 },
                 fmt::format("case {}", i));
    }
    results.Report();
  }
  if (!hooks::NativeHeapActive()) {
    return;
  }
  {
    // A real default allocator for each version and a matrix object's
    // description (type 1, as InitOutputs gives), made by the live
    // factory: the entry's object compared as an offset in the allocator's
    // block, which is then freed with it.
    Results results("XAudioVoice::CreateObject (sub_82461308)");
    for (int i = 0; engine && i < 32; i++) {
      uint32_t allocator = 0;
      FramedCase(h, results, block, __imp__sub_82461308, hooks::XAudioCreateObject,
                 [&](Random& random, PPCContext& t) {
                   const uint32_t out = block + 0x1FF0;
                   xa::DefaultAllocator(h.base, 0x61820006, 4096, out);
                   allocator = guest::LoadU32(h.base, out);
                   Store(h.base, out, 0);
                   Store(h.base, block + 8, allocator);
                   guest::StoreU16(h.base, block + 62, uint16_t(random.Below(64)));
                   const uint32_t description = block + 0x1F80;
                   for (uint32_t k = 0; k < 16; k += 4) {
                     Store(h.base, description + k, 0);
                   }
                   guest::StoreU8(h.base, description, 1);
                   Store(h.base, description + 4, block);
                   guest::StoreU8(h.base, description + 8, uint8_t(1 + random.Below(6)));
                   t.r3.u64 = block;
                   t.r4.u64 = block + 0x1FA0;
                   t.r5.u64 = description;
                   t.r6.u64 = random.Next();
                 },
                 [&](Outputs& out) {
                   const uint32_t made = guest::LoadU32(h.base, block + 0x1FA0);
                   out.Word(made != 0 ? made - allocator : 0);
                   // The two pointers to the version's own allocator.
                   Store(h.base, block + 0x1FA0, 0);
                   out.Word(guest::LoadU32(h.base, block + 8) == allocator);
                   Store(h.base, block + 8, 0);
                   AllocatorOutputs(out, h.base, allocator);
                 },
                 fmt::format("case {}", i));
    }
    results.Report();
  }
}

// --- Making voices and their passes ---

constexpr uint32_t kAllocatorBytes = 0x8000;

// The words of [address, address + bytes) with those pointing into the
// allocator's block (`allocator`, `allocator_bytes`) as offsets in it
// (marked), the rest as they are.
void Normalized(Outputs& out, uint8_t* base, uint32_t address, uint32_t bytes,
                uint32_t allocator, uint32_t allocator_bytes) {
  std::vector<uint8_t> words = Read(base, address, bytes);
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    const uint32_t word = guest::LoadU32(base, address + i);
    if (word >= allocator && word < allocator + allocator_bytes) {
      const uint32_t offset = 0xA110C000u ^ (word - allocator);
      words[i] = uint8_t(offset >> 24);
      words[i + 1] = uint8_t(offset >> 16);
      words[i + 2] = uint8_t(offset >> 8);
      words[i + 3] = uint8_t(offset);
    }
  }
  out.Bytes(words);
}

// A case over a real default allocator for each version (its data filled
// with the same pattern) and the live factory: `setup(random, t,
// allocator)`; the block and the allocator's block compared with pointers
// into the allocator as offsets, then the allocator freed (the objects made
// in it with it). The mastering voice's count, which default outputs
// AddRef, is put back.
template <typename Setup, typename After>
void RealCase(Context& h, Results& results, uint32_t block, GuestFunction recompiled,
              GuestFunction native, bool hold, Setup setup, After after,
              const std::string& what) {
  const Random start = h.random;
  Random random = start;
  Outputs out[2];
  const uint32_t bytes = kAllocatorBytes + 28;
  for (int v = 0; v < 2; v++) {
    random = start;
    for (uint32_t i = 0; i < kBlockBytes; i += 4) {
      Store(h.base, block + i, uint32_t(random.Next()));
    }
    PPCContext t = CallContext(h);
    for (uint32_t i = 4; i <= 0x400; i += 4) {
      Store(h.base, t.r1.u32 - i, uint32_t(random.Next()));
    }
    xa::DefaultAllocator(h.base, 0x61820006, kAllocatorBytes, block + 0x1FFC);
    const uint32_t allocator = guest::LoadU32(h.base, block + 0x1FFC);
    Store(h.base, block + 0x1FFC, 0);
    out[v].Word(allocator != 0);
    if (allocator == 0) {
      continue;
    }
    for (uint32_t i = 28; i < bytes; i += 4) {
      Store(h.base, allocator + i, 0x5EED0000u + i);
    }
    const uint32_t mastering = Mastering(h.base);
    const uint32_t count = guest::LoadU32(h.base, mastering + 4);
    setup(random, t, allocator);
    PPCContext lock = t;
    if (hold) {
      hooks::XAudioSpinTake(lock, h.base);
    }
    (v == 0 ? recompiled : native)(t, h.base);
    after(out[v]);
    if (hold) {
      hooks::XAudioSpinRelease(lock, h.base);
    }
    if (guest::LoadU32(h.base, mastering + 4) != count) {
      Store(h.base, mastering + 4, count);
    }
    out[v].Word(t.r3.u32);
    Normalized(out[v], h.base, block, kBlockBytes, allocator, bytes);
    Normalized(out[v], h.base, allocator, bytes, allocator, bytes);
    memory::crt::XMemFree(allocator, 0x61820006);
  }
  h.random = random;
  results.Check(out[0], out[1], what);
}

template <typename Setup>
void RealCase(Context& h, Results& results, uint32_t block, GuestFunction recompiled,
              GuestFunction native, Setup setup, const std::string& what) {
  RealCase(h, results, block, recompiled, native, false, setup, [](Outputs&) {}, what);
}

constexpr uint32_t kSourceVtable = 0x8204A720;
constexpr uint32_t kSubmixVtable = 0x8204A870;

// A matrix object's description (type 1, as InitOutputs gives) at `at`.
void MatrixDescription(uint8_t* base, uint32_t at, uint32_t voice, Random& random) {
  for (uint32_t k = 0; k < 16; k += 4) {
    Store(base, at + k, 0);
  }
  guest::StoreU8(base, at, 1);
  Store(base, at + 4, voice);
  guest::StoreU8(base, at + 8, uint8_t(1 + random.Below(6)));
}

// An effect chain at +0x1F00 of `count` matrix descriptions (the live
// factory's objects; the game's reverb isn't made here), or 0.
uint32_t Chain(uint8_t* base, uint32_t block, uint32_t voice, Random& random, uint32_t count) {
  const uint32_t chain = block + 0x1F00;
  guest::StoreU8(base, chain, uint8_t(count));
  Store(base, chain + 4, chain + 0x10);
  for (uint32_t k = 0; k < count; k++) {
    Store(base, chain + 0x10 + 4 * k, chain + 0x20 + 16 * k);
    MatrixDescription(base, chain + 0x20 + 16 * k, voice, random);
  }
  return chain;
}

// A voice description at `at` (InitBase's and InitOutputs'): a format of 1-6
// channels at 48 kHz, the chain, no buffer (made from the allocator) or one
// of the block's objects (its interface), the callback and its context, 1-2
// outputs of 1-6 channels, the default outputs (0).
void VoiceDescription(uint8_t* base, uint32_t at, uint32_t block, uint32_t voice, Random& random,
                      bool buffer) {
  for (uint32_t k = 0; k < 32; k += 4) {
    Store(base, at + k, 0);
  }
  guest::StoreU8(base, at + 1, uint8_t(1 + random.Below(6)));
  Store(base, at + 4, 48000);
  Store(base, at + 8, random.Below(3) == 0 ? 0 : Chain(base, block, voice, random, random.Below(3)));
  if (buffer) {
    // Object 5 with AddRef at its slot +4.
    const uint32_t object = block + 0x1400;
    Store(base, object, block + 0x1600);
    Store(base, block + 0x1604, 0x8245F860);
    Store(base, object + 4, random.Below(4));
    Store(base, at + 12, object + 8);
  }
  Store(base, at + 16, uint32_t(random.Next()));
  Store(base, at + 20, uint32_t(random.Next()));
  guest::StoreU8(base, at + 24, uint8_t(1 + random.Below(2)));
  guest::StoreU8(base, at + 25, uint8_t(1 + random.Below(6)));
}

void Making(Context& h, uint32_t block) {
  const bool engine = guest::LoadU32(h.base, xa::kEngine) != 0;
  if (!engine || !hooks::NativeHeapActive()) {
    return;
  }
  {
    Results results("XAudioVoice::CreateEffects (sub_82461600)");
    for (int i = 0; i < 32; i++) {
      RealCase(h, results, block, __imp__sub_82461600, hooks::XAudioCreateEffects,
               [&](Random& random, PPCContext& t, uint32_t allocator) {
                 Store(h.base, block + 8, allocator);
                 Store(h.base, block + 36, block + 0x1E00);
                 guest::StoreU16(h.base, block + 62, uint16_t(random.Below(64)));
                 t.r3.u64 = block;
                 t.r4.u64 = Chain(h.base, block, block, random, random.Below(4));
               },
               fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("XAudioVoice::InitBase (sub_82461D50)");
    for (int i = 0; i < 32; i++) {
      RealCase(h, results, block, __imp__sub_82461D50, hooks::XAudioInitBase,
               [&](Random& random, PPCContext& t, uint32_t allocator) {
                 Store(h.base, block + 8, allocator);
                 const uint32_t description = block + 0x1E80;
                 VoiceDescription(h.base, description, block, block, random, random.Below(3) == 0);
                 t.r3.u64 = block;
                 t.r4.u64 = description;
               },
               fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // The voice's vtable the source voice's: its slot +36 sets the default
    // outputs (the mastering voice).
    Results results("XAudioVoice::InitOutputs (sub_82460880)");
    for (int i = 0; i < 32; i++) {
      RealCase(h, results, block, __imp__sub_82460880, hooks::XAudioInitOutputs,
               [&](Random& random, PPCContext& t, uint32_t allocator) {
                 Store(h.base, block, kSourceVtable);
                 Store(h.base, block + 8, allocator);
                 guest::StoreU8(h.base, block + 61, uint8_t(random.Below(2)));
                 guest::StoreU8(h.base, block + 69, 0);
                 const uint32_t description = block + 0x1E80;
                 VoiceDescription(h.base, description, block, block, random, false);
                 t.r3.u64 = block;
                 t.r4.u64 = description;
               },
               fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // A submix description (+1 the channels, +8 the output count, +9 their
    // channels, +12 no chain); the voice's vtable the submix voice's.
    Results results("XAudioSubmixVoice::Init (sub_8245F230)");
    for (int i = 0; i < 32; i++) {
      RealCase(h, results, block, __imp__sub_8245F230, hooks::XAudioSubmixInit,
               [&](Random& random, PPCContext& t, uint32_t allocator) {
                 Store(h.base, block, kSubmixVtable);
                 Store(h.base, block + 8, allocator);
                 guest::StoreU8(h.base, block + 61, uint8_t(random.Below(2)));
                 guest::StoreU8(h.base, block + 69, 0);
                 const uint32_t description = block + 0x1E80;
                 for (uint32_t k = 0; k < 32; k += 4) {
                   Store(h.base, description + k, 0);
                 }
                 guest::StoreU8(h.base, description + 1, uint8_t(1 + random.Below(6)));
                 guest::StoreU8(h.base, description + 8, uint8_t(random.Below(3)));
                 guest::StoreU8(h.base, description + 9, uint8_t(random.Below(7)));
                 t.r3.u64 = block;
                 t.r4.u64 = description;
               },
               fmt::format("case {}", i));
    }
    results.Report();
  }
  // A live source voice's format (the engine's source list, +80, its node
  // at the voice's +88 offset), read only, for the source descriptions.
  const uint32_t engine_address = guest::LoadU32(h.base, xa::kEngine);
  const uint32_t head = engine_address + 80;
  const uint32_t node = guest::LoadU32(h.base, head);
  const uint32_t live = node != head && node != 0 ? node - guest::LoadU32(h.base, engine_address + 88)
                                                  : 0;
  // A source description at `at`: the live format, the category, 1-2
  // outputs of 1-6 channels, a chain or none, the default outputs, a
  // callback word and context; the codec's other words 0.
  const auto source = [&](Random& random, uint32_t at) {
    for (uint32_t k = 0; k < 92; k += 4) {
      Store(h.base, at + k, 0);
    }
    for (uint32_t k = 0; k < 56; k += 4) {
      Store(h.base, at + k, guest::LoadU32(h.base, live + 84 + k));
    }
    guest::StoreU8(h.base, at + 56, uint8_t(random.Below(2)));
    guest::StoreU8(h.base, at + 57, uint8_t(random.Below(3)));
    guest::StoreU8(h.base, at + 58, uint8_t(random.Below(7)));
    guest::StoreFloat(h.base, at + 60, float(random.Below(1000)) / 500.0f);
    Store(h.base, at + 68, random.Below(3) == 0 ? 0 : Chain(h.base, block, block, random, random.Below(3)));
    Store(h.base, at + 76, uint32_t(random.Next()));
    Store(h.base, at + 88, uint32_t(random.Next()));
  };
  {
    Results results("XAudioSourceVoice::Size (sub_8245E0D8)");
    for (int i = 0; live != 0 && i < 32; i++) {
      FramedCase(h, results, block, __imp__sub_8245E0D8, hooks::XAudioSourceSize,
                 [&](Random& random, PPCContext& t) {
                   source(random, block + 0x1E80);
                   t.r3.u64 = block + 0x1E80;
                   t.r4.u64 = block + 0x1FF0;
                 },
                 fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // The codec made for the live format (an XMA one keeps a small handle
    // of the native XMA layer, not freed).
    Results results("XAudioSourceVoice::Init (sub_8245E348)");
    for (int i = 0; live != 0 && i < 16; i++) {
      RealCase(h, results, block, __imp__sub_8245E348, hooks::XAudioSourceInit,
               [&](Random& random, PPCContext& t, uint32_t allocator) {
                 Store(h.base, block, kSourceVtable);
                 Store(h.base, block + 8, allocator);
                 guest::StoreU8(h.base, block + 61, uint8_t(random.Below(2)));
                 guest::StoreU8(h.base, block + 69, 0);
                 guest::StoreU16(h.base, block + 62, 0);
                 source(random, block + 0x1E80);
                 t.r3.u64 = block;
                 t.r4.u64 = block + 0x1E80;
               },
               fmt::format("case {}", i));
    }
    results.Report();
  }
}

void Passes(Context& h, uint32_t block) {
  const bool engine = guest::LoadU32(h.base, xa::kEngine) != 0;
  const uint32_t engine_address = guest::LoadU32(h.base, xa::kEngine);
  const auto buffer_word = [&](Random& random, PPCContext& t) {
    const uint32_t processor = guest::LoadU8(h.base, t.r13.u32 + xa::kPcrProcessor);
    const uint32_t pick = random.Below(4);
    return pick == 0   ? 0u
           : pick == 3 ? uint32_t(random.Next())
                       : guest::LoadU32(h.base, engine_address + 12 + 8 * processor + 4 * (pick - 1));
  };
  {
    Results results("XAudioVoice::ProcessEffects (sub_82461E88)");
    for (int i = 0; engine && i < 48; i++) {
      FramedCase(h, results, block, __imp__sub_82461E88, hooks::XAudioProcessEffects,
                 [&](Random& random, PPCContext& t) {
                   World(h.base, block, random);
                   Store(h.base, block + 0x1F90, buffer_word(random, t));
                   t.r3.u64 = block;
                   t.r4.u64 = block + 0x1F90;
                 },
                 fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("XAudioVoice::MixToOutputs (sub_82460CB8)");
    for (int i = 0; engine && i < 48; i++) {
      FramedCase(h, results, block, __imp__sub_82460CB8, hooks::XAudioMixToOutputs,
                 [&](Random& random, PPCContext& t) {
                   World(h.base, block, random);
                   Store(h.base, block + 0x1F90, buffer_word(random, t));
                   t.r3.u64 = block;
                   t.r4.u64 = block + 0x1F90;
                 },
                 fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // A buffer that can be cleared: object 13's, its samples at its +0x100
    // (a format of 1-2 channels at 8 kHz: 42 frames).
    Results results("XAudioSubmixVoice::Pass (sub_8245EFA0)");
    for (int i = 0; i < 48; i++) {
      FramedCase(h, results, block, __imp__sub_8245EFA0, hooks::XAudioSubmixPass,
                 [&](Random& random, PPCContext& t) {
                   World(h.base, block, random);
                   Store(h.base, block + 44, random.Below(2) == 0 ? 0 : 0x8245F860);
                   guest::StoreU16(h.base, block + 64, uint16_t(random.Below(3)));
                   const uint32_t buffer = Object(block, 13);
                   guest::StoreU8(h.base, buffer + 13, 2);
                   Store(h.base, buffer + 16, 48000);
                   Store(h.base, buffer + 24, random.Below(2) == 0 ? 8000 : 48000);
                   guest::StoreU8(h.base, buffer + 21, uint8_t(random.Below(3)));
                   Store(h.base, buffer + 28, buffer + 0x100);
                   guest::StoreU8(h.base, block + 53, uint8_t(random.Below(3)));
                   Store(h.base, block + 56, 8000);
                   Store(h.base, block + 32, buffer + 8);
                   t.r3.u64 = block;
                 },
                 fmt::format("case {}", i));
    }
    results.Report();
  }
}

// --- Under the voice lists' spin lock ---
//
// The live spin lock (0x82954F10) is taken as the game takes it, by this
// thread. With `hold`, the case holds it around the call (so the function
// takes it again, and the mixer can't see a voice put in a live list until
// `after` takes it out), and its depth and owner after the call are
// compared; without, the function takes it from free (or waits for the
// mixer).
template <typename Setup, typename After>
void LockedCase(Context& h, Results& results, uint32_t block, GuestFunction recompiled,
                GuestFunction native, bool hold, Setup setup, After after,
                const std::string& what) {
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
    PPCContext lock = t;
    if (hold) {
      hooks::XAudioSpinTake(lock, h.base);
    }
    (v == 0 ? recompiled : native)(t, h.base);
    after(out[v]);
    if (hold) {
      out[v].Word(guest::LoadU32(h.base, xa::kSpinLock + 4));
      out[v].Word(guest::LoadU32(h.base, xa::kSpinLock + 8) == t.r13.u32);
      hooks::XAudioSpinRelease(lock, h.base);
    }
    out[v].Bytes(Read(h.base, block, kBlockBytes));
    out[v].Word(t.r3.u32);
  }
  h.random = random;
  results.Check(out[0], out[1], what);
}

// A node at `node` taken out of a live list where `list` says it was
// added at its end (both checked), its words cleared so the versions
// compare. Under the spin lock (the case holds it).
void TakeBack(Outputs& out, uint8_t* base, uint32_t list, uint32_t node) {
  const bool linked = guest::LoadU32(base, node) == list && guest::LoadU32(base, list + 4) == node;
  out.Word(linked);
  if (linked) {
    const uint32_t prev = guest::LoadU32(base, node + 4);
    guest::StoreU32(base, prev, list);
    guest::StoreU32(base, list + 4, prev);
    Store(base, node, 0);
    Store(base, node + 4, 0);
  }
}

// The node at `node` linked alone into a fake list at `head` in the block,
// or left pointing at itself.
void FakeList(uint8_t* base, uint32_t head, uint32_t node, bool linked) {
  if (linked) {
    Store(base, head, node);
    Store(base, head + 4, node);
    Store(base, node, head);
    Store(base, node + 4, head);
  } else {
    Store(base, node, node);
    Store(base, node + 4, node);
  }
}

void Locked(Context& h, uint32_t block) {
  const uint32_t engine = guest::LoadU32(h.base, xa::kEngine);
  if (engine == 0) {
    return;
  }
  const auto none = [](Outputs&) {};
  {
    Results results("XAudioSourceVoice::GetState (sub_8245DDC8)");
    for (int i = 0; i < 32; i++) {
      LockedCase(h, results, block, __imp__sub_8245DDC8, hooks::XAudioSourceGetState, i % 2 == 0,
                 [&](Random& random, PPCContext& t) {
                   World(h.base, block, random);
                   Store(h.base, block + 76, AnyObject(random, block));
                   t.r3.u64 = block;
                   t.r4.u64 = block + 0x1F90;
                 },
                 none, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("XAudioVoice::Play (sub_82461798)");
    for (int i = 0; i < 32; i++) {
      LockedCase(h, results, block, __imp__sub_82461798, hooks::XAudioPlay, i % 2 == 0,
                 [&](Random& random, PPCContext& t) {
                   World(h.base, block, random);
                   t.r3.u64 = block;
                 },
                 none, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // Slot +52 the real GetType: type 0 (the engine's source active list),
    // 1 (a stage's, the stage below the engine's count +128) or 2 (none).
    // The voice is taken back out of the live list before the lock goes.
    Results results("XAudioVoice::Activate (sub_82460EF0)");
    const uint32_t stages = guest::LoadU8(h.base, engine + 128);
    for (int i = 0; i < 48; i++) {
      uint32_t list = 0;
      LockedCase(h, results, block, __imp__sub_82460EF0, hooks::XAudioActivate, true,
                 [&](Random& random, PPCContext& t) {
                   World(h.base, block, random);
                   Store(h.base, block + kVtable + 52, 0x8245E318);
                   uint32_t type = random.Below(3);
                   if (type == 1 && stages == 0) {
                     type = 2;
                   }
                   guest::StoreU8(h.base, block + 12, uint8_t(type));
                   guest::StoreU8(h.base, block + 76, uint8_t(stages != 0 ? random.Below(stages) : 0));
                   list = 0;
                   if (type == 0) {
                     list = guest::LoadU32(h.base, engine + 80 + 40);
                   } else if (type == 1 && stages != 0) {
                     const uint32_t lists = guest::LoadU32(h.base, engine + 124) +
                                            44 * guest::LoadU8(h.base, block + 76);
                     list = guest::LoadU32(h.base, lists + 40);
                   } else {
                     guest::StoreU8(h.base, block + 12, 2);
                   }
                   t.r3.u64 = block;
                 },
                 [&](Outputs& out) {
                   if (list != 0) {
                     TakeBack(out, h.base, list, block + guest::LoadU32(h.base, list + 8));
                   }
                 },
                 fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("XAudioVoice::Deactivate (sub_82460D50)");
    for (int i = 0; i < 32; i++) {
      LockedCase(h, results, block, __imp__sub_82460D50, hooks::XAudioDeactivate, i % 2 == 0,
                 [&](Random& random, PPCContext& t) {
                   World(h.base, block, random);
                   FakeList(h.base, block + 0x1FC0, block + 24, random.Below(2) == 0);
                   t.r3.u64 = block;
                 },
                 none, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // The engine's pending list (+68); the voice taken back out of it.
    Results results("XAudioSourceVoice::StartLater (sub_8245E7D0)");
    const uint32_t pending = engine + 68;
    for (int i = 0; i < 32; i++) {
      LockedCase(h, results, block, __imp__sub_8245E7D0, hooks::XAudioStartLater, true,
                 [&](Random& random, PPCContext& t) {
                   World(h.base, block, random);
                   t.r3.u64 = block;
                 },
                 [&](Outputs& out) {
                   TakeBack(out, h.base, pending, block + guest::LoadU32(h.base, pending + 8));
                 },
                 fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // The pending node (at the engine's +76 offset) in a fake list or alone.
    Results results("XAudioSourceVoice::CancelPendingStart (sub_8245E428)");
    const uint32_t offset = guest::LoadU32(h.base, engine + 76);
    for (int i = 0; offset < 0x100 && i < 32; i++) {
      LockedCase(h, results, block, __imp__sub_8245E428, hooks::XAudioCancelPendingStart,
                 i % 2 == 0,
                 [&](Random& random, PPCContext& t) {
                   World(h.base, block, random);
                   FakeList(h.base, block + 0x1FC0, block + offset, random.Below(2) == 0);
                   t.r3.u64 = block;
                 },
                 none, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("XAudioVoice::StopOrTail (sub_824619B0)");
    for (int i = 0; i < 48; i++) {
      LockedCase(h, results, block, __imp__sub_824619B0, hooks::XAudioStopOrTail, i % 2 == 0,
                 [&](Random& random, PPCContext& t) {
                   World(h.base, block, random);
                   t.r3.u64 = block;
                   t.r4.u64 = random.Next();
                 },
                 none, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // A fake engine at +0x1A00, the voice not among its six (the wait
    // itself, which needs the mixer's thread to move on, is the game's to
    // run); now and then this thread is the mixer's.
    Results results("XAudio::WaitForVoice (sub_8245AE60)");
    for (int i = 0; i < 32; i++) {
      LockedCase(h, results, block, __imp__sub_8245AE60, hooks::XAudioWaitForVoice, i % 2 == 0,
                 [&](Random& random, PPCContext& t) {
                   const uint32_t fake = block + 0x1A00;
                   const uint32_t mixer = guest::LoadU32(h.base, t.r13.u32 + 256);
                   const uint32_t processor = guest::LoadU8(h.base, t.r13.u32 + xa::kPcrProcessor);
                   const uint32_t pick = random.Below(4);
                   if (pick == 0) {
                     Store(h.base, fake + 300, mixer);
                   } else if (pick == 1) {
                     Store(h.base, fake + 332 + 4 * processor, mixer);
                   }
                   t.r3.u64 = fake;
                   t.r4.u64 = block;
                 },
                 none, fmt::format("case {}", i));
    }
    results.Report();
  }
}

// --- Starting, stopping, passes, making and destroying voices ---

// A voice made with the default allocator made for it (CreateSource's and
// CreateSubmix's with none given; its block [voice + 8], its size at +16):
// taken back out of its live list (`where` gives the list and the node),
// the allocator's block compared with pointers into it as offsets, then
// freed with the voice and the objects made in it; the out word cleared.
template <typename Where>
void MadeVoice(Outputs& out, uint8_t* base, uint32_t out_word, Where where) {
  const uint32_t voice = guest::LoadU32(base, out_word);
  out.Word(voice != 0);
  if (voice == 0) {
    return;
  }
  const auto [list, node] = where(voice);
  TakeBack(out, base, list, node);
  const uint32_t allocator = guest::LoadU32(base, voice + 8);
  const uint32_t bytes = guest::LoadU32(base, allocator + 16) + 28;
  out.Word(voice - allocator);
  out.Word(bytes);
  Store(base, out_word, 0);
  Normalized(out, base, allocator, bytes, allocator, bytes);
  memory::crt::XMemFree(allocator, 0x61820006);
}

void Lifecycle(Context& h, uint32_t block) {
  const uint32_t engine = guest::LoadU32(h.base, xa::kEngine);
  if (engine == 0) {
    return;
  }
  const uint32_t stages = guest::LoadU8(h.base, engine + 128);
  const uint32_t pending_offset = guest::LoadU32(h.base, engine + 76);
  const uint32_t source_offset = guest::LoadU32(h.base, engine + 88);
  if (pending_offset >= 0x100 || source_offset >= 0x100) {
    return;
  }
  const auto none = [](Outputs&) {};
  // The voice's three list nodes (active +24, pending, source) each in a
  // fake list of the block's or alone; its counts and buffer sane for the
  // destructors (four outputs and their matrices, the buffer an object's
  // interface or none).
  const auto nodes = [&](Random& random) {
    FakeList(h.base, block + 0x1FC0, block + 24, random.Below(2) == 0);
    FakeList(h.base, block + 0x1FC8, block + pending_offset, random.Below(2) == 0);
    FakeList(h.base, block + 0x1FD0, block + source_offset, random.Below(2) == 0);
    guest::StoreU8(h.base, block + 68, guest::LoadU8(h.base, block + 69));
    Store(h.base, block + 32, random.Below(3) == 0 ? 0 : AnyObject(random, block) + 8);
  };
  {
    // The pending list taken back (StartLater's insert, for bit 0).
    Results results("XAudioSourceVoice::Start (sub_8245EBA0)");
    for (int i = 0; i < 48; i++) {
      LockedCase(h, results, block, __imp__sub_8245EBA0, hooks::XAudioSourceStart, true,
                 [&](Random& random, PPCContext& t) {
                   World(h.base, block, random);
                   nodes(random);
                   Store(h.base, block + 76, AnyObject(random, block));  // read unchecked
                   t.r3.u64 = block;
                   t.r4.u64 = random.Next();
                 },
                 [&](Outputs& out) {
                   TakeBack(out, h.base, engine + 68, block + guest::LoadU32(h.base, engine + 76));
                 },
                 fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("XAudioSourceVoice::Stop (sub_8245E9F0)");
    for (int i = 0; i < 48; i++) {
      LockedCase(h, results, block, __imp__sub_8245E9F0, hooks::XAudioSourceStop, i % 2 == 0,
                 [&](Random& random, PPCContext& t) {
                   World(h.base, block, random);
                   nodes(random);
                   if (random.Below(4) == 0) {
                     Store(h.base, block + 76, 0);
                   }
                   t.r3.u64 = block;
                   t.r4.u64 = random.Next();
                 },
                 none, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("XAudioSourceVoice::Pass (sub_8245DED0)");
    const uint32_t processor_buffers = engine + 12;
    for (int i = 0; i < 48; i++) {
      LockedCase(h, results, block, __imp__sub_8245DED0, hooks::XAudioSourcePass, i % 2 == 0,
                 [&](Random& random, PPCContext& t) {
                   World(h.base, block, random);
                   nodes(random);
                   guest::StoreU8(h.base, block + 144, uint8_t(random.Below(40)));
                   Store(h.base, block + 76, AnyObject(random, block));  // read unchecked
                   const uint32_t processor = guest::LoadU8(h.base, t.r13.u32 + xa::kPcrProcessor);
                   Store(h.base, block + 0x1F90,
                         random.Below(2) == 0
                             ? guest::LoadU32(h.base, processor_buffers + 8 * processor +
                                                          4 * random.Below(2))
                             : uint32_t(random.Next()));
                   t.r3.u64 = block;
                   t.r4.u64 = block + 0x1F90;
                 },
                 none, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // The voice taken back out of the live source list.
    Results results("XAudioSourceVoice::XAudioSourceVoice (sub_8245E6A8)");
    for (int i = 0; i < 32; i++) {
      LockedCase(h, results, block, __imp__sub_8245E6A8, hooks::XAudioSourceConstruct, true,
                 [&](Random& random, PPCContext& t) {
                   World(h.base, block, random);
                   t.r3.u64 = block;
                   t.r4.u64 = random.Below(4) == 0 ? 0 : AnyObject(random, block);
                   t.r5.u64 = random.Next();
                 },
                 [&](Outputs& out) { TakeBack(out, h.base, engine + 80, block + source_offset); },
                 fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("XAudioSubmixVoice::XAudioSubmixVoice (sub_8245F298)");
    for (int i = 0; stages != 0 && i < 32; i++) {
      uint32_t list = 0;
      LockedCase(h, results, block, __imp__sub_8245F298, hooks::XAudioSubmixConstruct, true,
                 [&](Random& random, PPCContext& t) {
                   World(h.base, block, random);
                   const uint32_t description = block + 0x1F80;
                   const uint32_t stage = random.Below(stages);
                   guest::StoreU8(h.base, description + 10, uint8_t(stage));
                   list = guest::LoadU32(h.base, engine + 124) + 44 * stage;
                   t.r3.u64 = block;
                   t.r4.u64 = description;
                   t.r5.u64 = random.Below(4) == 0 ? 0 : AnyObject(random, block);
                 },
                 [&](Outputs& out) {
                   TakeBack(out, h.base, list, block + guest::LoadU32(h.base, list + 8));
                 },
                 fmt::format("case {}", i));
    }
    results.Report();
  }
  // The destructors: the source's on the real source vtable (its stop and
  // deactivate are the native ones), the submix's stage node in a fake
  // list.
  const auto destructor = [&](const char* name, GuestFunction recompiled, GuestFunction native,
                              bool source, bool submix) {
    Results results(name);
    for (int i = 0; (!submix || stages != 0) && i < 32; i++) {
      LockedCase(h, results, block, recompiled, native, i % 2 == 0,
                 [&](Random& random, PPCContext& t) {
                   World(h.base, block, random);
                   nodes(random);
                   if (submix) {
                     const uint32_t stage = random.Below(stages);
                     guest::StoreU8(h.base, block + 76, uint8_t(stage));
                     const uint32_t list = guest::LoadU32(h.base, engine + 124) + 44 * stage;
                     FakeList(h.base, block + 0x1FD8, block + guest::LoadU32(h.base, list + 8),
                              random.Below(2) == 0);
                   }
                   (void)source;
                   t.r3.u64 = block;
                 },
                 none, fmt::format("case {}", i));
    }
    results.Report();
  };
  destructor("XAudioVoice::~XAudioVoiceOutputs (sub_82460B50)", __imp__sub_82460B50,
             hooks::XAudioOutputsDestroy, false, false);
  destructor("XAudioSourceVoice::~XAudioSourceVoice (sub_8245E1B8)", __imp__sub_8245E1B8,
             hooks::XAudioSourceDestroy, true, false);
  destructor("XAudioSourceVoice::DeletingDestructor (sub_8245E7A0)", __imp__sub_8245E7A0,
             hooks::XAudioSourceDeletingDestructor, true, false);
  destructor("XAudioSubmixVoice::~XAudioSubmixVoice (sub_8245F108)", __imp__sub_8245F108,
             hooks::XAudioSubmixDestroy, false, true);
  destructor("XAudioSubmixVoice::DeletingDestructor (sub_8245F3A0)", __imp__sub_8245F3A0,
             hooks::XAudioSubmixDeletingDestructor, false, true);
  if (!hooks::NativeHeapActive()) {
    return;
  }
  // Making voices over a real allocator (RealCase), the spin lock held so
  // the new voice can be taken back out of its live list.
  const uint32_t node = guest::LoadU32(h.base, engine + 80);
  const uint32_t live = node != engine + 80 && node != 0 ? node - source_offset : 0;
  {
    Results results("XAudioVoice::CreateSource (sub_8245EAA0)");
    for (int i = 0; live != 0 && i < 16; i++) {
      RealCase(h, results, block, __imp__sub_8245EAA0, hooks::XAudioCreateSource, true,
               [&](Random& random, PPCContext& t, uint32_t allocator) {
                 const uint32_t description = block + 0x1E80;
                 for (uint32_t k = 0; k < 92; k += 4) {
                   Store(h.base, description + k, 0);
                 }
                 for (uint32_t k = 0; k < 56; k += 4) {
                   Store(h.base, description + k, guest::LoadU32(h.base, live + 84 + k));
                 }
                 guest::StoreU8(h.base, description + 56, uint8_t(random.Below(2)));
                 guest::StoreU8(h.base, description + 57, uint8_t(random.Below(3)));
                 guest::StoreU8(h.base, description + 58, uint8_t(random.Below(7)));
                 guest::StoreFloat(h.base, description + 60, 1.0f);
                 Store(h.base, block + 0x1FF4, 0);
                 (void)allocator;
                 t.r3.u64 = description;
                 t.r4.u64 = 0;  // the default allocator, made for the voice's size
                 t.r5.u64 = block + 0x1FF4;
               },
               [&](Outputs& out) {
                 MadeVoice(out, h.base, block + 0x1FF4,
                           [&](uint32_t voice) { return std::pair{engine + 80, voice + source_offset}; });
               },
               fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("XAudioVoice::CreateSubmix (sub_8245F3D0)");
    for (int i = 0; stages != 0 && i < 16; i++) {
      uint32_t list = 0;
      RealCase(h, results, block, __imp__sub_8245F3D0, hooks::XAudioCreateSubmix, true,
               [&](Random& random, PPCContext& t, uint32_t allocator) {
                 const uint32_t description = block + 0x1E80;
                 for (uint32_t k = 0; k < 32; k += 4) {
                   Store(h.base, description + k, 0);
                 }
                 guest::StoreU8(h.base, description + 1, uint8_t(1 + random.Below(6)));
                 guest::StoreU8(h.base, description + 8, uint8_t(random.Below(3)));
                 guest::StoreU8(h.base, description + 9, uint8_t(random.Below(7)));
                 const uint32_t stage = random.Below(stages);
                 guest::StoreU8(h.base, description + 10, uint8_t(stage));
                 list = guest::LoadU32(h.base, engine + 124) + 44 * stage;
                 Store(h.base, block + 0x1FF4, 0);
                 (void)allocator;
                 t.r3.u64 = description;
                 t.r4.u64 = 0;
                 t.r5.u64 = block + 0x1FF4;
               },
               [&](Outputs& out) {
                 MadeVoice(out, h.base, block + 0x1FF4, [&](uint32_t voice) {
                   return std::pair{list, voice + guest::LoadU32(h.base, list + 8)};
                 });
               },
               fmt::format("case {}", i));
    }
    results.Report();
  }
}

// --- The API entry points ---

// The API lock (the game's critical section) entered or left by this
// thread, as the game does.
void ApiLock(uint8_t* base, const PPCContext& t, bool enter) {
  PPCContext c = t;
  c.r3.u64 = xa::kApiLock;
  if (enter) {
    __imp__RtlEnterCriticalSection(c, base);
  } else {
    __imp__RtlLeaveCriticalSection(c, base);
  }
}

void Api(Context& h, uint32_t block) {
  const uint32_t engine = guest::LoadU32(h.base, xa::kEngine);
  if (engine == 0) {
    return;
  }
  const auto none = [](Outputs&) {};
  struct Method {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
    bool codec;
  };
  const Method methods[] = {
      {"XAudioVoice::AddRef, the API's (sub_82459DB0)", __imp__sub_82459DB0, hooks::XAudioApiAddRef, false},
      {"XAudioVoice::Release, the API's (sub_82459E18)", __imp__sub_82459E18, hooks::XAudioApiRelease, false},
      {"XAudioVoice::SetEffectParameters, the API's (sub_82459E80)", __imp__sub_82459E80,
       hooks::XAudioApiSetEffectParameters, false},
      {"XAudioVoice::SetOutputVoices, the API's (sub_82459EF0)", __imp__sub_82459EF0,
       hooks::XAudioApiSetOutputVoices, false},
      {"XAudioVoice::SetOutputMatrix (sub_82459F48)", __imp__sub_82459F48, hooks::XAudioApiSetOutputMatrix, false},
      {"XAudioVoice::GetState, the API's (sub_82459FA0)", __imp__sub_82459FA0, hooks::XAudioApiGetState, false},
      {"XAudioVoice::Stop, the API's (sub_8245A1B0)", __imp__sub_8245A1B0, hooks::XAudioApiStop, false},
      {"XAudioVoice::GetOutputFormat (sub_8245A3B8)", __imp__sub_8245A3B8, hooks::XAudioApiGetOutputFormat, false},
      {"XAudioSourceVoice::SubmitPacket (sub_8245A050)", __imp__sub_8245A050, hooks::XAudioApiSubmitPacket, true},
      {"XAudioSourceVoice, the codec's +36 (sub_8245A108)", __imp__sub_8245A108, hooks::XAudioApiCodec36, true},
      {"XAudioSourceVoice::FlushPackets (sub_8245A208)", __imp__sub_8245A208, hooks::XAudioApiFlushPackets, true},
      {"XAudioSourceVoice, the codec's +72 (sub_8245A270)", __imp__sub_8245A270, hooks::XAudioApiCodec72, true},
      {"XAudioSourceVoice::Reset (sub_8245A0B0)", __imp__sub_8245A0B0, hooks::XAudioApiReset, false},
      {"XAudioSourceVoice::SetVolume (sub_8245A2C8)", __imp__sub_8245A2C8, hooks::XAudioApiSetVolume, false},
      {"XAudioSourceVoice::SetFrequencyRatio (sub_8245A340)", __imp__sub_8245A340,
       hooks::XAudioApiSetFrequencyRatio, true},
      {"XAudioSourceVoice::SetFormat (sub_82459FF8)", __imp__sub_82459FF8, hooks::XAudioApiSetFormat, true},
  };
  for (const Method& m : methods) {
    Results results(m.name);
    for (int i = 0; i < 32; i++) {
      LockedCase(h, results, block, m.recompiled, m.native, false,
                 [&](Random& random, PPCContext& t) {
                   World(h.base, block, random);
                   if (m.codec) {
                     Store(h.base, block + 76, AnyObject(random, block));
                   }
                   t.r3.u64 = block;
                   // SetFormat's format and outputs; the others' words.
                   t.r4.u64 = m.recompiled == __imp__sub_82459FF8 ? block + 0x1F80 : random.Next();
                   t.r5.u64 = m.recompiled == __imp__sub_82459FF8
                                  ? (random.Below(2) == 0 ? 0 : block + kOutputs)
                                  : random.Next();
                   t.r6.u64 = random.Next();
                   t.r7.u64 = random.Next();
                   t.f1.f64 = double(int32_t(random.Below(4001)) - 2000) / 997.0;
                 },
                 none, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // The API lock entered before the spin lock (the game's order); the
    // pending list taken back.
    Results results("XAudioVoice::Start, the API's (sub_8245A168)");
    const uint32_t pending_offset = guest::LoadU32(h.base, engine + 76);
    for (int i = 0; pending_offset < 0x100 && i < 32; i++) {
      LockedCase(h, results, block, __imp__sub_8245A168, hooks::XAudioApiStart, true,
                 [&](Random& random, PPCContext& t) {
                   World(h.base, block, random);
                   FakeList(h.base, block + 0x1FC8, block + pending_offset, random.Below(2) == 0);
                   Store(h.base, block + 76, AnyObject(random, block));
                   t.r3.u64 = block;
                   t.r4.u64 = random.Next();
                   ApiLock(h.base, t, true);
                 },
                 [&](Outputs& out) {
                   TakeBack(out, h.base, engine + 68, block + pending_offset);
                 },
                 fmt::format("case {}", i));
      PPCContext t = CallContext(h);
      ApiLock(h.base, t, false);
      ApiLock(h.base, t, false);
    }
    results.Report();
  }
  {
    Results results("XAudioGetMasteringVoice (sub_8245A568)");
    for (int i = 0; i < 8; i++) {
      LockedCase(h, results, block, __imp__sub_8245A568, hooks::XAudioApiGetMasteringVoice, false,
                 [&](Random&, PPCContext& t) { t.r3.u64 = block + 0x1F80; },
                 [&](Outputs& out) {
                   // The live mastering voice compared as found, not as an address.
                   out.Word(guest::LoadU32(h.base, block + 0x1F80) == Mastering(h.base));
                   Store(h.base, block + 0x1F80, 0);
                 },
                 fmt::format("case {}", i));
    }
    results.Report();
  }
  if (!hooks::NativeHeapActive()) {
    return;
  }
  const uint32_t stages = guest::LoadU8(h.base, engine + 128);
  const uint32_t source_offset = guest::LoadU32(h.base, engine + 88);
  const uint32_t node = guest::LoadU32(h.base, engine + 80);
  const uint32_t live = node != engine + 80 && node != 0 ? node - source_offset : 0;
  {
    // The API lock entered before the spin lock the case holds.
    Results results("XAudioCreateSourceVoice (sub_8245A768)");
    for (int i = 0; live != 0 && i < 8; i++) {
      RealCase(h, results, block, __imp__sub_8245A768, hooks::XAudioApiCreateSourceVoice, true,
               [&](Random& random, PPCContext& t, uint32_t) {
                 const uint32_t description = block + 0x1E80;
                 for (uint32_t k = 0; k < 92; k += 4) {
                   Store(h.base, description + k, 0);
                 }
                 for (uint32_t k = 0; k < 56; k += 4) {
                   Store(h.base, description + k, guest::LoadU32(h.base, live + 84 + k));
                 }
                 guest::StoreU8(h.base, description + 57, uint8_t(random.Below(3)));
                 guest::StoreFloat(h.base, description + 60, 1.0f);
                 Store(h.base, block + 0x1FF4, 0);
                 t.r3.u64 = description;
                 t.r4.u64 = block + 0x1FF4;
                 ApiLock(h.base, t, true);
               },
               [&](Outputs& out) {
                 MadeVoice(out, h.base, block + 0x1FF4,
                           [&](uint32_t voice) { return std::pair{engine + 80, voice + source_offset}; });
               },
               fmt::format("case {}", i));
      ApiLock(h.base, CallContext(h), false);
      ApiLock(h.base, CallContext(h), false);
    }
    results.Report();
  }
  {
    Results results("XAudioCreateSubmixVoice (sub_8245A7C8)");
    for (int i = 0; stages != 0 && i < 8; i++) {
      uint32_t list = 0;
      RealCase(h, results, block, __imp__sub_8245A7C8, hooks::XAudioApiCreateSubmixVoice, true,
               [&](Random& random, PPCContext& t, uint32_t) {
                 const uint32_t description = block + 0x1E80;
                 for (uint32_t k = 0; k < 32; k += 4) {
                   Store(h.base, description + k, 0);
                 }
                 guest::StoreU8(h.base, description + 1, uint8_t(1 + random.Below(6)));
                 const uint32_t stage = random.Below(stages);
                 guest::StoreU8(h.base, description + 10, uint8_t(stage));
                 list = guest::LoadU32(h.base, engine + 124) + 44 * stage;
                 Store(h.base, block + 0x1FF4, 0);
                 t.r3.u64 = description;
                 t.r4.u64 = block + 0x1FF4;
                 ApiLock(h.base, t, true);
               },
               [&](Outputs& out) {
                 MadeVoice(out, h.base, block + 0x1FF4, [&](uint32_t voice) {
                   return std::pair{list, voice + guest::LoadU32(h.base, list + 8)};
                 });
               },
               fmt::format("case {}", i));
      ApiLock(h.base, CallContext(h), false);
      ApiLock(h.base, CallContext(h), false);
    }
    results.Report();
  }
}

}  // namespace

void XAudio(Context& h) {
  Block block;
  Formats(h, block.address);
  Buffers(h, block.address);
  Accessors(h, block.address);
  Calls(h, block.address);
  Descriptions(h, block.address);
  Objects(h, block.address);
  VoiceOutputs(h, block.address);
  Passes(h, block.address);
  Making(h, block.address);
  Locked(h, block.address);
  Lifecycle(h, block.address);
  Api(h, block.address);
}

}  // namespace harness
