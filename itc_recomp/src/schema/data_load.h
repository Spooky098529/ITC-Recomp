// The views of docs/functions/data-load.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/data-load.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// file; measure: Bits224
struct Bits224 : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  // 7 big-endian words, indexed by the bit
  uint32_t words_address() const { return address + 0; }
};

// file; measure: Bits480
struct Bits480 : View {
  using View::View;
  static constexpr uint32_t kSize = 60;
  // 15 big-endian words, indexed by the bit
  uint32_t words_address() const { return address + 0; }
};

// The image words docs/functions/data-load.md's code uses.
struct DataLoadGlobals {
  // 1.0 (.rdata), loaded with lfs by the records' constructors
  static float one() { return guest::LoadFloat(0x82000A30u); }
  static void set_one(float v) { guest::StoreFloat(0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // 0.0 (.rdata)
  static float zero() { return guest::LoadFloat(0x82000A34u); }
  static void set_zero(float v) { guest::StoreFloat(0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // a Bits480: Defaults::Table3044 (save-data.md); fixed bits, then the first bit of most of the 48 sets' ranges
  static constexpr uint32_t defaults_3044_address() { return 0x826C3044u; }
  // 48 Bits480, 60 bytes apart: a range of bits each by its index (none for 29)
  static constexpr uint32_t defaults_sets_address() { return 0x826C3080u; }
  // a Bits224: Defaults::Table3C24, bits 0-60
  static constexpr uint32_t defaults_3c24_address() { return 0x826C3C24u; }
  // a Bits480: ORed with sets 17, 35, 38, 41 and 47, Bits480::Fill, then cleared (it ends 0); Defaults::Init's r3
  static constexpr uint32_t defaults_4064_address() { return 0x826C4064u; }
  // a Bits480: Defaults::Table50A8
  static constexpr uint32_t defaults_50a8_address() { return 0x826C50A8u; }
  // a Bits224: Defaults::Table50E4, Bits224::Fill (bits 0-198)
  static constexpr uint32_t defaults_50e4_address() { return 0x826C50E4u; }
  // the race data (0xA3B00 bytes, race-setup.md), made by RaceData::Create
  static View race_data() { return View(guest::LoadU32(0x826C5B2Cu)); }
  static void set_race_data(uint32_t v) { guest::StoreU32(0x826C5B2Cu, v); }
  static constexpr uint32_t race_data_address() { return 0x826C5B2Cu; }
  // the input recordings (0x2225C0 bytes), made by InputRecordings::Create
  static View input_recordings() { return View(guest::LoadU32(0x826C5B4Cu)); }
  static void set_input_recordings(uint32_t v) { guest::StoreU32(0x826C5B4Cu, v); }
  static constexpr uint32_t input_recordings_address() { return 0x826C5B4Cu; }
  // the replay kept after a race, probably (0x13C8F0 bytes), made by Replay::Create
  static View replay() { return View(guest::LoadU32(0x826C5B50u)); }
  static void set_replay(uint32_t v) { guest::StoreU32(0x826C5B50u, v); }
  static constexpr uint32_t replay_address() { return 0x826C5B50u; }
  // BootData::Load's once flag
  static uint32_t loaded() { return guest::LoadU32(0x826C5B8Cu); }
  static void set_loaded(uint32_t v) { guest::StoreU32(0x826C5B8Cu, v); }
  static constexpr uint32_t loaded_address() { return 0x826C5B8Cu; }
  // the app's render interface: its slot 8's answer's slots 1 and 2 are the command list and the renderer
  static View render_manager() { return View(guest::LoadU32(0x826BABA0u)); }
  static void set_render_manager(uint32_t v) { guest::StoreU32(0x826BABA0u, v); }
  static constexpr uint32_t render_manager_address() { return 0x826BABA0u; }
  // the loading screen's shown flag
  static uint32_t loading_shown() { return guest::LoadU32(0x82815E60u); }
  static void set_loading_shown(uint32_t v) { guest::StoreU32(0x82815E60u, v); }
  static constexpr uint32_t loading_shown_address() { return 0x82815E60u; }
  // the loading screen's UI movies (40,976 bytes each), up to 16
  static constexpr uint32_t loading_movies_address() { return 0x82815E68u; }
  // the loading screen's movie count (the list's +64)
  static uint32_t loading_movie_count() { return guest::LoadU32(0x82815EA8u); }
  static void set_loading_movie_count(uint32_t v) { guest::StoreU32(0x82815EA8u, v); }
  static constexpr uint32_t loading_movie_count_address() { return 0x82815EA8u; }
  // the other UI movie set's movies (sub_822912A0's list), up to 16
  static constexpr uint32_t other_movies_address() { return 0x82816610u; }
  // the other set's movie count (the list's +64)
  static uint32_t other_movie_count() { return guest::LoadU32(0x82816650u); }
  static void set_other_movie_count(uint32_t v) { guest::StoreU32(0x82816650u, v); }
  static constexpr uint32_t other_movie_count_address() { return 0x82816650u; }
  // the other set's three asset ids (22560, 22595, 22559 in the image)
  static constexpr uint32_t other_movie_ids_address() { return 0x82638134u; }
  // 1 in the image, 0 after sub_82291328; read by the other set's users (not read)
  static uint32_t unknown_82638130() { return guest::LoadU32(0x82638130u); }
  static void set_unknown_82638130(uint32_t v) { guest::StoreU32(0x82638130u, v); }
  static constexpr uint32_t unknown_82638130_address() { return 0x82638130u; }
  // nine words sub_82291328 clears; the other set's users' (not read)
  static constexpr uint32_t unknown_828165D0_address() { return 0x828165D0u; }
  // cleared by sub_82291328; the other set's users' (not read)
  static uint32_t unknown_82816600() { return guest::LoadU32(0x82816600u); }
  static void set_unknown_82816600(uint32_t v) { guest::StoreU32(0x82816600u, v); }
  static constexpr uint32_t unknown_82816600_address() { return 0x82816600u; }
  // a static object with Replay::Create's layout (its seven records at +0x2BF10); Replay826D8520::StaticInit
  static constexpr uint32_t static_replay_a_address() { return 0x826D8520u; }
  // the same at 0x82816E50; Replay82816E50::StaticInit
  static constexpr uint32_t static_replay_b_address() { return 0x82816E50u; }
};

}  // namespace schema
