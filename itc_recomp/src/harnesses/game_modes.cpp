// The game modes and their state machines (src/game_modes/,
// docs/functions/game-modes.md) against the recompiled functions. Each case
// fills the scratch area and a block (a state machine is 9,608 bytes, more
// than the scratch area holds) with 0xCD, writes its inputs, runs the
// original (with --native_game_modes off, so its callees are the originals
// too) or the plain C++ through the register steps below, and compares the
// block, the scratch area and r3 (all 64 bits) exactly: nothing these
// functions write is computed as a float.

#include "harness.h"
#include "pending_frees.h"

#include "base/flags.h"
#include "base/log.h"
#include "game_modes/game_modes.h"
#include "hooks/game_modes.h"
#include "hooks/heap.h"
#include "hooks/models.h"
#include "memory/heap.h"
#include "models/memory_blocks.h"

#include <fmt/format.h>

#include <mutex>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_game_modes);
ITC_FLAG_DECLARE(bool, native_models);

namespace harness {

namespace {

namespace gm = game_modes;

// The register steps: the plain C++ through these, not the adapters.
void NativeStateKeyEquals(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = gm::StateKeyEquals(base, ctx.r3.u32, gm::LoadStateKey(base, ctx.r4.u32)) ? 1 : 0;
}

void NativeStateMachineIndexOf(PPCContext& ctx, uint8_t* base) {
  ctx.r3.s64 = gm::StateMachineIndexOf(base, ctx.r3.u32, gm::LoadStateKey(base, ctx.r4.u32));
}

void NativeStateMachineConstruct(PPCContext& ctx, uint8_t* base) {
  gm::StateMachineConstruct(base, ctx.r3.u32);
}

void NativeModeConstruct(PPCContext& ctx, uint8_t* base) {
  gm::ModeConstruct(base, ctx.r3.u32);
}

// Add, Find and Handle: their r3 steps are hooks' (the whole registers).
// Transition::Make and Assign (sub_82106100, sub_82105FE0) are merged into
// StateMachine::Add, whose cases cover them.
constexpr GuestFunction NativeStateMachineAdd = hooks::GameModesAdd;
constexpr GuestFunction NativeStateMachineFind = hooks::GameModesFind;
constexpr GuestFunction NativeStateMachineHandle = hooks::GameModesHandle;

void NativeOverlaysSetFlags(PPCContext& ctx, uint8_t* base) {
  gm::OverlaysSetFlags(base, ctx.r3.u32);
}

void NativeOverlaysClearFlags(PPCContext& ctx, uint8_t* base) {
  gm::OverlaysClearFlags(base, ctx.r3.u32);
}

void NativeOverlaysSetFlag820(PPCContext& ctx, uint8_t* base) {
  gm::OverlaysSetFlag820(base, ctx.r3.u32);
}

void NativeMember67760Construct(PPCContext& ctx, uint8_t* base) {
  gm::Member67760Construct(base, ctx.r3.u32);
}

void NativeMember13360Construct(PPCContext& ctx, uint8_t* base) {
  gm::Member13360Construct(base, ctx.r3.u32);
}

void NativeXprHolderConstruct(PPCContext& ctx, uint8_t* base) {
  gm::XprHolderConstruct(base, ctx.r3.u32);
}

void NativeRaceModeMemberDestruct(PPCContext& ctx, uint8_t* base) {
  if (const auto r3 = gm::RaceModeMemberDestruct(base, ctx.r3.u32)) {
    ctx.r3.u64 = *r3;
  } else {
    __imp__sub_82106AF0(ctx, base);
  }
}

// StateMachine::Find's static default and its guard (put back afterwards).
constexpr uint32_t kDefault = 0x826C5B68;
constexpr uint32_t kDefaultBytes = 0x24;

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
    ITC_FLAG(native_models) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_game_modes) = true;
    ITC_FLAG(native_models) = true;
    out[v].Bytes(Read(h.base, block.address, block.bytes));
    out[v].Bytes(Read(h.base, h.data, kScratchBytes));
    out[v].Bytes(Read(h.base, kDefault, kDefaultBytes));
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

// 0 to 200 transitions with keys from a few values (so they repeat), the
// state from them too; returns the count.
uint32_t WriteMachine(Context& h, const Block& block, Random& random, int c) {
  const uint32_t count = c == 0 ? 0 : c == 1 ? gm::kTransitions : random.Below(gm::kTransitions + 1);
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t transition = block.address + gm::kTransitionBytes * i;
    WriteKey(h.base, transition, random);
    WriteRandom(h.base, transition + 16, 8, random);
    if (random.Below(3) == 0) {
      guest::StoreU32(h.base, transition + 44, 0);
    }
  }
  guest::StoreU32(h.base, block.address + gm::kTransitionCount, count);
  guest::StoreU32(h.base, block.address + gm::kState, KeyWord(random));
  return count;
}

