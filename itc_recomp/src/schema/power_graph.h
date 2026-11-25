// The views of docs/functions/power-graph.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/power-graph.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// The image words docs/functions/power-graph.md's code uses.
struct PowerGraphGlobals {
  // the line recorder's render queue, stored by every Line::RecordFlat
  static uint32_t line_queue() { return guest::LoadU32(0x826D83D8u); }
  static void set_line_queue(uint32_t v) { guest::StoreU32(0x826D83D8u, v); }
  static constexpr uint32_t line_queue_address() { return 0x826D83D8u; }
  // the line recorder's command list, stored by every Line::RecordFlat
  static uint32_t line_list() { return guest::LoadU32(0x826D83DCu); }
  static void set_line_list(uint32_t v) { guest::StoreU32(0x826D83DCu, v); }
  static constexpr uint32_t line_list_address() { return 0x826D83DCu; }
  // Primitive::RecordTransform's command list
  static uint32_t transform_list() { return guest::LoadU32(0x826D83D4u); }
  static void set_transform_list(uint32_t v) { guest::StoreU32(0x826D83D4u, v); }
  static constexpr uint32_t transform_list_address() { return 0x826D83D4u; }
  // the front end's car holder; its +0 word is PowerGraph::Draw's r3
  static uint32_t stand_holder() { return guest::LoadU32(0x826D83E0u); }
  static void set_stand_holder(uint32_t v) { guest::StoreU32(0x826D83E0u, v); }
  static constexpr uint32_t stand_holder_address() { return 0x826D83E0u; }
  // the car list's graph drawn while set (power-graph-screen.md)
  static uint32_t graph_shown() { return guest::LoadU32(0x82815EC0u); }
  static void set_graph_shown(uint32_t v) { guest::StoreU32(0x82815EC0u, v); }
  static constexpr uint32_t graph_shown_address() { return 0x82815EC0u; }
};

}  // namespace schema
