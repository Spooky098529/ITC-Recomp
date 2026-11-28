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
#include "schema/boot_mode.h"
#include "schema/camera_modes.h"
#include "schema/game_modes.h"
#include "schema/network_mode.h"
#include "schema/race_overlays.h"
#include "schema/title_screen.h"
#include "schema/todays_result.h"
#include "schema/type10_mode.h"
#include "schema/type2_mode.h"
#include "schema/type5_mode.h"

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

void ModesConstruct(uint32_t holder, uint32_t type) {
  ITC_NATIVE_RUNNING("sub_821067F8 Modes::Modes");
  const schema::ModeHolder h(holder);
  h.set_vtable(schema::ModeHolder::kVtable);
  G::set_current_type(type);
  G::set_previous_type(type);
  h.set_mode(0);
  G::set_data_loaded(0);
  G::set_sign_in_state(0);
  for (uint32_t i = 0; i < 7; i++) {
    guest::StoreU32(G::mode_checks_address() + 4 * i, 0);
  }
  G::set_remake_save_data(0);
  G::set_play_frames(0);
}

StateKey LoadStateKey(uint32_t address) {
  const schema::TransitionKey k(address);
  StateKey key;
  key.words[0] = k.state();
  key.words[1] = k.message();
  key.words[2] = k.arg0();
  key.words[3] = k.arg1();
  return key;
}

bool StateKeyEquals(uint32_t a, const StateKey& b) {
  ITC_NATIVE_RUNNING("sub_82106040 StateKey::Equals");
  const schema::TransitionKey k(a);
  return int32_t(k.state()) == int32_t(b.words[0]) &&
         int32_t(k.message()) == int32_t(b.words[1]) &&
         int32_t(k.arg0()) == int32_t(b.words[2]) && int32_t(k.arg1()) == int32_t(b.words[3]);
}

int32_t StateMachineIndexOf(uint32_t machine, const StateKey& key) {
  ITC_NATIVE_RUNNING("sub_821060A0 StateMachine::IndexOf");
  const uint32_t count = schema::StateMachine(machine).transition_count();
  uint32_t transition = machine;
  for (uint32_t i = 0; i < count; i++, transition += schema::StateTransition::kSize) {
    if (StateKeyEquals(transition, key)) {
      return int32_t(i);
    }
  }
  return -1;
}

uint32_t StateMachineAdd(uint32_t machine, uint32_t key, uint32_t functor,
                         bool* replaced) {
  ITC_NATIVE_RUNNING("sub_82106158 StateMachine::Add");
  const StateKey k = LoadStateKey(key);
  const int32_t index = StateMachineIndexOf(machine, k);
  *replaced = index != -1;
  const schema::StateFunctorPart from(functor);
  if (index != -1) {
    // The functor part replaced, the key kept.
    const schema::StateFunctorPart part(schema::StateTransition(machine + schema::StateTransition::kSize *
                                                          uint32_t(index))
                  .functor_address());
    for (uint32_t i = 0; i < kStorageWords; i++) {
      guest::StoreU32(part.storage_address() + 4 * i,
                      guest::LoadU32(from.storage_address() + 4 * i));
    }
    part.set_functor(part.storage_address());
    part.set_next_state(from.next_state());
    return uint32_t(index);
  }
  // The original makes the transition on its stack (Transition::Make), then
  // counts it and copies it in (Transition::Assign): its words read first.
  uint32_t storage[kStorageWords];
  for (uint32_t i = 0; i < kStorageWords; i++) {
    storage[i] = guest::LoadU32(from.storage_address() + 4 * i);
  }
  const uint32_t next = from.next_state();
  const schema::StateMachine m(machine);
  const uint32_t count = m.transition_count();
  m.set_transition_count(count + 1);
  const schema::StateTransition t(machine + schema::StateTransition::kSize * count);
  t.set_state(k.words[0]);
  t.set_message(k.words[1]);
  t.set_arg0(k.words[2]);
  t.set_arg1(k.words[3]);
  for (uint32_t i = 0; i < kStorageWords; i++) {
    guest::StoreU32(t.storage_address() + 4 * i, storage[i]);
  }
  t.set_functor(t.storage_address());
  t.set_next_state(next);
  return t.address;
}