// A key at `address`: one of the machine's (with its state) or random.
void WriteLookup(Context& h, const Block& block, Random& random, uint32_t count,
                 uint32_t address, uint32_t words) {
  if (count != 0 && random.Below(3) != 0) {
    const uint32_t from = block.address + gm::kTransitionBytes * random.Below(count);
    if (words == 3) {
      // A message: the key's last three words; the state set to the first.
      guest::StoreU32(h.base, block.address + gm::kState, guest::LoadU32(h.base, from));
      for (uint32_t i = 0; i < 3; i++) {
        guest::StoreU32(h.base, address + 4 * i, guest::LoadU32(h.base, from + 4 + 4 * i));
      }
    } else {
      for (uint32_t i = 0; i < 4; i++) {
        guest::StoreU32(h.base, address + 4 * i, guest::LoadU32(h.base, from + 4 * i));
      }
    }
  } else {
    for (uint32_t i = 0; i < words; i++) {
      guest::StoreU32(h.base, address + 4 * i, KeyWord(random));
    }
  }
}

// The default's guard: set or not (the first call), the default random.
void WriteDefault(Context& h, Random& random) {
  WriteRandom(h.base, kDefault, kDefaultBytes / 4, random);
  if (random.Below(2) == 0) {
    guest::StoreU32(h.base, kDefault + 0x20, guest::LoadU32(h.base, kDefault + 0x20) & ~1u);
  }
}

void Lookups(Context& h, const Block& block) {
  Results add("StateMachine::Add (sub_82106158)");
  Results find("StateMachine::Find (sub_821061F0)");
  Results handle("StateMachine::Handle (sub_82106280)");
  for (int c = 0; c < 100; c++) {
    const uint32_t key = h.data + 64;
    const uint32_t functor = h.data + 128;
    Case(
        h, add, block,
        [&](Random& random) {
          const uint32_t count = WriteMachine(h, block, random, c);
          WriteLookup(h, block, random, count, key, 4);
          WriteRandom(h.base, functor, 8, random);
        },
        [&](PPCContext& t) {
          t.r3.u64 = block.address;
          t.r4.u64 = key;
          t.r5.u64 = functor;
        },
        __imp__sub_82106158, NativeStateMachineAdd, fmt::format("case {}", c));
    Case(
        h, find, block,
        [&](Random& random) {
          WriteDefault(h, random);
          const uint32_t count = WriteMachine(h, block, random, c);
          WriteLookup(h, block, random, count, key, 4);
        },
        [&](PPCContext& t) {
          t.r3.u64 = block.address;
          t.r4.u64 = key;
        },
        __imp__sub_821061F0, NativeStateMachineFind, fmt::format("case {}", c));
    Case(
        h, handle, block,
        [&](Random& random) {
          WriteDefault(h, random);
          const uint32_t count = WriteMachine(h, block, random, c);
          WriteLookup(h, block, random, count, key, 3);
        },
        [&](PPCContext& t) {
          t.r3.u64 = block.address;
          t.r4.u64 = key;
        },
        __imp__sub_82106280, NativeStateMachineHandle, fmt::format("case {}", c));
  }
  add.Report();
  find.Report();
  handle.Report();
}

// The race overlays' state machine's handlers: the 25 slots after a state
// machine, each empty or an overlay (832 bytes, after the machine), the
// overlays told apart; their vtable's slot 3 (for sub_8215DE10) is Mode::Mode
// (sub_82106C20), a leaf writing the overlay's first words.
constexpr uint32_t kOverlayArea = 10240;
constexpr uint32_t kOverlayBytes = 832;
constexpr uint32_t kSlot3Target = 0x82106C20;

