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
// the machine's words in that block too. The globals are put back after
// each side. Compared with harness::Outputs: the scratch area, the block,
// the frame's words below the stack pointer (the register saves left out:
// adapters don't save them), r1, r3 and the globals (the volumes as
// floats).

#include "harness.h"

#include "base/log.h"
#include "hooks/race_sounds.h"
#include "memory/heap.h"
#include "race_sounds/race_sounds.h"

#include <bit>
#include <initializer_list>
#include <string>

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

// In the block: the car sounds' array (the count at +0, the sounds from
// +16), the fake voices and their vtable, the machine's words (the machine
// at kMachineWords - 9600).
constexpr uint32_t kBlockBytes = 0x3000;
constexpr uint32_t kVoices = 0x2300, kVoiceBytes = 0x100, kVoiceCount = 10;
constexpr uint32_t kVoiceVtable = 0x2D00;
constexpr uint32_t kMachineWords = 0x2E00;
constexpr uint32_t kSoundHolders[] = {408 + 44, 408 + 128, 952, 584, 676, 736,
                                      840,      792,       888, 1016, 1072, 1136};

// The manager's slots the functions call.
constexpr uint32_t kSlots[] = {rs::kReleaseBankSlot,          rs::kIsBankLoadingSlot,
                               rs::kIsBankSlotEmptySlot,      rs::kStopCueSlot,
                               rs::kIsCuePlayingSlot,         rs::kSetListenerPositionSlot,
                               rs::kSetListenerOrientationSlot, rs::kSetListenerVelocitySlot,
                               rs::kGetCategorySlot,          rs::kSetCategoryVolumeSlot,
                               rs::kPauseCategorySlot,        rs::kResumeCategorySlot,
                               40};  // PlayCue2D (Sound::Play)

// The globals put back after each side; the first four floats.
constexpr uint32_t kGlobals[] = {rs::kMusicVolume,       rs::kKeptMusicVolume,
                                 rs::kEffectsVolume,     rs::kEffectsVolume2,
                                 rs::kSoundManager,      rs::kCarSounds,
                                 rs::kEffectsBankArrived};
constexpr int kFloatGlobals = 4;
constexpr int kGlobalCount = int(sizeof(kGlobals) / sizeof(kGlobals[0]));

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

uint32_t Pick(Random& random, std::initializer_list<uint32_t> choices) {
  return *(choices.begin() + random.Below(uint32_t(choices.size())));
}

// A volume: 0.0, 1.0, a fraction, a negative, a tiny one, a NaN or any bits.
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

uint32_t FakeVoice(uint32_t block, uint32_t k) { return block + kVoices + kVoiceBytes * k; }

// One case's world, laid out the same for both versions from `seed`.
struct World {
  uint64_t seed;
  uint32_t globals[kGlobalCount];
  bool no_sounds;  // [0x826C5C3C] 0
};

World MakeWorld(Random& r) {
  World w;
  w.seed = r.Next();
  for (int i = 0; i < kFloatGlobals; i++) {
    w.globals[i] = Volume(r);
  }
  w.globals[4] = 0;  // the manager and the array: laid out below
  w.globals[5] = 0;
  w.globals[6] = uint32_t(r.Next());
  w.no_sounds = false;
  return w;
}

void Lay(Context& h, uint32_t block, const World& w) {
  Random r(w.seed);
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
  // holders (each slot a fake voice or 0, their +24 words 0 or random).
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
        Store(base, sound + offset + 4 + 4 * i,
              r.Below(3) == 0 ? 0 : FakeVoice(block, r.Below(kVoiceCount)));
        Store(base, sound + offset + 24 + 4 * i, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
      }
    }
  }
  // The machine's flags: mostly the bits the handlers test.
  Store(base, block + kMachineWords + 44,
        r.Below(4) == 0 ? uint32_t(r.Next()) : (uint32_t(r.Next()) & 0x3C034));
  for (int i = 0; i < kGlobalCount; i++) {
    Store(base, kGlobals[i], w.globals[i]);
  }
  Store(base, rs::kSoundManager, manager);
  Store(base, rs::kCarSounds, w.no_sounds ? 0 : block + 16);
}

struct Saved {
  explicit Saved(uint8_t* base) : base(base) {
    for (int i = 0; i < kGlobalCount; i++) {
      words[i] = guest::LoadU32(base, kGlobals[i]);
    }
  }
  void Restore() const {
    for (int i = 0; i < kGlobalCount; i++) {
      guest::StoreU32(base, kGlobals[i], words[i]);
    }
  }
  uint8_t* base;
  uint32_t words[kGlobalCount];
};

// A group: `count` cases of `recompiled` against `native`, `args` setting
// r3 to r6 (r3 the machine unless it sets it); the frame's `frame` bytes
// below the stack pointer compared up to `saves` bytes from its top.
template <typename Args>
void Group(Context& h, uint32_t block, const std::string& name, int count,
           GuestFunction recompiled, GuestFunction native, uint32_t frame, uint32_t saves,
           Args args) {
  Results results(name);
  const Saved saved(h.base);
  const uint32_t machine = block + kMachineWords - 9600;
  for (int c = 0; c < count; c++) {
    const World w = MakeWorld(h.random);
    uint64_t regs[4] = {machine, h.random.Next(), h.random.Next(), h.random.Next()};
    args(h.random, regs);
    const uint32_t fill = uint32_t(h.random.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Lay(h, block, w);
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      for (uint32_t k = 0; k < 0x400; k += 4) {
        Store(h.base, sp - 0x400 + k, fill + k);
      }
      t.r3.u64 = regs[0];
      t.r4.u64 = regs[1];
      t.r5.u64 = regs[2];
      t.r6.u64 = regs[3];
      (v == 0 ? recompiled : native)(t, h.base);
      out[v].Word(t.r1.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      out[v].Word(t.r3.u32);
      out[v].Bytes(Read(h.base, h.data, kCompared));
      out[v].Bytes(Read(h.base, block, kBlockBytes));
      out[v].Bytes(Read(h.base, sp - frame, frame - saves));
      for (int i = 0; i < kGlobalCount; i++) {
        if (i < kFloatGlobals) {
          out[v].Float(guest::LoadU32(h.base, kGlobals[i]));
        } else {
          out[v].Word(guest::LoadU32(h.base, kGlobals[i]));
        }
      }
      saved.Restore();
    }
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
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
}

}  // namespace harness
