// The audio functions made native inside their wraps (2026-11-15): the
// sound manager's per-frame callees (src/sound_cues/manager_update.h,
// docs/functions/sound-cues.md) and the render driver's destructor
// (src/audio/render_driver.h, docs/functions/audio.md), against the
// recompiled functions. The originals run as `__imp__`, the plain C++
// through its adapter's step (lifted_call::EnterR3).
//
// XACT's objects are fakes in the scratch area whose vtables hold leaf
// functions (recompiled or lifted, the same on both sides, reading only r3
// and r4): nothing reaches the real XACT. Objects the functions delete
// (bank pairs and their holders, stopped Sounds with their emitters and
// list nodes) are allocated from the game's heap for each side; everything
// they keep is in the scratch area, so a correct run leaves no heap address
// in it. Compared with harness::Outputs: the scratch area, the frame's
// words below the stack pointer (the register saves left out: adapters
// don't make them), r3.

#include "harness.h"

#include "audio/render_driver.h"
#include "hooks/lifted_call.h"
#include "memory/c_runtime_heap.h"
#include "sound_cues/manager_update.h"

#include <initializer_list>
#include <string>
#include <vector>

namespace harness {

namespace {

namespace sc = sound_cues;

// Leaves: nothing (sub_82106540), [r4] = [r3 + 12] and r3 = 0
// (sub_82465418), [r4] = [r3 + 252] and r3 = [r3 + 248] (sub_82099580),
// r3 = [r3 + 60] (sub_82090770), [r3] and [r3 + 4] cleared (sub_82126860).
constexpr uint32_t kNothing = 0x82106540, kState12 = 0x82465418, kState252 = 0x82099580,
                   kGet60 = 0x82090770, kClear = 0x82126860;

// The scratch area's layout.
constexpr uint32_t kManager = 0x000;  // 1,544 bytes
constexpr uint32_t kBankVtableA = 0x620, kBankVtableB = 0x640, kCueVtable = 0x660,
                   kEngineVtable = 0x680;
constexpr uint32_t kVector = 0x6A0;  // up to 8 words
constexpr uint32_t kPairs = 0x6C0, kPairBytes = 0x58;  // 6
constexpr uint32_t kHolders = 0x8D0;                   // 6 of 16 bytes
constexpr uint32_t kBanks = 0x930;                     // 6 of 16 bytes
constexpr uint32_t kBigBank = 0x990;                   // 256 bytes
constexpr uint32_t kHead = 0xA90;
constexpr uint32_t kNodes = 0xAA0;     // 8 of 12 bytes
constexpr uint32_t kSounds = 0xB00;    // 8 of 24 bytes
constexpr uint32_t kEmitters = 0xBC0;  // 8 of 4 bytes
constexpr uint32_t kCues = 0xBE0;      // 8 of 16 bytes
constexpr uint32_t kDriver = 0xC60;
constexpr uint32_t kEngine = 0xC80, kXact = 0xCA0;
constexpr uint32_t kFreedBanks = 0xD00;  // 8 of 16 bytes
constexpr uint32_t kCompared = 0xD80;

using Step = void (*)(PPCContext&, uint8_t*);

uint32_t Pick(Random& random, std::initializer_list<uint32_t> choices) {
  return *(choices.begin() + random.Below(uint32_t(choices.size())));
}

void Vtables(Context& h) {
  const uint32_t d = h.data;
  guest::StoreU32(h.base, d + kBankVtableA + 16, kNothing);
  guest::StoreU32(h.base, d + kBankVtableA + 20, kState12);
  guest::StoreU32(h.base, d + kBankVtableB + 16, kNothing);
  guest::StoreU32(h.base, d + kBankVtableB + 20, kState252);
  guest::StoreU32(h.base, d + kCueVtable + 0, kNothing);
  guest::StoreU32(h.base, d + kCueVtable + 8, kState12);
  guest::StoreU32(h.base, d + kCueVtable + 12, kNothing);
}

// Runs one side: the stack below the stack pointer filled from `fill`,
// `lay(v)` writes the inputs, the function runs on r3 = `r3`, and the
// scratch area, `frame` bytes below the stack pointer without the top
// `saves`, r1 and r3 go into `out`.
template <typename Lay>
void Side(Context& h, int v, Step recompiled, Step native, uint32_t r3, uint32_t fill,
          uint32_t frame, uint32_t saves, Lay lay, Outputs& out) {
  Fill(h.base, h.data, kScratchBytes);
  Vtables(h);
  lay(v);
  PPCContext t = CallContext(h);
  const uint32_t sp = t.r1.u32;
  for (uint32_t k = 0; k < 512; k += 4) {
    guest::StoreU32(h.base, sp - 512 + k, fill + k * 0x01010101);
  }
  t.r3.u64 = r3;
  (v == 0 ? recompiled : native)(t, h.base);
  out.Word(t.r1.u32);
  out.Word(t.r3.u32);
  out.Bytes(Read(h.base, h.data, kCompared));
  out.Bytes(Read(h.base, sp - frame, frame - saves));
}

// --- AudioEngine::DoWork ---

void DoWork(Context& h) {
  Results results("sub_8259D1A8 AudioEngine::DoWork");
  for (int c = 0; c < 60; c++) {
    const uint32_t slot = Pick(h.random, {kNothing, kGet60, kClear});
    const uint32_t answer = uint32_t(h.random.Next());
    const uint32_t fill = uint32_t(h.random.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Side(
          h, v, __imp__sub_8259D1A8, lifted_call::EnterR3<sc::AudioEngineDoWork>,
          h.data + kEngine, fill, 64, 0,
          [&](int) {
            guest::StoreU32(h.base, h.data + kEngine + 8, h.data + kXact);
            guest::StoreU32(h.base, h.data + kXact, h.data + kEngineVtable);
            guest::StoreU32(h.base, h.data + kXact + 60, answer);
            guest::StoreU32(h.base, h.data + kEngineVtable + 16, slot);
          },
          out[v]);
    }
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  results.Report();
}

// --- AudioManager::FreeReleasedBanks ---

// A vector entry: 0, a pair in the scratch area BankPair::CanFree refuses
// (no release asked, no holder, no bank, the bank in use or its state
// failing), or a pair on the heap it accepts (its holders on the heap too).
enum class Entry { kNull, kKept, kFreed };

struct KeptPair {
  uint8_t release;
  int refuse;  // 0 no release asked, 1 no holder, 2 no bank, 3 in use, 4 failing
  uint32_t state;
};

struct BanksCase {
  bool empty;  // begin 0
  std::vector<Entry> entries;
  std::vector<KeptPair> kept;
  std::vector<uint32_t> freed_index;  // the freed pairs' bank index (+80)
  std::vector<uint32_t> freed_state;  // their bank's state (bit 0 clear)
};

BanksCase MakeBanksCase(Random& r) {
  BanksCase c;
  c.empty = r.Below(10) == 0;
  const uint32_t n = r.Below(9);
  for (uint32_t i = 0; i < n; i++) {
    Entry e = Entry(r.Below(3));
    // An empty vector (begin 0) frees nothing: no heap pairs in it.
    if (c.empty && e == Entry::kFreed) {
      e = Entry::kNull;
    }
    if (e == Entry::kKept && c.kept.size() == 6) {
      e = Entry::kNull;
    }
    c.entries.push_back(e);
    if (e == Entry::kKept) {
      KeptPair k;
      k.refuse = int(r.Below(5));
      k.release = k.refuse == 0 ? 0 : uint8_t(1 + r.Below(255));
      k.state = uint32_t(r.Next()) | 1;
      c.kept.push_back(k);
    } else if (e == Entry::kFreed) {
      c.freed_index.push_back(r.Below(3) == 0 ? 2 : r.Below(8));
      c.freed_state.push_back(uint32_t(r.Next()) & ~1u);
    }
  }
  return c;
}

void LayBanks(Context& h, const BanksCase& c) {
  uint8_t* base = h.base;
  const uint32_t d = h.data;
  const uint32_t m = d + kManager;
  uint32_t kept = 0, freed = 0;
  for (size_t i = 0; i < c.entries.size(); i++) {
    uint32_t pair = 0;
    if (c.entries[i] == Entry::kKept) {
      const KeptPair& k = c.kept[kept];
      pair = d + kPairs + kPairBytes * kept;
      const uint32_t holder = d + kHolders + 16 * kept;
      const uint32_t bank = d + kBanks + 16 * kept;
      guest::StoreU8(base, pair + 69, k.release);
      guest::StoreU32(base, pair + 76, k.refuse == 1 ? 0 : holder);
      guest::StoreU32(base, holder + 8, k.refuse == 2 ? 0 : (k.refuse == 4 ? d + kBigBank : bank));
      guest::StoreU32(base, bank, d + kBankVtableA);
      guest::StoreU32(base, bank + 12, k.state);
      kept++;
    } else if (c.entries[i] == Entry::kFreed) {
      pair = memory::crt::OperatorNew(base, 96).value_or(0);
      const uint32_t wave = memory::crt::OperatorNew(base, 16).value_or(0);
      const uint32_t sound = memory::crt::OperatorNew(base, 16).value_or(0);
      // The bank: one per freed pair (scratch; its Destroy does nothing).
      const uint32_t bank = d + kFreedBanks + 16 * freed;
      guest::StoreU32(base, bank, d + kBankVtableA);
      guest::StoreU32(base, bank + 12, c.freed_state[freed]);
      for (uint32_t k = 0; k < 96; k += 4) {
        guest::StoreU32(base, pair + k, 0);
      }
      guest::StoreU8(base, pair + 69, 1);
      guest::StoreU32(base, pair + 64, 15);  // the name's capacity: short, nothing to free
      guest::StoreU32(base, pair + 72, wave);
      guest::StoreU32(base, pair + 76, sound);
      guest::StoreU32(base, pair + 80, c.freed_index[freed]);
      guest::StoreU32(base, wave, 0);
      guest::StoreU32(base, wave + 4, 0);
      guest::StoreU32(base, wave + 8, 0);
      guest::StoreU32(base, wave + 12, 0xFFFFFFFF);
      guest::StoreU32(base, sound, 0);
      guest::StoreU32(base, sound + 4, 0);
      guest::StoreU32(base, sound + 8, bank);
      guest::StoreU32(base, sound + 12, 0);
      freed++;
    }
    guest::StoreU32(base, d + kVector + 4 * uint32_t(i), pair);
  }
  // The failing bank: state bit 0 clear but a negative result.
  guest::StoreU32(base, d + kBigBank, d + kBankVtableB);
  guest::StoreU32(base, d + kBigBank + 248, 0x80000000u | uint32_t(c.entries.size()));
  guest::StoreU32(base, d + kBigBank + 252, 0);
  guest::StoreU32(base, m + 1516, c.empty ? 0 : d + kVector);
  guest::StoreU32(base, m + 1520, d + kVector + 4 * uint32_t(c.entries.size()));
}

void FreeReleasedBanks(Context& h) {
  Results results("sub_821184E8 AudioManager::FreeReleasedBanks");
  for (int c = 0; c < 200; c++) {
    const BanksCase banks = MakeBanksCase(h.random);
    const uint32_t fill = uint32_t(h.random.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Side(
          h, v, __imp__sub_821184E8, lifted_call::EnterR3<sc::ManagerFreeReleasedBanks>,
          h.data + kManager, fill, 112, 32, [&](int) { LayBanks(h, banks); }, out[v]);
    }
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  results.Report();
}

// --- The Sounds' list ---

// A Sound: kept ones (and their node, emitter and cue) in the scratch area;
// stopped ones RemoveStoppedSounds deletes on the heap.
struct SoundCase {
  bool stopped;  // on the heap (RemoveStoppedSounds' cases only)
  bool cue;
  uint32_t state;
  uint8_t playing, start;
  uint32_t words[2];  // +0 (the owner, unread) and +20
};

std::vector<SoundCase> MakeSounds(Random& r, bool remove) {
  std::vector<SoundCase> sounds(r.Below(9));
  for (SoundCase& s : sounds) {
    s.stopped = remove && r.Below(3) == 0;
    s.cue = s.stopped || r.Below(4) != 0;
    s.state = uint32_t(r.Next());
    if (s.stopped) {
      s.state |= 0x20;
    } else if (remove && s.cue) {
      s.state &= ~0x20u;
    }
    s.playing = uint8_t(r.Below(2));
    s.start = uint8_t(r.Below(2));
    s.words[0] = uint32_t(r.Next());
    s.words[1] = uint32_t(r.Next());
  }
  return sounds;
}

void LaySounds(Context& h, const std::vector<SoundCase>& sounds) {
  uint8_t* base = h.base;
  const uint32_t d = h.data;
  const uint32_t list = d + kManager + 1528;
  const uint32_t head = d + kHead;
  guest::StoreU32(base, list + 4, head);
  guest::StoreU32(base, list + 8, uint32_t(sounds.size()));
  uint32_t previous = head;
  for (size_t i = 0; i < sounds.size(); i++) {
    const SoundCase& s = sounds[i];
    const uint32_t k = uint32_t(i);
    uint32_t node = d + kNodes + 12 * k;
    uint32_t sound = d + kSounds + 24 * k;
    uint32_t emitter = d + kEmitters + 4 * k;
    if (s.stopped) {
      node = memory::crt::OperatorNew(base, 12).value_or(0);
      sound = memory::crt::OperatorNew(base, 24).value_or(0);
      emitter = memory::crt::OperatorNew(base, 300).value_or(0);
    }
    const uint32_t cue = d + kCues + 16 * k;
    guest::StoreU32(base, cue, d + kCueVtable);
    guest::StoreU32(base, cue + 12, s.state);
    guest::StoreU32(base, emitter, s.cue ? cue : 0);
    guest::StoreU32(base, sound, s.words[0]);
    guest::StoreU32(base, sound + 4, emitter);
    guest::StoreU8(base, sound + 8, 0);   // not 3D (the 3D path never ran)
    guest::StoreU8(base, sound + 9, 0);   // no loop
    guest::StoreU8(base, sound + 10, s.playing);
    guest::StoreU8(base, sound + 11, s.start);
    guest::StoreU32(base, sound + 20, s.words[1]);
    guest::StoreU32(base, node + 4, previous);
    guest::StoreU32(base, node + 8, sound);
    guest::StoreU32(base, previous, node);
    previous = node;
  }
  guest::StoreU32(base, previous, head);
  guest::StoreU32(base, head + 4, previous);
}

void SoundsGroup(Context& h, const std::string& name, bool remove, Step recompiled, Step native) {
  Results results(name);
  for (int c = 0; c < 150; c++) {
    const std::vector<SoundCase> sounds = MakeSounds(h.random, remove);
    const uint32_t fill = uint32_t(h.random.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Side(h, v, recompiled, native, h.data + kManager, fill, 112, 8,
           [&](int) { LaySounds(h, sounds); }, out[v]);
    }
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  results.Report();
}

// --- AudioRenderDriver::Destroy (handle 0 only: what our wrapper leaves; a
// handle would be unregistered from the SDK's audio system for real) ---

void RenderDriverDestroy(Context& h) {
  Results results("sub_824658C8 AudioRenderDriver::Destroy");
  for (int c = 0; c < 40; c++) {
    uint32_t words[6];
    for (uint32_t& w : words) {
      w = uint32_t(h.random.Next());
    }
    words[4] = 0;
    const uint32_t fill = uint32_t(h.random.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Side(
          h, v, __imp__sub_824658C8, lifted_call::EnterR3<audio::RenderDriverDestroy>,
          h.data + kDriver, fill, 96, 16,
          [&](int) {
            for (uint32_t k = 0; k < 6; k++) {
              guest::StoreU32(h.base, h.data + kDriver + 4 * k, words[k]);
            }
          },
          out[v]);
    }
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  results.Report();
}

}  // namespace

void AudioWraps(Context& h) {
  DoWork(h);
  FreeReleasedBanks(h);
  SoundsGroup(h, "sub_82118FB0 AudioManager::UpdateSounds", false, __imp__sub_82118FB0,
              lifted_call::EnterR3<sc::ManagerUpdateSounds>);
  SoundsGroup(h, "sub_82118C18 AudioManager::RemoveStoppedSounds", true, __imp__sub_82118C18,
              lifted_call::EnterR3<sc::ManagerRemoveStoppedSounds>);
  RenderDriverDestroy(h);
}

}  // namespace harness
