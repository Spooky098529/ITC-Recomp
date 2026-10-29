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

}  // namespace

void XAudio(Context& h) {
  Block block;
  Formats(h, block.address);
  Buffers(h, block.address);
  Accessors(h, block.address);
  Calls(h, block.address);
  Descriptions(h, block.address);
  Objects(h, block.address);
}

}  // namespace harness
