// The views of docs/functions/race-hud.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/race-hud.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: sub_82212958.ret
struct RaceHudRunMovieTail : View {
  using View::View;
  static constexpr uint32_t kSize = 636;
  // really RaceHud's +632: the UI movie run in states 2 and 4 (RaceHud::RunMovie)
  uint32_t state2_movie() const { return guest::LoadU32(address + 632); }
  void set_state2_movie(uint32_t v) const { guest::StoreU32(address + 632, v); }
  uint32_t state2_movie_address() const { return address + 632; }
};

}  // namespace schema
