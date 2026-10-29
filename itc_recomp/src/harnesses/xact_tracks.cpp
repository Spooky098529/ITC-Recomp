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
#include "hooks/xact_tracks.h"
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

// The block's floats: what kRecordF1 stores at +112 of the objects it's
// given.
const std::vector<uint32_t> kFloats = {kEvent + 112, kEvent2 + 112, kWave + 112, kWave2 + 112,
                                       kVoice + 112};

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
  // The record's type: the play-wave kinds and the others.
  Store(base, block + kRecord,
        (Load(base, block + kRecord) & ~0x1Fu) | Pick(random, {1, 2, 4, 5, 3, 0, 31}));
}

// One case: the block's random words, the world, `setup(random, t)`, then
// the version; compared: the block (the floats as floats) and r3.
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
    World(h.base, block, random);
    setup(random, t);
    ITC_FLAG(native_xact_tracks) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_xact_tracks) = true;
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

}  // namespace

void XactTracks(Context& h) {
  Block block;
  if (!block.address) {
    ITC_LOG_ERROR("harness: xact_tracks: no block from the allocation layer");
    return;
  }
  Batch1(h, block.address);
}

}  // namespace harness