void Handlers(Context& h, const Block& block) {
  struct Handler {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
  };
  const Handler handlers[] = {
      {"sub_8215DD58 RaceOverlaysMachine::SetFlags", __imp__sub_8215DD58, NativeOverlaysSetFlags},
      {"sub_8215DD90 RaceOverlaysMachine::ClearFlags", __imp__sub_8215DD90,
       NativeOverlaysClearFlags},
      {"sub_8215DDD0 RaceOverlaysMachine::SetFlag820", __imp__sub_8215DDD0,
       NativeOverlaysSetFlag820},
      {"sub_8215DE10 RaceOverlaysMachine::ResetOverlays", __imp__sub_8215DE10,
       hooks::GameModesResetOverlays},
  };
  const uint32_t vtable = block.address + kOverlayArea - 64;
  for (const Handler& handler : handlers) {
    Results results(handler.name);
    for (int c = 0; c < 50; c++) {
      Case(
          h, results, block,
          [&](Random& random) {
            guest::StoreU32(h.base, vtable + 12, kSlot3Target);
            for (uint32_t i = 0; i < gm::kOverlaySlots; i++) {
              const uint32_t overlay = block.address + kOverlayArea + kOverlayBytes * i;
              WriteRandom(h.base, overlay + 816, 4, random);
              guest::StoreU32(h.base, overlay, vtable);
              const bool empty = c == 0 || (c != 1 && random.Below(3) == 0);
              guest::StoreU32(h.base, block.address + gm::kOverlays + 4 * i, empty ? 0 : overlay);
            }
          },
          [&](PPCContext& t) { t.r3.u64 = block.address; }, handler.recompiled, handler.native,
          fmt::format("case {}", c));
    }
    results.Report();
  }
}

void Member67760(Context& h, const Block& block) {
  Results results("sub_82155D90 Member67760::Member67760");
  for (int c = 0; c < 16; c++) {
    Case(
        h, results, block,
        [&](Random& random) { WriteRandom(h.base, block.address, 800 / 4, random); },
        [&](PPCContext& t) { t.r3.u64 = block.address + 4 * c; }, __imp__sub_82155D90,
        NativeMember67760Construct, fmt::format("case {}", c));
  }
  results.Report();
}

void Member13360(Context& h) {
  Results results("sub_82155118 Member13360::Member13360");
  const Block block(44 * 1024);
  for (int c = 0; c < 8; c++) {
    Case(
        h, results, block,
        [&](Random& random) { WriteRandom(h.base, block.address, block.bytes / 4, random); },
        [&](PPCContext& t) { t.r3.u64 = block.address + 4 * c; }, __imp__sub_82155118,
        NativeMember13360Construct, fmt::format("case {}", c));
  }
  results.Report();
}

