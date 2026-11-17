// The views of docs/functions/game-modes.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/game-modes.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: -
struct StateMachine : View {
  using View::View;
  static constexpr uint32_t kSize = 9616;
  // 200 transitions, 48 bytes each (key, functor, next state)
  uint32_t transitions_address() const { return address + 0; }
  uint32_t transition_count() const { return guest::LoadU32(base, address + 9600); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(base, address + 9600, v); }
  uint32_t transition_count_address() const { return address + 9600; }
  uint32_t state() const { return guest::LoadU32(base, address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // set once a mode's movies are loaded (the modes' WaitForMovies)
  uint32_t loaded() const { return guest::LoadU32(base, address + 9608); }
  void set_loaded(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t loaded_address() const { return address + 9608; }
  uint32_t reenter() const { return guest::LoadU32(base, address + 9612); }
  void set_reenter(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t reenter_address() const { return address + 9612; }
};

}  // namespace schema
