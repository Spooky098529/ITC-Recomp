// The game modes and their state machines as plain C++
// (docs/functions/game-modes.md); see game_modes.h.

#include "game_modes/game_modes.h"

#include "base/flags.h"
#include "base/log.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "models/memory_blocks.h"
#include "models/xpr.h"
#include "race_overlays/race_overlays.h"
#include "schema/game_modes.h"

#include <cstdlib>

ITC_FLAG_DECLARE(bool, multiplayer_menu_items);

namespace game_modes {

namespace {

using G = schema::GameModesGlobals;

// The mode's base vtables (+0 and +32, after the intermediate one at +0)
// and its default delegate's (at +8).
constexpr uint32_t kModeVtableIntermediate = 0x82006B44;
constexpr uint32_t kModeVtable = 0x820069EC;
constexpr uint32_t kModeVtable32 = 0x82006A0C;
constexpr uint32_t kDefaultDelegateVtable = 0x82006B20;

// A transition's functor storage: six words (schema::StateTransition's
// storage, schema::StateFunctorPart's from +4).
constexpr uint32_t kStorageWords = 6;

}  // namespace

void ModesConstruct(uint8_t* base, uint32_t holder, uint32_t type) {
  ITC_NATIVE_RUNNING("sub_821067F8 Modes::Modes");
  const schema::ModeHolder h(base, holder);
  h.set_vtable(schema::ModeHolder::kVtable);
  G::set_current_type(base, type);
  G::set_previous_type(base, type);
  h.set_mode(0);
  G::set_data_loaded(base, 0);
  G::set_sign_in_state(base, 0);
  for (uint32_t i = 0; i < 7; i++) {
    guest::StoreU32(base, G::mode_checks_address() + 4 * i, 0);
  }
  G::set_remake_save_data(base, 0);
  G::set_play_frames(base, 0);
}

StateKey LoadStateKey(uint8_t* base, uint32_t address) {
  const schema::TransitionKey k(base, address);
  StateKey key;
  key.words[0] = k.state();
  key.words[1] = k.message();
  key.words[2] = k.arg0();
  key.words[3] = k.arg1();
  return key;
}

bool StateKeyEquals(uint8_t* base, uint32_t a, const StateKey& b) {
  ITC_NATIVE_RUNNING("sub_82106040 StateKey::Equals");
  const schema::TransitionKey k(base, a);
  return int32_t(k.state()) == int32_t(b.words[0]) &&
         int32_t(k.message()) == int32_t(b.words[1]) &&
         int32_t(k.arg0()) == int32_t(b.words[2]) && int32_t(k.arg1()) == int32_t(b.words[3]);
}

int32_t StateMachineIndexOf(uint8_t* base, uint32_t machine, const StateKey& key) {
  ITC_NATIVE_RUNNING("sub_821060A0 StateMachine::IndexOf");
  const uint32_t count = schema::StateMachine(base, machine).transition_count();
  uint32_t transition = machine;
  for (uint32_t i = 0; i < count; i++, transition += schema::StateTransition::kSize) {
    if (StateKeyEquals(base, transition, key)) {
      return int32_t(i);
    }
  }
  return -1;
}

uint32_t StateMachineAdd(uint8_t* base, uint32_t machine, uint32_t key, uint32_t functor,
                         bool* replaced) {
  ITC_NATIVE_RUNNING("sub_82106158 StateMachine::Add");
  const StateKey k = LoadStateKey(base, key);
  const int32_t index = StateMachineIndexOf(base, machine, k);
  *replaced = index != -1;
  const schema::StateFunctorPart from(base, functor);
  if (index != -1) {
    // The functor part replaced, the key kept.
    const schema::StateFunctorPart part(
        base, schema::StateTransition(base, machine + schema::StateTransition::kSize *
                                                          uint32_t(index))
                  .functor_address());
    for (uint32_t i = 0; i < kStorageWords; i++) {
      guest::StoreU32(base, part.storage_address() + 4 * i,
                      guest::LoadU32(base, from.storage_address() + 4 * i));
    }
    part.set_functor(part.storage_address());
    part.set_next_state(from.next_state());
    return uint32_t(index);
  }
  // The original makes the transition on its stack (Transition::Make), then
  // counts it and copies it in (Transition::Assign): its words read first.
  uint32_t storage[kStorageWords];
  for (uint32_t i = 0; i < kStorageWords; i++) {
    storage[i] = guest::LoadU32(base, from.storage_address() + 4 * i);
  }
  const uint32_t next = from.next_state();
  const schema::StateMachine m(base, machine);
  const uint32_t count = m.transition_count();
  m.set_transition_count(count + 1);
  const schema::StateTransition t(base, machine + schema::StateTransition::kSize * count);
  t.set_state(k.words[0]);
  t.set_message(k.words[1]);
  t.set_arg0(k.words[2]);
  t.set_arg1(k.words[3]);
  for (uint32_t i = 0; i < kStorageWords; i++) {
    guest::StoreU32(base, t.storage_address() + 4 * i, storage[i]);
  }
  t.set_functor(t.storage_address());
  t.set_next_state(next);
  return t.address;
}

uint32_t StateMachineFind(uint8_t* base, uint32_t machine, const StateKey& key, bool* found) {
  ITC_NATIVE_RUNNING("sub_821061F0 StateMachine::Find");
  const uint32_t guard = G::default_guard(base);
  if ((guard & 1) == 0) {
    G::set_default_guard(base, guard | 1);
    const schema::StateFunctorPart d(base, G::default_transition_address());
    d.set_functor_vtable(kNoFunctor);
    d.set_functor(d.storage_address());
    d.set_next_state(0);
  }
  const int32_t index = StateMachineIndexOf(base, machine, key);
  *found = index != -1;
  if (index == -1) {
    return G::default_transition_address();
  }
  return schema::StateTransition(base, machine + schema::StateTransition::kSize * uint32_t(index))
      .functor_address();
}

uint32_t StateMachineHandle(uint8_t* base, uint32_t machine, uint32_t message, bool* found) {
  ITC_NATIVE_RUNNING("sub_82106280 StateMachine::Handle");
  // The key as the original builds it on its stack: the message's words,
  // then the state.
  const schema::ModeMessage m(base, message);
  const schema::StateMachine sm(base, machine);
  StateKey key;
  key.words[1] = m.id();
  key.words[2] = m.arg0();
  key.words[3] = m.arg1();
  key.words[0] = sm.state();
  const uint32_t part = StateMachineFind(base, machine, key, found);
  const uint32_t next = schema::StateFunctorPart(base, part).next_state();
  if (next != 0) {
    sm.set_state(next);
  }
  return part;
}

void StateMachineConstruct(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_82107108 StateMachine::StateMachine");
  for (uint32_t i = 0; i < kTransitions; i++) {
    const schema::StateTransition t(base, machine + schema::StateTransition::kSize * i);
    t.set_state(0);
    t.set_message(0);
    t.set_arg0(0);
    t.set_arg1(0);
    t.set_functor(0);
    // The functor's storage gets the empty functor's vtable (a null
    // storage address, which can't happen, would get nothing).
    const uint32_t storage = t.storage_address();
    if (storage != 0) {
      t.set_functor_vtable(kNoFunctor);
    }
    t.set_next_state(0);
    t.set_functor(storage);
  }
}

void ModeConstruct(uint8_t* base, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_82106C20 Mode::Mode");
  guest::StoreU32(base, mode + 0, kModeVtableIntermediate);
  guest::StoreU32(base, mode + 4, 0);
  const uint32_t delegate = mode + 8;
  uint32_t pointer = 0;
  if (delegate != 0) {
    pointer = delegate;
    guest::StoreU32(base, delegate, kDefaultDelegateVtable);
  }
  guest::StoreU32(base, mode + 4, pointer);
  guest::StoreU32(base, mode + 32, kModeVtable32);
  guest::StoreU32(base, mode + 0, kModeVtable);
  guest::StoreU32(base, mode + 32, kModeVtable32);
}

namespace {

// The overlays' handlers: `each(slot)` for each of the 25 slots that holds
// an overlay (the slot read again for each store, as the originals do).
template <typename Each>
void ForEachOverlay(uint8_t* base, uint32_t machine, Each each) {
  const schema::RaceOverlaysMachine m(base, machine);
  for (uint32_t i = 0; i < kOverlaySlots; i++) {
    const uint32_t slot = m.overlays_address() + 4 * i;
    if (guest::LoadU32(base, slot) != 0) {
      each(slot);
    }
  }
}

void StoreInOverlay(uint8_t* base, uint32_t slot, uint32_t offset, uint32_t value) {
  guest::StoreU32(base, guest::LoadU32(base, slot) + offset, value);
}

}  // namespace

void OverlaysSetFlags(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_8215DD58 RaceOverlaysMachine::SetFlags");
  ForEachOverlay(base, machine, [&](uint32_t slot) {
    StoreInOverlay(base, slot, 820, 1);
    StoreInOverlay(base, slot, 824, 1);
  });
}

void OverlaysClearFlags(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_8215DD90 RaceOverlaysMachine::ClearFlags");
  ForEachOverlay(base, machine, [&](uint32_t slot) {
    StoreInOverlay(base, slot, 820, 0);
    StoreInOverlay(base, slot, 824, 0);
    StoreInOverlay(base, slot, 828, 0);
  });
}

void OverlaysSetFlag820(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_8215DDD0 RaceOverlaysMachine::SetFlag820");
  ForEachOverlay(base, machine, [&](uint32_t slot) {
    StoreInOverlay(base, slot, 820, 1);
    StoreInOverlay(base, slot, 824, 0);
  });
}

void OverlaysReset(uint8_t* base, uint32_t machine,
                   const std::function<void(uint32_t overlay, uint32_t target)>& reset) {
  ITC_NATIVE_RUNNING("sub_8215DE10 RaceOverlaysMachine::ResetOverlays");
  ForEachOverlay(base, machine, [&](uint32_t slot) {
    const uint32_t overlay = guest::LoadU32(base, slot);
    reset(overlay, guest::LoadU32(base, guest::LoadU32(base, overlay) + 12));
    StoreInOverlay(base, slot, 828, 1);
  });
}

namespace {

constexpr uint32_t kMember67760Vtable = 0x820093C4;
constexpr uint32_t kMember13360Vtable = 0x820093BC;
constexpr uint32_t kMember43136Vtable = 0x82006B24;

// The 1.0s the camera modes' objects start with: from +44 to +252 and +284
// to +332, every 16 bytes (the w words of their matrices, probably).
void StoreOnes(uint8_t* base, uint32_t object, uint32_t one) {
  for (uint32_t offset = 44; offset <= 252; offset += 16) {
    guest::StoreU32(base, object + offset, one);
  }
  for (uint32_t offset = 284; offset <= 332; offset += 16) {
    guest::StoreU32(base, object + offset, one);
  }
}
// 1.0 in .rdata, the w words' value, loaded as its bits.
uint32_t LoadOne(uint8_t* base) { return guest::LoadU32(base, G::one_address()); }

}  // namespace

void Member67760Construct(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82155D90 Member67760::Member67760");
  // The original loads the float and stores it back as a single (the same
  // bits for the constant 1.0).
  const uint32_t one = LoadOne(base);
  guest::StoreU32(base, object + 16, 0);
  guest::StoreU32(base, object + 20, 0);
  StoreOnes(base, object, one);
  guest::StoreU32(base, object + 0, kMember67760Vtable);
  guest::StoreU32(base, object + 748, 0);
  guest::StoreU32(base, object + 752, 0);
  guest::StoreU32(base, object + 756, 2);
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreU16(base, object + 352 + 4 * i + 2, uint16_t(i));
    guest::StoreU16(base, object + 352 + 4 * i, uint16_t(i + 1));
  }
  guest::StoreU16(base, object + 416, 0);
  guest::StoreU16(base, object + 424, 0);
  guest::StoreU16(base, object + 412, 0xFFFF);
  guest::StoreU16(base, object + 420, 0xFFFF);
  guest::StoreU16(base, object + 422, 0xFFFF);
  guest::StoreU16(base, object + 418, 0xFFFF);
}

