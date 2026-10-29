// XACT's tracks, track instances and events (src/xact/tracks.h,
// docs/functions/xact-tracks.md) against the recompiled functions. The game's
// live objects aren't touched: each case builds its objects in a block from
// the allocation layer, filled with the same random words for both versions,
// and runs the original (with --native_xact_tracks off, so its callees are
// the originals too) or the plain C++ through the register steps
// (hooks/xact_tracks.h). The block and r3 are compared with
// harness::Outputs: exactly, except the fields the recorders store as
// floats.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/heap.h"
#include "hooks/xact_tracks.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "xact/tracks.h"

#include <fmt/format.h>

#include <bit>
#include <initializer_list>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_xact_tracks);

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

// Now and then a special float (zeros, infinities, NaNs, denormals, the
// extremes), else an ordinary one.
double AnyFloat(Random& random) {
  static constexpr uint32_t kSpecial[] = {0x00000000, 0x80000000, 0x7F800000, 0xFF800000,
                                          0x7FC00000, 0xFFC00000, 0x7F800001, 0xFFA00001,
                                          0x00000001, 0x807FFFFF, 0x7F7FFFFF, 0xFF7FFFFF};
  switch (random.Below(4)) {
    case 0:
      return double(std::bit_cast<float>(kSpecial[random.Below(12)]));
    case 1:
      return double(random.Below(2000001)) / 1000000.0;
    default:
      return double(int32_t(random.Next())) / 65536.0;
  }
}

// The leaf recorders the fake vtables' slots hold (recompiled or ours, the
// same on both sides): XAudioVoice::AddRef sub_8245F860 (the count at +4 up
// and in r3), sub_820CE9F8 (r4 and r5 to +444 and +448), sub_820B6950 (f1 to
// +112 as a float), the getters sub_82090770 (+60) and sub_82090778 (+64),
// sub_825C8388 (0).
constexpr uint32_t kAddRef = 0x8245F860, kRecordR4 = 0x820CE9F8, kRecordF1 = 0x820B6950,
                   kGet60 = 0x82090770, kGet64 = 0x82090778, kReturn0 = 0x825C8388;

// The objects, at offsets in the block: the track instance, the track, the
// play-wave event and a second one, two waves, a voice, a record, and the
// three fake vtables (the event's starts as a copy of the play-wave event's
// real one, 0x82076CE8, 32 slots).
constexpr uint32_t kInstance = 0x000, kTrack = 0x080, kEvent = 0x100, kEvent2 = 0x180,
                   kWave = 0x400, kWave2 = 0x600, kVoice = 0x800, kRecord = 0x900,
                   kEventVtable = 0x1000, kWaveVtable = 0x1100, kVoiceVtable = 0x1200;
// Batch 2's: the track's sound (its record, an engine without global
// settings, so the category's volume is 1.0, and its cue), the cue's sound
// bank with its cached wave banks, a wave bank (its real vtable) with four
// entries, a track to construct, an object for the destructors to delete
// (the wave vtable's slot +0), and RPC entries.
constexpr uint32_t kSound = 0xA00, kSoundRecord = 0xA80, kEngine = 0xB00, kCue = 0xB80,
                   kSoundBank = 0xC00, kBankCache = 0xD00, kWaveBank = 0xD80, kEntries = 0xE80,
                   kTrack2 = 0xF00, kNext = 0x1300, kRpcEntries = 0x1600;
constexpr uint32_t kWaveBankVtable = 0x82075AE8;
// Batch 3's: a fake wave bank (its vtable all AddRef but CreateWave, +40:
// sub_82425450, the new wave at [r6] = r4 x 128 + r5, the owner event, so
// the record's wave index 6 or 10 makes it kWave or kWave2 from kEvent; or
// AddRef, its count at +4 making r3 a failure), and an engine with
// its critical section at +144 for the instances' destructor.
constexpr uint32_t kFakeBankVtable = 0x1280, kFakeBank = 0x1800, kEngine2 = 0x1900;
constexpr uint32_t kCreateWaveAt = 0x82425450;
// Batch 4's: the instance's events array when it isn't freed.
constexpr uint32_t kEventArray = 0x1A00;
// The waves' Prepare recorder: sub_820A8420 (r4's low byte to +320, r5 to
// +332, r6 to +336; r3 the wave).
constexpr uint32_t kRecordR4R6 = 0x820A8420;

// The block's floats: what kRecordF1 stores at +112 of the objects it's
// given.
// Batch 2's: the track's volume and the constructed track's floats.
const std::vector<uint32_t> kFloats = {kEvent + 112,  kEvent2 + 112, kWave + 112,  kWave2 + 112,
                                       kVoice + 112,  kTrack + 40,   kTrack2 + 40, kTrack2 + 44,
                                       kTrack2 + 56, kTrack2 + 60};

uint32_t Pick(Random& random, std::initializer_list<uint32_t> choices) {
  return *(choices.begin() + random.Below(uint32_t(choices.size())));
}

