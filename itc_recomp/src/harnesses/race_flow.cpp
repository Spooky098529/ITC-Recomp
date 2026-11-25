// The race flow (src/race_flow/, docs/functions/race-flow.md) against the
// recompiled functions. The original runs with --native_race_flow off, the
// plain C++ through the register step (hooks/race_flow.h). RaceFlow::Init:
// an empty state machine or one with random transitions already (fewer
// than 140, so the 53 fit), some with the keys Init adds (the last,
// (37, 50), among them: its r3 is then the index); the race data's kind
// (+0xA3AF0) 6 or not; the original's frame filled with the same random
// words for both (the words it never writes go into the transitions); the
// machine, the frame (but its back chain and saved registers), the scratch
// area and r3 compared. The race data's pointer is put back after each
// side. Compared with harness::Outputs.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "game_modes/game_modes.h"
#include "hooks/race_flow.h"
#include "memory/heap.h"
#include "race_flow/race_flow.h"

#include <fmt/format.h>

ITC_FLAG_DECLARE(bool, native_race_flow);

namespace harness {

namespace {

namespace rf = race_flow;

constexpr uint32_t kMachineBytes = 0x2600;
constexpr uint32_t kRaceBytes = 0xA4000;  // to the kind (+0xA3AF0)

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

void Inits(Context& h, const Block& machine, const Block& race) {
  namespace gm = game_modes;
  constexpr uint32_t kSaves = 112;  // __savegprlr_19: r19 at -112, lr at -8
  Results results("RaceFlow::Init (sub_82160AE0)");
  const uint32_t frame = CallContext(h).r1.u32 - rf::kInitFrame;
  // Keys Init adds, planted in the odd cases.
  constexpr uint32_t kPlanted[][2] = {{1, 0}, {23, 29}, {30, 7}, {37, 50}};
  for (uint64_t seed = 0; seed < 12; seed++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed * 151 + 67);
      const uint32_t saved_race = guest::LoadU32(h.base, rf::kRaceData);
      WriteRandom(h.base, h.data, kScratchBytes, r);
      WriteRandom(h.base, machine.address, machine.bytes, r);
      WriteRandom(h.base, frame, rf::kInitFrame, r);
      // The race kind: 6 (both players 0) in a third of the cases.
      guest::StoreU32(h.base, race.address + rf::kRaceKind,
                      seed % 3 == 0 ? 6 : (r.Below(2) ? r.Below(8) : uint32_t(r.Next())));
      guest::StoreU32(h.base, rf::kRaceData, race.address);
      if (seed % 2 == 0) {
        gm::StateMachineConstruct(h.base, machine.address);
        guest::StoreU32(h.base, machine.address + gm::kTransitionCount, 0);
      } else {
        const uint32_t count = 8 + r.Below(130);
        guest::StoreU32(h.base, machine.address + gm::kTransitionCount, count);
        for (uint32_t i = 0; i < 200; i++) {  // keys of small numbers, some matching
          for (uint32_t k = 0; k < 4; k++) {
            guest::StoreU32(h.base, machine.address + 48 * i + 4 * k,
                            r.Below(k < 2 ? 60 : 2));
          }
        }
        for (const auto& key : kPlanted) {
          if (seed % 4 == 1 && key[0] == 37) {
            continue;  // seeds 1, 5 and 9: the last Add makes a new transition
          }
          const uint32_t at = machine.address + 48 * r.Below(count);
          guest::StoreU32(h.base, at, key[0]);
          guest::StoreU32(h.base, at + 4, key[1]);
          guest::StoreU32(h.base, at + 8, 0);
          guest::StoreU32(h.base, at + 12, 0);
        }
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = machine.address;
      if (v == 0) {
        ITC_FLAG(native_race_flow) = false;
        __imp__sub_82160AE0(t, h.base);
        ITC_FLAG(native_race_flow) = true;
      } else {
        hooks::RaceFlowInit(t, h.base);
      }
      guest::StoreU32(h.base, rf::kRaceData, saved_race);
      out[v].Bytes(Read(h.base, machine.address, machine.bytes));
      out[v].Bytes(Read(h.base, frame + 80, rf::kInitFrame - 80 - kSaves));
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("seed {}", seed));
  }
  results.Report();
}

}  // namespace

void RaceFlow(Context& h) {
  const Block machine(kMachineBytes);
  const Block race(kRaceBytes);
  if (!machine.address || !race.address) {
    ITC_LOG_ERROR("harness: race_flow: no block from the allocation layer");
    return;
  }
  Inits(h, machine, race);
}

}  // namespace harness
