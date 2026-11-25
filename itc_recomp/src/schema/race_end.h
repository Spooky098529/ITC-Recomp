// The views of docs/functions/race-end.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/race-end.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// The image words docs/functions/race-end.md's code uses.
struct RaceEndGlobals {
  // the race data: +0x59AE4 the flags (0x8 an event race, 0x10 a rival race, 0x8000 to the title, 0x10000 to type 12), +0xA3AF0 the kind of race, +0xA3AF4 the race mode
  static View race_data() { return View(guest::LoadU32(0x826C5B2Cu)); }
  static void set_race_data(uint32_t v) { guest::StoreU32(0x826C5B2Cu, v); }
  static constexpr uint32_t race_data_address() { return 0x826C5B2Cu; }
  // which front-end menu to show on return, probably: set to 1 when the race goes to the title
  static uint32_t title_flag() { return guest::LoadU32(0x826364ECu); }
  static void set_title_flag(uint32_t v) { guest::StoreU32(0x826364ECu, v); }
  static constexpr uint32_t title_flag_address() { return 0x826364ECu; }
  // the network session, 0 when none (0 in play): kind 5 goes to type 12 with one and race mode 7, else type 10
  static View session() { return View(guest::LoadU32(0x82816E3Cu)); }
  static void set_session(uint32_t v) { guest::StoreU32(0x82816E3Cu, v); }
  static constexpr uint32_t session_address() { return 0x82816E3Cu; }
  // the passes word ScreenFades::FadeIn is given
  static uint32_t fade_in_passes() { return guest::LoadU32(0x82001BECu); }
  static void set_fade_in_passes(uint32_t v) { guest::StoreU32(0x82001BECu, v); }
  static constexpr uint32_t fade_in_passes_address() { return 0x82001BECu; }
  // the boot movies' five ids (11 ending the list): stages 6 and 8 queue two
  static constexpr uint32_t boot_movie_ids_address() { return 0x826C5BACu; }
  // the mode type after the boot movies (9, the career's result)
  static uint32_t boot_movies_next_mode() { return guest::LoadU32(0x826C5BC0u); }
  static void set_boot_movies_next_mode(uint32_t v) { guest::StoreU32(0x826C5BC0u, v); }
  static constexpr uint32_t boot_movies_next_mode_address() { return 0x826C5BC0u; }
  // 1: a button skips the movies
  static uint32_t boot_movies_skippable() { return guest::LoadU32(0x826C5BC4u); }
  static void set_boot_movies_skippable(uint32_t v) { guest::StoreU32(0x826C5BC4u, v); }
  static constexpr uint32_t boot_movies_skippable_address() { return 0x826C5BC4u; }
  // the index of the next movie (0)
  static uint32_t boot_movies_index() { return guest::LoadU32(0x826C5BC8u); }
  static void set_boot_movies_index(uint32_t v) { guest::StoreU32(0x826C5BC8u, v); }
  static constexpr uint32_t boot_movies_index_address() { return 0x826C5BC8u; }
};

}  // namespace schema
