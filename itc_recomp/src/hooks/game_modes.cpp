// The game modes and their state machines natively: adapters in front of
// src/game_modes/ (docs/functions/game-modes.md). Each leaves r3 as the
// original does. Functions reached only from native callers have no
// adapters (the flag switches their callers): StateKey::Equals
// (sub_82106040), from StateMachine::IndexOf; StateMachine::IndexOf
// (sub_821060A0), from StateMachine::Add and Find; StateMachine::Find
// (sub_821061F0), from StateMachine::Handle (its register step,
// hooks::GameModesFind, kept for the harness); Buffer28::~Buffer28
// (sub_82152D28), from sub_82106A00; Mode::Mode (sub_82106C20), from the
// modes' constructors; sub_82106A00, from
// RaceModeMember::~RaceModeMember; RaceModeMember::RaceModeMember
// (sub_82107168), from RaceMode::RaceMode; Member56896::Member56896
// (sub_82106A50), from RaceModeMember::RaceModeMember; the modes'
// constructors (sub_82106DB8 and the others, ModeTypeConstruct) and
// MachineMember::MachineMember (sub_82108160), from Mode::Create; Member13360::Member13360
// (sub_82155118), from RaceModeMember::RaceModeMember.

#include "guest.h"

#include "base/flags.h"
#include "game_modes/game_modes.h"
#include "hooks/game_modes.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"
#include "hooks/models.h"

ITC_FLAG_BOOL(native_game_modes, true, "ITC",
              "Run the game modes and their state machines (docs/functions/game-modes.md: "
              "StateMachine::IndexOf sub_821060A0 with StateKey::Equals sub_82106040 (no adapters: "
              "only Add and Find call IndexOf, and only IndexOf Equals), StateMachine::StateMachine sub_82107108, StateMachine::Add "
              "sub_82106158, StateMachine::Find sub_821061F0 (no adapter: only Handle calls it), "
              "StateMachine::Handle sub_82106280 (with Transition::Make sub_82106100 and Transition::Assign sub_82105FE0 merged into Add), "
              "Mode::Mode sub_82106C20 (no adapter: only the modes' constructors call it), the race overlays' state machine's handlers "
              "sub_8215DD58, sub_8215DD90, sub_8215DDD0 and sub_8215DE10, the last's virtual "
              "calls guest calls, RaceOverlaysMachine::Init sub_8215E6C8, the race mode's member's "
              "parts sub_82155D90 and sub_82155118 (no adapters: only RaceModeMember::RaceModeMember "
              "calls them), and with "
              "--native_heap Buffer28::Free sub_82152028, Reset sub_82152D38, ~Buffer28 "
              "sub_82152D28 (no adapter: only sub_82106A00 calls it) and sub_82106A00 (no adapter: "
              "only RaceModeMember::~RaceModeMember calls it), "
              "XprHolder::XprHolder sub_821067A0 (XPR::XPR's plain C++ called directly), and "
              "with --native_heap and the memory blocks' host lock "
              "RaceModeMember::~RaceModeMember sub_82106AF0 (XPR::~XPR's and sub_82106A00's "
              "plain C++ called directly), RaceMode::RaceMode sub_821073B8 with "
              "RaceModeMember::RaceModeMember sub_82107168 and Member56896::Member56896 "
              "sub_82106A50 (no adapters: only RaceMode::RaceMode calls the member's "
              "constructor, and only it sub_82106A50; XPR::XPR's and RaceOverlays::Clear's "
              "plain C++ called directly), and with --native_heap Mode::Create sub_821077A8 "
              "(operator new's plain C++ called directly; type 8, sub_82107670, never seen in "
              "play, the original's) with the modes' constructors sub_82106DB8, sub_82106C80 "
              "(with MachineMember::MachineMember sub_82108160), sub_82106E88, sub_82106F08, "
              "sub_82235E88, sub_82246340, sub_82107008, sub_82107088 and sub_82106F88 (no "
              "adapters: only Mode::Create calls them; sub_821077EC, a piece of its switch, "
              "merged into it)) "
              "natively instead of through the translated code (the same writes; checked by "
              "--run_harnesses=game_modes)");

// Defines an adapter: the body (the arguments after the address) uses `ctx`
// and `base`.
#define ITC_GAME_MODES(address, ...)     \
  extern "C" REX_FUNC(sub_##address) {   \
    REX_FUNC_PROLOGUE();                 \
    if (!ITC_FLAG(native_game_modes)) {  \
      __imp__sub_##address(ctx, base);   \
      return;                            \
    }                                    \
    __VA_ARGS__;                         \
  }

using namespace game_modes;

// StateMachine::StateMachine: r3 = the state machine (left).
ITC_GAME_MODES(82107108, StateMachineConstruct(base, ctx.r3.u32))


// StateMachine::Add: r3 = the state machine, r4 = the key, r5 = the functor
// part; r3 = the index, or the new transition (as the original computes it,
// from the machine's whole register).
ITC_GAME_MODES(82106158, hooks::GameModesAdd(ctx, base))

// StateMachine::Handle: r3 = the state machine, r4 = the message; r3 = the
// functor part.
ITC_GAME_MODES(82106280, hooks::GameModesHandle(ctx, base))

// The race overlays' state machine's handlers: r3 = the state machine.
ITC_GAME_MODES(8215DD58, OverlaysSetFlags(base, ctx.r3.u32))
ITC_GAME_MODES(8215DD90, OverlaysClearFlags(base, ctx.r3.u32))
ITC_GAME_MODES(8215DDD0, OverlaysSetFlag820(base, ctx.r3.u32))
ITC_GAME_MODES(8215DE10, hooks::GameModesResetOverlays(ctx, base))