// The world every case starts from (after the block's random words): the
// fake vtables with each called slot the real function or a recorder, the
// links between the objects (some of them 0), states and flags random.
void World(uint8_t* base, uint32_t block, Random& random) {
  const uint32_t event_vtable = block + kEventVtable;
  for (uint32_t slot = 0; slot < 128; slot += 4) {
    Store(base, event_vtable + slot, Load(base, xact::kPlayWaveEventVtable + slot));
  }
  Store(base, event_vtable + 16, Pick(random, {xact::kPlayWaveSetQueued, kAddRef}));
  Store(base, event_vtable + 44, Pick(random, {0x825D81E8u, kAddRef}));
  Store(base, event_vtable + 56, Pick(random, {kGet60, kGet60, kGet64}));
  Store(base, event_vtable + 64, Pick(random, {0x825D7FE0u, kRecordF1}));
  Store(base, event_vtable + 88, Pick(random, {0x825D8268u, kRecordR4}));
  Store(base, event_vtable + 104, Pick(random, {xact::kPlayWaveSetWave, kRecordR4}));
  for (uint32_t slot = 0; slot < 128; slot += 4) {
    Store(base, block + kWaveVtable + slot, Pick(random, {kAddRef, kRecordR4}));
    Store(base, block + kVoiceVtable + slot, Pick(random, {kAddRef, kRecordF1}));
  }
  Store(base, block + kWaveVtable + 20, Pick(random, {kReturn0, kAddRef}));
  for (const uint32_t event : {kEvent, kEvent2}) {
    Store(base, block + event, event_vtable);
    Store(base, block + event + 60, random.Below(4) == 0 ? 0 : block + kWave);
    Store(base, block + event + 64, random.Below(4) == 0 ? 0 : block + kWave2);
    Store(base, block + event + 16, block + kRecord);
    Store(base, block + event + 44, block + kInstance);
  }
  for (const uint32_t wave : {kWave, kWave2}) {
    Store(base, block + wave, block + kWaveVtable);
    Store(base, block + wave + 40,
          random.Below(4) == 0 ? uint32_t(random.Next()) : random.Below(9) - 1);
    Store(base, block + wave + 44, random.Below(4) == 0 ? 0 : block + kVoice);
    if (random.Below(3) == 0) {
      Store(base, block + wave + 60, 0);
    }
  }
  Store(base, block + kVoice, block + kVoiceVtable);
  Store(base, block + kInstance + 36, random.Below(4) == 0 ? 0 : block + kEvent);
  Store(base, block + kTrack + 24, random.Below(4) == 0 ? 0 : block + kInstance);
  // The track, its sound, the cue and the banks.
  const uint32_t track = block + kTrack, sound = block + kSound;
  Store(base, block + kInstance, block + kWaveVtable);
  Store(base, block + kInstance + 16, track);
  Store(base, track + 20, sound);
  Store(base, track + 28, random.Below(3) == 0 ? 0 : block + kNext);
  Store(base, track + 36, 0);  // no transition event (sub_825D0B08 never ran)
  Store(base, track + 52, random.Below(4) == 0 ? block + kRpcEntries : 0);
  for (const uint32_t at : {track + 40, track + 56, sound + 8, sound + 88, sound + 112,
                            block + kCue + 28}) {
    guest::StoreFloat(base, at, float(AnyFloat(random)));
  }
  Store(base, block + kNext, block + kWaveVtable);
  Store(base, block + kRpcEntries - 4, 1 + random.Below(3));
  Store(base, block + kRpcEntries + 4, block + kWaveVtable);
  Store(base, sound + 28, block + kSoundRecord);
  Store(base, sound + 32, block + kEngine);
  Store(base, sound + 48, block + kCue);
  // No RPCs (with them the plain C++ declines to the original).
  guest::StoreU8(base, block + kSoundRecord, guest::LoadU8(base, block + kSoundRecord) & ~0x4);
  Store(base, block + kEngine + 124, 0);
  Store(base, block + kCue + 296, block + kSoundBank);
  Store(base, block + kSoundBank + 244, block + kBankCache);
  for (uint32_t i = 0; i < 4; i++) {
    Store(base, block + kBankCache + 4 * i, block + kWaveBank);
    // A non-zero rate (bits 5-22 of the entry's format, +4).
    const uint32_t format = block + kEntries + 24 * i + 4;
    Store(base, format, (Load(base, format) & ~(0x3FFFFu << 5)) | ((8000 + random.Below(44100)) << 5));
  }
  Store(base, block + kWaveBank, kWaveBankVtable);
  Store(base, block + kWaveBank + 272, block + kEntries);
  // The record's type: the play-wave kinds and the others.
  Store(base, block + kRecord,
        (Load(base, block + kRecord) & ~0x1Fu) | Pick(random, {1, 2, 4, 5, 3, 0, 31}));
  // Its wave bank byte, loop count (255 forever) and wave index.
  guest::StoreU8(base, block + kRecord + 10, uint8_t(random.Below(4)));
  guest::StoreU8(base, block + kRecord + 11, uint8_t(Pick(random, {0, 1, 3, 255, random.Below(256)})));
  guest::StoreU16(base, block + kRecord + 8, uint16_t(random.Below(4)));
}

// One case: the block's random words, the world, `setup(random, t)`, then
// the version; compared: what `after(out, t)` adds, the block (the floats as
// floats) and r3.
template <typename Setup, typename After>
void CaseWith(Context& h, Results& results, uint32_t block, GuestFunction recompiled,
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
    World(h.base, block, random);
    setup(random, t);
    ITC_FLAG(native_xact_tracks) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_xact_tracks) = true;
    // `after` adds its outputs first, and may normalize the block (pointers
    // to allocations) before it's read.
    after(out[v], t);
    std::vector<uint8_t> bytes = Read(h.base, block, kBlockBytes);
    for (const uint32_t offset : kFloats) {
      for (uint32_t i = 0; i < 4; i++) {
        bytes[offset + i] = 0;
      }
    }
    out[v].Bytes(bytes);
    for (const uint32_t offset : kFloats) {
      out[v].Float(Load(h.base, block + offset));
    }
    out[v].Word(t.r3.u32);
  }
  h.random = random;
  results.Check(out[0], out[1], what);
}

template <typename Setup>
void Case(Context& h, Results& results, uint32_t block, GuestFunction recompiled,
          GuestFunction native, Setup setup, const std::string& what) {
  CaseWith(h, results, block, recompiled, native, setup, [](Outputs&, PPCContext&) {}, what);
}

struct Entry {
  const char* name;
  GuestFunction recompiled;
  GuestFunction native;
  // The registers: r3 the object at this offset in the block.
  uint32_t object;
  bool f1 = false;
  // The event's wave (+60) never 0: read unchecked (SetQueued, and Fire
  // through it).
  bool wave = false;
};

