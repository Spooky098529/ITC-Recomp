// The type 12 mode, the game's Xbox Live mode (src/network_mode/,
// docs/functions/network-mode.md), against the recompiled functions. Each
// case fills a world of its own (one block from the allocation layer: the
// type 12 block with its fourteen machines at their offsets) with random
// words from the case's seed; runs the original (with --native_network_mode
// off, so what it calls of the mode's is the original too) or the register
// steps (hooks/network_mode.h); and compares the block, the function's
// frames without their register saves, the caller's area above r1, r1 and
// r3.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "game_modes/game_modes.h"
#include "hooks/heap.h"
#include "hooks/network_mode.h"
#include "memory/heap.h"
#include "network_mode/network_mode.h"

#include <fmt/format.h>

#include <functional>
#include <vector>

ITC_FLAG_DECLARE(bool, native_network_mode);

namespace harness {

namespace {

namespace nm = network_mode;

constexpr uint32_t kBlockBytes = 0x21100;  // the type 12 block (135,208 bytes)
constexpr uint32_t kStackBytes = 4096;     // below the call's stack pointer
constexpr uint32_t kCallerBytes = 256;     // above it

struct World {
  explicit World(uint8_t* base)
      : base(base), allocation(memory::Alloc(kBlockBytes + 16, 0)),
        block((allocation + 15) & ~15u) {}
  ~World() { memory::Free(allocation); }
  World(const World&) = delete;
  World& operator=(const World&) = delete;
  bool ok() const { return allocation != 0; }
  uint32_t at(uint32_t offset) const { return block + offset; }
  uint8_t* base;
  uint32_t allocation;
  uint32_t block;
};

void WriteRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

// A stack range compared, relative to the call's r1: [from, to).
struct Range {
  int32_t from;
  int32_t to;
};

using SetupFn = std::function<void(PPCContext&, Random&, const World&)>;

struct Spec {
  std::string name;
  int cases;
  GuestFunction recompiled;
  GuestFunction native;
  std::vector<Range> stack;  // compared below r1 (frames without their saves)
  SetupFn setup;
};

void Run(Context& h, const World& w, const Spec& spec) {
  Results results(spec.name);
  for (int c = 0; c < spec.cases; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      WriteRandom(h.base, w.block, kBlockBytes, r);
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      WriteRandom(h.base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
      t.r3.u64 = r.Next();
      t.r4.u64 = r.Next();
      t.r5.u64 = r.Next();
      spec.setup(t, r, w);
      t.lr = 0x82000000 | (r.Below(0x10000) << 2);
      if (v == 0) {
        const bool native = ITC_FLAG(native_network_mode);
        ITC_FLAG(native_network_mode) = false;
        spec.recompiled(t, h.base);
        ITC_FLAG(native_network_mode) = native;
      } else {
        spec.native(t, h.base);
      }
      out[v].Bytes(Read(h.base, w.block, kBlockBytes));
      for (const Range& range : spec.stack) {
        out[v].Bytes(Read(h.base, sp + range.from, uint32_t(range.to - range.from)));
      }
      out[v].Bytes(Read(h.base, sp, kCallerBytes));
      out[v].Word(t.r1.u32);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// The machine empty or holding random transitions with keys of small
// numbers, some the Init's (Add replaces those), as the type 9 harness's.
void InitMachine(Random& r, uint8_t* base, uint32_t machine) {
  namespace gm = game_modes;
  if (r.Below(2) == 0) {
    gm::StateMachineConstruct(base, machine);
    guest::StoreU32(base, machine + gm::kTransitionCount, 0);
  } else {
    const uint32_t count = 8 + r.Below(160);
    guest::StoreU32(base, machine + gm::kTransitionCount, count);
    for (uint32_t i = 0; i < gm::kTransitions; i++) {
      const uint32_t at = machine + gm::kTransitionBytes * i;
      guest::StoreU32(base, at, r.Below(12));
      guest::StoreU32(base, at + 4, r.Below(48));
      guest::StoreU32(base, at + 8, r.Below(8) == 0 ? 1 : 0);
      guest::StoreU32(base, at + 12, 0);
    }
  }
}

// The machines by their InitStates.
struct Machine {
  const char* name;
  uint32_t address;
  GuestFunction recompiled;
  GuestFunction native;
  uint32_t at;
  uint32_t frame;
  // Machines 6 to 13: their index and base Init.
  uint32_t index = 0;
  const char* base_name = nullptr;
  uint32_t base_address = 0;
  GuestFunction base_recompiled = nullptr;
  GuestFunction base_native = nullptr;
};

const Machine kMachines[] = {
    {"NetModeSelect::InitStates", 0x8227D680, __imp__sub_8227D680,
     hooks::NetModeSelectInitStates, nm::kModeSelectAt, nm::kModeSelectInitFrame},
    {"Type12Screen1::InitStates", 0x82276510, __imp__sub_82276510,
     hooks::Type12Screen1InitStates, nm::kScreen1At, nm::kScreen1InitFrame},
    {"Type12Screen3::InitStates", 0x8225F330, __imp__sub_8225F330,
     hooks::Type12Screen3InitStates, nm::kScreen3At, nm::kScreen3InitFrame},
    {"NetRanking::InitStates", 0x822873A0, __imp__sub_822873A0, hooks::NetRankingInitStates,
     nm::kRankingAt, nm::kRankingInitFrame},
    {"NetTaCourse::InitStates", 0x82281858, __imp__sub_82281858, hooks::NetTaCourseInitStates,
     nm::kTaCourseAt, nm::kTaCourseInitFrame},
    {"PowerGraphScreen12::InitStates", 0x821B96B8, __imp__sub_821B96B8,
     hooks::PowerGraphScreen12InitStates, nm::kPowerGraphAt, nm::kPowerGraphInitFrame},
    {"Type12Screen6::InitStates", 0x82268410, __imp__sub_82268410,
     hooks::Type12Screen6InitStates, nm::kScreen6At, nm::kScreen6InitFrame, 6,
     "Type12Screen6::InitBase", 0x82278C20, __imp__sub_82278C20, hooks::Type12Screen6InitBase},
    {"Type12Screen7::InitStates", 0x8226EB40, __imp__sub_8226EB40,
     hooks::Type12Screen7InitStates, nm::kScreen7At, nm::kScreen7InitFrame, 7,
     "Type12Screen7::InitBase", 0x82279118, __imp__sub_82279118, hooks::Type12Screen7InitBase},
    {"Type12Screen8::InitStates", 0x82262A88, __imp__sub_82262A88,
     hooks::Type12Screen8InitStates, nm::kScreen8At, nm::kScreen8InitFrame, 8,
     "Type12Screen8::InitBase", 0x8227A000, __imp__sub_8227A000, hooks::Type12Screen8InitBase},
    {"Type12Screen9::InitStates", 0x822646B0, __imp__sub_822646B0,
     hooks::Type12Screen9InitStates, nm::kScreen9At, nm::kScreen9InitFrame, 9,
     "Type12Screen9::InitBase", 0x8227A4F8, __imp__sub_8227A4F8, hooks::Type12Screen9InitBase},
    {"Type12Dialog10::InitStates", 0x82288600, __imp__sub_82288600,
     hooks::Type12Dialog10InitStates, nm::kDialog10At, nm::kDialog10InitFrame, 10,
     "Type12Dialog10::InitBase", 0x82279610, __imp__sub_82279610,
     hooks::Type12Dialog10InitBase},
    {"Type12Dialog11::InitStates", 0x82277B18, __imp__sub_82277B18,
     hooks::Type12Dialog11InitStates, nm::kDialog11At, nm::kDialog11InitFrame, 11,
     "Type12Dialog11::InitBase", 0x82279B08, __imp__sub_82279B08,
     hooks::Type12Dialog11InitBase},
    {"Type12Dialog12::InitStates", 0x8227F6C8, __imp__sub_8227F6C8,
     hooks::Type12Dialog12InitStates, nm::kDialog12At, nm::kDialog12InitFrame, 12,
     "Type12Dialog12::InitBase", 0x8227A9F0, __imp__sub_8227A9F0,
     hooks::Type12Dialog12InitBase},
    {"Type12Dialog13::InitStates", 0x8227EA50, __imp__sub_8227EA50,
     hooks::Type12Dialog13InitStates, nm::kDialog13At, nm::kDialog13InitFrame, 13,
     "Type12Dialog13::InitBase", 0x8227AEE8, __imp__sub_8227AEE8,
     hooks::Type12Dialog13InitBase},
};

// Another machine of the block than `m`'s, for the base Inits' r4 now and
// then (a machine-sized block of the world).
uint32_t OtherMachine(Random& r, const World& w, const Machine& m) {
  const Machine& other = kMachines[r.Below(std::size(kMachines))];
  return w.at(other.at == m.at ? nm::kModeSelectAt : other.at);
}

// The base Init's frame (below the InitStates' or the dispatcher's caller's
// r1 by `above`) without its register saves.
Range BaseFrame(int32_t above) {
  return {-above - int32_t(nm::kInitBaseFrame), -above - 48};
}

void Inits(Context& h, const World& w) {
  for (const Machine& m : kMachines) {
    std::vector<Range> stack = {{-int32_t(m.frame), -48}};
    if (m.index != 0) {
      stack.push_back(BaseFrame(int32_t(m.frame)));
    }
    Run(h, w, {fmt::format("{} (sub_{:08X})", m.name, m.address), 24, m.recompiled, m.native,
               stack, [&m](PPCContext& t, Random& r, const World& w) {
                 InitMachine(r, w.base, w.at(m.at));
                 t.r3.u64 = w.at(m.at);
               }});
  }
  for (const Machine& m : kMachines) {
    if (m.index == 0) {
      continue;
    }
    // r4 the machine (play) or, one case in four, another machine.
    Run(h, w, {fmt::format("{} (sub_{:08X})", m.base_name, m.base_address), 24,
               m.base_recompiled, m.base_native, {BaseFrame(0)},
               [&m](PPCContext& t, Random& r, const World& w) {
                 InitMachine(r, w.base, w.at(m.at));
                 t.r3.u64 = w.at(m.at);
                 t.r4.u64 = r.Below(4) == 0 ? OtherMachine(r, w, m) : w.at(m.at);
               }});
  }
  // NetScreen::InitBase for each index of play (6 to 13), r5 the machine
  // or, one case in four, another.
  for (const Machine& m : kMachines) {
    if (m.index == 0) {
      continue;
    }
    Run(h, w, {fmt::format("NetScreen::InitBase (sub_8227B3E0) index {}", m.index), 12,
               __imp__sub_8227B3E0, hooks::NetScreenInitBase, {BaseFrame(0)},
               [&m](PPCContext& t, Random& r, const World& w) {
                 InitMachine(r, w.base, w.at(m.at));
                 t.r3.u64 = w.at(m.at);
                 t.r4.u64 = m.index;
                 t.r5.u64 = r.Below(4) == 0 ? OtherMachine(r, w, m) : w.at(m.at);
               }});
  }
}

}  // namespace

// ModeType12::Init's cases, in src/harnesses/type67_modes.cpp (the stand's
// world: StandSaved, StandTake, StandRecorder).
void NetworkModeInits(Context& h);

void NetworkMode(Context& h) {
  const World w(h.base);
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: network_mode: no block from the allocation layer");
    return;
  }
  if (hooks::NativeHeapActive()) {
    Inits(h, w);
  }
  NetworkModeInits(h);
}

}  // namespace harness
