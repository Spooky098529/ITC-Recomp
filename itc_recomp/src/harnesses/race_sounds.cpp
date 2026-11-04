// The race's sounds (src/race_sounds/, docs/functions/race-sounds.md)
// against the recompiled functions. The originals run as `__imp__`, the
// plain C++ through the register steps (hooks/race_sounds.h). The sound
// manager pointer [0x826C5C80] is pointed at a fake manager in the scratch
// area whose vtable's slots hold leaf recorders (recompiled, the same on
// both sides; as the menu sounds' harness), so no sound plays; the car
// sounds' array [0x826C5C3C] at seven car sounds in a block from the
// allocation layer, their holders' voices fake ones there with recorder
// vtables (as the car sounds' harness builds them), so the car sounds'
// plain C++ (called by both versions) walks them without XAudio's engine;
// the machine's words, the scene models' slots' buffers and the effects'
// bank (one wave bank with PCM waves), the cars' views, the racers,
// the race's progress, the race data, the sound parameters' lists
// (0x82616318) and the item table ([0x826C5CA0]) in that block too, with no
// vehicles ([0x826C2E34] 0, so CarSound::Update does nothing). The globals
// are put back after each side. CarSound::Start (SetUpCars, UpdateCars)
// makes XAudio's own voices over that bank: as the car sounds'
// harness's real cases, the submix word [0x826C5C44] 0 while a case runs,
// the mixer held out by its lock, and every voice made (a holder's slot
// that isn't a fake voice) stopped, taken back out of the engine's source
// list and freed before the next version, its pointers compared as (its
// index, the offset). Compared with harness::Outputs: the scratch
// area, the block, the frame's words below the stack pointer (the register
// saves left out: adapters don't save them), r1, r3 and the globals (the
// volumes as floats).

#include "harness.h"

#include "base/log.h"
#include "game_modes/game_modes.h"
#include "hooks/heap.h"
#include "hooks/race_sounds.h"
#include "hooks/xaudio.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "race_sounds/race_sounds.h"
#include "xaudio/voices.h"

#include <bit>
#include <initializer_list>
#include <string>
#include <vector>

