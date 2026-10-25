// The game modes and their state machines (src/game_modes/,
// docs/functions/game-modes.md) against the recompiled functions. Each case
// fills the scratch area and a block (a state machine is 9,608 bytes, more
// than the scratch area holds) with 0xCD, writes its inputs, runs the
// original (with --native_game_modes off, so its callees are the originals
// too) or the plain C++ through the register steps below, and compares the
// block, the scratch area and r3 (all 64 bits) exactly: nothing these
// functions write is computed as a float.

#include "harness.h"

#include "base/flags.h"
#include "game_modes/game_modes.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <string>

ITC_FLAG_DECLARE(bool, native_game_modes);

namespace harness {

namespace {

namespace gm = game_modes;

// The register steps: the plain C++ through these, not the adapters.
void NativeStateKeyEquals(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = gm::StateKeyEquals(base, ctx.r3.u32, ctx.r4.u32) ? 1 : 0;
}

void NativeStateMachineIndexOf(PPCContext& ctx, uint8_t* base) {
  ctx.r3.s64 = gm::StateMachineIndexOf(base, ctx.r3.u32, ctx.r4.u32);
}

void NativeTransitionMake(PPCContext& ctx, uint8_t* base) {
  gm::TransitionMake(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

void NativeTransitionAssign(PPCContext& ctx, uint8_t* base) {
  gm::TransitionAssign(base, ctx.r3.u32, ctx.r4.u32);
}

void NativeStateMachineConstruct(PPCContext& ctx, uint8_t* base) {
  gm::StateMachineConstruct(base, ctx.r3.u32);
}

void NativeModeConstruct(PPCContext& ctx, uint8_t* base) {
  gm::ModeConstruct(base, ctx.r3.u32);
}

// A block of guest memory for the cases, freed at the end.
struct Block {
  explicit Block(uint32_t bytes) : bytes(bytes), address(memory::Alloc(bytes, 0)) {}
  ~Block() { memory::Free(address); }
  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;
  uint32_t bytes;
  uint32_t address;
};

// One case through both versions: `setup(random)` writes the inputs (handed
// a copy of the same generator state both times), `args(t)` sets the
// registers.
template <typename Setup, typename Args>
void Case(Context& h, Results& results, const Block& block, Setup setup, Args args,
          GuestFunction recompiled, GuestFunction native, const std::string& what) {
  Outputs out[2];
  const Random start = h.random;
  Random after;
  for (int v = 0; v < 2; v++) {
    Random random = start;
    Fill(h.base, h.data, kScratchBytes);
    Fill(h.base, block.address, block.bytes);
    setup(random);
    after = random;
    PPCContext t = CallContext(h);
    args(t);
    ITC_FLAG(native_game_modes) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_game_modes) = true;
    out[v].Bytes(Read(h.base, block.address, block.bytes));
    out[v].Bytes(Read(h.base, h.data, kScratchBytes));
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  h.random = after;
  results.Check(out[0], out[1], what);
}

// A key word: mostly from a few values, so keys often match.
uint32_t KeyWord(Random& random) {
  static constexpr uint32_t kWords[] = {0, 1, 2, 5, 0xFFFFFFFF, 0x80000000};
  return random.Below(4) == 0 ? uint32_t(random.Next()) : kWords[random.Below(6)];
}

void WriteKey(uint8_t* base, uint32_t address, Random& random) {
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU32(base, address + 4 * i, KeyWord(random));
  }
}

void WriteRandom(uint8_t* base, uint32_t address, uint32_t words, Random& random) {
  for (uint32_t i = 0; i < words; i++) {
    guest::StoreU32(base, address + 4 * i, uint32_t(random.Next()));
  }
}

void Keys(Context& h, const Block& block) {
  Results results("StateKey::Equals (sub_82106040)");
  for (int c = 0; c < 64; c++) {
    Case(
        h, results, block,
        [&](Random& random) {
          WriteKey(h.base, h.data, random);
          // The second key: the first with one word changed, or random.
          for (uint32_t i = 0; i < 4; i++) {
            guest::StoreU32(h.base, h.data + 16 + 4 * i, guest::LoadU32(h.base, h.data + 4 * i));
          }
          if (random.Below(3) != 0) {
            guest::StoreU32(h.base, h.data + 16 + 4 * random.Below(4), KeyWord(random));
          }
        },
        [&](PPCContext& t) {
          t.r3.u64 = h.data;
          t.r4.u64 = h.data + 16;
        },
        __imp__sub_82106040, NativeStateKeyEquals, fmt::format("case {}", c));
  }
  results.Report();
}

void IndexOf(Context& h, const Block& block) {
  Results results("StateMachine::IndexOf (sub_821060A0)");
  for (int c = 0; c < 64; c++) {
    Case(
        h, results, block,
        [&](Random& random) {
          // 0 to 200 transitions (the edges among them), keys from a few
          // values; the key looked up taken from one of them or not.
          const uint32_t count = c == 0   ? 0
                                 : c == 1 ? gm::kTransitions
                                          : random.Below(gm::kTransitions + 1);
          for (uint32_t i = 0; i < count; i++) {
            WriteKey(h.base, block.address + gm::kTransitionBytes * i, random);
          }
          guest::StoreU32(h.base, block.address + gm::kTransitionCount, count);
          if (count != 0 && random.Below(3) != 0) {
            const uint32_t from = block.address + gm::kTransitionBytes * random.Below(count);
            for (uint32_t i = 0; i < 4; i++) {
              guest::StoreU32(h.base, h.data + 4 * i, guest::LoadU32(h.base, from + 4 * i));
            }
          } else {
            WriteKey(h.base, h.data, random);
          }
        },
        [&](PPCContext& t) {
          t.r3.u64 = block.address;
          t.r4.u64 = h.data;
        },
        __imp__sub_821060A0, NativeStateMachineIndexOf, fmt::format("case {}", c));
  }
  results.Report();
}

// Make and Assign: a destination and a source in the scratch area, apart
// or overlapping (the copies are word by word in the original's order).
void Copies(Context& h, const Block& block) {
  Results make("Transition::Make (sub_82106100)");
  Results assign("Transition::Assign (sub_82105FE0)");
  for (int c = 0; c < 64; c++) {
    const bool overlap = c % 4 == 3;
    const uint32_t destination = h.data + 256 + 4 * (c % 8);
    const uint32_t source =
        overlap ? destination + 4 * (int32_t(c % 16) - 8) : h.data + 1024 + 4 * (c % 5);
    const uint32_t key = h.data + 2048;
    auto setup = [&](Random& random) {
      WriteRandom(h.base, h.data, kScratchBytes / 4 - 256, random);
    };
    Case(
        h, make, block, setup,
        [&](PPCContext& t) {
          t.r3.u64 = destination;
          t.r4.u64 = key;
          t.r5.u64 = source;
        },
        __imp__sub_82106100, NativeTransitionMake,
        fmt::format("case {}{}", c, overlap ? " (overlapping)" : ""));
    Case(
        h, assign, block, setup,
        [&](PPCContext& t) {
          t.r3.u64 = destination;
          t.r4.u64 = source;
        },
        __imp__sub_82105FE0, NativeTransitionAssign,
        fmt::format("case {}{}", c, overlap ? " (overlapping)" : ""));
  }
  make.Report();
  assign.Report();
}

void Constructors(Context& h, const Block& block) {
  Results machine("StateMachine::StateMachine (sub_82107108)");
  for (int c = 0; c < 4; c++) {
    Case(
        h, machine, block,
        [&](Random& random) {
          if (c != 0) {
            WriteRandom(h.base, block.address, gm::kState / 4 + 1, random);
          }
        },
        [&](PPCContext& t) { t.r3.u64 = block.address; }, __imp__sub_82107108,
        NativeStateMachineConstruct, fmt::format("case {}", c));
  }
  machine.Report();
  Results mode("Mode::Mode (sub_82106C20)");
  for (int c = 0; c < 16; c++) {
    Case(
        h, mode, block,
        [&](Random& random) { WriteRandom(h.base, h.data, 128, random); },
        [&](PPCContext& t) { t.r3.u64 = h.data + 4 * c; }, __imp__sub_82106C20,
        NativeModeConstruct, fmt::format("case {}", c));
  }
  mode.Report();
}

}  // namespace

void GameModes(Context& h) {
  const Block block(gm::kState + 4 + 1024);
  Keys(h, block);
  IndexOf(h, block);
  Copies(h, block);
  Constructors(h, block);
}

}  // namespace harness