uint32_t StateMachineFind(uint32_t machine, const StateKey& key, bool* found) {
  ITC_NATIVE_RUNNING("sub_821061F0 StateMachine::Find");
  const uint32_t guard = G::default_guard();
  if ((guard & 1) == 0) {
    G::set_default_guard(guard | 1);
    const schema::StateFunctorPart d(G::default_transition_address());
    d.set_functor_vtable(kNoFunctor);
    d.set_functor(d.storage_address());
    d.set_next_state(0);
  }
  const int32_t index = StateMachineIndexOf(machine, key);
  *found = index != -1;
  if (index == -1) {
    return G::default_transition_address();
  }
  return schema::StateTransition(machine + schema::StateTransition::kSize * uint32_t(index))
      .functor_address();
}

uint32_t StateMachineHandle(uint32_t machine, uint32_t message, bool* found) {
  ITC_NATIVE_RUNNING("sub_82106280 StateMachine::Handle");
  // The key as the original builds it on its stack: the message's words,
  // then the state.
  const schema::ModeMessage m(message);
  const schema::StateMachine sm(machine);
  StateKey key;
  key.words[1] = m.id();
  key.words[2] = m.arg0();
  key.words[3] = m.arg1();
  key.words[0] = sm.state();
  const uint32_t part = StateMachineFind(machine, key, found);
  const uint32_t next = schema::StateFunctorPart(part).next_state();
  if (next != 0) {
    sm.set_state(next);
  }
  return part;
}

