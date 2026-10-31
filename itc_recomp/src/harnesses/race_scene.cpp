// The race scene (src/race_scene/, docs/functions/race-scene.md) against
// the recompiled functions. The originals run with --native_race_scene off
// (their callees' own flags as they are: those were checked by their
// harnesses), the plain C++ through the register steps (hooks/race_scene.h)
// or directly where it has no adapter. The inputs go in blocks from the
// allocation layer, the stack below the call's stack pointer and the game's
// globals the functions read (put back after each side). Compared with
// harness::Outputs.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "game_modes/game_modes.h"
#include "hooks/race_scene.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "post_effects/post_effects.h"
#include "race_scene/race_scene.h"

#include <fmt/format.h>

#include <vector>

ITC_FLAG_DECLARE(bool, native_race_scene);

namespace harness {

namespace {

namespace rs = race_scene;

// A block of guest memory for the cases, freed at the end.
struct Block {
  explicit Block(uint32_t bytes) : bytes(bytes), address(memory::Alloc(bytes, 0)) {}
  ~Block() { memory::Free(address); }
  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;
  uint32_t bytes;
  uint32_t address;
};

void WriteRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

// The original with --native_race_scene off.
template <GuestFunction recompiled>
void Original(PPCContext& ctx, uint8_t* base) {
  ITC_FLAG(native_race_scene) = false;
  recompiled(ctx, base);
  ITC_FLAG(native_race_scene) = true;
}

// The game's globals a case changes, put back when it ends.
class Globals {
 public:
  Globals(uint8_t* base, std::vector<uint32_t> addresses) : base_(base), addresses_(addresses) {
    for (uint32_t a : addresses_) {
      saved_.push_back(guest::LoadU32(base_, a));
    }
  }
  ~Globals() {
    for (size_t i = 0; i < addresses_.size(); i++) {
      guest::StoreU32(base_, addresses_[i], saved_[i]);
    }
  }