void Batch1(Context& h, uint32_t block) {
  const Entry entries[] = {
      {"XACTTrackInstance::XACTTrackInstance (sub_825D57D0)", __imp__sub_825D57D0,
       hooks::XactTrackInstanceConstruct, kInstance},
      {"XACTTrackInstance::CheckDone (sub_825D55E0)", __imp__sub_825D55E0,
       hooks::XactTrackInstanceCheckDone, kInstance},
      {"XACTTrackInstance::SetVolume (sub_825D5678)", __imp__sub_825D5678,
       hooks::XactTrackInstanceSetVolume, kInstance, true},
      {"XACTTrackInstance::IsBusy (sub_825D5780)", __imp__sub_825D5780,
       hooks::XactTrackInstanceIsBusy, kInstance},
      {"XACTTrackInstance::Pause (sub_825D57A8)", __imp__sub_825D57A8,
       hooks::XactTrackInstancePause, kInstance},
      {"XACTTrack::Pause (sub_825D05A8)", __imp__sub_825D05A8, hooks::XactTrackPause, kTrack},
      {"XACTTrack::IsBusy (sub_825D0590)", __imp__sub_825D0590, hooks::XactTrackIsBusy, kTrack},
      {"XACTEvent::XACTEvent (sub_825D1760)", __imp__sub_825D1760, hooks::XactEventConstruct,
       kEvent},
      {"XACTEvent::~XACTEvent (sub_825D0EF0)", __imp__sub_825D0EF0, hooks::XactEventDestruct,
       kEvent},
      {"XACTEvent::Fire (sub_825D1358)", __imp__sub_825D1358, hooks::XactEventFire, kEvent,
       false, true},
      {"XACTPlayWaveEvent::XACTPlayWaveEvent (sub_825D82E8)", __imp__sub_825D82E8,
       hooks::XactPlayWaveConstruct, kEvent},
      {"XACTPlayWaveEvent::SetQueued (sub_825D8530)", __imp__sub_825D8530,
       hooks::XactPlayWaveSetQueued, kEvent, false, true},
      {"XACTPlayWaveEvent::GetLoopCount (sub_825D8358)", __imp__sub_825D8358,
       hooks::XactPlayWaveGetLoopCount, kEvent},
      {"XACTPlayWaveEvent::SetWave (sub_82483208)", __imp__sub_82483208,
       hooks::XactPlayWaveSetWave, kEvent},
      {"XACTWaveVariationEvent::SetWave (sub_825D7B78)", __imp__sub_825D7B78,
       hooks::XactWaveVariationSetWave, kEvent},
      {"XACTPlayWaveEvent::GetVariationRecord (sub_825D8920)", __imp__sub_825D8920,
       hooks::XactPlayWaveGetVariationRecord, kEvent},
      {"XACTPlayWaveEvent::ForgetWave (sub_825D8360)", __imp__sub_825D8360,
       hooks::XactPlayWaveForgetWave, kEvent},
      {"XACTPlayWaveEvent::GetWaveBank (sub_825D84C0)", __imp__sub_825D84C0,
       hooks::XactPlayWaveGetWaveBank, kEvent},
      {"XACTPlayWaveEvent::IsPrepared (sub_825D76A0)", __imp__sub_825D76A0,
       hooks::XactPlayWaveIsPrepared, kEvent},
      {"XACTPlayWaveEvent::IsPlaying (sub_825D7708)", __imp__sub_825D7708,
       hooks::XactPlayWaveIsPlaying, kEvent},
      {"XACTPlayWaveEvent::IsBusy (sub_825D81E8)", __imp__sub_825D81E8,
       hooks::XactPlayWaveIsBusy, kEvent},
      {"XACTPlayWaveEvent::Pause (sub_825D8268)", __imp__sub_825D8268, hooks::XactPlayWavePause,
       kEvent},
      {"XACTPlayWaveEvent::SetVolume (sub_825D7FE0)", __imp__sub_825D7FE0,
       hooks::XactPlayWaveSetVolume, kEvent, true},
      {"XACTWave::SetVolume (sub_825D1CC8)", __imp__sub_825D1CC8, hooks::XactWaveSetVolume, kWave,
       true},
  };
  for (const Entry& entry : entries) {
    Results results(entry.name);
    for (int c = 0; c < 96; c++) {
      Case(h, results, block, entry.recompiled, entry.native,
           [&](Random& random, PPCContext& t) {
             t.r3.u64 = block + entry.object;
             // The other arguments (constructors' fields, pause and queued
             // flags, a wave): random, small or 0.
             t.r4.u64 = Pick(random, {0, 1, uint32_t(random.Next())});
             t.r5.u64 = random.Below(2) == 0 ? block + kInstance : uint32_t(random.Next());
             // The record (the play-wave event's constructor reads its byte
             // +11): in the block.
             t.r6.u64 = random.Below(2) == 0 ? block + kRecord
                                             : block + random.Below(kBlockBytes - 16);
             t.r7.u64 = random.Below(2) == 0 ? 0 : block + kEvent2;
             t.r8.u64 = Pick(random, {0, 1, uint32_t(random.Next())});
             if (entry.wave && Load(h.base, block + kEvent + 60) == 0) {
               Store(h.base, block + kEvent + 60, block + kWave2);
             }
             if (entry.f1) {
               t.f1.f64 = AnyFloat(random);
             }
             // The fired, uncounted and queued bits each set or clear.
             guest::StoreU8(h.base, block + kEvent + 40, uint8_t(random.Next()));
             // CheckDone: the counts 0 and the helper list empty one time in
             // two.
             if (random.Below(2) == 0) {
               const uint32_t instance = block + kInstance;
               guest::StoreU8(h.base, instance + 44, uint8_t(random.Below(3) == 0));
               guest::StoreU8(h.base, instance + 45, uint8_t(random.Below(3) == 0));
               const uint32_t list = instance + 48;
               Store(h.base, list + 4, random.Below(4) == 0 ? block : list);
               Store(h.base, list + 8, random.Below(4) == 0 ? block : list);
             }
           },
           fmt::format("case {}", c));
    }
    results.Report();
  }
}

