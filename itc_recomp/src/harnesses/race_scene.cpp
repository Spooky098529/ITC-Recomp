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

}  // namespace

void RaceScene(Context& h) {
  Inits(h);
  Lights(h);
}

}  // namespace harness