 private:
  uint8_t* base_;
  std::vector<uint32_t> addresses_;
  std::vector<uint32_t> saved_;
};

// --- RaceSceneMachine::Init: an empty state machine or one with random
// transitions already (fewer than 150, so the 11 fit), some with the keys
// Init adds (the last, (37, 50), among them: its r3 is then the index); the
// original's frame filled with the same random words for both (the words it
// never writes go into the transitions), and the scene's block operator new
// gives filled the same (a probe block of its size filled and freed first:
// the layer is first fit, so both sides get it); the machine, the scene (its
// pointer at 0x826D82D4 as the block, its 31,168 bytes: the records' and
// lights' words RaceScene::RaceScene writes, the rest as the probe left it),
// the frame (but its back chain and saved registers) and r3 compared ---

void Inits(Context& h) {
  namespace gm = game_modes;
  constexpr uint32_t kSaves = 56;  // __savegprlr_26: r26 at -56, lr at -8
  const Block machine(0x2600);
  if (!machine.address) {
    ITC_LOG_ERROR("harness: race_scene: no block from the allocation layer");
    return;
  }
  Results results("RaceSceneMachine::Init (sub_8215D808)");
  const uint32_t frame = CallContext(h).r1.u32 - rs::kInitFrame;
  // Keys Init adds, planted in the odd cases.
  constexpr uint32_t kPlanted[][2] = {{1, 0}, {38, 29}, {32, 3}, {37, 50}};
  for (uint64_t seed = 0; seed < 8; seed++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed * 151 + 29);
      Globals globals(h.base, {rs::kScene});
      WriteRandom(h.base, h.data, kScratchBytes, r);
      WriteRandom(h.base, machine.address, machine.bytes, r);
      WriteRandom(h.base, frame, rs::kInitFrame, r);
      if (seed % 2 == 0) {
        gm::StateMachineConstruct(h.base, machine.address);
        guest::StoreU32(h.base, machine.address + gm::kTransitionCount, 0);
      } else {
        const uint32_t count = 8 + r.Below(140);
        guest::StoreU32(h.base, machine.address + gm::kTransitionCount, count);
        for (uint32_t i = 0; i < 200; i++) {  // keys of small numbers, some matching
          for (uint32_t k = 0; k < 4; k++) {
            guest::StoreU32(h.base, machine.address + 48 * i + 4 * k, r.Below(k < 2 ? 60 : 2));
          }
        }
        for (const auto& key : kPlanted) {
          if (seed % 4 == 1 && key[0] == 37) {
            continue;  // seeds 1 and 5: the last Add makes a new transition
          }
          const uint32_t at = machine.address + 48 * r.Below(count);
          guest::StoreU32(h.base, at, key[0]);
          guest::StoreU32(h.base, at + 4, key[1]);
          guest::StoreU32(h.base, at + 8, 0);
          guest::StoreU32(h.base, at + 12, 0);
        }
      }
      uint32_t probe = 0;
      if (const auto block = memory::crt::OperatorNew(h.base, rs::kSceneBytes)) {
        probe = *block;
        WriteRandom(h.base, probe, rs::kSceneBytes, r);
        memory::crt::OperatorDelete(probe);
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = machine.address;
      (v == 0 ? Original<__imp__sub_8215D808> : hooks::RaceSceneMachineInit)(t, h.base);
      const uint32_t block = guest::LoadU32(h.base, rs::kScene);
      out[v].Word(block != 0 && block == probe ? 1 : 0);
      out[v].Bytes(Read(h.base, machine.address, machine.bytes));
      if (block != 0) {
        out[v].Bytes(Read(h.base, block, rs::kSceneBytes));
        memory::crt::OperatorDelete(block);
      }
      out[v].Bytes(Read(h.base, frame + 80, rs::kInitFrame - 80 - kSaves));
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("seed {}", seed));
  }
  results.Report();
}

// --- The light set: RaceLights::InitEntry on a random entry for each index
// from -2 to 13 (the kinds' branches, the colors' cases, and 11 and more:
// nothing), twice each; RaceLights::SetUp on a random set. The words they
// store as floats compared as floats, the rest exactly; SetUp's r3 and f1
// too ---

void Lights(Context& h) {
  constexpr uint32_t kSetBytes = 16 + 96 * 11 + 64;  // past +1072
  const Block set(kSetBytes);
  if (!set.address) {
    ITC_LOG_ERROR("harness: race_scene: no block from the allocation layer");
    return;
  }
  // An entry's words: +0 the kind, +4 to +15 untouched, then float vectors.
  auto entry_out = [&](Outputs& out, uint32_t entry) {
    out.Bytes(Read(h.base, entry, 16));
    out.Floats(h.base, entry + 16, 20);
  };
  {
    Results results("RaceLights::InitEntry (sub_8215C368)");
    int c = 0;
    for (int32_t index = -2; index <= 13; index++) {
      for (int k = 0; k < 2; k++, c++) {
        Outputs out[2];
        for (int v = 0; v < 2; v++) {
          Random r(uint64_t(c) * 97 + 11);
          WriteRandom(h.base, set.address, kSetBytes, r);
          if (v == 0) {
            PPCContext t = CallContext(h);
            t.r3.u64 = set.address;
            t.r4.s64 = index;
            __imp__sub_8215C368(t, h.base);
          } else {
            rs::LightsInitEntry(h.base, set.address, index);
          }
          entry_out(out[v], set.address);
        }
        results.Check(out[0], out[1], fmt::format("index {} ({})", index, k));
      }
    }
    results.Report();
  }
  {
    Results results("RaceLights::SetUp (sub_8215CF78)");
    for (uint64_t seed = 0; seed < 4; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 61 + 7);
        WriteRandom(h.base, set.address, kSetBytes, r);
        PPCContext t = CallContext(h);
        t.r3.u64 = set.address;
        (v == 0 ? Original<__imp__sub_8215CF78> : hooks::RaceSceneLightsSetUp)(t, h.base);
        out[v].Bytes(Read(h.base, set.address, 16));
        for (uint32_t i = 0; i < 11; i++) {
          entry_out(out[v], set.address + 16 + 96 * i);
        }
        out[v].Bytes(Read(h.base, set.address + 16 + 96 * 11, kSetBytes - 16 - 96 * 11));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        out[v].Double(t.f1.u64);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
}

// --- The post effects' parameters: a world of our own (the 15 effect
// pointers at 0x826C5D80, the viewports' holder at 0x826C2CE8 with its +8,
// split screen or not): three glows (effects 1, 5, 9) and three color
// filters (2, 6, 10) in random blocks, their vtables with the setters (one
// case in eight a slot elsewhere, so the adapter's original runs; never
// address 0's). RaceScene::SetEffectParams on kinds -1 to 9 and indices 0 to
// 2 (the image's table, and 0x82637C94 from kind 8); Glows::SetParams on
// random entries. The setters' floats compared as floats (8436 to 8476 and
// 8120), every other byte of the effects exactly, r3 and f1 ---

void EffectParams(Context& h) {
  namespace pe = post_effects;
  constexpr uint32_t kGlowBytes = 8480;
  constexpr uint32_t kFilterBytes = 8128;
  constexpr uint32_t kGlows[] = {1, 5, 9};
  constexpr uint32_t kFilters[] = {2, 6, 10};
  const Block glows(3 * kGlowBytes);
  const Block filters(3 * kFilterBytes);
  const Block misc(0x200);  // the vtables (+0, +64) and the holder (+128)
  if (!glows.address || !filters.address || !misc.address) {
    ITC_LOG_ERROR("harness: race_scene: no block from the allocation layer");
    return;
  }
  const uint32_t glow_vtable = misc.address;
  const uint32_t filter_vtable = misc.address + 64;
  const uint32_t holder = misc.address + 128;
  // The world: `wrong` puts a slot elsewhere (a glow's slot 2 + wrong % 11,
  // or the filters' slot 8).
  auto world = [&](Random& r, bool split, int wrong) {
    WriteRandom(h.base, glows.address, glows.bytes, r);
    WriteRandom(h.base, filters.address, filters.bytes, r);
    WriteRandom(h.base, misc.address, misc.bytes, r);
    for (uint32_t k = 2; k <= 12; k++) {
      guest::StoreU32(h.base, glow_vtable + 4 * k, 0x82136EF8 + 8 * (k - 2));
    }
    guest::StoreU32(h.base, filter_vtable + 32, 0x82135148);
    if (wrong >= 0) {
      if (wrong % 2 == 0) {  // another glow setter in the slot
        const uint32_t k = 2 + uint32_t(wrong) % 11;
        guest::StoreU32(h.base, glow_vtable + 4 * k, 0x82136EF8 + 8 * ((k - 1) % 11));
      } else {  // the filter's Set8116
        guest::StoreU32(h.base, filter_vtable + 32, 0x82135140);
      }
    }
    for (uint32_t i = 0; i < pe::kCount; i++) {
      guest::StoreU32(h.base, pe::kEffects + 4 * i, 0);
    }
    for (uint32_t i = 0; i < 3; i++) {
      const uint32_t glow = glows.address + kGlowBytes * i;
      const uint32_t filter = filters.address + kFilterBytes * i;
      guest::StoreU32(h.base, glow, glow_vtable);
      guest::StoreU32(h.base, filter, filter_vtable);
      guest::StoreU32(h.base, pe::kEffects + 4 * kGlows[i], glow);
      guest::StoreU32(h.base, pe::kEffects + 4 * kFilters[i], filter);
    }
    guest::StoreU32(h.base, 0x826C2CE8, holder);
    guest::StoreU32(h.base, holder + 8, split ? 1 + r.Below(5) : 0);
  };
  auto effects_out = [&](Outputs& out) {
    for (uint32_t i = 0; i < 3; i++) {
      const uint32_t glow = glows.address + kGlowBytes * i;
      out.Bytes(Read(h.base, glow, 8436));
      out.Floats(h.base, glow + 8436, 11);
      out.Bytes(Read(h.base, glow + 8480, kGlowBytes - 8480));
      const uint32_t filter = filters.address + kFilterBytes * i;
      out.Bytes(Read(h.base, filter, 8120));
      out.Float(guest::LoadU32(h.base, filter + 8120));
      out.Bytes(Read(h.base, filter + 8124, kFilterBytes - 8124));
    }
  };
  std::vector<uint32_t> globals = {0x826C2CE8};
  for (uint32_t i = 0; i < pe::kCount; i++) {
    globals.push_back(pe::kEffects + 4 * i);
  }
  {
    Results results("RaceScene::SetEffectParams (sub_8228F5C0)");
    int c = 0;
    for (int32_t kind = -1; kind <= 9; kind++) {
      for (uint32_t index = 0; index < 3; index++, c++) {
        Outputs out[2];
        for (int v = 0; v < 2; v++) {
          Random r(uint64_t(c) * 131 + 17);
          Globals saved(h.base, globals);
          world(r, c % 3 == 1, c % 8 == 5 ? int(r.Below(22)) : -1);
          PPCContext t = CallContext(h);
          t.r3.s64 = kind;
          t.r4.u64 = index;
          (v == 0 ? Original<__imp__sub_8228F5C0> : hooks::RaceSceneSetEffectParams)(t, h.base);
          effects_out(out[v]);
          out[v].Word(t.r3.u32);
          out[v].Word(uint32_t(t.r3.u64 >> 32));
          out[v].Double(t.f1.u64);
        }
        results.Check(out[0], out[1], fmt::format("kind {} index {}", kind, index));
      }
    }
    results.Report();
  }
  {
    Results results("Glows::SetParams (sub_8228F0B0)");
    const uint32_t entry = h.data;
    for (uint64_t seed = 0; seed < 8; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 53 + 3);
        Globals saved(h.base, globals);
        world(r, seed % 2 == 1, -1);
        WriteRandom(h.base, h.data, kScratchBytes, r);
        uint32_t r3 = 0;
        if (v == 0) {
          PPCContext t = CallContext(h);
          t.r3.u64 = entry;
          __imp__sub_8228F0B0(t, h.base);
          r3 = t.r3.u32;
        } else {
          r3 = rs::GlowsSetParams(h.base, entry);
        }
        effects_out(out[v]);
        out[v].Word(r3);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
}

// --- The machine's toggle: a world of our own (the scene at 0x826D82D4 with
// its three screen objects, each there or not; the 15 effect pointers, some
// 0; the course scenery at 0x826C551C and the scene at 0x826C2E44, which
// Race::ToggleEffects flips); TogglePause and Resume on a random machine
// whose +9608 is 0, 1 or random. The machine's word, every block and r3
// compared exactly ---

void Toggles(Context& h) {
  namespace pe = post_effects;
  constexpr uint32_t kSmall = 64;  // a screen object's or effect's block
  const Block race(rs::kSceneBytes);
  const Block objects(3 * kSmall);
  const Block effects(pe::kCount * kSmall);
  const Block others(0x1400);  // the scenery at +0, the scene at +0x200
  const Block machine(0x2600);
  if (!race.address || !objects.address || !effects.address || !others.address ||
      !machine.address) {
    ITC_LOG_ERROR("harness: race_scene: no block from the allocation layer");
    return;
  }
  std::vector<uint32_t> globals = {rs::kScene, 0x826C551C, 0x826C2E44};
  for (uint32_t i = 0; i < pe::kCount; i++) {
    globals.push_back(pe::kEffects + 4 * i);
  }
  struct Function {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
  };
  const Function functions[] = {
      {"RaceSceneMachine::TogglePause (sub_8215C750)", Original<__imp__sub_8215C750>,
       hooks::RaceSceneTogglePause},
      {"RaceSceneMachine::Resume (sub_8215D0D8)", Original<__imp__sub_8215D0D8>,
       hooks::RaceSceneResume},
  };
  for (const Function& f : functions) {
    Results results(f.name);
    for (uint64_t seed = 0; seed < 12; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 71 + 13);
        Globals saved(h.base, globals);
        WriteRandom(h.base, race.address, race.bytes, r);
        WriteRandom(h.base, objects.address, objects.bytes, r);
        WriteRandom(h.base, effects.address, effects.bytes, r);
        WriteRandom(h.base, others.address, others.bytes, r);
        WriteRandom(h.base, machine.address, machine.bytes, r);
        for (uint32_t i = 0; i < 3; i++) {
          guest::StoreU32(h.base, race.address + 31152 + 4 * i,
                          r.Below(3) != 0 ? objects.address + kSmall * i : 0);
        }
        for (uint32_t i = 0; i < pe::kCount; i++) {
          guest::StoreU32(h.base, pe::kEffects + 4 * i,
                          r.Below(4) != 0 ? effects.address + kSmall * i : 0);
          guest::StoreU32(h.base, effects.address + kSmall * i + 44, r.Below(3));
        }
        guest::StoreU32(h.base, rs::kScene, race.address);
        guest::StoreU32(h.base, 0x826C551C, others.address);
        guest::StoreU32(h.base, 0x826C2E44, others.address + 0x200);
        const uint32_t paused = seed % 3 == 0 ? 0 : seed % 3 == 1 ? 1 : uint32_t(r.Next());
        guest::StoreU32(h.base, machine.address + rs::kPaused, paused);
        PPCContext t = CallContext(h);
        t.r3.u64 = machine.address;
        (v == 0 ? f.recompiled : f.native)(t, h.base);
        out[v].Bytes(Read(h.base, machine.address, machine.bytes));
        out[v].Bytes(Read(h.base, race.address, race.bytes));
        out[v].Bytes(Read(h.base, objects.address, objects.bytes));
        out[v].Bytes(Read(h.base, effects.address, effects.bytes));
        out[v].Bytes(Read(h.base, others.address, others.bytes));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
}

// --- RaceScene::Destroy: no scene, or a scene from operator new filled
// with random words; whether the block was freed (no longer the layer's),
// the pointer (left) and r3 compared ---

void Destroys(Context& h) {
  Results results("RaceScene::Destroy (sub_8215D790)");
  for (uint64_t seed = 0; seed < 4; seed++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed * 89 + 31);
      Globals saved(h.base, {rs::kScene});
      uint32_t block = 0;
      if (seed != 0) {
        if (const auto made = memory::crt::OperatorNew(h.base, rs::kSceneBytes)) {
          block = *made;
          WriteRandom(h.base, block, rs::kSceneBytes, r);
        }
      }
      guest::StoreU32(h.base, rs::kScene, block);
      PPCContext t = CallContext(h);
      t.r3.u64 = 0x1234;
      (v == 0 ? Original<__imp__sub_8215D790> : hooks::RaceSceneDestroy)(t, h.base);
      const bool freed = block != 0 && !memory::IsHeapBlock(block);
      out[v].Word(block != 0 ? 1 : 0);
      out[v].Word(freed ? 1 : 0);
      out[v].Word(guest::LoadU32(h.base, rs::kScene) == block ? 1 : 0);
      out[v].Word(t.r3.u32 == block ? 0xFFFFFFFF : t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      if (block != 0 && !freed) {
        memory::crt::OperatorDelete(block);
      }
    }
    results.Check(out[0], out[1], fmt::format("seed {}", seed));
  }
  results.Report();
}

}  // namespace

void RaceScene(Context& h) {
  Inits(h);
  Lights(h);
  EffectParams(h);
  Toggles(h);
  Destroys(h);
}

}  // namespace harness