void Member13360Construct(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82155118 Member13360::Member13360");
  const uint32_t one = LoadOne(base);
  guest::StoreU32(base, object + 16, 0);
  guest::StoreU32(base, object + 20, 0);
  StoreOnes(base, object, one);
  guest::StoreU32(base, object + 0, kMember13360Vtable);
  guest::StoreU32(base, object + 396, one);
  guest::StoreU32(base, object + 412, one);
  guest::StoreU32(base, object + 428, one);
  for (uint32_t i = 0; i < 4; i++) {
    const schema::CameraPathBuffer buffer(base,
                                          object + kBuffers28 + schema::CameraPathBuffer::kSize * i);
    buffer.set_index(0xFFFFFFFF);
    buffer.set_vtable(schema::CameraPathBuffer::kVtable);
    buffer.set_serial(0xFFFFFFFF);
    buffer.set_block(0);
    buffer.set_state(0);
  }
  const uint32_t inner = object + 43136;
  guest::StoreU32(base, inner + 16, 0);
  guest::StoreU32(base, inner + 20, 0);
  guest::StoreU32(base, inner + 44, one);
  guest::StoreU32(base, inner + 0, kMember43136Vtable);
  StoreOnes(base, inner, one);
  guest::StoreU32(base, inner + 364, one);
  const uint32_t word = G::tv_camera_range_count(base);
  guest::StoreU32(base, object + 360, 0xFFFFFFFF);
  guest::StoreU32(base, object + 448, 0);
  guest::StoreU32(base, object + 452, word);
  // Each buffer reset (Buffer28::Reset; with no block yet, nothing freed).
  for (uint32_t i = 0; i < 4; i++) {
    Buffer28Reset(base, object + kBuffers28 + schema::CameraPathBuffer::kSize * i);
  }
}