namespace harness {

namespace {

namespace rs = race_sounds;

// The leaf recorders: sub_820A8420 (r4's low byte to +320, r5 to +332, r6
// to +336; r3 left), sub_820CE9F8 (r4, r5 to +444, +448; r3 left), the
// getter sub_82090770 (r3 = +60).
constexpr uint32_t kRecordR4R6 = 0x820A8420, kRecordR4 = 0x820CE9F8, kGet60 = 0x82090770;
// The voices' recorders, as the car sounds' harness picks them.
constexpr uint32_t kVoiceRecorders[] = {0x8245F860, 0x820CE9F8, 0x820B6950};

// In the scratch area: the fake manager (+0 its vtable) and its vtable.
constexpr uint32_t kManager = 0x000, kVtable = 0x400;
constexpr uint32_t kCompared = 0x500;

// In the block.
constexpr uint32_t kBlockBytes = 0x4000;
constexpr uint32_t kVoices = 0x2300, kVoiceBytes = 0x100, kVoiceCount = 10;  // the fake voices
constexpr uint32_t kVoiceVtable = 0x2D00;
constexpr uint32_t kMachineWords = 0x2E00;  // the machine at this - 9600
constexpr uint32_t kBanks = 0x2F00;         // the wave bank (0x300 bytes)
constexpr uint32_t kViews = 0x3300;         // 14 views, 0x20 bytes each
constexpr uint32_t kRaceAt = 0x3500;        // the race's +2308 (its +2364 the count)
constexpr uint32_t kRacers = 0x3540;        // the racers' records, 0x20 bytes each
constexpr uint32_t kResults = 0x3600;       // the progress' +2184
constexpr uint32_t kRaceDataAt = 0x3E00;    // the race data's +32
constexpr uint32_t kParameterLists = 0x3E40;
constexpr uint32_t kParameters = 0x3F00;
constexpr uint32_t kItem = 0x3F80;          // the case's item
constexpr uint32_t kSoundHolders[] = {408 + 44, 408 + 128, 952, 584, 676, 736,
                                      840,      792,       888, 1016, 1072, 1136};

constexpr uint32_t kVehicles = 0x826C2E34;
constexpr uint32_t kSaveData = 0x826C5B58;  // player 0's save data
constexpr uint32_t kSaveAt = 0x3FC0;        // its +6100 in the block
// For AfterRaceTick, a bigger block holding the race data's racer records
// (+0x59AE8: the count at +0x49FE0, 3,936 bytes each) and its +0xA3AF4.
constexpr uint32_t kBigBytes = 0x4B000;
constexpr uint32_t kTables = 0x826C5CA0;
constexpr uint32_t kSoundParameters = 0x82616318;
constexpr uint32_t kTocLow = 0x826A5120;  // the TOC of assets below 23011
constexpr uint32_t kTocEntry = 0x3FD0;    // the effects' bank's TOC entry in the block

// The manager's slots the functions call (and Music::Play's slot 4,
// Sound::Play's slot 10).
constexpr uint32_t kSlots[] = {16,
                               40,
                               rs::kReleaseBankSlot,
                               rs::kIsBankLoadingSlot,
                               rs::kIsBankSlotEmptySlot,
                               rs::kStopCueSlot,
                               rs::kIsCuePlayingSlot,
                               rs::kSetListenerPositionSlot,
                               rs::kSetListenerOrientationSlot,
                               rs::kSetListenerVelocitySlot,
                               rs::kGetCategorySlot,
                               rs::kSetCategoryVolumeSlot,
                               rs::kPauseCategorySlot,
                               rs::kResumeCategorySlot};

// The globals put back after each side; the first four floats.
constexpr int kFloatGlobals = 4;
// The harness thread's rand seed (+20 of its C runtime data), put back
// with the globals once known.
uint32_t g_seed = 0;

std::vector<uint32_t> Globals() {
  std::vector<uint32_t> g = {rs::kMusicVolume, rs::kKeptMusicVolume, rs::kEffectsVolume,
                             rs::kEffectsVolume2, rs::kSoundManager, rs::kCarSounds,
                             rs::kEffectsBankArrived, rs::kRace, rs::kProgress, rs::kRaceData,
                             rs::kSystemUi0, rs::kSystemUi1, kVehicles, kTables,
                             kSaveData, rs::kAfterRaceWord};
  for (uint32_t i = 0; i < 7; i++) {
    for (const uint32_t at : {rs::kSlotBuffer0, rs::kSlotBuffer1, rs::kSlotAsked,
                              rs::kSlotArrived, rs::kBankIndexI, rs::kBankIndexJ}) {
      g.push_back(at + 4 * i);
    }
    g.push_back(rs::kCarViews + 8 * i);
    g.push_back(rs::kCarViews + 8 * i + 4);
  }
  for (uint32_t i = 0; i < 4; i++) {
    g.push_back(kSoundParameters + 16 * i);
  }
  g.push_back(kTocLow);
  if (g_seed != 0) {
    g.push_back(g_seed);
  }
  return g;
}

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

// XAudio's mixer lock and the car sounds' submix word.
constexpr uint32_t kMixLock = 0x8264D4F8;
constexpr uint32_t kSubmix = 0x826C5C44;

struct RealVoice {
  uint32_t voice, allocator, bytes;
};

// The voices made in the car sounds' holders' slots (not 0, not a fake
// voice), in order.
std::vector<RealVoice> MadeVoices(uint8_t* base, uint32_t block) {
  std::vector<RealVoice> made;
  for (uint32_t k = 0; k < rs::kCarSoundCount; k++) {
    const uint32_t sound = block + 16 + rs::kCarSoundBytes * k;
    for (const uint32_t offset : kSoundHolders) {
      for (uint32_t i = 0; i < 4; i++) {
        const uint32_t voice = guest::LoadU32(base, sound + offset + 4 + 4 * i);
        if (voice == 0 || (voice >= block && voice < block + kBlockBytes)) {
          continue;
        }
        const uint32_t allocator = guest::LoadU32(base, voice + 8);
        made.push_back({voice, allocator, guest::LoadU32(base, allocator + 16) + 28});
      }
    }
  }
  return made;
}

// The words of [address, address + bytes) with those pointing into a made
// voice's allocator block as (its index, the offset), the engine's two
// stage lists as which of the two is the next one (the car sounds'
// harness's Relative), the rest as they are.
void Relative(Outputs& out, uint8_t* base, uint32_t address, uint32_t bytes,
              const std::vector<RealVoice>& made) {
  std::vector<uint8_t> words = Read(base, address, bytes);
  const uint32_t engine = guest::LoadU32(base, xaudio::kEngine);
  const uint32_t next_list = guest::LoadU32(base, engine + 120);
  const uint32_t other_list = next_list == engine + 92 ? engine + 104 : engine + 92;
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    const uint32_t word = guest::LoadU32(base, address + i);
    uint32_t value = word;
    if (word == next_list || word == other_list) {
      value = word == next_list ? 0xB0000000u : 0xB0000001u;
    } else {
      for (uint32_t k = 0; k < made.size(); k++) {
        if (word >= made[k].allocator && word < made[k].allocator + made[k].bytes) {
          value = 0xA0000000u | (k << 20) | (word - made[k].allocator);
          break;
        }
      }
    }
    for (uint32_t b = 0; b < 4; b++) {
      words[i + b] = uint8_t(value >> (24 - 8 * b));
    }
  }
  out.Bytes(words);
}

void CriticalSection(const Context& h, uint32_t section, bool enter) {
  PPCContext c = CallContext(h);
  c.r3.u64 = section;
  if (enter) {
    __imp__RtlEnterCriticalSection(c, h.base);
  } else {
    __imp__RtlLeaveCriticalSection(c, h.base);
  }
}

uint32_t MasteringVoice(uint8_t* base) {
  return guest::LoadU32(base, guest::LoadU32(base, xaudio::kEngine) + 64);
}

uint32_t Pick(Random& random, std::initializer_list<uint32_t> choices) {
  return *(choices.begin() + random.Below(uint32_t(choices.size())));
}

// A volume: 0.0, 1.0, -0.0, a negative, just under 0.1, any bits or a
// fraction.
uint32_t Volume(Random& r) {
  switch (r.Below(8)) {
    case 0:
      return 0;
    case 1:
      return 0x3F800000u;
    case 2:
      return 0x80000000u;  // -0.0
    case 3:
      return 0xBDCCCCCDu;  // -0.1
    case 4:
      return 0x3DCCCCCCu;  // just under 0.1
    case 5:
      return uint32_t(r.Next());
    default: {
      const float value = float(r.Below(1001)) / 1000.0f;
      return std::bit_cast<uint32_t>(value);
    }
  }
}

// A PCM format: 8 or 16 bits, 1 or 2 channels, 22,050 to 48,000 Hz.
uint32_t PcmFormat(Random& random) {
  static constexpr uint32_t kRates[] = {22050, 24000, 32000, 44100, 48000};
  const uint32_t channels = 1 + random.Below(2);
  const uint32_t rate = kRates[random.Below(5)];
  return (random.Below(2) == 0 ? 0x80000000u : 0) | (rate << 5) | (channels << 2);
}

uint32_t FakeVoice(uint32_t block, uint32_t k) { return block + kVoices + kVoiceBytes * k; }

// The world, laid out the same for both versions from `r` (a copy).
void Lay(Context& h, uint32_t block, Random r, bool fake_voices) {
  uint8_t* base = h.base;
  // The fake manager: random words, its vtable's slots recorders, +60 the
  // getter's answer (0, 1 or any word).
  Fill(base, h.data, kScratchBytes);
  const uint32_t manager = h.data + kManager;
  for (uint32_t i = 0; i < kVtable; i += 4) {
    Store(base, manager + i, uint32_t(r.Next()));
  }
  Store(base, manager, h.data + kVtable);
  Store(base, manager + 60, Pick(r, {0u, 1u, 1u, 0u, uint32_t(r.Next())}));
  for (uint32_t i = 0; i < 0x100; i += 4) {
    Store(base, h.data + kVtable + i, uint32_t(r.Next()));
  }
  for (const uint32_t slot : kSlots) {
    Store(base, h.data + kVtable + slot, Pick(r, {kRecordR4R6, kRecordR4, kGet60}));
  }
  // The block: random words, the voices and their vtable, the car sounds'
  // holders (each slot a fake voice or 0, their +24 words 0 or random; no
  // fake voice where real ones are made: XAudio would take them into its
  // lists), no vehicle (+400).
  for (uint32_t i = 0; i < kBlockBytes; i += 4) {
    Store(base, block + i, uint32_t(r.Next()));
  }
  for (uint32_t slot = 0; slot < 0x100; slot += 4) {
    Store(base, block + kVoiceVtable + slot, kVoiceRecorders[r.Below(3)]);
  }
  for (uint32_t k = 0; k < kVoiceCount; k++) {
    const uint32_t voice = FakeVoice(block, k);
    Store(base, voice, block + kVoiceVtable);
    Store(base, voice + 4, r.Below(3));
    Store(base, voice + 76, FakeVoice(block, r.Below(kVoiceCount)));
    guest::StoreU8(base, voice + 61, guest::LoadU8(base, voice + 61) & 0xBF);
  }
  Store(base, block, rs::kCarSoundCount);
  for (uint32_t k = 0; k < rs::kCarSoundCount; k++) {
    const uint32_t sound = block + 16 + rs::kCarSoundBytes * k;
    for (const uint32_t offset : kSoundHolders) {
      for (uint32_t i = 0; i < 4; i++) {
        const uint32_t voice = r.Below(3) == 0 ? 0 : FakeVoice(block, r.Below(kVoiceCount));
        Store(base, sound + offset + 4 + 4 * i, fake_voices ? voice : 0);
        Store(base, sound + offset + 24 + 4 * i, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
      }
    }
    Store(base, sound + 400, 0);
  }
  // The machine: the flags mostly the bits the handlers test, the wait
  // mostly 0 or below, the effects' bank an empty bank.
  const uint32_t machine = block + kMachineWords - 9600;
  Store(base, machine + rs::kFlags,
        r.Below(4) == 0 ? uint32_t(r.Next()) : (uint32_t(r.Next()) & 0x3FFF4));
  Store(base, machine + rs::kSetUpWait, Pick(r, {0u, 0u, 0u, 1u, 10u, 0xFFFFFFFFu}));
  Store(base, machine + rs::kCarsPaused, r.Below(2));
  // One wave bank for every slot and the effects (offsets at +8, +16 and
  // +40: the header at +0x40, the entries at +0x100, the wave data at
  // +0x200), 4 to 6 waves with PCM formats XAudio takes (as the car
  // sounds' harness's real cases), and the views.
  const uint32_t bank = block + kBanks;
  for (uint32_t i = 0; i < 0x300; i += 4) {
    Store(base, bank + i, 0);
  }
  Store(base, bank + 8, 0x40);
  Store(base, bank + 16, 0x100);
  Store(base, bank + 40, 0x200);
  Store(base, bank + 0x40, r.Below(2) == 0 ? 0 : 0x20000);
  Store(base, bank + 0x44, 4 + r.Below(3));
  Store(base, bank + 0x40 + 84, PcmFormat(r));
  for (uint32_t i = 0; i < 8; i++) {
    const uint32_t entry = bank + 0x100 + 24 * i;
    Store(base, entry + 4, PcmFormat(r));
    Store(base, entry + 8, 4 * r.Below(32));
    Store(base, entry + 12, 4 * (1 + r.Below(32)));
    Store(base, entry + 16, r.Below(32));
    Store(base, entry + 20, r.Below(32));
  }
  Store(base, machine + rs::kEffectsBank, bank);
  for (uint32_t i = 0; i < 7; i++) {
    Store(base, rs::kSlotBuffer0 + 4 * i, bank);
    Store(base, rs::kSlotBuffer1 + 4 * i, bank);
    Store(base, rs::kSlotAsked + 4 * i, r.Below(2));
    Store(base, rs::kSlotArrived + 4 * i, r.Below(4) == 0 ? 0 : 1 + r.Below(2));
    Store(base, rs::kBankIndexI + 4 * i, r.Below(4));
    Store(base, rs::kBankIndexJ + 4 * i, r.Below(4));
    Store(base, rs::kCarViews + 8 * i, block + kViews + 0x40 * i);
    Store(base, rs::kCarViews + 8 * i + 4, block + kViews + 0x40 * i + 0x20);
  }
  // The racers: up to seven, each a car's index.
  const uint32_t racers = r.Below(8);
  Store(base, block + kRaceAt + 56, racers);
  for (uint32_t n = 0; n < 7; n++) {
    Store(base, block + kRaceAt + 4 * n, block + kRacers + 0x20 * n);
    Store(base, block + kRacers + 0x20 * n + 20, r.Below(7));
  }
  // The race's progress: one or two results, the last's item, the item
  // table's entries landing on the block's kItem (its class byte at +35).
  {
    const uint32_t count = 1 + r.Below(2);
    const uint32_t item = Pick(r, {0xFFFFFFFFu, 383u, 391u, 395u, 398u, 5u});
    Store(base, block + kResults, count);
    Store(base, block + kResults + 972 * count - 964, item);
    Store(base, kTables, block + kItem - 16);
    Store(base, block + kItem, block + kItem - 3008 * item);
    guest::StoreU8(base, block + kItem + 35, uint8_t(r.Below(7)));
  }
  // The sound parameters' lists: entry i a list of pointers into the block.
  for (uint32_t i = 0; i < 4; i++) {
    Store(base, kSoundParameters + 16 * i, block + kParameterLists + 0x10 * i);
    for (uint32_t j = 0; j < 4; j++) {
      Store(base, block + kParameterLists + 0x10 * i + 4 * j, block + kParameters + 4 * r.Below(32));
    }
  }
  for (int i = 0; i < kFloatGlobals; i++) {
    static constexpr uint32_t kVolumes[] = {rs::kMusicVolume, rs::kKeptMusicVolume,
                                            rs::kEffectsVolume, rs::kEffectsVolume2};
    Store(base, kVolumes[i], Volume(r));
  }
  Store(base, rs::kEffectsBankArrived, r.Below(4) == 0 ? 0 : 1);
  Store(base, rs::kRace, block + kRaceAt - 2308);
  Store(base, rs::kProgress, block + kResults - 2184);
  Store(base, rs::kRaceData, block + kRaceDataAt - 32);
  Store(base, rs::kSystemUi0, r.Below(3) == 0 ? 1 : 0);
  Store(base, rs::kSystemUi1, r.Below(3) == 0 ? uint32_t(r.Next()) : 0);
  Store(base, kVehicles, 0);
  Store(base, rs::kSoundManager, manager);
  Store(base, rs::kCarSounds, block + 16);
}

// A group: `count` cases of `recompiled` against `native`; after the world
// is laid, `setup(random, regs, block)` (the same random state for both
// versions) changes it and sets r3 to r6 (r3 the machine, r4 to r6 random
// words before it); the frame's `frame` bytes below the stack pointer
// compared up to `saves` bytes from its top.
template <typename Setup>
void Group(Context& h, uint32_t block, const std::string& name, int count,
           GuestFunction recompiled, GuestFunction native, uint32_t frame, uint32_t saves,
           Setup setup, bool real = false) {
  Results results(name);
  const std::vector<uint32_t> globals = Globals();
  std::vector<uint32_t> saved;
  for (const uint32_t at : globals) {
    saved.push_back(guest::LoadU32(h.base, at));
  }
  const uint32_t machine = block + kMachineWords - 9600;
  uint32_t source_offset = 0, saved_submix = 0;
  if (real) {
    source_offset = guest::LoadU32(h.base, guest::LoadU32(h.base, xaudio::kEngine) + 88);
    saved_submix = guest::LoadU32(h.base, kSubmix);
  }
  uint32_t voices_made = 0;
  for (int c = 0; c < count; c++) {
    const Random start = h.random;
    Random random = start;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      random = start;
      Lay(h, block, Random(random.Next()), !real);
      uint64_t regs[4] = {machine, random.Next(), random.Next(), random.Next()};
      setup(random, regs);
      const uint32_t fill = uint32_t(random.Next());
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      for (uint32_t k = 0; k < 0x400; k += 4) {
        Store(h.base, sp - 0x400 + k, fill + k);
      }
      t.r3.u64 = regs[0];
      t.r4.u64 = regs[1];
      t.r5.u64 = regs[2];
      t.r6.u64 = regs[3];
      uint32_t mastering = 0, mastering_count = 0, mastering_active = 0;
      if (real) {
        Store(h.base, kSubmix, 0);
        CriticalSection(h, kMixLock, true);
        mastering = MasteringVoice(h.base);
        mastering_count = guest::LoadU32(h.base, mastering + 4);
        mastering_active = guest::LoadU32(h.base, mastering + 40);
      }
      (v == 0 ? recompiled : native)(t, h.base);
      const std::vector<RealVoice> made = real ? MadeVoices(h.base, block) : std::vector<RealVoice>();
      if (v == 1) {
        voices_made += uint32_t(made.size());
      }
      out[v].Word(t.r1.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(made.size()));
      out[v].Bytes(Read(h.base, h.data, kCompared));
      Relative(out[v], h.base, block, kBlockBytes, made);
      for (const RealVoice& m : made) {
        Relative(out[v], h.base, m.allocator, m.bytes, made);
      }
      out[v].Bytes(Read(h.base, sp - frame, frame - saves));
      for (size_t i = 0; i < globals.size(); i++) {
        if (i < size_t(kFloatGlobals)) {
          out[v].Float(guest::LoadU32(h.base, globals[i]));
        } else {
          out[v].Word(guest::LoadU32(h.base, globals[i]));
        }
      }
      if (real) {
        for (const RealVoice& m : made) {
          PPCContext stop = CallContext(h);
          stop.r3.u64 = m.voice;
          stop.r4.u64 = 1;
          hooks::XAudioApiStop(stop, h.base);
          PPCContext lock = CallContext(h);
          hooks::XAudioSpinTake(lock, h.base);
          const uint32_t node = m.voice + source_offset;
          const uint32_t prev = guest::LoadU32(h.base, node + 4);
          const uint32_t next = guest::LoadU32(h.base, node);
          out[v].Word(guest::LoadU32(h.base, prev) == node &&
                      guest::LoadU32(h.base, next + 4) == node);
          guest::StoreU32(h.base, prev, next);
          guest::StoreU32(h.base, next + 4, prev);
          hooks::XAudioSpinRelease(lock, h.base);
          memory::crt::XMemFree(m.allocator, 0x61820006);
        }
        out[v].Word(guest::LoadU32(h.base, mastering + 4) - mastering_count);
        out[v].Word(guest::LoadU32(h.base, mastering + 40) - mastering_active);
        Store(h.base, mastering + 4, mastering_count);
        Store(h.base, mastering + 40, mastering_active);
        CriticalSection(h, kMixLock, false);
        Store(h.base, kSubmix, saved_submix);
      }
      for (size_t i = 0; i < globals.size(); i++) {
        guest::StoreU32(h.base, globals[i], saved[i]);
      }
    }
    h.random = random;
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  if (real) {
    ITC_LOG_INFO("harness: race_sounds: {}: {} voices made", name, voices_made);
  }
  results.Report();
}

// Begin: its allocations real (XPhysicalAlloc's block and the 14 views from
// operator new, freed after each version), Archive::Load recorded
// (ArchiveLoads), the TOC of assets below 23011 pointed so asset 22889's
// entry is in the block (+4 the stored size, +8 the unpacked one). The
// allocated pointers are compared as whether they're 0, and the effects'
// buffer in the load's call as whether it is the buffer.
void BeginCases(Context& h, uint32_t block) {
  Results results("RaceSounds::Begin (sub_82176618)");
  const std::vector<uint32_t> globals = Globals();
  std::vector<uint32_t> saved;
  for (const uint32_t at : globals) {
    saved.push_back(guest::LoadU32(h.base, at));
  }
  uint8_t* base = h.base;
  const uint32_t machine = block + kMachineWords - 9600;
  for (int c = 0; c < 30; c++) {
    const Random start = h.random;
    Random random = start;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      random = start;
      Lay(h, block, Random(random.Next()), false);
      Store(base, kTocLow, block + kTocEntry - 0x596A0);
      Store(base, block + kTocEntry + 4, 0x100 * (1 + random.Below(16)));
      Store(base, block + kTocEntry + 8, random.Below(2) == 0 ? 0 : 0x100 * (1 + random.Below(16)));
      Store(base, machine + rs::kKind, Pick(random, {0u, 1u, 2u, 3u, 4u, 5u, 6u, 7u}));
      Store(base, g_seed, uint32_t(random.Next()));
      const uint32_t fill = uint32_t(random.Next());
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      for (uint32_t k = 0; k < 0x400; k += 4) {
        Store(base, sp - 0x400 + k, fill + k);
      }
      t.r3.u64 = machine;
      ArchiveLoads loads({1});
      (v == 0 ? __imp__sub_82176618 : hooks::RaceSoundsBegin)(t, base);
      const uint32_t buffer = guest::LoadU32(base, machine + rs::kEffectsBank);
      out[v].Word(buffer != 0);
      Store(base, machine + rs::kEffectsBank, 0);
      uint32_t views[14];
      for (uint32_t i = 0; i < 14; i++) {
        views[i] = guest::LoadU32(base, rs::kCarViews + 4 * i);
        out[v].Word(views[i] != 0);
        if (views[i] != 0) {
          out[v].Bytes(Read(base, views[i], 24));
        }
        Store(base, rs::kCarViews + 4 * i, 0);
      }
      std::vector<uint32_t> calls = loads.calls();
      for (size_t i = 1; i < calls.size(); i += 6) {
        calls[i] = calls[i] == buffer;
      }
      for (const uint32_t word : calls) {
        out[v].Word(word);
      }
      out[v].Word(t.r1.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      out[v].Word(t.r3.u32);
      out[v].Bytes(Read(base, h.data, kCompared));
      out[v].Bytes(Read(base, block, kBlockBytes));
      out[v].Bytes(Read(base, sp - 192, 192 - 40));
      for (size_t i = 0; i < globals.size(); i++) {
        out[v].Word(guest::LoadU32(base, globals[i]));
      }
      if (buffer != 0) {
        memory::FreePhysical(buffer);
      }
      for (const uint32_t view : views) {
        if (view != 0) {
          memory::crt::OperatorDelete(view);
        }
      }
      for (size_t i = 0; i < globals.size(); i++) {
        guest::StoreU32(base, globals[i], saved[i]);
      }
    }
    h.random = random;
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  results.Report();
}

// Init: a machine of its own (an empty state machine or one with random
// transitions already, some with keys Init adds; the last, (10, 32), among
// them in some cases: its r3 is then the index), the kind from the race
// data's +0xA3AF0 (every class: 5, 1, 2, 4 and 6, the others), the frame
// filled with the same random words for both (the words the original never
// writes go into the transitions). Its allocations are real: the seven
// slots' two buffers (XPhysicalAlloc, compared as whether they're set and
// cleared, freed after each version) and CarSounds::Init's array, matrices
// and submix voice (compared as the car sounds' harness does: the matrices
// as whether they're set, the submix's allocator block with pointers into
// it as offsets; then the submix released and the blocks freed). The
// machine, the slots' globals 0x826D8334 to 0x826D83C8, the frame (but its
// saved registers), r1 and r3 compared.
void InitCases(Context& h, uint32_t block) {
  namespace gm = game_modes;
  constexpr uint32_t kMachineBytes = 0x2600;
  constexpr uint32_t kSaves = 112;  // __savegprlr_19: r19 at -112, lr at -8
  constexpr uint32_t kSlots = 0x826D8334, kSlotsBytes = 0x94;
  constexpr uint32_t kSlotBuffers = 0x826D8358, kSlotBuffersEnd = 0x826D8390;
  constexpr uint32_t kBufferBytes[2] = {0x5D2D0, 0x6B080};  // kSlotBuffer0's, kSlotBuffer1's
  constexpr uint32_t kCarSoundsUnknown = 0x826C5C40;
  constexpr uint32_t kMatrix = 176;
  Results results("RaceSounds::Init (sub_82177378)");
  uint8_t* base = h.base;
  const uint32_t machine = memory::Alloc(kMachineBytes, 0);
  if (machine == 0) {
    ITC_LOG_ERROR("harness: race_sounds: no machine block: Init's cases skipped");
    return;
  }
  const std::vector<uint32_t> globals = Globals();
  std::vector<uint32_t> saved;
  for (const uint32_t at : globals) {
    saved.push_back(guest::LoadU32(base, at));
  }
  const std::vector<uint8_t> saved_slots = Read(base, kSlots, kSlotsBytes);
  const uint32_t saved_unknown = guest::LoadU32(base, kCarSoundsUnknown);
  const uint32_t saved_submix = guest::LoadU32(base, kSubmix);
  constexpr uint32_t kPlanted[][2] = {{1, 4}, {38, 0}, {23, 0}, {30, 49}, {10, 32}};
  for (uint64_t seed = 0; seed < 18; seed++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed * 131 + 17);
      for (uint32_t i = 0; i < kMachineBytes; i += 4) {
        Store(base, machine + i, uint32_t(r.Next()));
      }
      if (seed % 2 == 0) {
        gm::StateMachineConstruct(base, machine);
        Store(base, machine + gm::kTransitionCount, 0);
      } else {
        const uint32_t count = 8 + r.Below(120);
        Store(base, machine + gm::kTransitionCount, count);
        for (uint32_t i = 0; i < gm::kTransitions; i++) {  // keys of small numbers, some matching
          for (uint32_t k = 0; k < 4; k++) {
            Store(base, machine + gm::kTransitionBytes * i + 4 * k, r.Below(k < 2 ? 60 : 2));
          }
        }
        for (const auto& key : kPlanted) {
          if (seed % 4 == 1 && key[0] == 10) {
            continue;  // the last Add makes a new transition
          }
          const uint32_t at = machine + gm::kTransitionBytes * r.Below(count);
          Store(base, at, key[0]);
          Store(base, at + 4, key[1]);
          Store(base, at + 8, 0);
          Store(base, at + 12, 0);
        }
      }
      static constexpr uint32_t kKinds[] = {5, 1, 2, 4, 6, 0, 3, 7};
      const uint32_t kind = seed < 16 ? kKinds[seed % 8] : uint32_t(r.Next());
      Store(base, rs::kRaceData, block + kRaceDataAt - 0xA3AF0);
      Store(base, block + kRaceDataAt, kind);
      for (uint32_t i = 0; i < kSlotsBytes; i += 4) {
        Store(base, kSlots + i, uint32_t(r.Next()));
      }
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      const uint32_t frame = sp - rs::kInitFrame;
      for (uint32_t i = 0; i < rs::kInitFrame; i += 4) {
        Store(base, frame + i, uint32_t(r.Next()));
      }
      Store(base, kCarSoundsUnknown, uint32_t(r.Next()));
      Store(base, kSubmix, 0);
      CriticalSection(h, kMixLock, true);
      const uint32_t mastering = MasteringVoice(base);
      const uint32_t count = guest::LoadU32(base, mastering + 4);
      const uint32_t active = guest::LoadU32(base, mastering + 40);
      t.r3.u64 = machine;
      (v == 0 ? __imp__sub_82177378 : hooks::RaceSoundsInit)(t, base);
      out[v].Word(t.r1.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      out[v].Word(t.r3.u32);
      out[v].Bytes(Read(base, machine, kMachineBytes));
      out[v].Bytes(Read(base, frame, rs::kInitFrame - kSaves));
      // The slots' globals, the buffers as whether they're set and cleared.
      std::vector<uint32_t> buffers;
      for (uint32_t at = kSlots; at < kSlots + kSlotsBytes; at += 4) {
        const uint32_t word = guest::LoadU32(base, at);
        if (at < kSlotBuffers || at >= kSlotBuffersEnd) {
          out[v].Word(word);
          continue;
        }
        out[v].Word(word != 0);
        if (word != 0) {
          const uint32_t bytes = kBufferBytes[at < rs::kSlotBuffer1 ? 0 : 1];
          bool clear = true;
          for (uint32_t i = 0; i < bytes && clear; i += 4) {
            clear = guest::LoadU32(base, word + i) == 0;
          }
          out[v].Word(clear);
          buffers.push_back(word);
        }
      }
      // CarSounds::Init's effects.
      const uint32_t array = guest::LoadU32(base, rs::kCarSounds);
      const uint32_t submix = guest::LoadU32(base, kSubmix);
      std::vector<RealVoice> made;
      if (submix != 0) {
        const uint32_t allocator = guest::LoadU32(base, submix + 8);
        made.push_back({submix, allocator, guest::LoadU32(base, allocator + 16) + 28});
      }
      out[v].Word(array != 0);
      out[v].Word(uint32_t(made.size()));
      out[v].Word(guest::LoadU32(base, kCarSoundsUnknown));
      uint32_t matrices[7] = {};
      if (array != 0) {
        out[v].Word(guest::LoadU32(base, array - 16));
        for (uint32_t k = 0; k < 7; k++) {
          const uint32_t sound = array + rs::kCarSoundBytes * k;
          matrices[k] = guest::LoadU32(base, sound + kMatrix);
          out[v].Word(matrices[k] != 0);
          Store(base, sound + kMatrix, 0);
          if (matrices[k] != 0) {
            out[v].Bytes(Read(base, matrices[k], 24));
          }
        }
        Relative(out[v], base, array, 7 * rs::kCarSoundBytes, made);
      }
      for (const RealVoice& m : made) {
        Relative(out[v], base, m.allocator, m.bytes, made);
      }
      if (submix != 0) {
        PPCContext release = CallContext(h);
        release.r3.u64 = submix;
        hooks::XAudioApiRelease(release, base);
        out[v].Word(release.r3.u32);
      }
      out[v].Word(guest::LoadU32(base, mastering + 4) - count);
      out[v].Word(guest::LoadU32(base, mastering + 40) - active);
      Store(base, mastering + 4, count);
      Store(base, mastering + 40, active);
      CriticalSection(h, kMixLock, false);
      for (const uint32_t matrix : matrices) {
        if (matrix != 0) {
          memory::crt::OperatorDeleteArray(matrix);
        }
      }
      if (array != 0) {
        memory::crt::OperatorDeleteArray(array - 16);
      }
      for (const uint32_t buffer : buffers) {
        memory::FreePhysical(buffer);
      }
      for (size_t i = 0; i < globals.size(); i++) {
        guest::StoreU32(base, globals[i], saved[i]);
      }
      for (uint32_t i = 0; i < kSlotsBytes; i++) {
        guest::StoreU8(base, kSlots + i, saved_slots[i]);
      }
      Store(base, kCarSoundsUnknown, saved_unknown);
      Store(base, kSubmix, saved_submix);
    }
    results.Check(out[0], out[1], "seed " + std::to_string(seed));
  }
  memory::Free(machine);
  results.Report();
}

void Keep(Random&, uint64_t*) {}

}  // namespace

void RaceSounds(Context& h) {
  Block owned;
  if (!owned.address) {
    ITC_LOG_ERROR("harness: race_sounds: no block from the allocation layer");
    return;
  }
  const uint32_t block = owned.address;
  uint8_t* base = h.base;
  {
    PPCContext probe = CallContext(h);
    __imp__sub_825AA580(probe, base);
    g_seed = probe.r3.u32 != 0 ? probe.r3.u32 + 20 : 0;
  }
  const uint32_t engine = guest::LoadU32(base, xaudio::kEngine);
  const bool real = engine != 0 && MasteringVoice(base) != 0 && hooks::NativeHeapActive();
  const uint32_t machine = block + kMachineWords - 9600;
  Group(h, block, "RaceSounds::SetListener (sub_821758A0)", 60, __imp__sub_821758A0,
        hooks::RaceSoundsSetListener, 112, 32, [](Random& r, uint64_t* regs) {
          regs[0] = r.Next();
        });
  Group(h, block, "RaceSounds::FadeMusic (sub_82175908)", 300, __imp__sub_82175908,
        hooks::RaceSoundsFadeMusic, 112, 16, Keep);
  Group(h, block, "RaceSounds::RestoreMusicVolume (sub_821759C8)", 100, __imp__sub_821759C8,
        hooks::RaceSoundsRestoreMusicVolume, 112, 16, Keep);
  Group(h, block, "RaceSounds::EndTick (sub_82175A40)", 60, __imp__sub_82175A40,
        hooks::RaceSoundsEndTick, 112, 24, Keep);
  Group(h, block, "RaceSounds::Pause (sub_82175AE8)", 40, __imp__sub_82175AE8,
        hooks::RaceSoundsPause, 112, 24, Keep);
  Group(h, block, "RaceSounds::Resume (sub_82175B80)", 40, __imp__sub_82175B80,
        hooks::RaceSoundsResume, 112, 24, Keep);
  Group(h, block, "RaceSounds::PauseCars (sub_82175C00)", 40, __imp__sub_82175C00,
        hooks::RaceSoundsPauseCars, 112, 24, Keep);
  Group(h, block, "RaceSounds::ResumeCars (sub_82175C58)", 40, __imp__sub_82175C58,
        hooks::RaceSoundsResumeCars, 112, 24, Keep);
  Group(h, block, "RaceSounds::OnStageCountdown (sub_82175D20)", 40, __imp__sub_82175D20,
        hooks::RaceSoundsOnStageCountdown, 64, 0, Keep);
  Group(h, block, "RaceSounds::OnRollingStartDone (sub_82175D48)", 60, __imp__sub_82175D48,
        hooks::RaceSoundsOnRollingStartDone, 96, 16, Keep);
  Group(h, block, "RaceSounds::OnEffectsBankLoaded (sub_82175ED0)", 20, __imp__sub_82175ED0,
        hooks::RaceSoundsOnEffectsBankLoaded, 64, 0, Keep);
  if (!real) {
    ITC_LOG_ERROR("harness: race_sounds: no XAudio engine or native heap: SetUpCars' and "
                  "UpdateCars' cases skipped");
  } else {
  Group(h, block, "RaceSounds::SetUpCars (sub_82176188)", 60, __imp__sub_82176188,
        hooks::RaceSoundsSetUpCars, 224, 144, Keep, true);
  Group(h, block, "RaceSounds::UpdateCars (sub_82175F20)", 120, __imp__sub_82175F20,
        hooks::RaceSoundsUpdateCars, 240, 160, [&](Random& r, uint64_t*) {
          // Mostly past the wait with the cars set up.
          if (r.Below(4) != 0) {
            Store(base, machine + rs::kSetUpWait, 0);
            Store(base, machine + rs::kFlags,
                  (guest::LoadU32(base, machine + rs::kFlags) | 0x20000) & ~0x4000u);
          }
        },
        true);
  }
  Group(h, block, "RaceSounds::PickRivalMusic (sub_821764F0)", 200, __imp__sub_821764F0,
        hooks::RaceSoundsPickRivalMusic, 112, 24, [&](Random& r, uint64_t*) {
          // The results' count (0 to 3) and the last one's item, its class
          // byte in the block (the item table's +16 pointed so it lands
          // there).
          const uint32_t results = block + kResults;
          const uint32_t count = Pick(r, {0u, 1u, 2u, 2u, 3u});
          const uint32_t item =
              Pick(r, {0xFFFFFFFFu, 382u, 383u, 386u, 390u, 391u, 392u, 395u, 397u, 398u, 399u,
                       5u, r.Below(500)});
          Store(base, results, count);
          Store(base, results + 972 * count - 964, item);
          Store(base, kTables, block + kItem - 16);
          Store(base, block + kItem - 16 + 16, block + kItem - 3008 * item);
          guest::StoreU8(base, block + kItem + 35,
                         uint8_t(Pick(r, {0u, 1u, 2u, 3u, 3u, 4u, 4u, 5u, 6u, 0xFFu,
                                          uint32_t(r.Next())})));
        });
  Group(h, block, "RaceSounds::PlayRivalMusic (sub_821762F8)", 200, __imp__sub_821762F8,
        hooks::RaceSoundsPlayRivalMusic, 144, 40, [&](Random& r, uint64_t*) {
          // One flag mostly (as in play), sometimes several; the course 0 to
          // 3 or any word.
          const uint32_t one = 0x40u << r.Below(8);
          Store(base, machine + rs::kFlags,
                r.Below(4) == 0 ? (uint32_t(r.Next()) & 0x3FC0) : one);
          Store(base, block + kRaceDataAt, Pick(r, {0u, 1u, 2u, 3u, uint32_t(r.Next())}));
        });
  // The handlers above them.
  Group(h, block, "RaceSounds::StageTick (sub_82176F28)", 200, __imp__sub_82176F28,
        hooks::RaceSoundsStageTick, 160, 40, Keep);
  Group(h, block, "RaceSounds::RollingStartTick (sub_82177130)", 200, __imp__sub_82177130,
        hooks::RaceSoundsRollingStartTick, 160, 40, Keep);
  Group(h, block, "RaceSounds::OnCountdown (sub_82176968)", 60, __imp__sub_82176968,
        hooks::RaceSoundsOnCountdown, 96, 16, Keep);
  Group(h, block, "RaceSounds::OnNextStage (sub_82176EC8)", 60, __imp__sub_82176EC8,
        hooks::RaceSoundsOnNextStage, 96, 16, Keep);
  if (real) {
    const auto steps = [&](Random& r, uint64_t*) {
      Store(base, machine + rs::kCarsStep, Pick(r, {1u, 2u, 2u, 2u, 0u, 3u}));
      if (r.Below(4) != 0) {
        Store(base, machine + rs::kSetUpWait, 0);
        Store(base, machine + rs::kFlags,
              (guest::LoadU32(base, machine + rs::kFlags) | 0x20000) & ~0x4000u);
      }
    };
    Group(h, block, "RaceSounds::RaceTick (sub_821769D0)", 200, __imp__sub_821769D0,
          hooks::RaceSoundsRaceTick, 160, 40, steps, true);
    Group(h, block, "RaceSounds::OnRaceEnd (sub_82176BA0)", 60, __imp__sub_82176BA0,
          hooks::RaceSoundsOnRaceEnd, 96, 16, steps, true);
    Group(h, block, "RaceSounds::OnLeaveAfterRace (sub_82176E60)", 60, __imp__sub_82176E60,
          hooks::RaceSoundsOnLeaveAfterRace, 96, 16, steps, true);
    Group(h, block, "RaceSounds::OnRollingStart (sub_821770C0)", 60, __imp__sub_821770C0,
          hooks::RaceSoundsOnRollingStart, 96, 16, steps, true);
    const uint32_t big = memory::Alloc(kBigBytes, 0);
    if (big == 0) {
      ITC_LOG_ERROR("harness: race_sounds: no big block: AfterRaceTick's cases skipped");
    } else {
      Group(h, block, "RaceSounds::AfterRaceTick (sub_82176C10)", 200, __imp__sub_82176C10,
            hooks::RaceSoundsAfterRaceTick, 176, 48,
            [&](Random& r, uint64_t* regs) {
              steps(r, regs);
              // The race data's records at `big`: one or two, the last's
              // result; the race mode; the kind; the after-race word; player
              // 0's save data's +6100.
              const uint32_t count = 1 + r.Below(2);
              Store(base, rs::kRaceData, big - 0x59AE8);
              Store(base, big + 0x49FE0, count);
              Store(base, big + 3936 * (count - 1) + 3924, Pick(r, {1u, 2u, 0u, uint32_t(r.Next())}));
              Store(base, big + 0x4A00C, Pick(r, {7u, 7u, 0u, uint32_t(r.Next())}));
              Store(base, machine + rs::kKind, Pick(r, {6u, 6u, 0u, 1u, 2u, 5u}));
              Store(base, rs::kAfterRaceWord, r.Below(2));
              Store(base, kSaveData, block + kSaveAt - 6100);
              Store(base, block + kSaveAt, Pick(r, {1u, 1u, 0u, 999u, 1000u, uint32_t(r.Next())}));
            },
            true);
      memory::Free(big);
    }
  }
  // rand's callers (the seed set per case).
  Group(h, block, "RaceSounds::PlayStartMusic (sub_82175DB0)", 200, __imp__sub_82175DB0,
        hooks::RaceSoundsPlayStartMusic, 144, 8, [&](Random& r, uint64_t*) {
          Store(base, machine + rs::kKind,
                Pick(r, {0u, 1u, 2u, 3u, 4u, 5u, 6u, 7u, 0xFFFFFFFFu, uint32_t(r.Next())}));
          if (g_seed != 0) {
            Store(base, g_seed, uint32_t(r.Next()));
          }
        });
  if (real) {
    Group(h, block, "RaceSounds::IdleTick (sub_821767A8)", 200, __imp__sub_821767A8,
          hooks::RaceSoundsIdleTick, 160, 40,
          [&](Random& r, uint64_t*) {
            Store(base, machine + rs::kCarsStep, Pick(r, {1u, 2u, 2u, 2u, 0u, 3u}));
            if (r.Below(4) != 0) {
              Store(base, machine + rs::kSetUpWait, 0);
              Store(base, machine + rs::kFlags,
                    (guest::LoadU32(base, machine + rs::kFlags) | 0x20000) & ~0x4000u);
            }
            Store(base, machine + rs::kKind, Pick(r, {0u, 1u, 2u, 4u, 6u}));
            if (g_seed != 0) {
              Store(base, g_seed, uint32_t(r.Next()));
            }
          },
          true);
  }
  if (g_seed != 0 && hooks::NativeHeapActive()) {
    BeginCases(h, block);
  } else {
    ITC_LOG_ERROR("harness: race_sounds: no thread data or native heap: Begin's cases skipped");
  }
  if (real) {
    InitCases(h, block);
  } else {
    ITC_LOG_ERROR("harness: race_sounds: no XAudio engine or native heap: Init's cases skipped");
  }
}

}  // namespace harness
