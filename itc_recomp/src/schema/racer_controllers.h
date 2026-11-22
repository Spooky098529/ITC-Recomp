// The views of docs/functions/racer-controllers.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/racer-controllers.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: RaceFlow
struct RaceFlow : View {
  using View::View;
  static constexpr uint32_t kSize = 9620;
  // 200 transitions, 48 bytes each (StateTransition, game-modes.toml); 53 added by RaceFlow::Init
  uint32_t transitions_address() const { return address + 0; }
  uint32_t transition_count() const { return guest::LoadU32(base, address + 9600); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(base, address + 9600, v); }
  uint32_t transition_count_address() const { return address + 9600; }
  // 1 at set-up, 3 from RaceFlow::OnAbort (sub_8215F7B0)
  uint32_t state() const { return guest::LoadU32(base, address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // bit 0x1 set by RaceFlow::OnSetUp (unless the stage is of type 2 or 3, or kind 6's stage word +0 isn't 1)
  uint32_t flags() const { return guest::LoadU32(base, address + 9608); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t flags_address() const { return address + 9608; }
  // the player of the human car whose vehicle flags (+6148) have 0x1 (0)
  uint32_t player_a() const { return guest::LoadU32(base, address + 9612); }
  void set_player_a(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t player_a_address() const { return address + 9612; }
  // the player of the human car whose vehicle flags have 0x80 (1, or 0 when the race data's +0xA3AF0 is 6)
  uint32_t player_b() const { return guest::LoadU32(base, address + 9616); }
  void set_player_b(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t player_b_address() const { return address + 9616; }
};

// embedded; measure: RaceFlow::OnState23.r4, PlayerControl::ReadPad.r6
struct RacerControlsDispatcher : View {
  using View::View;
  static constexpr uint32_t kSize = 812;
  // 50 handler pointers (state machines)
  uint32_t handlers_address() const { return address + 0; }
  uint32_t handler_count() const { return guest::LoadU32(base, address + 200); }
  void set_handler_count(uint32_t v) const { guest::StoreU32(base, address + 200, v); }
  uint32_t handler_count_address() const { return address + 200; }
  // 50 messages of 12 bytes (ModeMessage, game-modes.toml): 29 and 30 the pause (probably), 34 the replay's end posted here
  uint32_t ring_address() const { return address + 204; }
  uint32_t ring_count() const { return guest::LoadU32(base, address + 804); }
  void set_ring_count(uint32_t v) const { guest::StoreU32(base, address + 804, v); }
  uint32_t ring_count_address() const { return address + 804; }
  // the ring's first slot
  uint32_t ring_start() const { return guest::LoadU32(base, address + 808); }
  void set_ring_start(uint32_t v) const { guest::StoreU32(base, address + 808, v); }
  uint32_t ring_start_address() const { return address + 808; }
};

// embedded; measure: Pad
struct PlayerMapperEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // the player's action mapper (+2888 the pad's player)
  template <typename T = ActionMapper>
  T mapper() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_mapper(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_mapper(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t mapper_address() const { return address + 0; }
  // 1 when the mapper isn't read (PlayerInput::Read answers 0)
  uint32_t unread() const { return guest::LoadU32(base, address + 4); }
  void set_unread(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unread_address() const { return address + 4; }
};

// embedded; measure: PlayerInput::Read.r6, PlayerInput::Control.r6
struct PlayerInputValue : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // kinds 0 to 2 a byte made a word (held, pressed, fired), 3 the analog word (signed)
  uint32_t value() const { return guest::LoadU32(base, address + 0); }
  void set_value(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
  // kind 4's float
  float value_float() const { return guest::LoadFloat(base, address + 0); }
  void set_value_float(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t value_float_address() const { return address + 0; }
};

// embedded; measure: AiDriver::Setup.r5
struct AiDriverSetupHold : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // not 0: AiDriver::ResetControl (the driver held); 0: AiDriver::StartRace
  uint32_t hold() const { return guest::LoadU32(base, address + 0); }
  void set_hold(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t hold_address() const { return address + 0; }
};

// embedded; measure: RaceMode@+97548
struct RaceFlowAt97548 : View {
  using View::View;
  static constexpr uint32_t kSize = 9620;
  uint32_t unknown_9604() const { return guest::LoadU32(base, address + 9604); }
  void set_unknown_9604(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t unknown_9604_address() const { return address + 9604; }
  uint32_t unknown_9608() const { return guest::LoadU32(base, address + 9608); }
  void set_unknown_9608(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t unknown_9608_address() const { return address + 9608; }
  uint32_t unknown_9612() const { return guest::LoadU32(base, address + 9612); }
  void set_unknown_9612(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t unknown_9612_address() const { return address + 9612; }
  uint32_t unknown_9616() const { return guest::LoadU32(base, address + 9616); }
  void set_unknown_9616(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t unknown_9616_address() const { return address + 9616; }
};

// The image words docs/functions/racer-controllers.md's code uses.
struct RacerControllersGlobals {
  // the race data (a pointer): +0x59620 the stages, +0x59AE4 the flags, +0xA3AD8 a finish count, +0xA3AF0 the kind (mode)
  static uint32_t race_data(uint8_t* base) { return guest::LoadU32(base, 0x826C5B2Cu); }
  static void set_race_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B2Cu, v); }
  static constexpr uint32_t race_data_address() { return 0x826C5B2Cu; }
  // seven pointers to the cars' 20-byte input records (CarInput), by the racer's index; CarInputs::Create and Destroy
  static constexpr uint32_t input_records_address() { return 0x826C5B30u; }
  // the input recordings (a pointer): 0x107AC4 bytes a player, 54,000 records of 20 bytes, the count at +0x107AC0
  static uint32_t input_recordings(uint8_t* base) { return guest::LoadU32(base, 0x826C5B4Cu); }
  static void set_input_recordings(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B4Cu, v); }
  static constexpr uint32_t input_recordings_address() { return 0x826C5B4Cu; }
  // the racers' manager (a pointer): +2308 the racers' records, +2364 their count
  static uint32_t race(uint8_t* base) { return guest::LoadU32(base, 0x826D82F4u); }
  static void set_race(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D82F4u, v); }
  static constexpr uint32_t race_address() { return 0x826D82F4u; }
  // the input manager (a pointer; ActionMappers::Init's object): slot 9 (+36) finds the pad object
  static uint32_t input_manager(uint8_t* base) { return guest::LoadU32(base, 0x826BAB78u); }
  static void set_input_manager(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BAB78u, v); }
  static constexpr uint32_t input_manager_address() { return 0x826BAB78u; }
  // four 8-byte PlayerMapperEntry
  static constexpr uint32_t mapper_entries_address() { return 0x826BAB80u; }
  // each player's mapper entry index
  static constexpr uint32_t player_entries_address() { return 0x826BABA4u; }
  // the pad blocked: ReadPad posts the pause (or returns) when either word is set
  static uint32_t pad_blocked0(uint8_t* base) { return guest::LoadU32(base, 0x826C7960u); }
  static void set_pad_blocked0(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C7960u, v); }
  static constexpr uint32_t pad_blocked0_address() { return 0x826C7960u; }
  static uint32_t pad_blocked1(uint8_t* base) { return guest::LoadU32(base, 0x826C7964u); }
  static void set_pad_blocked1(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C7964u, v); }
  static constexpr uint32_t pad_blocked1_address() { return 0x826C7964u; }
  // 1.0: ReadPad's speed threshold
  static float one(uint8_t* base) { return guest::LoadFloat(base, 0x82000A30u); }
  static void set_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // 0.0
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
};

}  // namespace schema
