// The views of docs/functions/achievements.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/achievements.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: Time::Less.r4
struct AchievementsPackedTime : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t packed() const { return guest::LoadU32(base, address + 0); }
  void set_packed(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t packed_address() const { return address + 0; }
};

// embedded; measure: Achievements::Check.r4
struct AchievementFlag : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // 1 when the pair of the check's kind was earned, else 0
  uint32_t earned() const { return guest::LoadU32(base, address + 0); }
  void set_earned(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t earned_address() const { return address + 0; }
};

// image; measure: -
struct AchievementPair : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // 0 items beaten, 1 a course time, 2 a count of the player's
  int32_t kind() const { return int32_t(guest::LoadU32(base, address + 0)); }
  void set_kind(int32_t v) const { guest::StoreU32(base, address + 0, uint32_t(v)); }
  uint32_t kind_address() const { return address + 0; }
  // 2 to 28, in order
  uint32_t id() const { return guest::LoadU32(base, address + 4); }
  void set_id(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t id_address() const { return address + 4; }
};

// image; measure: -
struct AchievementCourseTarget : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  int32_t route() const { return int32_t(guest::LoadU32(base, address + 0)); }
  void set_route(int32_t v) const { guest::StoreU32(base, address + 0, uint32_t(v)); }
  uint32_t route_address() const { return address + 0; }
  // a packed time
  uint32_t time() const { return guest::LoadU32(base, address + 4); }
  void set_time(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t time_address() const { return address + 4; }
};

// image; measure: -
struct AchievementEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // player 0's controller's user ([0x826BABA4])
  uint32_t user() const { return guest::LoadU32(base, address + 0); }
  void set_user(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t user_address() const { return address + 0; }
  uint32_t id() const { return guest::LoadU32(base, address + 4); }
  void set_id(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t id_address() const { return address + 4; }
};

// image; measure: -
struct AchievementOverlapped : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  // InternalLow: the result
  uint32_t internal_low() const { return guest::LoadU32(base, address + 0); }
  void set_internal_low(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t internal_low_address() const { return address + 0; }
  // InternalHigh
  uint32_t internal_high() const { return guest::LoadU32(base, address + 4); }
  void set_internal_high(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t internal_high_address() const { return address + 4; }
  // InternalContext
  uint32_t internal_context() const { return guest::LoadU32(base, address + 8); }
  void set_internal_context(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t internal_context_address() const { return address + 8; }
  // hEvent
  uint32_t event() const { return guest::LoadU32(base, address + 12); }
  void set_event(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t event_address() const { return address + 12; }
  // pCompletionRoutine
  uint32_t completion_routine() const { return guest::LoadU32(base, address + 16); }
  void set_completion_routine(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t completion_routine_address() const { return address + 16; }
  // dwCompletionContext
  uint32_t completion_context() const { return guest::LoadU32(base, address + 20); }
  void set_completion_context(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t completion_context_address() const { return address + 20; }
  // dwExtendedError
  uint32_t extended_error() const { return guest::LoadU32(base, address + 24); }
  void set_extended_error(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t extended_error_address() const { return address + 24; }
};

// The image words docs/functions/achievements.md's code uses.
struct AchievementsGlobals {
  // the first of the 27 AchievementPair (its kind)
  static int32_t table(uint8_t* base) { return int32_t(guest::LoadU32(base, 0x82011FB8u)); }
  static void set_table(uint8_t* base, int32_t v) { guest::StoreU32(base, 0x82011FB8u, uint32_t(v)); }
  static constexpr uint32_t table_address() { return 0x82011FB8u; }
  // read as this + 4 x the pair's index (23 to 26 land on the thresholds at 0x820120D0: 1, 10, 50, 100)
  static uint32_t thresholds(uint8_t* base) { return guest::LoadU32(base, 0x82012074u); }
  static void set_thresholds(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82012074u, v); }
  static constexpr uint32_t thresholds_address() { return 0x82012074u; }
  // the first of the 8 AchievementCourseTarget (its route)
  static int32_t course_targets(uint8_t* base) { return int32_t(guest::LoadU32(base, 0x82012090u)); }
  static void set_course_targets(uint8_t* base, int32_t v) { guest::StoreU32(base, 0x82012090u, uint32_t(v)); }
  static constexpr uint32_t course_targets_address() { return 0x82012090u; }
  // player 0's controller's user
  static uint32_t player_user(uint8_t* base) { return guest::LoadU32(base, 0x826BABA4u); }
  static void set_player_user(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BABA4u, v); }
  static constexpr uint32_t player_user_address() { return 0x826BABA4u; }
  // the user table's (0x826C7670) +364: a word per signed-in slot, probably the network battles' wins
  static uint32_t user_counts(uint8_t* base) { return guest::LoadU32(base, 0x826C77DCu); }
  static void set_user_counts(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C77DCu, v); }
  static constexpr uint32_t user_counts_address() { return 0x826C77DCu; }
  // the race's progress (+6076 the route, +2204 the player's time)
  static View progress(uint8_t* base) { return View(base, guest::LoadU32(base, 0x826D82E8u)); }
  static void set_progress(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D82E8u, v); }
  static constexpr uint32_t progress_address() { return 0x826D82E8u; }
  // the first of the write's 27 AchievementOverlapped (its InternalLow)
  static uint32_t overlappeds(uint8_t* base) { return guest::LoadU32(base, 0x828161F8u); }
  static void set_overlappeds(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828161F8u, v); }
  static constexpr uint32_t overlappeds_address() { return 0x828161F8u; }
  // the first of the write's 27 AchievementEntry (its user)
  static uint32_t entries(uint8_t* base) { return guest::LoadU32(base, 0x828164F0u); }
  static void set_entries(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828164F0u, v); }
  static constexpr uint32_t entries_address() { return 0x828164F0u; }
};

}  // namespace schema