bool Buffer28Freeable(uint8_t* base, uint32_t buffer) {
  const uint32_t block = schema::CameraPathBuffer(base, buffer).block();
  return block == 0 || memory::IsPages(block);
}

uint32_t Buffer28Free(uint8_t* base, uint32_t buffer) {
  ITC_NATIVE_RUNNING("sub_82152028 Buffer28::Free");
  const schema::CameraPathBuffer b(base, buffer);
  const uint32_t block = b.block();
  if (block == 0) {
    return 0;
  }
  const uint32_t r3 = memory::FreePages(block) ? 1 : 0;
  b.set_block(0);
  return r3;
}

uint32_t Buffer28Reset(uint8_t* base, uint32_t buffer) {
  ITC_NATIVE_RUNNING("sub_82152D38 Buffer28::Reset");
  const uint32_t r3 = Buffer28Free(base, buffer);
  const schema::CameraPathBuffer b(base, buffer);
  b.set_index(0xFFFFFFFF);
  b.set_serial(0xFFFFFFFF);
  b.set_block(0);
  b.set_state(0);
  return r3;
}

uint32_t Buffer28Destroy(uint8_t* base, uint32_t buffer) {
  ITC_NATIVE_RUNNING("sub_82152D28 Buffer28::~Buffer28");
  schema::CameraPathBuffer(base, buffer).set_vtable(schema::CameraPathBuffer::kVtable);
  return Buffer28Free(base, buffer);
}