// RaceOverlaysMachine::Init: an empty state machine (as RaceMode::RaceMode
// leaves it) or one with transitions already, and the original's frame
// below the stack pointer filled with the same random words for both (the
// words it never writes go into the transitions); the machine, the frame
// (but its back chain and the saved registers) and r3 compared.
void RaceOverlaysInit(Context& h, const Block& block) {
  Results results("RaceOverlaysMachine::Init (sub_8215E6C8)");
  const uint32_t stack = h.data - 1024;  // CallContext's r1
  const uint32_t frame = stack - gm::kRaceOverlaysMachineInitFrame;
  for (int c = 0; c < 20; c++) {
    Outputs out[2];
    const Random start = h.random;
    Random after;
    for (int v = 0; v < 2; v++) {
      Random random = start;
      Fill(h.base, h.data, kScratchBytes);
      Fill(h.base, block.address, block.bytes);
      WriteRandom(h.base, frame, gm::kRaceOverlaysMachineInitFrame / 4, random);
      if (c % 2 == 0) {
        gm::StateMachineConstruct(h.base, block.address);
        guest::StoreU32(h.base, block.address + gm::kTransitionCount, 0);
        guest::StoreU32(h.base, block.address + gm::kState, 0);
      } else {
        WriteMachine(h, block, random, c);
        // Fewer, so the 33 fit below the slots.
        guest::StoreU32(h.base, block.address + gm::kTransitionCount,
                        guest::LoadU32(h.base, block.address + gm::kTransitionCount) % 150);
      }
      after = random;
      PPCContext t = CallContext(h);
      t.r3.u64 = block.address;
      ITC_FLAG(native_game_modes) = v == 1;
      (v == 0 ? GuestFunction(__imp__sub_8215E6C8) : hooks::GameModesRaceOverlaysMachineInit)(
          t, h.base);
      ITC_FLAG(native_game_modes) = true;
      out[v].Bytes(Read(h.base, block.address, gm::kOverlays + 4 * gm::kOverlaySlots));
      out[v].Bytes(Read(h.base, frame + 80, gm::kRaceOverlaysMachineInitFrame - 80 - 64));
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    h.random = after;
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// The 28-byte buffers: each version gets its own blocks (VirtualAlloc's
// pages, or none), made in the same order with the same sizes; the buffers'
// bytes (their blocks' addresses written as the block's number, so the two
// versions' addresses don't matter), whether each block is still the
// layer's afterwards, and r3 are compared.
void Buffers(Context& h, const Block& block) {
  struct Function {
    const char* name;
    GuestFunction recompiled;
    uint32_t (*native)(uint8_t*, uint32_t);
    uint32_t object_offset;  // r3 below the first buffer
  };
  const Function functions[] = {
      {"Buffer28::Free (sub_82152028)", __imp__sub_82152028, gm::Buffer28Free, 0},
      {"Buffer28::Reset (sub_82152D38)", __imp__sub_82152D38, gm::Buffer28Reset, 0},
      {"Buffer28::~Buffer28 (sub_82152D28)", __imp__sub_82152D28, gm::Buffer28Destroy, 0},
      {"sub_82106A00 Buffers28::~Buffers28", __imp__sub_82106A00, gm::Buffers28Destroy,
       gm::kBuffers28},
  };
  if (!hooks::NativeHeapActive()) {
    return;  // the adapters' condition
  }
  const uint32_t object = h.data + 64;  // the buffers at +0 (or r3 + 43016)
  for (const Function& f : functions) {
    Results results(f.name);
    for (int c = 0; c < 40; c++) {
      Outputs out[2];
      const Random start = h.random;
      for (int v = 0; v < 2; v++) {
        Random random = start;
        Fill(h.base, h.data, kScratchBytes);
        uint32_t blocks[4] = {};
        for (uint32_t i = 0; i < 4; i++) {
          const uint32_t buffer = object + gm::kBuffer28Bytes * i;
          WriteRandom(h.base, buffer, gm::kBuffer28Bytes / 4, random);
          const uint32_t size = 4096 * (1 + random.Below(4));
          if (c != 0 && random.Below(4) != 0) {
            blocks[i] = memory::AllocPages(size, 4096);
          }
          guest::StoreU32(h.base, buffer + gm::kBuffer28Block, blocks[i]);
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = object - f.object_offset;
        ITC_FLAG(native_game_modes) = false;
        if (v == 0) {
          f.recompiled(t, h.base);
        } else {
          t.r3.u64 = f.native(h.base, t.r3.u32);
        }
        ITC_FLAG(native_game_modes) = true;
        for (uint32_t i = 0; i < 4; i++) {
          const uint32_t buffer = object + gm::kBuffer28Bytes * i;
          const uint32_t at = guest::LoadU32(h.base, buffer + gm::kBuffer28Block);
          uint32_t number = at;
          for (uint32_t b = 0; b < 4; b++) {
            if (at != 0 && at == blocks[b]) {
              number = 0xB0000000 | b;
            }
          }
          guest::StoreU32(h.base, buffer + gm::kBuffer28Block, number);
          out[v].Word(blocks[i] != 0 && memory::IsPages(blocks[i]));
          if (blocks[i] != 0 && memory::IsPages(blocks[i])) {
            memory::FreePages(blocks[i]);
          }
        }
        out[v].Bytes(Read(h.base, h.data, kScratchBytes));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
}

void XprHolders(Context& h, const Block& block) {
  Results results("sub_821067A0 XprHolder::XprHolder");
  for (int c = 0; c < 16; c++) {
    Case(
        h, results, block,
        [&](Random& random) { WriteRandom(h.base, block.address, 128 / 4, random); },
        [&](PPCContext& t) { t.r3.u64 = block.address + 4 * c; }, __imp__sub_821067A0,
        NativeXprHolderConstruct, fmt::format("case {}", c));
  }
  results.Report();
}

// RaceModeMember::~RaceModeMember: the member's words random, its XPR's
// header and data blocks none, untagged or tagged (on the game's pending
// frees, held under the memory blocks' lock and put back after each case,
// as the models harness does), and its four buffers' blocks VirtualAlloc's
// pages or none (each side its own, numbered as in Buffers). The member's
// bytes, the nodes added, whether each buffer's block is still the layer's,
// and r3 compared.
void RaceModeMembers(Context& h) {
  Results results("sub_82106AF0 RaceModeMember::~RaceModeMember (on the game's list)");
  if (!hooks::NativeHeapActive() || !hooks::ModelsHostLock()) {
    ITC_LOG_WARN("harness: game_modes: --native_heap or the memory blocks' host lock is off: "
                 "RaceModeMember::~RaceModeMember's cases skipped (the adapter's condition)");
    return;
  }
  constexpr uint32_t kMemberBytes = 67744;
  constexpr uint32_t kBuffers = 13360 + gm::kBuffers28;
  const Block block(kMemberBytes + 1024);
  std::lock_guard<std::recursive_mutex> lock(models::MemoryBlocksMutex());
  for (int c = 0; c < 32; c++) {
    const uint32_t header_kind = c < 9 ? uint32_t(c) % 3 : h.random.Below(3);
    const uint32_t data_kind = c < 9 ? uint32_t(c) / 3 : h.random.Below(3);
    const uint32_t member = block.address + 4 * h.random.Below(4);
    const uint32_t first = block.address + kMemberBytes + 64 + 16 * h.random.Below(4);
    const uint32_t second = block.address + kMemberBytes + 256 + 16 * h.random.Below(4);
    uint32_t words[2][3];
    for (auto& set : words) {
      for (uint32_t& word : set) {
        word = uint32_t(h.random.Next());
      }
    }
    Outputs out[2];
    const Random start = h.random;
    Random after;
    for (int v = 0; v < 2; v++) {
      Random random = start;
      Fill(h.base, h.data, kScratchBytes);
      Fill(h.base, block.address, block.bytes);
      WriteRandom(h.base, member, 64 / 4, random);
      WriteRandom(h.base, member + gm::kRaceModeMemberXprHolder, 64 / 4, random);
      WriteRandom(h.base, member + kBuffers, 4 * gm::kBuffer28Bytes / 4, random);
      const uint32_t xpr = member + gm::kRaceModeMemberXprHolder + 4;
      guest::StoreU32(h.base, xpr + 0,
                      pending_frees::WriteBlock(h.base, header_kind, first, words[0]));
      guest::StoreU32(h.base, xpr + 8,
                      pending_frees::WriteBlock(h.base, data_kind, second, words[1]));
      uint32_t blocks[4] = {};
      for (uint32_t i = 0; i < 4; i++) {
        const uint32_t size = 4096 * (1 + random.Below(4));
        if (c != 0 && random.Below(4) != 0) {
          blocks[i] = memory::AllocPages(size, 4096);
        }
        guest::StoreU32(h.base, member + kBuffers + gm::kBuffer28Bytes * i + gm::kBuffer28Block,
                        blocks[i]);
      }
      after = random;
      const pending_frees::ListState was = pending_frees::SaveList(h.base);
      PPCContext t = CallContext(h);
      t.r3.u64 = member;
      ITC_FLAG(native_game_modes) = v == 1;
      ITC_FLAG(native_models) = v == 1;
      (v == 0 ? GuestFunction(__imp__sub_82106AF0) : NativeRaceModeMemberDestruct)(t, h.base);
      ITC_FLAG(native_game_modes) = true;
      ITC_FLAG(native_models) = true;
      pending_frees::TakeAddedNodes(h.base, was, out[v]);
      for (uint32_t i = 0; i < 4; i++) {
        const uint32_t at = member + kBuffers + gm::kBuffer28Bytes * i + gm::kBuffer28Block;
        const uint32_t now = guest::LoadU32(h.base, at);
        for (uint32_t b = 0; b < 4; b++) {
          if (now != 0 && now == blocks[b]) {
            guest::StoreU32(h.base, at, 0xB0000000 | b);
          }
        }
        const bool kept = blocks[i] != 0 && memory::IsPages(blocks[i]);
        out[v].Word(kept);
        if (kept) {
          memory::FreePages(blocks[i]);
        }
      }
      out[v].Bytes(Read(h.base, block.address, block.bytes));
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    h.random = after;
    results.Check(out[0], out[1],
                  fmt::format("case {} (header {}, data {})", c, header_kind, data_kind));
  }
  results.Report();
}

}  // namespace

void GameModes(Context& h) {
  const std::vector<uint8_t> saved_default = Read(h.base, kDefault, kDefaultBytes);
  {
    const Block block(kOverlayArea + kOverlayBytes * gm::kOverlaySlots);
    Keys(h, block);
    IndexOf(h, block);
    Constructors(h, block);
    Lookups(h, block);
    Handlers(h, block);
    Member67760(h, block);
    RaceOverlaysInit(h, block);
    Buffers(h, block);
    XprHolders(h, block);
  }
  Member13360(h);
  RaceModeMembers(h);
  Write(h.base, kDefault, saved_default.data(), kDefaultBytes);
}

}  // namespace harness