// The deleting destructors with bit 0: the object from the allocation layer
// (XMemAlloc with its attributes) for each version, its +0 and links copied
// from the block's object at `copy`; compared: the block and whether r3 is
// the object.
void FreeCases(Context& h, uint32_t block, const char* name, GuestFunction recompiled,
               GuestFunction native, uint32_t bytes, uint32_t attributes, uint32_t copy) {
  Results results(name);
  if (!hooks::NativeHeapActive()) {
    results.Report();
    return;
  }
  for (int c = 0; c < 48; c++) {
    const Random start = h.random;
    Random random = start;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      random = start;
      for (uint32_t i = 0; i < kBlockBytes; i += 4) {
        Store(h.base, block + i, uint32_t(random.Next()));
      }
      PPCContext t = CallContext(h);
      World(h.base, block, random);
      const uint32_t object = memory::crt::XMemAlloc(h.base, bytes, attributes);
      for (uint32_t i = 0; i < bytes; i += 4) {
        Store(h.base, object + i, Load(h.base, block + copy + i));
      }
      t.r3.u64 = object;
      t.r4.u64 = 1 | (uint32_t(random.Next()) & ~1u);
      ITC_FLAG(native_xact_tracks) = v == 1;
      (v == 0 ? recompiled : native)(t, h.base);
      ITC_FLAG(native_xact_tracks) = true;
      out[v].Bytes(Read(h.base, block, kBlockBytes));
      out[v].Word(t.r3.u32 == object);
    }
    h.random = random;
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

void Batch2(Context& h, uint32_t block) {
  struct Function {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
    uint32_t object;
  };
  const Function functions[] = {
      {"XACTTrack::SetVolume (sub_825D0BF8)", __imp__sub_825D0BF8, hooks::XactTrackSetVolume,
       kTrack},
      {"XACTTrack::UpdateVolume (sub_825D0428)", __imp__sub_825D0428,
       hooks::XactTrackUpdateVolume, kTrack},
      {"XACTTrack::CreateRpcs (sub_825D06B8)", __imp__sub_825D06B8, hooks::XactTrackCreateRpcs,
       kTrack},
      {"XACTTrack::StartRpcs (sub_825D0800)", __imp__sub_825D0800, hooks::XactTrackStartRpcs,
       kTrack},
      {"XACTTrack::XACTTrack (sub_825D0CE0)", __imp__sub_825D0CE0, hooks::XactTrackConstruct,
       kTrack2},
      {"XACTTrack::Update (sub_825D0E30)", __imp__sub_825D0E30, hooks::XactTrackUpdate, kTrack},
      {"XACTTrack::~XACTTrack (sub_825D0110)", __imp__sub_825D0110, hooks::XactTrackDestruct,
       kTrack},
      {"XACTTrack::~XACTTrack, deleting, kept (sub_825D0978)", __imp__sub_825D0978,
       hooks::XactTrackDeletingDestruct, kTrack},
      {"XACTPlayWaveEvent::~XACTPlayWaveEvent, deleting, kept (sub_825D8660)",
       __imp__sub_825D8660, hooks::XactPlayWaveDeletingDestruct, kEvent},
      {"XACTPlayWaveEvent::ApplyVariation (sub_825D7558)", __imp__sub_825D7558,
       hooks::XactPlayWaveApplyVariation, kEvent},
      {"XACTPlayWaveEvent::GetDuration (sub_825D8870)", __imp__sub_825D8870,
       hooks::XactPlayWaveGetDuration, kEvent},
  };
  for (const Function& f : functions) {
    Results results(f.name);
    const bool deleting = f.recompiled == __imp__sub_825D0978 || f.recompiled == __imp__sub_825D8660;
    const bool variation = f.recompiled == __imp__sub_825D7558;
    for (int c = 0; c < 96; c++) {
      Case(h, results, block, f.recompiled, f.native,
           [&](Random& random, PPCContext& t) {
             t.r3.u64 = block + f.object;
             t.f1.f64 = AnyFloat(random);
             // The constructor's engine, sound, record (none, or a byte in
             // the block) and index; the destructors' flags (bit 0 clear:
             // the block's objects aren't freed).
             t.r4.u64 = deleting ? uint32_t(random.Next()) & ~1u : uint32_t(random.Next());
             t.r5.u64 = block + kSound;
             t.r6.u64 = random.Below(3) == 0 ? 0 : block + random.Below(kBlockBytes);
             t.r7.u64 = random.Next();
             if (variation) {
               // A variation record only through the getter's recorder (0):
               // sub_825D1478 never ran in play and isn't called here.
               Store(h.base, block + kEventVtable + 92,
                     Pick(random, {0x825D8920u, kReturn0}));
               Store(h.base, block + kRecord,
                     (Load(h.base, block + kRecord) & ~0x1Fu) | Pick(random, {1, 2, 3, 0, 31}));
             }
           },
           fmt::format("case {}", c));
    }
    results.Report();
  }
  FreeCases(h, block, "XACTTrack::~XACTTrack, deleting, freed (sub_825D0978)",
            __imp__sub_825D0978, hooks::XactTrackDeletingDestruct, 64, xact::kTrackAttributes,
            kTrack);
  FreeCases(h, block, "XACTPlayWaveEvent::~XACTPlayWaveEvent, deleting, freed (sub_825D8660)",
            __imp__sub_825D8660, hooks::XactPlayWaveDeletingDestruct, 72,
            xact::kPlayWaveAttributes, kEvent);
}

// The fake wave bank for the cue's sound bank's four cached banks: prepared
// or not (+252), its list (+240) empty, CreateWave the placing recorder or
// AddRef (`placing` false: always AddRef, its count making r3 a failure).
void FakeBank(uint8_t* base, uint32_t block, Random& random, bool placing) {
  const uint32_t bank = block + kFakeBank;
  for (uint32_t slot = 0; slot < 128; slot += 4) {
    Store(base, block + kFakeBankVtable + slot, kAddRef);
  }
  const bool place = placing && random.Below(4) != 0;
  Store(base, block + kFakeBankVtable + 40, place ? kCreateWaveAt : kAddRef);
  Store(base, bank, block + kFakeBankVtable);
  // AddRef as CreateWave always fails (it leaves no wave).
  Store(base, bank + 4, 0x9FFFFFF0u + random.Below(15));
  Store(base, bank + 252, Pick(random, {0, 1, 1, uint32_t(random.Next())}));
  Store(base, bank + 244, bank + 240);
  Store(base, bank + 248, bank + 240);
  for (uint32_t i = 0; i < 4; i++) {
    Store(base, block + kBankCache + 4 * i, bank);
  }
  // The waves' Prepare: the recorder or AddRef (their counts random).
  Store(base, block + kWaveVtable + 4, Pick(random, {kRecordR4R6, kRecordR4R6, kAddRef}));
  Store(base, block + kEventVtable + 48, Pick(random, {0x825D8358u, kAddRef}));
  guest::StoreU16(base, block + kRecord + 8, uint16_t(Pick(random, {6, 10})));
}

// A pointer word normalized: 1 + its offset into `object` when inside it, 0
// for 0, else 0xFFFFFFFF.
uint32_t Into(uint32_t word, uint32_t object, uint32_t bytes) {
  if (word == 0) {
    return 0;
  }
  return word >= object && word < object + bytes ? 1 + word - object : 0xFFFFFFFFu;
}

void Batch3(Context& h, uint32_t block) {
  {
    // The record's time against offsets before, at and after it.
    Results results("XACTPlayWaveEvent::Prepare (sub_825D86F8)");
    for (int c = 0; c < 128; c++) {
      CaseWith(h, results, block, __imp__sub_825D86F8, hooks::XactPlayWavePrepare,
               [&](Random& random, PPCContext& t) {
                 FakeBank(h.base, block, random, true);
                 const uint32_t time = (Load(h.base, block + kRecord) >> 5) & 0xFFFFFF;
                 t.r3.u64 = block + kEvent;
                 t.r4.u64 = Pick(random, {time, time + 1 + random.Below(5000),
                                          time - random.Below(5000), uint32_t(random.Next()), 0});
               },
               [&](Outputs& out, PPCContext& t) {
                 out.Word(Load(h.base, t.r1.u32 - 128 + 80));  // the new wave
               },
               fmt::format("case {}", c));
    }
    results.Report();
  }
  if (!hooks::NativeHeapActive()) {
    return;
  }
  {
    // A new event's Prepare (its real slots) on the fake bank: not prepared
    // or CreateWave failing (a new wave needs the real CreateWave: the
    // success path is the attract race's). Compared: the instance (its
    // array and event normalized), the array and the event (its self links
    // normalized); then both freed (the failure keeps them, as the original
    // does).
    Results results("XACTTrackInstance::PrepareSimple (sub_825D53D8)");
    for (int c = 0; c < 64; c++) {
      CaseWith(h, results, block, __imp__sub_825D53D8, hooks::XactTrackInstancePrepareSimple,
               [&](Random& random, PPCContext& t) {
                 FakeBank(h.base, block, random, false);
                 t.r3.u64 = block + kInstance;
                 t.r4.u64 = block + kRecord;
                 t.r5.u64 = random.Below(2) == 0 ? 0 : uint32_t(random.Next());
               },
               [&](Outputs& out, PPCContext&) {
                 const uint32_t instance = block + kInstance;
                 const uint32_t array = Load(h.base, instance + 20);
                 const uint32_t event = array != 0 ? Load(h.base, array) : 0;
                 out.Word(array != 0);
                 out.Word(event != 0);
                 out.Word(Into(Load(h.base, instance + 36), event, 72));
                 Store(h.base, instance + 20, 0);
                 Store(h.base, instance + 36, 0);
                 if (event != 0) {
                   std::vector<uint8_t> bytes = Read(h.base, event, 72);
                   for (const uint32_t at : {4u, 8u}) {
                     out.Word(Into(Load(h.base, event + at), event, 72));
                     for (uint32_t i = 0; i < 4; i++) {
                       bytes[at + i] = 0;
                     }
                   }
                   out.Bytes(bytes);
                   memory::crt::XMemFree(event, xact::kPlayWaveAttributes);
                 }
                 if (array != 0) {
                   memory::crt::XMemFree(array, xact::kEventArrayAttributes);
                 }
               },
               fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // As above through the track: the instance made, its prepare failing,
    // and the instance deleted again (its real destructor, the engine's
    // critical section initialized for each version).
    Results results("XACTTrack::PrepareSimple (sub_825D09D0)");
    for (int c = 0; c < 64; c++) {
      CaseWith(h, results, block, __imp__sub_825D09D0, hooks::XactTrackPrepareSimple,
               [&](Random& random, PPCContext& t) {
                 FakeBank(h.base, block, random, false);
                 Store(h.base, block + kTrack + 12, block + kEngine2);
                 PPCContext c2 = t;
                 c2.r3.u64 = block + kEngine2 + 144;
                 __imp__RtlInitializeCriticalSection(c2, h.base);
                 t.r3.u64 = block + kTrack;
                 t.r4.u64 = block + kRecord;
                 t.r5.u64 = random.Below(2) == 0 ? 0 : uint32_t(random.Next());
               },
               [&](Outputs& out, PPCContext&) {
                 // The critical section's words as left (its owner and
                 // counts; nothing holds it after).
                 out.Bytes(Read(h.base, block + kEngine2 + 144, 28));
               },
               fmt::format("case {}", c));
    }
    results.Report();
  }
}

// The world for the instances' cancelling, stopping and deleting: the
// engine (kEngine2) with its critical section initialized for each version
// and its +76 list empty; the instance's helper list empty (a helper never
// was in play; a recorder wouldn't take it off, and the original would spin);
// its node linked to itself or between kNext and kTrack2; up to three events
// (kEvent, kEvent2, kWave2 as a third), each linked to itself, queued or
// not, in `array` (the block's, or one from the allocation layer); the
// event vtable's slot +0 a recorder (the block's events aren't freed) and
// +60 (Stop) a recorder.
void InstanceWorld(Context& h, uint32_t block, Random& random, PPCContext& t, uint32_t array) {
  uint8_t* base = h.base;
  const uint32_t instance = block + kInstance, engine = block + kEngine2;
  Store(base, instance + 12, engine);
  PPCContext c2 = t;
  c2.r3.u64 = engine + 144;
  __imp__RtlInitializeCriticalSection(c2, base);
  Store(base, engine + 80, engine + 76);
  Store(base, engine + 84, engine + 76);
  Store(base, instance + 52, instance + 48);
  Store(base, instance + 56, instance + 48);
  if (random.Below(2) == 0) {
    Store(base, instance + 4, instance);
    Store(base, instance + 8, instance);
  } else {
    Store(base, instance + 4, block + kNext);
    Store(base, instance + 8, block + kTrack2);
  }
  Store(base, block + kEventVtable + 0, Pick(random, {kAddRef, kRecordR4}));
  Store(base, block + kEventVtable + 60, Pick(random, {kAddRef, kRecordR4}));
  Store(base, block + kEventVtable + 24, Pick(random, {0x825D7708u, kAddRef}));
  const uint32_t count = random.Below(4);
  guest::StoreU8(base, instance + 24, uint8_t(count));
  Store(base, instance + 20, array);
  if (array == 0) {
    return;
  }
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t event = block + Pick(random, {kEvent, kEvent2, kWave2});
    Store(base, event, block + kEventVtable);
    Store(base, event + 4, event);
    Store(base, event + 8, event);
    Store(base, event + 44, instance);
    Store(base, event + 60, block + kWave);  // SetQueued reads it unchecked
    guest::StoreU8(base, event + 40, uint8_t(random.Next()));
    // Never 0: CancelEvents reads each entry's flags unchecked.
    Store(base, array + 4 * i, event);
  }
}

void Batch4(Context& h, uint32_t block) {
  const auto lock = [&](Outputs& out, PPCContext&) {
    out.Bytes(Read(h.base, block + kEngine2 + 144, 28));
  };
  {
    Results results("XACTTrackInstance::CancelEvents (sub_825D5520)");
    for (int c = 0; c < 96; c++) {
      CaseWith(h, results, block, __imp__sub_825D5520, hooks::XactTrackInstanceCancelEvents,
               [&](Random& random, PPCContext& t) {
                 InstanceWorld(h, block, random, t, block + kEventArray);
                 t.r3.u64 = block + kInstance;
               },
               lock, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // Stop: every state, the first immediate stop or not, immediate or not.
    Results results("XACTTrackInstance::Stop (sub_825D5D18)");
    for (int c = 0; c < 128; c++) {
      CaseWith(h, results, block, __imp__sub_825D5D18, hooks::XactTrackInstanceStop,
               [&](Random& random, PPCContext& t) {
                 InstanceWorld(h, block, random, t, block + kEventArray);
                 Store(h.base, block + kInstance + 40, random.Below(8));
                 Store(h.base, block + kInstance + 60, Pick(random, {0, 1, uint32_t(random.Next())}));
                 t.r3.u64 = block + kInstance;
                 t.r4.u64 = Pick(random, {0, 1, uint32_t(random.Next())});
               },
               lock, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // The track's Stop over the instance (or none) and a next instance
    // (kNext, or none) linked into the engine's list.
    Results results("XACTTrack::Stop (sub_825D01D8)");
    for (int c = 0; c < 128; c++) {
      CaseWith(h, results, block, __imp__sub_825D01D8, hooks::XactTrackStop,
               [&](Random& random, PPCContext& t) {
                 InstanceWorld(h, block, random, t, block + kEventArray);
                 Store(h.base, block + kInstance + 40, random.Below(8));
                 Store(h.base, block + kInstance + 60, random.Below(2));
                 Store(h.base, block + kTrack + 12, block + kEngine2);
                 Store(h.base, block + kTrack + 24,
                       random.Below(4) == 0 ? 0 : block + kInstance);
                 t.r3.u64 = block + kTrack;
                 t.r4.u64 = Pick(random, {0, 0, 1, uint32_t(random.Next())});
               },
               lock, fmt::format("case {}", c));
    }
    results.Report();
  }
  if (!hooks::NativeHeapActive()) {
    return;
  }
  // The destructors: the events array from the allocation layer (freed by
  // them; none one time in four), r3 normalized (XMemFree's, the array or
  // 0).
  const auto destruct = [&](const char* name, GuestFunction recompiled, GuestFunction native,
                            bool deleting) {
    Results results(name);
    for (int c = 0; c < 96; c++) {
      uint32_t array = 0;
      CaseWith(h, results, block, recompiled, native,
               [&](Random& random, PPCContext& t) {
                 array = random.Below(4) == 0
                             ? 0
                             : memory::crt::XMemAlloc(h.base, 16, xact::kEventArrayAttributes);
                 InstanceWorld(h, block, random, t, array);
                 Store(h.base, block + kInstance + 40, random.Below(8));
                 t.r3.u64 = block + kInstance;
                 t.r4.u64 = deleting ? uint32_t(random.Next()) & ~1u : random.Next();
               },
               [&](Outputs& out, PPCContext& t) {
                 lock(out, t);
                 out.Word(array != 0 && t.r3.u32 == array ? 1 : t.r3.u32);
                 t.r3.u64 = 0;
               },
               fmt::format("case {}", c));
    }
    results.Report();
  };
  destruct("XACTTrackInstance::~XACTTrackInstance (sub_825D5838)", __imp__sub_825D5838,
           hooks::XactTrackInstanceDestruct, false);
  destruct("XACTTrackInstance::~XACTTrackInstance, deleting, kept (sub_825D5E58)",
           __imp__sub_825D5E58, hooks::XactTrackInstanceDeletingDestruct, true);
  {
    // Deleting with bit 0: the instance from the allocation layer (no
    // events, its node and helper list linked to itself), freed.
    Results results("XACTTrackInstance::~XACTTrackInstance, deleting, freed (sub_825D5E58)");
    for (int c = 0; c < 32; c++) {
      const Random start = h.random;
      Random random = start;
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        random = start;
        PPCContext t = CallContext(h);
        const uint32_t instance =
            memory::crt::XMemAlloc(h.base, 64, xact::kTrackInstanceAttributes);
        for (uint32_t i = 0; i < 64; i += 4) {
          Store(h.base, instance + i, uint32_t(random.Next()));
        }
        Store(h.base, instance + 4, instance);
        Store(h.base, instance + 8, instance);
        Store(h.base, instance + 20, 0);
        Store(h.base, instance + 52, instance + 48);
        Store(h.base, instance + 56, instance + 48);
        Store(h.base, instance + 40, random.Below(8));
        t.r3.u64 = instance;
        t.r4.u64 = 1 | (uint32_t(random.Next()) & ~1u);
        ITC_FLAG(native_xact_tracks) = v == 1;
        (v == 0 ? __imp__sub_825D5E58 : hooks::XactTrackInstanceDeletingDestruct)(t, h.base);
        ITC_FLAG(native_xact_tracks) = true;
        out[v].Word(t.r3.u32 == instance);
      }
      h.random = random;
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
}

void Batch5(Context& h, uint32_t block) {
  if (!hooks::NativeHeapActive()) {
    return;
  }
  // Create: the play-wave types (1, 2, 4, 5), 20 (nothing made: [out] as it
  // was, 0 or not), above 20 (an error) and the stop event (0: the
  // original's, as both sides run it). Compared: the block with [out]
  // normalized, the new event (its self links normalized), r3; the event
  // freed.
  const uint32_t out_word = block + kEventArray + 64;
  Results results("XACTEvent::Create (sub_825D0F00)");
  for (int c = 0; c < 128; c++) {
    uint32_t type = 0;
    CaseWith(h, results, block, __imp__sub_825D0F00, hooks::XactEventCreate,
             [&](Random& random, PPCContext& t) {
               type = Pick(random, {1, 2, 4, 5, 1, 4, 20, 20, 21, 31, 0});
               Store(h.base, block + kRecord, (Load(h.base, block + kRecord) & ~0x1Fu) | type);
               Store(h.base, out_word, random.Below(2) == 0 ? 0 : uint32_t(random.Next()));
               t.r3.u64 = random.Next();
               t.r4.u64 = block + kInstance;
               t.r5.u64 = block + kRecord;
               t.r6.u64 = random.Below(2) == 0 ? 0 : block + kEvent2;
               t.r7.u64 = out_word;
             },
             [&](Outputs& out, PPCContext&) {
               const uint32_t event = Load(h.base, out_word);
               const bool made = type <= 5 || type == 0;
               out.Word(made ? event != 0 : event);
               if (made && event != 0) {
                 std::vector<uint8_t> bytes = Read(h.base, event, type == 0 ? 48 : 72);
                 for (const uint32_t at : {4u, 8u}) {
                   out.Word(Into(Load(h.base, event + at), event, 72));
                   for (uint32_t i = 0; i < 4; i++) {
                     bytes[at + i] = 0;
                   }
                 }
                 out.Bytes(bytes);
                 memory::crt::XMemFree(event, type == 0 ? 0x20848013u : xact::kPlayWaveAttributes);
               }
               Store(h.base, out_word, 0);
             },
             fmt::format("case {}", c));
  }
  results.Report();
}

// The second migration's: a sound bank for the instances' Prepare (its fake
// vtable: FirstEvent +24, NextEvent +28 and GetHeader +64 the real ones; the
// cached wave banks the fake bank's), its header with the track's event
// records, and the out word of CreateInstance.
constexpr uint32_t kSoundBankVtable2 = 0x1B00, kSoundBank2 = 0x1B80, kHeader = 0x1CA0,
                   kInstanceOut = 0x1A48;

// The records' sizes by type, as NextEvent steps over them (the types used).
uint32_t RecordBytes(uint32_t type) {
  switch (type) {
    case 0: return 8;
    case 1: return 12;
    case 2: return 16;
    case 4: return 19;
    case 5: return 23;
    case 9: return 11;
    case 12: return 42;
    case 14: return 13;
    default: return 48;  // 20 and 31 end the walk (a failure)
  }
}

// The track's event records: 1 to 5 at the header's `key` (a count byte, then
// the records): the play-wave kinds (their Prepare fails on the fake bank:
// not prepared, or CreateWave's AddRef count one of the errors, the first
// XACTENGINE_E_SEEKTIMEBEYONDCUEEND one time in three), markers before the
// offset (dropped), the stop event and types 12 and 14 (the base's Prepare:
// kept), 20 (nothing made) and 31 (invalid). Chained one time in two: the
// first record kept and the others' previous event the array's entry 0;
// else each record has no previous event (bit 0x20000000). Returns the key;
// `offset` is set at 0x100 or more, after every marker's time.
uint32_t TrackRecords(uint8_t* base, uint32_t block, Random& random, uint32_t& offset) {
  const uint32_t bank = block + kSoundBank2, vtable = block + kSoundBankVtable2;
  for (uint32_t slot = 0; slot < 128; slot += 4) {
    Store(base, vtable + slot, kAddRef);
  }
  Store(base, vtable + 24, 0x825C7F30);
  Store(base, vtable + 28, 0x825C7F98);
  Store(base, vtable + 64, 0x825C7AA8);
  Store(base, bank, vtable);
  Store(base, bank + 244, block + kBankCache);
  Store(base, bank + 260, block + kHeader);
  Store(base, block + kCue + 296, bank);
  FakeBank(base, block, random, false);
  Store(base, block + kFakeBank + 4,
        Pick(random, {0x8AC70018u, 0x8AC70017u, 0x9FFFFFF0u + random.Below(15)}));
  const bool chained = random.Below(2) == 0;
  const uint32_t key = random.Below(16);
  const uint32_t count = 1 + random.Below(5);
  guest::StoreU8(base, block + kHeader + key, uint8_t(count));
  uint32_t at = block + kHeader + key + 1;
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t type = chained && i == 0
                              ? Pick(random, {0, 12, 14})
                              : Pick(random, {1, 2, 4, 5, 1, 9, 9, 0, 12, 14, 12, 14, 20, 31});
    const uint32_t time = type == 9 ? random.Below(0x100) : random.Below(0x1000000);
    const bool none = !chained || i == 0 || random.Below(4) == 0;
    Store(base, at, (uint32_t(random.Next()) & 0xC0000000u) | (none ? 0x20000000u : 0) |
                        (time << 5) | type);
    guest::StoreU8(base, at + 6, 0);
    guest::StoreU16(base, at + 8, uint16_t(Pick(random, {6, 10})));
    guest::StoreU8(base, at + 10, uint8_t(random.Below(4)));
    at += RecordBytes(type);
  }
  offset = 0x100 + Pick(random, {random.Below(0x100), random.Below(0x1000000),
                                 uint32_t(random.Next()) & 0x3FFFFFFF});
  return key;
}

// An instance's events (the array at +20, byte +24 of them) and the
// instance compared with their pointers normalized (0; inside the instance;
// an event of the array; inside the event; else as is), then the events
// deleted (their real deleting destructors) and the array freed; the
// instance's +20 and +36 cleared.
void InstanceOut(Context& h, Outputs& out, uint32_t instance, PPCContext& t) {
  uint8_t* base = h.base;
  const uint32_t array = Load(base, instance + 20);
  const uint32_t count = guest::LoadU8(base, instance + 24);
  std::vector<uint32_t> events;
  if (array != 0) {
    for (uint32_t i = 0; i < count; i++) {
      events.push_back(Load(base, array + 4 * i));
    }
  }
  const auto norm = [&](uint32_t word, uint32_t self, uint32_t bytes) {
    if (word == 0) {
      return 0u;
    }
    if (word >= instance && word < instance + 64) {
      return 0x1000 + word - instance;
    }
    for (uint32_t i = 0; i < events.size(); i++) {
      if (word == events[i]) {
        return 0x2000 + i;
      }
    }
    if (word >= self && word < self + bytes) {
      return 0x3000 + word - self;
    }
    return word;
  };
  out.Word(array != 0);
  out.Word(count);
  {
    std::vector<uint8_t> bytes = Read(base, instance, 64);
    for (const uint32_t at : {4u, 8u, 20u, 36u, 52u, 56u}) {
      const uint32_t word = Load(base, instance + at);
      out.Word(at == 20 ? word != 0 : norm(word, instance, 64));
      for (uint32_t i = 0; i < 4; i++) {
        bytes[at + i] = 0;
      }
    }
    out.Bytes(bytes);
  }
  for (const uint32_t event : events) {
    out.Word(event != 0);
    if (event == 0) {
      continue;
    }
    const uint32_t vtable = Load(base, event);
    GuestFunction destruct = nullptr;
    uint32_t bytes = 48;
    switch (vtable) {
      case 0x82076CE8: destruct = __imp__sub_825D8660; bytes = 72; break;
      case 0x82076ADC: destruct = __imp__sub_825D5F80; break;
      case 0x82076B44: destruct = __imp__sub_825D7148; bytes = 52; break;
      case 0x82076D68: destruct = __imp__sub_825D8990; break;
      default: break;
    }
    out.Word(vtable);
    std::vector<uint8_t> data = Read(base, event, bytes);
    for (const uint32_t at : {4u, 8u, 36u, 44u}) {
      out.Word(norm(Load(base, event + at), event, bytes));
      for (uint32_t i = 0; i < 4; i++) {
        data[at + i] = 0;
      }
    }
    out.Bytes(data);
    if (destruct != nullptr) {
      PPCContext d = t;
      d.r1.u64 = CallContext(h).r1.u64;
      d.r3.u64 = event;
      d.r4.u64 = 1;
      destruct(d, base);
    }
  }
  if (array != 0) {
    memory::crt::XMemFree(array, xact::kEventArrayAttributes);
  }
  Store(base, instance + 20, 0);
  Store(base, instance + 36, 0);
}

// XACTTrackInstance::Prepare and the track's CreateInstance and Prepare over
// it: the records above walked by the real FirstEvent and NextEvent, the
// events made by the real Create (the play-wave kinds natively, the others
// the original on both sides). Every play-wave event's Prepare fails here (a
// new wave needs the real CreateWave: its success path, a play-wave event
// kept as +36 and IsPrepared at the end, is the attract race's); the kept
// events are the stop event's and types 12's and 14's.
void Batch6(Context& h, uint32_t block) {
  if (!hooks::NativeHeapActive()) {
    return;
  }
  {
    Results results("XACTTrackInstance::Prepare (sub_825D5A48)");
    for (int c = 0; c < 128; c++) {
      CaseWith(h, results, block, __imp__sub_825D5A48, hooks::XactTrackInstancePrepare,
               [&](Random& random, PPCContext& t) {
                 uint32_t offset = 0;
                 t.r4.u64 = TrackRecords(h.base, block, random, offset);
                 t.r3.u64 = block + kInstance;
                 t.r5.u64 = offset;
                 Store(h.base, block + kInstance + 36, random.Below(2) == 0 ? 0 : block + kEvent);
               },
               [&](Outputs& out, PPCContext& t) { InstanceOut(h, out, block + kInstance, t); },
               fmt::format("case {}", c));
    }
    results.Report();
  }
  // The track's: the instance from the allocation layer (deleted by the
  // function on a failure, its real destructor taking the engine's critical
  // section; compared and freed here on a success), [out] or +24.
  const auto track_cases = [&](const char* name, GuestFunction recompiled, GuestFunction native,
                               bool create) {
    Results results(name);
    for (int c = 0; c < 96; c++) {
      const uint32_t at = create ? block + kInstanceOut : block + kTrack + 24;
      uint32_t before = 0;
      CaseWith(h, results, block, recompiled, native,
               [&](Random& random, PPCContext& t) {
                 uint32_t offset = 0;
                 const uint32_t key = TrackRecords(h.base, block, random, offset);
                 Store(h.base, block + kTrack + 12, block + kEngine2);
                 PPCContext c2 = t;
                 c2.r3.u64 = block + kEngine2 + 144;
                 __imp__RtlInitializeCriticalSection(c2, h.base);
                 before = random.Below(2) == 0 ? 0 : uint32_t(random.Next());
                 Store(h.base, at, before);
                 t.r3.u64 = block + kTrack;
                 t.r4.u64 = key;
                 if (create) {
                   t.r5.u64 = at;
                   t.r6.u64 = offset;
                 } else {
                   t.r5.u64 = offset;
                 }
               },
               [&](Outputs& out, PPCContext& t) {
                 out.Bytes(Read(h.base, block + kEngine2 + 144, 28));
                 const uint32_t instance = Load(h.base, at);
                 const bool made = int32_t(t.r3.u32) >= 0;
                 out.Word(made);
                 if (made) {
                   InstanceOut(h, out, instance, t);
                   memory::crt::XMemFree(instance, xact::kTrackInstanceAttributes);
                 } else {
                   out.Word(instance == before);
                 }
                 Store(h.base, at, 0);
               },
               fmt::format("case {}", c));
    }
    results.Report();
  };
  track_cases("XACTTrack::CreateInstance (sub_825D02F8)", __imp__sub_825D02F8,
              hooks::XactTrackCreateInstance, true);
  track_cases("XACTTrack::Prepare (sub_825D0AA0)", __imp__sub_825D0AA0, hooks::XactTrackPrepare,
              false);
}

}  // namespace

void XactTracks(Context& h) {
  Block block;
  if (!block.address) {
    ITC_LOG_ERROR("harness: xact_tracks: no block from the allocation layer");
    return;
  }
  Batch1(h, block.address);
  Batch2(h, block.address);
  Batch3(h, block.address);
  Batch4(h, block.address);
  Batch5(h, block.address);
  Batch6(h, block.address);
}

}  // namespace harness