bool Buffers28Freeable(uint8_t* base, uint32_t object) {
  for (uint32_t i = 0; i < 4; i++) {
    if (!Buffer28Freeable(base, object + kBuffers28 + schema::CameraPathBuffer::kSize * i)) {
      return false;
    }
  }
  return true;
}

uint32_t Buffers28Destroy(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82106A00 Buffers28::~Buffers28");
  uint32_t r3 = 0;
  for (int32_t i = 3; i >= 0; i--) {
    r3 = Buffer28Destroy(base, object + kBuffers28 + schema::CameraPathBuffer::kSize * uint32_t(i));
  }
  return r3;
}

void XprHolderConstruct(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_821067A0 XprHolder::XprHolder");
  const schema::XprHolder h(base, object);
  models::XprConstruct(base, h.xpr_address());
  h.set_record0_word0(0);
  h.set_record0_word1(0);
  h.set_record0_word2(0);
  h.set_record1_word0(0);
  h.set_record1_word1(0);
  h.set_record1_word2(0);
}

namespace {

constexpr uint32_t kRaceModeMemberVtable = 0x82006B60;
constexpr uint32_t kRaceModeMemberBuffers = 13360;

}  // namespace

std::optional<uint32_t> RaceModeMemberDestruct(uint8_t* base, uint32_t member) {
  ITC_NATIVE_RUNNING("sub_82106AF0 RaceModeMember::~RaceModeMember");
  // What XPR::~XPR and the buffers' destructors would leave to the
  // originals, before anything changes: room for the XPR's two nodes, and
  // each buffer's block the layer's pages (or none).
  if (!models::PendingFreesHaveRoom(base, 2) ||
      !Buffers28Freeable(base, member + kRaceModeMemberBuffers)) {
    return std::nullopt;
  }
  const schema::XprHolder holder(base, member + kRaceModeMemberXprHolder);
  guest::StoreU32(base, member + 0, kRaceModeMemberVtable);
  holder.set_record1_word0(0);
  holder.set_record1_word1(0);
  holder.set_record1_word2(0);
  holder.set_record0_word0(0);
  holder.set_record0_word1(0);
  holder.set_record0_word2(0);
  if (!models::XprDestruct(base, holder.xpr_address())) {
    ITC_LOG_ERROR("RaceModeMember::~RaceModeMember: XPR::~XPR refused after its checks");
    std::abort();
  }
  return Buffers28Destroy(base, member + kRaceModeMemberBuffers);
}

