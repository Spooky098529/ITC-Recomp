// The boot mode (src/boot_mode/, docs/functions/boot-mode.md) against the
// recompiled functions. The originals run with --native_boot_mode off (their
// callees' own flags as they are: those were checked by their harnesses),
// the plain C++ through the register steps (hooks/boot_mode.h). Each case
// fills a world block, the mode and the stack below the call's r1 with the
// same random words for both sides; the movie class (0x826C1CDC) is a fake
// object there, its vtable's slots our player's real ones (on a fake object
// with no player: they only change its state words) or leaf recorders
// (sub_8245F860, +4 up; sub_820A7620, r4 to +12; sub_82090770, r3 = +60);
// the action mapper the skip test asks is a fake too; the game's globals the
// functions read and write are put back after each side. The functions copy
// words without float math, so everything is compared exactly. Frames are
// compared but for the back chains and the saved registers' slots (the plain
// C++ doesn't write those).

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "boot_mode/boot_mode.h"
#include "game_modes/game_modes.h"
#include "hooks/boot_mode.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <initializer_list>
#include <vector>

ITC_FLAG_DECLARE(bool, native_boot_mode);

namespace harness {

namespace {

namespace bm = boot_mode;

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

void Put(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

uint32_t Pick(Random& r, std::initializer_list<uint32_t> choices) {
  return *(choices.begin() + r.Below(uint32_t(choices.size())));
}

// The original with --native_boot_mode off.
template <GuestFunction recompiled>
void Original(PPCContext& ctx, uint8_t* base) {
  ITC_FLAG(native_boot_mode) = false;
  recompiled(ctx, base);
  ITC_FLAG(native_boot_mode) = true;
}

// The game's globals the functions touch, put back when a side ends: the
// boot movies' (0x826C5BA4-0x826C5BC8), the pad's blocked words, the movie
// class, the action mappers and the current player, the screen fades'
// (0x826A50AC-0x826A50E8) and the thread's last error.
class Globals {
 public:
  Globals(uint8_t* base, uint32_t last_error) : base_(base) {
    for (uint32_t a = 0x826C5BA4; a <= 0x826C5BC8; a += 4) addresses_.push_back(a);
    for (uint32_t a = 0x826BAB80; a < 0x826BABA0; a += 4) addresses_.push_back(a);
    for (uint32_t a = 0x826A50AC; a <= 0x826A50E8; a += 4) addresses_.push_back(a);
    for (uint32_t a : {bm::kPadBlocked0, bm::kPadBlocked1, bm::kMovieClass, bm::kCurrentPlayer,
                       last_error}) {
      addresses_.push_back(a);
    }
    for (uint32_t a : addresses_) saved_.push_back(guest::LoadU32(base_, a));
  }
  ~Globals() {
    for (size_t i = 0; i < addresses_.size(); i++) {
      guest::StoreU32(base_, addresses_[i], saved_[i]);
    }
  }
  // The words the functions may change, for the outputs.
  void Out(Outputs& out) const {
    for (uint32_t a : addresses_) out.Word(guest::LoadU32(base_, a));
  }

 private:
  uint8_t* base_;
  std::vector<uint32_t> addresses_;
  std::vector<uint32_t> saved_;
};

uint32_t LastErrorSlot(Context& h) {
  return guest::LoadU32(h.base, h.ctx.r13.u32 + 256) + 352;
}

// The world block's areas.
constexpr uint32_t kWorldBytes = 0x1000;
constexpr uint32_t kMovie = 0x000;          // the fake movie object (0x200 bytes)
constexpr uint32_t kMovieVtable = 0x200;    // its 8 slots
constexpr uint32_t kMapper = 0x300;         // the fake action mapper
constexpr uint32_t kMapperVtable = 0x380;   // its 12 slots
constexpr uint32_t kDelegate = 0x400;       // a mode delegate
constexpr uint32_t kModeVtableFake = 0x480; // a mode's 8 slots
constexpr uint32_t kAnswer = 0x500;         // an 8-byte answer
constexpr uint32_t kDispatcherArea = 0x600; // a dispatcher (its ring to +812)
constexpr uint32_t kWorldStack = 1024;      // the stack below the call's r1

constexpr uint32_t kAddRef = 0x8245F860, kStoreR4At12 = 0x820A7620, kGet60 = 0x82090770;
constexpr uint32_t kNoNextMode = 0x8222FC90;

// The world every case starts from: random words, the movie class at the
// fake object with each slot ours or a recorder, the action mappers' entries
// at the fake mapper (blocked or not), the boot globals small random values.
void World(Context& h, uint32_t world, Random& r) {
  uint8_t* base = h.base;
  WriteRandom(base, world, kWorldBytes, r);
  const uint32_t movie = world + kMovie, vtable = world + kMovieVtable;
  Put(base, movie, vtable);
  Put(base, movie + 4, r.Below(100));
  Put(base, movie + 24, Pick(r, {3, 3, 3, 1}));            // the open's status
  Put(base, movie + 60, Pick(r, {0, 1, 1, 2, 3, 4, 7}));   // kGet60's answer
  Put(base, movie + 308, Pick(r, {0, 1, 1, 2, 3, 9}));     // our player's state
  Put(base, vtable + 4, kStoreR4At12);
  Put(base, vtable + 8, Pick(r, {bm::kMovieStart, kAddRef}));
  Put(base, vtable + 12, Pick(r, {bm::kMovieStop, kAddRef}));
  Put(base, vtable + 16, Pick(r, {bm::kMoviePause, kAddRef}));
  Put(base, vtable + 20, Pick(r, {bm::kMovieResume, kAddRef}));
  Put(base, vtable + 24, Pick(r, {bm::kMovieGetState, kGet60, kGet60}));
  Put(base, vtable + 28, Pick(r, {bm::kMovieRelease, kAddRef}));
  Put(base, bm::kMovieClass, movie);
  const uint32_t mapper = world + kMapper;
  Put(base, mapper, world + kMapperVtable);
  Put(base, mapper + 60, Pick(r, {0, 1, 0x100, 0x7F}));
  Put(base, world + kMapperVtable + 40, Pick(r, {kGet60, kGet60, kStoreR4At12}));
  for (uint32_t i = 0; i < 4; i++) {
    Put(base, bm::kActionMappers + 8 * i, mapper);
    Put(base, bm::kActionMappers + 8 * i + 4, Pick(r, {0, 0, 1, 2}));
  }
  Put(base, bm::kCurrentPlayer, r.Below(4));
  Put(base, bm::kSkippable, Pick(r, {0, 1}));
  Put(base, bm::kPaused, Pick(r, {0, 1}));
  Put(base, bm::kModeAsked, Pick(r, {13, 13, 13, 2, 4, 12}));
  Put(base, bm::kMovieIndex, r.Below(7));
  for (uint32_t i = 0; i < 5; i++) {
    Put(base, bm::kMovieTable + 4 * i, Pick(r, {r.Below(11), 10, 11}));
  }
  Put(base, bm::kNextType, Pick(r, {4, r.Below(13)}));
  Put(base, bm::kPadBlocked0, Pick(r, {0, 0, 0, 1}));
  Put(base, bm::kPadBlocked1, Pick(r, {0, 0, 0, 1}));
  Put(base, LastErrorSlot(h), 0xCDCDCDCD);
}

// The world's outputs: the block, the globals.
void WorldOut(Context& h, uint32_t world, const Globals& globals, Outputs& out) {
  out.Bytes(Read(h.base, world, kWorldBytes));
  globals.Out(out);
}

// A stack range [from, to) as bytes.
std::vector<uint8_t> Stack(Context& h, uint32_t from, uint32_t to) {
  return Read(h.base, from, to - from);
}

// --- ScreenMachine::ScreenMachine on random machines ---

void ScreenMachines(Context& h, Random& random) {
  const Block machine(0x2600);
  if (!machine.address) return;
  Results results("ScreenMachine::ScreenMachine (sub_82107358)");
  for (int c = 0; c < 6; c++) {
    Outputs out[2];
    const uint64_t seed = random.Next();
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      WriteRandom(h.base, machine.address, machine.bytes, r);
      PPCContext t = CallContext(h);
      t.r3.u64 = machine.address;
      (v == 0 ? Original<__imp__sub_82107358> : hooks::ScreenMachineConstruct)(t, h.base);
      out[v].Bytes(Read(h.base, machine.address, machine.bytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// A machine at `machine`: empty (StateMachine::StateMachine, count 0) or
// random transitions with small keys, some of them the keys BootMachine::Init
// adds ((0, 0), (1, 0), (1, 1)); `count_room` keeps 3 free.
void RandomMachine(uint8_t* base, uint32_t machine, uint64_t seed, Random& r) {
  namespace gm = game_modes;
  if (seed % 2 == 0) {
    gm::StateMachineConstruct(base, machine);
    Put(base, machine + gm::kTransitionCount, 0);
    return;
  }
  const uint32_t count = 4 + r.Below(150);
  Put(base, machine + gm::kTransitionCount, count);
  for (uint32_t i = 0; i < 200; i++) {
    for (uint32_t k = 0; k < 4; k++) {
      Put(base, machine + 48 * i + 4 * k, r.Below(k < 2 ? 4 : 2));
    }
  }
  constexpr uint32_t kPlanted[][2] = {{0, 0}, {1, 0}, {1, 1}};
  for (const auto& key : kPlanted) {
    if (seed % 4 == 1 && key[1] == 1) {
      continue;  // the last Add makes a new transition (unless random keys match)
    }
    const uint32_t at = machine + 48 * r.Below(count);
    Put(base, at, key[0]);
    Put(base, at + 4, key[1]);
    Put(base, at + 8, 0);
    Put(base, at + 12, 0);
  }
}

// --- BootMachine::Init on a machine, BootMode::Init on a mode; the frames
// (the stack's words the transitions keep) random for both ---

void Inits(Context& h, Random& random) {
  const Block mode(bm::kMode3Bytes + 12);
  if (!mode.address) return;
  const uint32_t r1 = CallContext(h).r1.u32;
  const uint32_t pcr_slot = LastErrorSlot(h);
  {
    Results results("BootMachine::Init (sub_82107DD0)");
    const uint32_t frame = r1 - bm::kMachineInitFrame;
    for (uint64_t c = 0; c < 8; c++) {
      Outputs out[2];
      const uint64_t seed = random.Next();
      for (int v = 0; v < 2; v++) {
        Random r(seed);
        const uint32_t machine = mode.address + bm::kMachine;
        WriteRandom(h.base, mode.address, mode.bytes, r);
        WriteRandom(h.base, r1 - 512, 512, r);
        RandomMachine(h.base, machine, c, r);
        PPCContext t = CallContext(h);
        t.r3.u64 = machine;
        (v == 0 ? Original<__imp__sub_82107DD0> : hooks::BootMachineInit)(t, h.base);
        out[v].Bytes(Read(h.base, mode.address, mode.bytes));
        out[v].Bytes(Stack(h, frame + 4, r1 - 40));  // __savegprlr_28: -40 to -8
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("BootMode::Init (sub_82107F68)");
    const uint32_t frame = r1 - bm::kInitFrame;
    const uint32_t machine_frame = frame - bm::kMachineInitFrame;
    for (uint64_t c = 0; c < 8; c++) {
      Outputs out[2];
      const uint64_t seed = random.Next();
      for (int v = 0; v < 2; v++) {
        Random r(seed);
        Globals globals(h.base, pcr_slot);
        WriteRandom(h.base, mode.address, mode.bytes, r);
        WriteRandom(h.base, r1 - 1024, 1024, r);
        RandomMachine(h.base, mode.address + bm::kMachine, c, r);
        Put(h.base, mode.address + bm::kDispatcher + bm::kHandlerCount, r.Below(40));
        PPCContext t = CallContext(h);
        t.r3.u64 = mode.address;
        (v == 0 ? Original<__imp__sub_82107F68> : hooks::BootModeInit)(t, h.base);
        out[v].Bytes(Read(h.base, mode.address, mode.bytes));
        out[v].Bytes(Stack(h, machine_frame + 4, frame - 40));
        out[v].Bytes(Stack(h, frame + 4, r1 - 24));  // r30, r31 at -24, lr at -8
        out[v].Word(guest::LoadU32(h.base, bm::kModeAsked));
        out[v].Word(guest::LoadU32(h.base, bm::kPaused));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- The boot machine's handlers ---

void Handlers(Context& h, Random& random) {
  const Block world(kWorldBytes);
  const Block machine(0x2600);
  if (!world.address || !machine.address) return;
  const uint32_t r1 = CallContext(h).r1.u32;
  const uint32_t pcr_slot = LastErrorSlot(h);
  const uint32_t dispatcher = world.address + kDispatcherArea;
  {
    Results results("Boot::StartMovie (sub_82107B90)");
    for (int c = 0; c < 12; c++) {
      Outputs out[2];
      const uint64_t seed = random.Next();
      for (int v = 0; v < 2; v++) {
        Random r(seed);
        Globals globals(h.base, pcr_slot);
        World(h, world.address, r);
        Put(h.base, bm::kMovieIndex, r.Below(5));
        for (uint32_t i = 0; i < 5; i++) {
          Put(h.base, bm::kMovieTable + 4 * i, r.Below(11));  // the paths' table: 0 to 10
        }
        WriteRandom(h.base, r1 - kWorldStack, kWorldStack, r);
        PPCContext t = CallContext(h);
        t.r3.u64 = machine.address;
        t.r4.u64 = dispatcher;
        (v == 0 ? Original<__imp__sub_82107B90> : hooks::BootStartMovie)(t, h.base);
        WorldOut(h, world.address, globals, out[v]);
        out[v].Bytes(Stack(h, r1 - bm::kStartMovieFrame + 4, r1 - 16));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("Boot::EndMovie (sub_82107BF8)");
    for (int c = 0; c < 10; c++) {
      Outputs out[2];
      const uint64_t seed = random.Next();
      for (int v = 0; v < 2; v++) {
        Random r(seed);
        Globals globals(h.base, pcr_slot);
        World(h, world.address, r);
        WriteRandom(h.base, r1 - kWorldStack, kWorldStack, r);
        PPCContext t = CallContext(h);
        t.r3.u64 = machine.address;
        t.r4.u64 = dispatcher;
        (v == 0 ? Original<__imp__sub_82107BF8> : hooks::BootEndMovie)(t, h.base);
        WorldOut(h, world.address, globals, out[v]);
        out[v].Bytes(Stack(h, r1 - bm::kEndMovieFrame + 4, r1 - 8));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // The movie's states (ours and kGet60's: 0 to 3, and above, which reads
    // Movies::State's frame word), the started flag 0, 1 or 2, skippable or
    // not with the mapper blocked or not, the dispatcher's ring anywhere.
    Results results("Boot::WaitForMovie (sub_82107C58)");
    for (int c = 0; c < 40; c++) {
      Outputs out[2];
      const uint64_t seed = random.Next();
      for (int v = 0; v < 2; v++) {
        Random r(seed);
        Globals globals(h.base, pcr_slot);
        World(h, world.address, r);
        WriteRandom(h.base, machine.address, machine.bytes, r);
        Put(h.base, machine.address + bm::kMovieStarted, Pick(r, {0, 1, 1, 2}));
        Put(h.base, dispatcher + bm::kRing + bm::kRingCount, r.Below(10));
        Put(h.base, dispatcher + bm::kRing + bm::kRingStart, r.Below(200));
        WriteRandom(h.base, r1 - kWorldStack, kWorldStack, r);
        PPCContext t = CallContext(h);
        t.r3.u64 = machine.address;
        t.r4.u64 = dispatcher;
        if (v == 0) {
          Original<__imp__sub_82107C58>(t, h.base);
        } else {
          hooks::BootWaitForMovie(t, h.base, {});
        }
        WorldOut(h, world.address, globals, out[v]);
        out[v].Bytes(Read(h.base, machine.address + 9600, 16));
        out[v].Bytes(Stack(h, r1 - bm::kWaitForMovieFrame + 4, r1 - 32));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("Movies::State (sub_820A4450)");
    for (int c = 0; c < 12; c++) {
      Outputs out[2];
      const uint64_t seed = random.Next();
      for (int v = 0; v < 2; v++) {
        Random r(seed);
        Globals globals(h.base, pcr_slot);
        World(h, world.address, r);
        WriteRandom(h.base, r1 - kWorldStack, kWorldStack, r);
        PPCContext t = CallContext(h);
        if (v == 0) {
          Original<__imp__sub_820A4450>(t, h.base);
        } else {
          hooks::BootMoviesState(t, h.base, {});
        }
        WorldOut(h, world.address, globals, out[v]);
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- BootMode::NextMode, Mode::NextModeThunk and ModeDelegate::Invoke: the
// mode's dispatcher without handlers (GameMessages::Dispatch takes the
// queued messages and delivers none), its ring anywhere, the machine's state
// 0 to 3; the thunk's mode's slot 6 the boot mode's or Mode::NoNextMode, the
// delegate's function the thunk or Mode::NoNextMode ---

void NextModes(Context& h, Random& random) {
  const Block world(kWorldBytes);
  const Block mode(bm::kMode3Bytes + 12);
  if (!world.address || !mode.address) return;
  const uint32_t r1 = CallContext(h).r1.u32;
  const uint32_t pcr_slot = LastErrorSlot(h);
  const uint32_t answer = world.address + kAnswer;
  auto set_up = [&](Random& r) {
    World(h, world.address, r);
    WriteRandom(h.base, mode.address, mode.bytes, r);
    Put(h.base, mode.address, world.address + kModeVtableFake);
    Put(h.base, world.address + kModeVtableFake + 24, Pick(r, {bm::kNextMode, bm::kNextMode,
                                                              kNoNextMode}));
    Put(h.base, mode.address + bm::kDispatcher + bm::kHandlerCount, 0);
    Put(h.base, mode.address + 840, r.Below(4));
    Put(h.base, mode.address + 844, r.Below(200));
    Put(h.base, mode.address + bm::kMachineState, Pick(r, {0, 1, 2, 2, 2, 3}));
    const uint32_t delegate = world.address + kDelegate;
    const uint32_t adjustment = Pick(r, {0, 0, 0, 16});
    Put(h.base, delegate + 4, mode.address);
    Put(h.base, delegate + 8, adjustment ? kNoNextMode : Pick(r, {bm::kNextModeThunk,
                                                                  bm::kNextModeThunk,
                                                                  kNoNextMode}));
    Put(h.base, delegate + 12, adjustment);
    WriteRandom(h.base, r1 - kWorldStack, kWorldStack, r);
  };
  auto mode_out = [&](Outputs& out) {
    out.Bytes(Read(h.base, mode.address, 1024));
    out.Bytes(Read(h.base, mode.address + bm::kMachineState, 8));
  };
  {
    Results results("BootMode::NextMode (sub_82108018)");
    const uint32_t frame = r1 - bm::kNextModeFrame;
    for (int c = 0; c < 40; c++) {
      Outputs out[2];
      const uint64_t seed = random.Next();
      for (int v = 0; v < 2; v++) {
        Random r(seed);
        Globals globals(h.base, pcr_slot);
        set_up(r);
        PPCContext t = CallContext(h);
        t.r3.u64 = answer;
        t.r4.u64 = mode.address;
        (v == 0 ? Original<__imp__sub_82108018> : hooks::BootModeNextMode)(t, h.base);
        WorldOut(h, world.address, globals, out[v]);
        mode_out(out[v]);
        out[v].Bytes(Stack(h, frame + 4, r1 - 56));  // __savegprlr_26: -56 to -8
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("Mode::NextModeThunk (sub_82235C08)");
    const uint32_t frame = r1 - bm::kNextModeFrame;
    for (int c = 0; c < 12; c++) {
      Outputs out[2];
      const uint64_t seed = random.Next();
      for (int v = 0; v < 2; v++) {
        Random r(seed);
        Globals globals(h.base, pcr_slot);
        set_up(r);
        PPCContext t = CallContext(h);
        t.r3.u64 = answer;
        t.r4.u64 = mode.address;
        (v == 0 ? Original<__imp__sub_82235C08> : hooks::ModeNextModeThunk)(t, h.base);
        WorldOut(h, world.address, globals, out[v]);
        mode_out(out[v]);
        out[v].Bytes(Stack(h, frame + 4, r1 - 56));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("ModeDelegate::Invoke (sub_8223D440)");
    const uint32_t frame = r1 - bm::kInvokeFrame;
    const uint32_t next_frame = frame - bm::kNextModeFrame;
    for (int c = 0; c < 24; c++) {
      Outputs out[2];
      const uint64_t seed = random.Next();
      for (int v = 0; v < 2; v++) {
        Random r(seed);
        Globals globals(h.base, pcr_slot);
        set_up(r);
        PPCContext t = CallContext(h);
        t.r3.u64 = answer;
        t.r4.u64 = world.address + kDelegate;
        (v == 0 ? Original<__imp__sub_8223D440> : hooks::ModeDelegateInvoke)(t, h.base);
        WorldOut(h, world.address, globals, out[v]);
        mode_out(out[v]);
        out[v].Bytes(Stack(h, next_frame + 4, frame - 56));
        out[v].Bytes(Stack(h, frame + 4, r1 - 16));  // r31 at -16, lr at -8
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- The destructors: ~BootMode on a random block; the deleting destructors
// on a block from operator new (each side its own: the layer is first fit,
// so both get the same one), the flags' bit 0 set or not; freed when a new
// block of the size comes back at the same address ---

void Destructors(Context& h, Random& random) {
  const uint32_t r1 = CallContext(h).r1.u32;
  {
    const Block block(0x400);
    if (!block.address) return;
    Results results("BootMode::~BootMode (sub_82106D08)");
    for (int c = 0; c < 4; c++) {
      Outputs out[2];
      const uint64_t seed = random.Next();
      for (int v = 0; v < 2; v++) {
        Random r(seed);
        WriteRandom(h.base, block.address, block.bytes, r);
        PPCContext t = CallContext(h);
        t.r3.u64 = block.address;
        (v == 0 ? Original<__imp__sub_82106D08> : hooks::BootModeDestruct)(t, h.base);
        out[v].Bytes(Read(h.base, block.address, block.bytes));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  struct Kind {
    const char* name;
    GuestFunction original;
    GuestFunction step;
    uint32_t bytes;
  };
  const Kind kinds[] = {
      {"BootMode::DeletingDestructor (sub_82106D68)", Original<__imp__sub_82106D68>,
       hooks::BootModeDeletingDestructor, bm::kMode3Bytes},
      {"Mode::DeletingDestructor (sub_82106E38)", Original<__imp__sub_82106E38>,
       hooks::ModeDeletingDestructor, game_modes::kModeBytes},
  };
  for (const Kind& kind : kinds) {
    Results results(kind.name);
    for (int c = 0; c < 8; c++) {
      Outputs out[2];
      const uint64_t seed = random.Next();
      bool skipped = false;
      for (int v = 0; v < 2; v++) {
        Random r(seed);
        const auto block = memory::crt::OperatorNew(h.base, kind.bytes);
        if (!block) {
          skipped = true;
          break;
        }
        WriteRandom(h.base, *block, kind.bytes, r);
        WriteRandom(h.base, r1 - kWorldStack, kWorldStack, r);
        const uint32_t flags = Pick(r, {0, 1, 2, 3});
        PPCContext t = CallContext(h);
        t.r3.u64 = *block;
        t.r4.u64 = flags;
        (v == 0 ? kind.original : kind.step)(t, h.base);
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        if (flags & 1) {
          const auto probe = memory::crt::OperatorNew(h.base, kind.bytes);
          out[v].Word(probe && *probe == *block ? 1 : 0);
          if (probe) memory::crt::OperatorDelete(*probe);
        } else {
          out[v].Bytes(Read(h.base, *block, 64));
          memory::crt::OperatorDelete(*block);
        }
      }
      if (!skipped) {
        results.Check(out[0], out[1], fmt::format("case {}", c));
      }
    }
    results.Report();
  }
}

}  // namespace

void BootMode(Context& h) {
  if (!hooks::BootModeNative()) {
    ITC_LOG_INFO("harness: boot_mode: needs --native_boot_mode");
    return;
  }
  Random& random = h.random;
  ScreenMachines(h, random);
  Inits(h, random);
  Handlers(h, random);
  NextModes(h, random);
  Destructors(h, random);
}

}  // namespace harness
