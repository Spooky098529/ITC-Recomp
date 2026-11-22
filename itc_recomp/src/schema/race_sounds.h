// The views of docs/functions/race-sounds.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/race-sounds.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: RaceSounds
struct RaceSounds : View {
  using View::View;
  static constexpr uint32_t kSize = 9660;
  // 200 transitions, 48 bytes each (StateMachine's)
  uint32_t transitions_address() const { return address + 0; }
  uint32_t transition_count() const { return guest::LoadU32(base, address + 9600); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(base, address + 9600, v); }
  uint32_t transition_count_address() const { return address + 9600; }
  // 1 at set-up; 38 before the countdown, 23 racing against a rival, 24 and 25 paused, 30 after the race, 17 the next stage, 10 a rolling start, 32 aborted, 37 the end, 35 unloaded (probably)
  uint32_t state() const { return guest::LoadU32(base, address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // the effects' wave bank view (a CarSoundWaveBankView; XPR::XPR by RaceMode::RaceMode)
  uint32_t effects_view_address() const { return address + 9608; }
  // the effects' wave bank buffer (asset 22889, XPhysicalAlloc by Begin)
  template <typename T = CarSoundWaveBank>
  T effects_bank() const { return T(base, guest::LoadU32(base, address + 9632)); }
  void set_effects_bank(const View& v) const { guest::StoreU32(base, address + 9632, v.address); }
  void set_effects_bank(uint32_t v) const { guest::StoreU32(base, address + 9632, v); }
  uint32_t effects_bank_address() const { return address + 9632; }
  // 0 at Begin; not read here
  uint32_t unknown_9636() const { return guest::LoadU32(base, address + 9636); }
  void set_unknown_9636(uint32_t v) const { guest::StoreU32(base, address + 9636, v); }
  uint32_t unknown_9636_address() const { return address + 9636; }
  // 1 while the car sounds are paused for the system UI
  uint32_t cars_paused() const { return guest::LoadU32(base, address + 9640); }
  void set_cars_paused(uint32_t v) const { guest::StoreU32(base, address + 9640, v); }
  uint32_t cars_paused_address() const { return address + 9640; }
  // 0x2 the music bank released, 0x4 load the next music, 0x10 its cue 0 started, 0x20 faded and stopped, 0x40 to 0x2000 the next music's pick, 0x4000 the cars' sounds stopped, 0x8000 fade the music out, 0x10000 only cleared, 0x20000 the cars' sounds set up
  uint32_t flags() const { return guest::LoadU32(base, address + 9644); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 9644, v); }
  uint32_t flags_address() const { return address + 9644; }
  // the cars' sounds step: 1 to set up, 2 running
  uint32_t cars_step() const { return guest::LoadU32(base, address + 9648); }
  void set_cars_step(uint32_t v) const { guest::StoreU32(base, address + 9648, v); }
  uint32_t cars_step_address() const { return address + 9648; }
  // the race kind (the race data's +0xA3AF0): 0 the attract race, 1 to 3 and 6 the race HUD's, 4 probably the split screen, 5 the alternate HUD's
  uint32_t kind() const { return guest::LoadU32(base, address + 9652); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 9652, v); }
  uint32_t kind_address() const { return address + 9652; }
  // ticks (10 after the cars' set-up) before UpdateCars gives the cars their banks again
  uint32_t set_up_wait() const { return guest::LoadU32(base, address + 9656); }
  void set_set_up_wait(uint32_t v) const { guest::StoreU32(base, address + 9656, v); }
  uint32_t set_up_wait_address() const { return address + 9656; }
};

// embedded; measure: RaceMode@+138284
struct RaceSoundsAt138284 : View {
  using View::View;
  static constexpr uint32_t kSize = 9656;
  uint32_t unknown_9604() const { return guest::LoadU32(base, address + 9604); }
  void set_unknown_9604(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t unknown_9604_address() const { return address + 9604; }
  uint32_t unknown_9652() const { return guest::LoadU32(base, address + 9652); }
  void set_unknown_9652(uint32_t v) const { guest::StoreU32(base, address + 9652, v); }
  uint32_t unknown_9652_address() const { return address + 9652; }
};

// The image words docs/functions/race-sounds.md's code uses.
struct RaceSoundsGlobals {
  // car i's two 24-byte wave bank views at +8i and +8i+4 (CarSoundWaveBankView pointers)
  static constexpr uint32_t car_views_address() { return 0x826D82FCu; }
  // the scene models' sound slots' load callback contexts (seven words)
  static constexpr uint32_t slot_contexts_address() { return 0x826D8334u; }
  // the files arrived, a byte a slot (counts to 2)
  static constexpr uint32_t slot_files_arrived_address() { return 0x826D8350u; }
  // the slots' first buffers (0x5D2D0 bytes; seven words)
  static constexpr uint32_t slot_buffers_0_address() { return 0x826D8358u; }
  // the slots' second buffers (0x6B080 bytes; seven words)
  static constexpr uint32_t slot_buffers_1_address() { return 0x826D8374u; }
  // 1 when asked (seven words)
  static constexpr uint32_t slot_asked_address() { return 0x826D8390u; }
  // 1 when both files arrived (seven words)
  static constexpr uint32_t slot_arrived_address() { return 0x826D83ACu; }
  // 1 once the effects' wave bank has arrived (OnEffectsBankLoaded; 0 at Begin)
  static uint32_t effects_bank_arrived(uint8_t* base) { return guest::LoadU32(base, 0x826D83C8u); }
  static void set_effects_bank_arrived(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D83C8u, v); }
  static constexpr uint32_t effects_bank_arrived_address() { return 0x826D83C8u; }
  // the race's 13 music names (pointers into 0x82006A18)
  static constexpr uint32_t music_names_address() { return 0x826193D4u; }
  // the music's volume ("Music")
  static float music_volume(uint8_t* base) { return guest::LoadFloat(base, 0x82616BECu); }
  static void set_music_volume(uint8_t* base, float v) { guest::StoreFloat(base, 0x82616BECu, v); }
  static constexpr uint32_t music_volume_address() { return 0x82616BECu; }
  // the music's volume kept before a fade
  static float kept_music_volume(uint8_t* base) { return guest::LoadFloat(base, 0x82616C34u); }
  static void set_kept_music_volume(uint8_t* base, float v) { guest::StoreFloat(base, 0x82616C34u, v); }
  static constexpr uint32_t kept_music_volume_address() { return 0x82616C34u; }
  // the effects' volume option's gain
  static float effects_volume(uint8_t* base) { return guest::LoadFloat(base, 0x82616C38u); }
  static void set_effects_volume(uint8_t* base, float v) { guest::StoreFloat(base, 0x82616C38u, v); }
  static constexpr uint32_t effects_volume_address() { return 0x82616C38u; }
  // the effects' volume option's gain (put back into effects_volume)
  static float effects_volume_2(uint8_t* base) { return guest::LoadFloat(base, 0x82616C3Cu); }
  static void set_effects_volume_2(uint8_t* base, float v) { guest::StoreFloat(base, 0x82616C3Cu, v); }
  static constexpr uint32_t effects_volume_2_address() { return 0x82616C3Cu; }
};

}  // namespace schema