namespace {

constexpr uint32_t kMember56896Vtable = 0x82006B3C;
constexpr uint32_t kRecordVtable = 0x82006B34;
constexpr uint32_t kRecord11856Vtable = 0x82006B2C;
constexpr uint32_t kRecordBytes = 1104;

// A 1,104-byte record of the member's: +16 and +20 = 0, the 1.0s, its
// vtable, and 1.0 at +1100.
void RecordConstruct(uint8_t* base, uint32_t record, uint32_t vtable, uint32_t one) {
  guest::StoreU32(base, record + 16, 0);
  guest::StoreU32(base, record + 20, 0);
  StoreOnes(base, record, one);
  guest::StoreU32(base, record + 0, vtable);
  guest::StoreU32(base, record + 1100, one);
}

// The race mode's +32 vtable (its +0 one is schema::RaceMode::kVtable).
constexpr uint32_t kRaceModeVtable32 = 0x82006D28;
// The dispatcher's place in every mode (boot-mode.toml's GameMode).
constexpr uint32_t kModeDispatcher = 36;

void MachineConstruct(uint8_t* base, uint32_t machine) {
  StateMachineConstruct(base, machine);
  const schema::StateMachine m(base, machine);
  m.set_transition_count(0);
  m.set_state(0);
}

// What every mode's constructor does first: the base (Mode::Mode), the
// dispatcher at +36 emptied (its handler count, the ring's 50 messages,
// their count and start), +848 = 0 for type 6, then the type's vtables.
void ModeBaseConstruct(uint8_t* base, uint32_t mode, uint32_t vtable, uint32_t vtable32,
                       bool word848 = false) {
  ModeConstruct(base, mode);
  const uint32_t dispatcher = mode + kModeDispatcher;
  guest::StoreU32(base, dispatcher + 200, 0);
  for (uint32_t i = 0; i < 50 * 3; i++) {
    guest::StoreU32(base, dispatcher + 204 + 4 * i, 0);
  }
  guest::StoreU32(base, dispatcher + 804, 0);
  guest::StoreU32(base, dispatcher + 808, 0);
  if (word848) {
    schema::GameModeType6(base, mode).set_word_848(0);
  }
  guest::StoreU32(base, mode + 0, vtable);
  guest::StoreU32(base, mode + 32, vtable32);
}

}  // namespace