void StateMachineConstruct(uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_82107108 StateMachine::StateMachine");
  for (uint32_t i = 0; i < kTransitions; i++) {
    const schema::StateTransition t(machine + schema::StateTransition::kSize * i);
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

void ModeConstruct(uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_82106C20 Mode::Mode");
  const schema::GameMode m(mode);
  m.set_vtable(kModeVtableIntermediate);
  m.set_delegate_ptr(0);
  const uint32_t delegate = m.address + 8;  // the delegate (a handle's interior in host memory)
  uint32_t pointer = 0;
  if (delegate != 0) {
    pointer = delegate;
    schema::ModeDelegate(delegate).set_vtable(kDefaultDelegateVtable);
  }
  m.set_delegate_ptr(pointer);
  m.set_vtable32(kModeVtable32);
  m.set_vtable(kModeVtable);
  m.set_vtable32(kModeVtable32);
}

namespace {

// The overlays' handlers: `each(slot)` for each of the 25 slots that holds
// an overlay (the slot read again for each store, as the originals do).
template <typename Each>
void ForEachOverlay(uint32_t machine, Each each) {
  const schema::RaceOverlaysMachine m(machine);
  for (uint32_t i = 0; i < kOverlaySlots; i++) {
    const uint32_t slot = m.overlays_address() + 4 * i;
    if (guest::LoadU32(slot) != 0) {
      each(slot);
    }
  }
}

// The overlay in the slot (race-overlays.toml's CarOverlays), the slot read
// again for each store.
schema::CarOverlays OverlayIn(uint32_t slot) {
  return schema::CarOverlays(guest::LoadU32(slot));
}

}  // namespace

void OverlaysSetFlags(uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_8215DD58 RaceOverlaysMachine::SetFlags");
  ForEachOverlay(machine, [&](uint32_t slot) {
    OverlayIn(slot).set_unknown_820(1);
    OverlayIn(slot).set_unknown_824(1);
  });
}

void OverlaysClearFlags(uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_8215DD90 RaceOverlaysMachine::ClearFlags");
  ForEachOverlay(machine, [&](uint32_t slot) {
    OverlayIn(slot).set_unknown_820(0);
    OverlayIn(slot).set_unknown_824(0);
    OverlayIn(slot).set_unknown_828(0);
  });
}

void OverlaysSetFlag820(uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_8215DDD0 RaceOverlaysMachine::SetFlag820");
  ForEachOverlay(machine, [&](uint32_t slot) {
    OverlayIn(slot).set_unknown_820(1);
    OverlayIn(slot).set_unknown_824(0);
  });
}

void OverlaysReset(uint32_t machine,
                   const std::function<void(uint32_t overlay, uint32_t target)>& reset) {
  ITC_NATIVE_RUNNING("sub_8215DE10 RaceOverlaysMachine::ResetOverlays");
  ForEachOverlay(machine, [&](uint32_t slot) {
    const schema::CarOverlays overlay = OverlayIn(slot);
    reset(overlay.address, schema::OverlayVtable(overlay.vtable()).reset());
    OverlayIn(slot).set_unknown_828(1);
  });
}

namespace {

constexpr uint32_t kMember67760Vtable = 0x820093C4;
constexpr uint32_t kMember13360Vtable = 0x820093BC;
constexpr uint32_t kMember43136Vtable = 0x82006B24;

// The 1.0s the camera modes' objects start with, in their target tracker
// (camera-modes.toml's CameraTarget, at +32 in every camera): from +12 to
// +220 and +252 to +300, every 16 bytes (the w words of its vectors and
// matrices; the camera's +44 to +252 and +284 to +332).
void StoreOnes(uint32_t tracker, uint32_t one) {
  for (uint32_t offset = 12; offset <= 220; offset += 16) {
    guest::StoreU32(tracker + offset, one);
  }
  for (uint32_t offset = 252; offset <= 300; offset += 16) {
    guest::StoreU32(tracker + offset, one);
  }
}
// 1.0 in .rdata, the w words' value, loaded as its bits.
uint32_t LoadOne() { return guest::LoadU32(G::one_address()); }

}  // namespace

void Member67760Construct(uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82155D90 Member67760::Member67760");
  // The original loads the float and stores it back as a single (the same
  // bits for the constant 1.0).
  const schema::IntroCamera c(object);
  const uint32_t one = LoadOne();
  c.set_vehicle(0);
  c.set_viewport(0u);
  StoreOnes(c.tracker_address(), one);
  c.set_vtable(kMember67760Vtable);
  c.set_shot(0);
  c.set_frames(0);
  c.set_phase(2);
  // The shots' List16 (camera-modes.md): the 16 nodes' (next, previous)
  // halfwords, then the list's head words after them.
  const uint32_t shots = c.shots_address();
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreU16(shots + 4 * i + 2, uint16_t(i));
    guest::StoreU16(shots + 4 * i, uint16_t(i + 1));
  }
  guest::StoreU16(shots + 64, 0);
  guest::StoreU16(shots + 72, 0);
  guest::StoreU16(shots + 60, 0xFFFF);
  guest::StoreU16(shots + 68, 0xFFFF);
  guest::StoreU16(shots + 70, 0xFFFF);
  guest::StoreU16(shots + 66, 0xFFFF);
}

void Member13360Construct(uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82155118 Member13360::Member13360");
  const schema::TvCamera c(object);
  const uint32_t one = LoadOne();
  c.set_vehicle(0);
  c.set_viewport(0u);
  StoreOnes(c.tracker_address(), one);
  c.set_vtable(kMember13360Vtable);
  // The 1.0's bits stored as a word, as the original's single store.
  guest::StoreU32(c.eye_w_address(), one);
  guest::StoreU32(c.target_w_address(), one);
  guest::StoreU32(c.unknown_416_w_address(), one);
  for (uint32_t i = 0; i < 4; i++) {
    const schema::CameraPathBuffer buffer(c.buffers_address() + schema::CameraPathBuffer::kSize * i);
    buffer.set_index(0xFFFFFFFF);
    buffer.set_vtable(schema::CameraPathBuffer::kVtable);
    buffer.set_serial(0xFFFFFFFF);
    buffer.set_block(0);
    buffer.set_state(0);
  }
  const schema::ChaseCamera inner(c.fallback_address());
  inner.set_vehicle(0);
  inner.set_viewport(0u);
  guest::StoreU32(schema::CameraTarget(inner.tracker_address()).position_w_address(),
                  one);
  inner.set_vtable(kMember43136Vtable);
  StoreOnes(inner.tracker_address(), one);
  guest::StoreU32(inner.unknown_364_address(), one);
  const uint32_t word = G::tv_camera_range_count();
  c.set_spot(0xFFFFFFFF);
  c.set_paused(0);
  c.set_range_count(word);
  // Each buffer reset (Buffer28::Reset; with no block yet, nothing freed).
  for (uint32_t i = 0; i < 4; i++) {
    Buffer28Reset(c.buffers_address() + schema::CameraPathBuffer::kSize * i);
  }
}

bool Buffer28Freeable(uint32_t buffer) {
  const uint32_t block = schema::CameraPathBuffer(buffer).block();
  return block == 0 || memory::IsPages(block);
}

uint32_t Buffer28Free(uint32_t buffer) {
  ITC_NATIVE_RUNNING("sub_82152028 Buffer28::Free");
  const schema::CameraPathBuffer b(buffer);
  const uint32_t block = b.block();
  if (block == 0) {
    return 0;
  }
  const uint32_t r3 = memory::FreePages(block) ? 1 : 0;
  b.set_block(0);
  return r3;
}

uint32_t Buffer28Reset(uint32_t buffer) {
  ITC_NATIVE_RUNNING("sub_82152D38 Buffer28::Reset");
  const uint32_t r3 = Buffer28Free(buffer);
  const schema::CameraPathBuffer b(buffer);
  b.set_index(0xFFFFFFFF);
  b.set_serial(0xFFFFFFFF);
  b.set_block(0);
  b.set_state(0);
  return r3;
}

uint32_t Buffer28Destroy(uint32_t buffer) {
  ITC_NATIVE_RUNNING("sub_82152D28 Buffer28::~Buffer28");
  schema::CameraPathBuffer(buffer).set_vtable(schema::CameraPathBuffer::kVtable);
  return Buffer28Free(buffer);
}

bool Buffers28Freeable(uint32_t object) {
  for (uint32_t i = 0; i < 4; i++) {
    if (!Buffer28Freeable(schema::TvCamera(object).buffers_address() +
                                    schema::CameraPathBuffer::kSize * i)) {
      return false;
    }
  }
  return true;
}

uint32_t Buffers28Destroy(uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82106A00 Buffers28::~Buffers28");
  uint32_t r3 = 0;
  for (int32_t i = 3; i >= 0; i--) {
    r3 = Buffer28Destroy(schema::TvCamera(object).buffers_address() +
                                   schema::CameraPathBuffer::kSize * uint32_t(i));
  }
  return r3;
}

void XprHolderConstruct(uint32_t object) {
  ITC_NATIVE_RUNNING("sub_821067A0 XprHolder::XprHolder");
  const schema::XprHolder h(object);
  models::XprConstruct(h.xpr_address());
  h.set_record0_word0(0);
  h.set_record0_word1(0);
  h.set_record0_word2(0);
  h.set_record1_word0(0);
  h.set_record1_word1(0);
  h.set_record1_word2(0);
}

namespace {

// The camera director's vtable (camera-modes.toml's CameraDirector has no
// field at +0 yet).
constexpr uint32_t kRaceModeMemberVtable = 0x82006B60;

// The XPR holder in the director's cutscene's rival logo (its +20).
schema::XprHolder RivalLogoHolder(const schema::CameraDirector& d) {
  const schema::Cutscene cutscene(d.cutscene_address());
  return schema::XprHolder(schema::RivalLogo(cutscene.rival_logo_address()).holder_address());
}

}  // namespace

std::optional<uint32_t> RaceModeMemberDestruct(uint32_t member) {
  ITC_NATIVE_RUNNING("sub_82106AF0 RaceModeMember::~RaceModeMember");
  // What XPR::~XPR and the buffers' destructors would leave to the
  // originals, before anything changes: room for the XPR's two nodes, and
  // each buffer's block the layer's pages (or none).
  const schema::CameraDirector d(member);
  if (!models::PendingFreesHaveRoom(2) || !Buffers28Freeable(d.tv_camera_address())) {
    return std::nullopt;
  }
  const schema::XprHolder holder = RivalLogoHolder(d);
  guest::StoreU32(member + 0, kRaceModeMemberVtable);
  holder.set_record1_word0(0);
  holder.set_record1_word1(0);
  holder.set_record1_word2(0);
  holder.set_record0_word0(0);
  holder.set_record0_word1(0);
  holder.set_record0_word2(0);
  if (!models::XprDestruct(holder.xpr_address())) {
    ITC_LOG_ERROR("RaceModeMember::~RaceModeMember: XPR::~XPR refused after its checks");
    std::abort();
  }
  return Buffers28Destroy(d.tv_camera_address());
}

namespace {

constexpr uint32_t kMember56896Vtable = 0x82006B3C;
constexpr uint32_t kRecordVtable = 0x82006B34;
constexpr uint32_t kRecord11856Vtable = 0x82006B2C;
constexpr uint32_t kRecordBytes = 1104;

// A 1,104-byte view camera of the director's (camera-modes.toml's
// ViewCamera1 or ViewCamera2): +16 and +20 = 0, the 1.0s, its vtable, and
// 1.0 at +1100 (the rotation's w).
template <typename Camera>
void RecordConstruct(uint32_t record, uint32_t vtable, uint32_t one) {
  const Camera c(record);
  c.set_vehicle(0);
  c.set_viewport(0u);
  StoreOnes(c.tracker_address(), one);
  c.set_vtable(vtable);
  guest::StoreU32(c.rotation_w_address(), one);
}

// The race mode's +32 vtable (its +0 one is schema::RaceMode::kVtable).
constexpr uint32_t kRaceModeVtable32 = 0x82006D28;

void MachineConstruct(uint32_t machine) {
  StateMachineConstruct(machine);
  const schema::StateMachine m(machine);
  m.set_transition_count(0);
  m.set_state(0);
}

// What every mode's constructor does first: the base (Mode::Mode), the
// dispatcher at +36 emptied (its handler count, the ring's 50 messages,
// their count and start), +848 = 0 for type 6, then the type's vtables.
void ModeBaseConstruct(uint32_t mode, uint32_t vtable, uint32_t vtable32,
                       bool word848 = false) {
  ModeConstruct(mode);
  // The dispatcher at +36 (boot-mode.toml's GameMode, ModeDispatcher).
  // Dual views (E5 step 4): the 852-byte modes are in host memory.
  const schema::ModeDispatcher dispatcher(mode + 36);
  dispatcher.set_handler_count(0);
  for (uint32_t i = 0; i < 50 * 3; i++) {
    schema::host::StoreU32(dispatcher.ring() + 4 * i, 0);
  }
  dispatcher.set_ring_count(0);
  dispatcher.set_ring_start(0);
  if (word848) {
    schema::GameModeType6(mode).set_word_848(0);
  }
  const schema::GameMode m(mode);
  m.set_vtable(vtable);
  m.set_vtable32(vtable32);
}

}  // namespace

void Member56896Construct(uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82106A50 Member56896::Member56896");
  const schema::Cutscene c(object);
  const uint32_t one = LoadOne();
  c.set_vehicle(0);
  c.set_viewport(0u);
  StoreOnes(c.tracker_address(), one);
  c.set_vtable(kMember56896Vtable);
  XprHolderConstruct(schema::RivalLogo(c.rival_logo_address()).holder_address());
}

void RaceModeMemberConstruct(uint32_t member) {
  ITC_NATIVE_RUNNING("sub_82107168 RaceModeMember::RaceModeMember");
  const schema::CameraDirector d(member);
  MachineConstruct(d.machine_address());
  guest::StoreU32(member + 0, kRaceModeMemberVtable);
  // The original loads the 1.0 once (f31) for all its stores.
  const uint32_t one = LoadOne();
  RecordConstruct<schema::ViewCamera2>(d.view2_camera_address(), kRecordVtable, one);
  RecordConstruct<schema::ViewCamera2>(d.view2_camera2_address(), kRecordVtable, one);
  RecordConstruct<schema::ViewCamera1>(d.view1_camera_address(), kRecord11856Vtable, one);
  const schema::ChaseCamera inner(d.view0_camera_address());
  inner.set_vehicle(0);
  inner.set_viewport(0u);
  StoreOnes(inner.tracker_address(), one);
  inner.set_vtable(kMember43136Vtable);
  guest::StoreU32(inner.unknown_364_address(), one);
  Member13360Construct(d.tv_camera_address());
  Member56896Construct(d.cutscene_address());
  Member67760Construct(d.intro_camera_address());
  // Not in CameraDirector yet (camera-modes.toml): the 1.0s at +69628, every
  // 1,104 bytes, and the words at +77376 to +77388.
  for (uint32_t i = 0; i < 8; i++) {
    guest::StoreU32(member + 69628 + kRecordBytes * i, one);
  }
  guest::StoreU32(member + 77380, 0);
  guest::StoreU32(member + 77384, 0);
  guest::StoreU32(member + 77388, 0);
  guest::StoreU32(member + 77376, 0xFFFFFFFF);
}

void RaceModeConstruct(uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_821073B8 RaceMode::RaceMode");
  ModeBaseConstruct(mode, schema::RaceMode::kVtable, kRaceModeVtable32);
  const schema::RaceMode m(mode);
  MachineConstruct(m.course_setup_address());
  RaceModeMemberConstruct(m.camera_director_address());
  MachineConstruct(m.race_scene_address());
  MachineConstruct(m.race_flow_address());
  MachineConstruct(m.race_states_address());
  MachineConstruct(m.race_world_address());
  MachineConstruct(m.race_hud_machine_address());
  MachineConstruct(m.race_sounds_address());
  models::XprConstruct(m.xpr_address());
  MachineConstruct(m.overlays_machine_address());
  race_overlays::RaceOverlaysClear(m.overlays_address());
  MachineConstruct(m.machine_157652_address());
}

void MachineMemberConstruct(uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_82108160 MachineMember::MachineMember");
  MachineConstruct(machine);
}

namespace {

#define ITC_MODE_TYPE(address, type, vtable, vtable32, word848)                  \
  void ModeType##type##Construct(uint32_t mode) {                \
    ITC_NATIVE_RUNNING("sub_" #address " ModeType" #type "::ModeType" #type);    \
    ModeBaseConstruct(mode, vtable, vtable32, word848);                 \
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

}  // namespace

void ModeType3Construct(uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_82106C80 ModeType3::ModeType3");
  ModeBaseConstruct(mode, 0x82006B70, 0x82006B64, false);
  MachineMemberConstruct(mode + 848);
}

void ModeTypeConstruct(uint32_t type, uint32_t mode) {
  switch (type) {
    case 2: ModeType2Construct(mode); break;
    case 3: ModeType3Construct(mode); break;
    case 4: ModeType4Construct(mode); break;
    case 5: ModeType5Construct(mode); break;
    case 6: ModeType6Construct(mode); break;
    case 7: ModeType7Construct(mode); break;
    case 9: ModeType9Construct(mode); break;
    case 10: ModeType10Construct(mode); break;
    case 12:
      // Not made without --multiplayer_menu_items (the bypass, 2026-11-14:
      // only the main menu's Xbox Live item opened it; ModeCreate declines
      // it then).
      if (ITC_FLAG(multiplayer_menu_items)) {
        ModeType12Construct(mode);
      }
      break;
    default: break;
  }
}

namespace {

// A mode in host memory (E5 step 4): its schema type's New (the race mode
// and the 852-byte ones).
uint32_t NewMode(uint32_t type) {
  switch (type) {
    case 1: return schema::RaceMode::New().address;
    case 2: return schema::ModeType2::New().address;
    case 4: return schema::ModeType4::New().address;
    case 5: return schema::ModeType5::New().address;
    case 6: return schema::GameModeType6::New().address;
    case 7: return schema::GameModeType7::New().address;
    case 9: return schema::ModeType9::New().address;
    case 10: return schema::ModeType10::New().address;
    case 12: return schema::ModeType12::New().address;
    default: return 0;
  }
}

}  // namespace

uint32_t ModeBytes(uint32_t type) {
  switch (type) {
    case 1: return kRaceModeBytes;
    case 3: return kMode3Bytes;
    case 8: return 87768;
    case 2: case 4: case 5: case 6: case 7: case 9: case 10: case 12: return kModeBytes;
    default: return 0;
  }
}

std::optional<uint32_t> ModeCreate(uint32_t type) {
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
  // The boot mode (type 3) in guest memory, the race mode and the 852-byte
  // ones in host memory.
  const auto mode = type == 1 || bytes == kModeBytes
                        ? std::optional<uint32_t>(NewMode(type))
                        : memory::crt::OperatorNew(bytes);
  if (!mode) {
    return std::nullopt;
  }
  if (*mode == 0) {
    return 0;
  }
  if (type == 1) {
    RaceModeConstruct(*mode);
  } else {
    ModeTypeConstruct(type, *mode);
  }
  return *mode;
}

}  // namespace game_modes
