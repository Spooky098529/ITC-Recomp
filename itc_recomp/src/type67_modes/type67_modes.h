// The type 6 and 7 game modes natively (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/type6-type7-modes.md, the layouts in
// docs/known-data.md: "type 6 screens", "type 7 screens", "type 6 and 7
// modes"): the front-end hubs over the player's car on the stand, their
// screens' state machines and handlers. Plain C++ over guest memory (the
// modes, their screen blocks and machines are shared with the recompiled
// code), with the originals' loads and stores in their order. Where an
// original builds values in its stack frame (the transitions' functors and
// keys), the stores are replayed in its frame at `frame` (its r1 after its
// stwu), so stack words it never writes reach the same places; its back
// chain and saved registers aren't written. Adapters:
// src/hooks/type67_modes.cpp (behind --native_type67_modes), register steps
// shared with the harness in src/hooks/type67_modes.h.

#pragma once

#include <cstdint>

namespace type67_modes {

// --- The state machine Inits (inits.cpp): each adds its machine's
// transitions through StateMachine::Add's plain C++, as the original builds
// them in its frame (the step tables in init_tables.h). `object` is the
// original's r3; each returns the last Add's answer (`replaced`: the index,
// else the new transition) and `machine` the machine it was added to. ---

struct InitResult {
  uint32_t r3;
  uint32_t machine;
  bool replaced;
};

// An Init: its address, name, frame (its stwu size) and the registers
// __savegprlr_<n> saves from (r<n> to r31 and lr: (32 - n) * 8 + 8 bytes
// below the caller's r1).
struct InitInfo {
  uint32_t address;
  const char* name;
  uint32_t frame;
  uint32_t first_saved;
};

// sub_82235138 Type6Screen0::Init (r3 the screen, its machine at +0),
// sub_822454B8 Type7Screen0::Init (machine at +4), sub_822329A8
// Type6Dialog::Init, sub_82242420 Type7Dialog::Init, sub_821C9918
// Type6ControllerOptions::Init, sub_821C9430 Type7ControllerOptions::Init,
// sub_821CB6E8 Type6Options::Init, sub_821CB090 Type7Options::Init,
// sub_821A8210 Type6SaveScreen::Init (all on r3), and the screens' slot
// InitMachines (machine at +4 but the node screen's +460): sub_8223F9A8
// Type6ListScreen, sub_822436A0 Type7ListScreen, sub_82230F80
// Type6PanelScreen, sub_82240998 Type7PanelScreen, sub_82237320
// Type6NodeScreen, sub_82237B78 PowerGraphScreen6, sub_822470A8
// PowerGraphScreen7.
constexpr int kInitCount = 16;
extern const InitInfo kInits[kInitCount];
// The Init at `address` (one of kInits) on `object`, its frame at `frame`.
InitResult RunInit(uint8_t* base, uint32_t address, uint32_t object, uint32_t frame);

}  // namespace type67_modes