void Member56896Construct(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82106A50 Member56896::Member56896");
  const uint32_t one = LoadOne(base);
  guest::StoreU32(base, object + 16, 0);
  guest::StoreU32(base, object + 20, 0);
  StoreOnes(base, object, one);
  guest::StoreU32(base, object + 0, kMember56896Vtable);
  XprHolderConstruct(base, object + 10780);
}

void RaceModeMemberConstruct(uint8_t* base, uint32_t member) {
  ITC_NATIVE_RUNNING("sub_82107168 RaceModeMember::RaceModeMember");
  MachineConstruct(base, member + 16);
  guest::StoreU32(base, member + 0, kRaceModeMemberVtable);
  // The original loads the 1.0 once (f31) for all its stores.
  const uint32_t one = LoadOne(base);
  RecordConstruct(base, member + 9648, kRecordVtable, one);
  RecordConstruct(base, member + 9648 + kRecordBytes, kRecordVtable, one);
  RecordConstruct(base, member + 11856, kRecord11856Vtable, one);
  const uint32_t inner = member + 12976;
  guest::StoreU32(base, inner + 16, 0);
  guest::StoreU32(base, inner + 20, 0);
  StoreOnes(base, inner, one);
  guest::StoreU32(base, inner + 0, kMember43136Vtable);
  guest::StoreU32(base, inner + 364, one);
  Member13360Construct(base, member + kRaceModeMemberBuffers);
  Member56896Construct(base, member + 56896);
  Member67760Construct(base, member + 67760);
  for (uint32_t i = 0; i < 8; i++) {
    guest::StoreU32(base, member + 69628 + kRecordBytes * i, one);
  }
  guest::StoreU32(base, member + 77380, 0);
  guest::StoreU32(base, member + 77384, 0);
  guest::StoreU32(base, member + 77388, 0);
  guest::StoreU32(base, member + 77376, 0xFFFFFFFF);
}

void RaceModeConstruct(uint8_t* base, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_821073B8 RaceMode::RaceMode");
  ModeBaseConstruct(base, mode, schema::RaceMode::kVtable, kRaceModeVtable32);
  const schema::RaceMode m(base, mode);
  MachineConstruct(base, m.course_setup_address());
  RaceModeMemberConstruct(base, m.camera_director_address());
  MachineConstruct(base, m.race_scene_address());
  MachineConstruct(base, m.race_flow_address());
  MachineConstruct(base, m.race_states_address());
  MachineConstruct(base, m.race_world_address());
  MachineConstruct(base, m.race_hud_machine_address());
  MachineConstruct(base, m.race_sounds_address());
  models::XprConstruct(base, m.xpr_address());
  MachineConstruct(base, m.overlays_machine_address());
  race_overlays::RaceOverlaysClear(base, m.overlays_address());
  MachineConstruct(base, m.machine_157652_address());
}

void MachineMemberConstruct(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_82108160 MachineMember::MachineMember");
  MachineConstruct(base, machine);
}

