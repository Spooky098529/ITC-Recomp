// The menu sounds (src/menu_sounds/, docs/functions/menu-sounds.md) against
// the recompiled functions. The originals run as `__imp__`, the plain C++
// through the register steps (hooks/menu_sounds.h). The sound manager
// pointer [0x826C5C80] is pointed at a fake manager in the scratch area
// whose vtable's slots 10, 29 and 30 hold leaf recorders (recompiled, the
// same on both sides), so no sound plays and no category's volume changes;
// the pointer and the volume globals are put back after each side.
// Compared with harness::Outputs: the scratch area (the fake manager and
// what the recorders wrote), the frame's words below the stack pointer (the
// saved link register and r31 left out: adapters don't save them), r3 and
// the volume globals (floats).

#include "harness.h"

#include "hooks/menu_sounds.h"
#include "menu_sounds/menu_sounds.h"

#include <initializer_list>
#include <string>

namespace harness {

namespace {

namespace ms = menu_sounds;

// The leaf recorders: sub_820A8420 (r4's low byte to +320, r5 to +332, r6
// to +336; r3 left), sub_820CE9F8 (r4, r5 to +444, +448; r3 left), the
// getter sub_82090770 (r3 = +60).
constexpr uint32_t kRecordR4R6 = 0x820A8420, kRecordR4 = 0x820CE9F8, kGet60 = 0x82090770;

// In the scratch area: the fake manager (+0 its vtable) and its vtable.
constexpr uint32_t kManager = 0x000, kVtable = 0x400;
constexpr uint32_t kCompared = 0x480;

uint32_t Pick(Random& random, std::initializer_list<uint32_t> choices) {
  return *(choices.begin() + random.Below(uint32_t(choices.size())));
}

struct Saved {
  explicit Saved(uint8_t* base) : base(base) {
    for (int i = 0; i < 4; i++) {
      words[i] = guest::LoadU32(base, kAddresses[i]);
    }
  }
  void Restore() const {
    for (int i = 0; i < 4; i++) {
      guest::StoreU32(base, kAddresses[i], words[i]);
    }
  }
  static constexpr uint32_t kAddresses[4] = {ms::kSoundManager, ms::kEffectsVolume,
                                             ms::kEffectsVolume2, ms::kMusicVolume};
  uint8_t* base;
  uint32_t words[4];
};

// One case's world: the fake manager's words random, its slots picked, the
// answer at +60 one of -1, a small index or a random word.
struct World {
  uint32_t words[kVtable / 4];
  uint32_t play_slot, get_slot, set_slot;
  uint32_t answer;
};

World MakeWorld(Random& r) {
  World w;
  for (uint32_t& word : w.words) {
    word = uint32_t(r.Next());
  }
  w.play_slot = Pick(r, {kRecordR4R6, kRecordR4R6, kGet60});
  w.get_slot = Pick(r, {kGet60, kGet60, kGet60, kRecordR4});
  w.set_slot = Pick(r, {kRecordR4, kRecordR4, kRecordR4R6});
  switch (r.Below(3)) {
    case 0:
      w.answer = 0xFFFFFFFFu;
      break;
    case 1:
      w.answer = r.Below(8);
      break;
    default:
      w.answer = uint32_t(r.Next());
  }
  return w;
}

void Lay(Context& h, const World& w) {
  Fill(h.base, h.data, kScratchBytes);
  const uint32_t manager = h.data + kManager;
  for (uint32_t i = 0; i < kVtable / 4; i++) {
    guest::StoreU32(h.base, manager + 4 * i, w.words[i]);
  }
  guest::StoreU32(h.base, manager, h.data + kVtable);
  guest::StoreU32(h.base, manager + 60, w.answer);
  guest::StoreU32(h.base, h.data + kVtable + ms::kPlayCue2DSlot, w.play_slot);
  guest::StoreU32(h.base, h.data + kVtable + ms::kGetCategorySlot, w.get_slot);
  guest::StoreU32(h.base, h.data + kVtable + ms::kSetCategoryVolumeSlot, w.set_slot);
  guest::StoreU32(h.base, ms::kSoundManager, manager);
}

// A group: `count` cases of `function` (recompiled) against `native`, each
// with r3 and r4 from `args`; the frame's `frame` bytes below the stack
// pointer compared up to `saves` bytes from its top.
template <typename Args>
void Group(Context& h, const std::string& name, int count, GuestFunction recompiled,
           GuestFunction native, uint32_t frame, uint32_t saves, bool volumes, Args args) {
  Results results(name);
  const Saved saved(h.base);
  for (int c = 0; c < count; c++) {
    const World w = MakeWorld(h.random);
    uint64_t r3 = 0, r4 = 0;
    args(h.random, r3, r4);
    const uint32_t fill = uint32_t(h.random.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Lay(h, w);
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      for (uint32_t k = 0; k < 256; k += 4) {
        guest::StoreU32(h.base, sp - 256 + k, fill + k);
      }
      for (const uint32_t address : {ms::kEffectsVolume, ms::kEffectsVolume2, ms::kMusicVolume}) {
        guest::StoreU32(h.base, address, fill ^ address);
      }
      t.r3.u64 = r3;
      t.r4.u64 = r4;
      (v == 0 ? recompiled : native)(t, h.base);
      out[v].Word(t.r1.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      out[v].Word(t.r3.u32);
      out[v].Bytes(Read(h.base, h.data, kCompared));
      out[v].Bytes(Read(h.base, sp - frame, frame - saves));
      out[v].Word(guest::LoadU32(h.base, ms::kSoundManager));
      if (volumes) {
        for (const uint32_t address :
             {ms::kEffectsVolume, ms::kEffectsVolume2, ms::kMusicVolume}) {
          out[v].Float(guest::LoadU32(h.base, address));
        }
      } else {
        for (const uint32_t address :
             {ms::kEffectsVolume, ms::kEffectsVolume2, ms::kMusicVolume}) {
          out[v].Word(guest::LoadU32(h.base, address));
        }
      }
      saved.Restore();
    }
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  results.Report();
}

// A volume setting: mostly 0 to 15, else any byte (negative ones too),
// with random upper bits (only the low byte is read).
uint64_t Setting(Random& r) {
  const uint64_t high = r.Next() & ~uint64_t(0xFF);
  switch (r.Below(4)) {
    case 0:
      return high | Pick(r, {0u, 15u});
    case 1:
      return high | r.Below(256);
    default:
      return high | r.Below(16);
  }
}

}  // namespace

void MenuSounds(Context& h) {
  Group(h, "MenuSound::Play (sub_821103F0)", 100, __imp__sub_821103F0, hooks::MenuSoundPlay, 112,
        8, false, [&](Random& r, uint64_t& r3, uint64_t&) {
          r3 = r.Below(3) == 0 ? r.Next() : Pick(r, {0u, 2u, 4u, 5u, 8u, 9u, 10u});
        });
  Group(h, "Sound::Play (sub_82175850)", 100, __imp__sub_82175850, hooks::SoundPlay, 96, 8,
        false, [&](Random& r, uint64_t& r3, uint64_t& r4) {
          r3 = r.Next();
          r4 = r.Next();
        });
  Group(h, "SoundOptions::SetEffectsVolume (sub_821B9C20)", 200, __imp__sub_821B9C20,
        hooks::SoundOptionsSetEffectsVolume, 112, 16, true,
        [&](Random& r, uint64_t& r3, uint64_t&) { r3 = Setting(r); });
  Group(h, "SoundOptions::SetMusicVolume (sub_821B9CF0)", 200, __imp__sub_821B9CF0,
        hooks::SoundOptionsSetMusicVolume, 112, 16, true,
        [&](Random& r, uint64_t& r3, uint64_t&) { r3 = Setting(r); });
}

}  // namespace harness
