// The game modes and their state machines as plain C++ (B7 in
// docs/rexglue-removal.md; the functions are in
// docs/functions/game-modes.md, the layouts in docs/known-data.md: "game
// mode", "race mode", "state machine", "transition"). The modes, their state
// machines and the race overlays' list are guest data shared with the
// recompiled code, so these work on guest memory, with the originals' loads
// and stores in their order. Adapters: src/hooks/game_modes.cpp.

#pragma once

#include <cstdint>
#include <functional>
#include <optional>

namespace game_modes {

// --- The state machines (StateMachine::, Transition::, StateKey::) ---

// A state machine: up to 200 48-byte transitions from +0, their count at
// +9600 and the state at +9604.
constexpr uint32_t kTransitionBytes = 48;
constexpr uint32_t kTransitions = 200;
constexpr uint32_t kTransitionCount = 9600;
constexpr uint32_t kState = 9604;
// The empty functor's vtable (a transition with nothing to run).
constexpr uint32_t kNoFunctor = 0x820069BC;

// A transition's key: (state, the message's three words). Read once: the
// lookups only read, so a key in guest memory is the same throughout.
struct StateKey {
  uint32_t words[4];
};
StateKey LoadStateKey(uint8_t* base, uint32_t address);

// sub_82106040 StateKey::Equals: whether the 16-byte key at `a` and `b` have
// the same four words.
bool StateKeyEquals(uint8_t* base, uint32_t a, const StateKey& b);
// sub_821060A0 StateMachine::IndexOf: the first transition whose key matches,
// or -1.
int32_t StateMachineIndexOf(uint8_t* base, uint32_t machine, const StateKey& key);
// sub_82106158 StateMachine::Add: the key's transition given the functor
// part's functor and next state, or a new transition made (no bound check,
// as the original); the index (`replaced` true) or the new transition.
uint32_t StateMachineAdd(uint8_t* base, uint32_t machine, uint32_t key, uint32_t functor,
                         bool* replaced);
// sub_821061F0 StateMachine::Find: the key's transition's functor part (+16),
// or the static default (made on the first call; `found` false). The
// original's r3 for the default is the address sign-extended, and for a
// transition the machine's r3 plus the offset (the adapters' r3).
uint32_t StateMachineFind(uint8_t* base, uint32_t machine, const StateKey& key, bool* found);
constexpr uint32_t kDefaultTransition = 0x826C5B68;
// sub_82106280 StateMachine::Handle: the functor part for the current state
// and the 12-byte message at `message`; the state moved to the transition's
// next one when that isn't 0.
uint32_t StateMachineHandle(uint8_t* base, uint32_t machine, uint32_t message, bool* found);
// sub_82107108 StateMachine::StateMachine: the 200 transitions emptied.
void StateMachineConstruct(uint8_t* base, uint32_t machine);

// --- The modes (Mode::) ---

// sub_82106C20 Mode::Mode: the base's vtables and default delegate.
void ModeConstruct(uint8_t* base, uint32_t mode);

// --- The race overlays' state machine's message handlers ---

// The race overlays' list in their state machine: 25 slots from +9608.
constexpr uint32_t kOverlays = 9608;
constexpr uint32_t kOverlaySlots = 25;

// sub_8215DD58: each overlay's +820 and +824 = 1.
void OverlaysSetFlags(uint8_t* base, uint32_t machine);
// sub_8215DD90: each overlay's +820, +824 and +828 = 0.
void OverlaysClearFlags(uint8_t* base, uint32_t machine);
// sub_8215DDD0: each overlay's +820 = 1, +824 = 0.
void OverlaysSetFlag820(uint8_t* base, uint32_t machine);
// sub_8215DE10: each overlay's slot 3 (`reset(overlay, target)`, a guest
// call: CarOverlays::Reset in races), then its +828 = 1.
void OverlaysReset(uint8_t* base, uint32_t machine,
                   const std::function<void(uint32_t overlay, uint32_t target)>& reset);

// sub_8215E6C8 RaceOverlaysMachine::Init: the race overlays' state
// machine's state = 0 and its 33 transitions added, built in the original's
// stack frame at `frame` (its r1 less 2,272: see race_overlays_machine.cpp);
// returns the last StateMachine::Add's answer.
uint32_t RaceOverlaysMachineInit(uint8_t* base, uint32_t machine, uint32_t frame,
                                 bool* replaced);
constexpr uint32_t kRaceOverlaysMachineInitFrame = 2272;

// --- The race mode's member at +10512's parts (in the camera modes' range,
// 82152028-82156CC0) ---

// sub_82155D90: the object at the member's +67760 set up (its vtable, 1.0s
// in its w words, 16 linked halfword pairs).
void Member67760Construct(uint8_t* base, uint32_t object);

// sub_82155118: the object at the member's +13360 set up (its vtable, 1.0s,
// four 28-byte buffers at +43016 made and reset, an object at +43136);
// returns it.
void Member13360Construct(uint8_t* base, uint32_t object);

// A 28-byte buffer (vtable 0x82009390) holding a block at +20 from
// VirtualAlloc. Freeing goes over the allocation layer only (the callers'
// adapters check hooks::NativeHeapActive): Buffer28Freeable says whether the
// block is the layer's pages (or none), else the originals run.
constexpr uint32_t kBuffer28Block = 20;
bool Buffer28Freeable(uint8_t* base, uint32_t buffer);
// sub_82152028 Buffer28::Free: the block freed (VirtualFree, MEM_RELEASE)
// and +20 = 0; returns r3 as the original leaves it (0 without a block, else
// whether it was freed).
uint32_t Buffer28Free(uint8_t* base, uint32_t buffer);
// sub_82152D38 Buffer28::Reset: freed, then +12 and +16 = -1, +20 and +24 =
// 0; r3 as Free's.
uint32_t Buffer28Reset(uint8_t* base, uint32_t buffer);
// sub_82152D28 Buffer28::~Buffer28: the vtable put back, then freed; r3 as
// Free's.
uint32_t Buffer28Destroy(uint8_t* base, uint32_t buffer);
// sub_82106A00: the four buffers at +43100 down to +43016 destroyed; r3 as
// the last Free's. Only when each is Buffer28Freeable.
constexpr uint32_t kBuffers28 = 43016;
constexpr uint32_t kBuffer28Bytes = 28;
bool Buffers28Freeable(uint8_t* base, uint32_t object);
uint32_t Buffers28Destroy(uint8_t* base, uint32_t object);

// sub_821067A0 XprHolder::XprHolder: an object holding an XPR at +4
// (XPR::XPR, models/xpr.h) and two 12-byte records at +32, cleared. The
// race mode's member has one at +67676.
void XprHolderConstruct(uint8_t* base, uint32_t object);

// sub_82106AF0 RaceModeMember::~RaceModeMember: the member at the race mode's
// +10512 torn down: its vtable put back, the XPR holder's two records (+67720,
// then +67708) cleared, its XPR (+67680) destroyed (XPR::~XPR), then the four
// buffers (sub_82106A00 on +13360). Returns r3 as the original leaves it
// (sub_82106A00's); nullopt, with nothing done, where the original must run
// (the pending frees can't take the XPR's blocks, or a buffer's block isn't
// the allocation layer's). Over the allocation layer and the memory blocks'
// host lock only (the adapter's condition).
constexpr uint32_t kRaceModeMemberXprHolder = 67676;
std::optional<uint32_t> RaceModeMemberDestruct(uint8_t* base, uint32_t member);

// sub_82106A50 Member56896::Member56896: the object at the race mode's
// member's +56896 set up (its vtable, 1.0s in its w words, the XPR holder at
// +10780).
void Member56896Construct(uint8_t* base, uint32_t object);

// sub_82107168 RaceModeMember::RaceModeMember: the member at the race mode's
// +10512 (77,392 bytes) set up: its state machine at +16, its vtable, three
// 1,104-byte records at +9648 (two) and +11856, an object at +12976, the
// objects at +13360, +56896 and +67760 (Member13360, Member56896,
// Member67760), eight more records' 1.0s from +69628, and +77376 to +77388.
constexpr uint32_t kRaceModeMemberBytes = 77392;
void RaceModeMemberConstruct(uint8_t* base, uint32_t member);

// sub_821073B8 RaceMode::RaceMode: the race mode (167,264 bytes) set up: the
// base (Mode::Mode), the dispatcher at +36 emptied, its vtables, the ten
// state machines, the member at +10512 (RaceModeMember::RaceModeMember), an
// XPR at +147892 (XPR::XPR) and the race overlays' list at +157552
// (RaceOverlays::Clear).
constexpr uint32_t kRaceModeBytes = 167264;
void RaceModeConstruct(uint8_t* base, uint32_t mode);

// The other modes' constructors: the base (Mode::Mode), the dispatcher at +36
// emptied, the type's vtables at +0 and +32 (852 bytes; type 3's 10,460, with
// a state machine at +848). Type 2 sub_82106DB8, 3 sub_82106C80, 4
// sub_82106E88, 5 sub_82106F08, 6 sub_82235E88 (+848 = 0 too), 7
// sub_82246340, 9 sub_82107008, 10 sub_82107088, 12 sub_82106F88.
constexpr uint32_t kModeBytes = 852;
constexpr uint32_t kMode3Bytes = 10460;
void ModeTypeConstruct(uint8_t* base, uint32_t type, uint32_t mode);
// sub_82108160 MachineMember::MachineMember: StateMachine::StateMachine, then
// its count and state = 0.
void MachineMemberConstruct(uint8_t* base, uint32_t machine);
// The bytes Mode::Create allocates for a type (1 to 12), 0 for none (11).
uint32_t ModeBytes(uint32_t type);

// sub_821077A8 Mode::Create: the mode of `type` allocated (operator new,
// the C runtime heap's) and built; 0 for type 11 or out of range. nullopt,
// with nothing done, where the original must run: type 8 (sub_82107670,
// never seen in play) and an allocation operator new's plain C++ leaves to
// the original.
std::optional<uint32_t> ModeCreate(uint8_t* base, uint32_t type);

}  // namespace game_modes