namespace {

#define ITC_MODE_TYPE(address, type, vtable, vtable32, word848)                  \
  void ModeType##type##Construct(uint8_t* base, uint32_t mode) {                \
    ITC_NATIVE_RUNNING("sub_" #address " ModeType" #type "::ModeType" #type);    \
    ModeBaseConstruct(base, mode, vtable, vtable32, word848);                 \
  }

ITC_MODE_TYPE(82106DB8, 2, 0x82006B9C, 0x82006B90, false)
ITC_MODE_TYPE(82106E88, 4, 0x82006BE0, 0x82006BD4, false)
ITC_MODE_TYPE(82106F08, 5, 0x82006C24, 0x82006C18, false)
ITC_MODE_TYPE(82235E88, 6, 0x82010A48, 0x82010A3C, true)
ITC_MODE_TYPE(82246340, 7, 0x82010FB0, 0x82010FA4, false)
ITC_MODE_TYPE(82107008, 9, 0x82006CAC, 0x82006CA0, false)
ITC_MODE_TYPE(82107088, 10, 0x82006CF0, 0x82006CE4, false)
ITC_MODE_TYPE(82106F88, 12, 0x82006C6C, 0x82006C60, false)

#undef ITC_MODE_TYPE

void ModeType3Construct(uint8_t* base, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_82106C80 ModeType3::ModeType3");
  ModeBaseConstruct(base, mode, 0x82006B70, 0x82006B64, false);
  MachineMemberConstruct(base, mode + 848);
}

}  // namespace

void ModeTypeConstruct(uint8_t* base, uint32_t type, uint32_t mode) {
  switch (type) {
    case 2: ModeType2Construct(base, mode); break;
    case 3: ModeType3Construct(base, mode); break;
    case 4: ModeType4Construct(base, mode); break;
    case 5: ModeType5Construct(base, mode); break;
    case 6: ModeType6Construct(base, mode); break;
    case 7: ModeType7Construct(base, mode); break;
    case 9: ModeType9Construct(base, mode); break;
    case 10: ModeType10Construct(base, mode); break;
    case 12:
      // Not made without --multiplayer_menu_items (the bypass, 2026-11-14:
      // only the main menu's Xbox Live item opened it; ModeCreate declines
      // it then).
      if (ITC_FLAG(multiplayer_menu_items)) {
        ModeType12Construct(base, mode);
      }
      break;
    default: break;
  }
}

uint32_t ModeBytes(uint32_t type) {
  switch (type) {
    case 1: return kRaceModeBytes;
    case 3: return kMode3Bytes;
    case 8: return 87768;
    case 2: case 4: case 5: case 6: case 7: case 9: case 10: case 12: return kModeBytes;
    default: return 0;
  }
}

std::optional<uint32_t> ModeCreate(uint8_t* base, uint32_t type) {
  ITC_NATIVE_RUNNING("sub_821077A8 Mode::Create");
  const uint32_t bytes = ModeBytes(type);
  if (bytes == 0) {
    return 0;  // 11, or out of range
  }
  if (type == 8) {
    return std::nullopt;  // sub_82107670, never seen in play
  }
  if (type == 12 && !ITC_FLAG(multiplayer_menu_items)) {
    // The Xbox Live mode, bypassed at the main menu since 2026-11-14 (the
    // user's call): nothing asks for it, so the original's (the fallback)
    // would make it if anything did.
    ITC_LOG_ERROR("Mode::Create: type 12 (Xbox Live) asked for with --multiplayer_menu_items off");
    return std::nullopt;
  }
  const auto mode = memory::crt::OperatorNew(base, bytes);
  if (!mode) {
    return std::nullopt;
  }
  if (*mode == 0) {
    return 0;
  }
  if (type == 1) {
    RaceModeConstruct(base, *mode);
  } else {
    ModeTypeConstruct(base, type, *mode);
  }
  return *mode;
}

}  // namespace game_modes
