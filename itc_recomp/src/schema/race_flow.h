// The views of docs/functions/race-flow.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/race-flow.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// The image words docs/functions/race-flow.md's code uses.
struct RaceFlowGlobals {
  // the race data (a pointer): +0xA3AF0 the kind of race (6: both human cars' players 0)
  static uint32_t race_data(uint8_t* base) { return guest::LoadU32(base, 0x826C5B2Cu); }
  static void set_race_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B2Cu, v); }
  static constexpr uint32_t race_data_address() { return 0x826C5B2Cu; }
};

}  // namespace schema