namespace {

// The functor part's r3 as the original leaves it: a transition's is the
// machine's register plus the offset, the default's address sign-extended.
void SetPartR3(PPCContext& ctx, uint64_t machine_r3, uint32_t machine, uint32_t part,
               bool found) {
  if (found) {
    ctx.r3.u64 = machine_r3 + (part - machine);
  } else {
    ctx.r3.s64 = int32_t(part);
  }
}

}  // namespace

void hooks::GameModesAdd(PPCContext& ctx, uint8_t* base) {
  const uint64_t machine_r3 = ctx.r3.u64;
  const uint32_t machine = ctx.r3.u32;
  bool replaced = false;
  const uint32_t r3 = StateMachineAdd(base, machine, ctx.r4.u32, ctx.r5.u32, &replaced);
  ctx.r3.u64 = replaced ? r3 : machine_r3 + (r3 - machine);
}

void hooks::GameModesRaceOverlaysMachineInit(PPCContext& ctx, uint8_t* base) {
  const uint64_t machine_r3 = ctx.r3.u64;
  const uint32_t machine = ctx.r3.u32;
  bool replaced = false;
  const uint32_t r3 = RaceOverlaysMachineInit(
      base, machine, ctx.r1.u32 - kRaceOverlaysMachineInitFrame, &replaced);
  ctx.r3.u64 = replaced ? r3 : machine_r3 + (r3 - machine);
}

void hooks::GameModesFind(PPCContext& ctx, uint8_t* base) {
  const uint64_t machine_r3 = ctx.r3.u64;
  const uint32_t machine = ctx.r3.u32;
  bool found = false;
  const uint32_t part = StateMachineFind(base, machine, LoadStateKey(base, ctx.r4.u32), &found);
  SetPartR3(ctx, machine_r3, machine, part, found);
}

void hooks::GameModesHandle(PPCContext& ctx, uint8_t* base) {
  const uint64_t machine_r3 = ctx.r3.u64;
  const uint32_t machine = ctx.r3.u32;
  bool found = false;
  const uint32_t part = StateMachineHandle(base, machine, ctx.r4.u32, &found);
  SetPartR3(ctx, machine_r3, machine, part, found);
}


// RaceOverlaysMachine::Init: r3 = the race overlays' state machine; r3 as
// the last StateMachine::Add leaves it.
ITC_GAME_MODES(8215E6C8, hooks::GameModesRaceOverlaysMachineInit(ctx, base))

// The 28-byte buffers: r3 = the buffer; r3 as the originals leave it. Over the allocation layer only, and
// only when the blocks are its pages (else the originals, whose VirtualFree
// takes its other paths).
#define ITC_GAME_MODES_FREE(address, freeable, function)                    \
  extern "C" REX_FUNC(sub_##address) {                                     \
    REX_FUNC_PROLOGUE();                                                   \
    if (!ITC_FLAG(native_game_modes) || !hooks::NativeHeapActive() ||      \
        !freeable(base, ctx.r3.u32)) {                                     \
      __imp__sub_##address(ctx, base);                                     \
      return;                                                              \
    }                                                                      \
    ctx.r3.u64 = function(base, ctx.r3.u32);                               \
  }

ITC_GAME_MODES_FREE(82152028, Buffer28Freeable, Buffer28Free)
ITC_GAME_MODES_FREE(82152D38, Buffer28Freeable, Buffer28Reset)

// sub_8215DE10's work with its virtual calls (each overlay's slot 3) made
// from a frame like the original's (112 bytes, returning to 0x8215DE4C).
void hooks::GameModesResetOverlays(PPCContext& ctx, uint8_t* base) {
  OverlaysReset(base, ctx.r3.u32, [&](uint32_t overlay, uint32_t target) {
    guest_call::InFrame(ctx, base, 112, 0x8215DE4C, [&](uint32_t) {
      ctx.r3.u64 = overlay;
      ctx.ctr.u64 = target;
      REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
    });
  });
}

// XprHolder::XprHolder: r3 = the object (left).
ITC_GAME_MODES(821067A0, XprHolderConstruct(base, ctx.r3.u32))

// RaceModeMember::~RaceModeMember: r3 = the member; r3 as sub_82106A00 leaves
// it. Over the allocation layer and the memory blocks' host lock only (XPR::~XPR
// puts its blocks on the pending frees under that lock), and only when the
// plain C++'s checks pass (else the original, from the start).
extern "C" REX_FUNC(sub_82106AF0) {
  REX_FUNC_PROLOGUE();
  if (ITC_FLAG(native_game_modes) && hooks::NativeHeapActive() && hooks::ModelsHostLock()) {
    if (const auto r3 = RaceModeMemberDestruct(base, ctx.r3.u32)) {
      ctx.r3.u64 = *r3;
      return;
    }
  }
  __imp__sub_82106AF0(ctx, base);
}

// RaceMode::RaceMode: r3 = the race mode (left).
ITC_GAME_MODES(821073B8, RaceModeConstruct(base, ctx.r3.u32))

// Mode::Create: r3 = the holder (unused), r4 = the type; r3 = the mode or 0.
// Over the allocation layer only (operator new's plain C++), and the
// original from the start where the plain C++ says so.
extern "C" REX_FUNC(sub_821077A8) {
  REX_FUNC_PROLOGUE();
  if (ITC_FLAG(native_game_modes) && hooks::NativeHeapActive()) {
    if (const auto r3 = ModeCreate(base, ctx.r4.u32)) {
      ctx.r3.u64 = *r3;
      return;
    }
  }
  __imp__sub_821077A8(ctx, base);
}
