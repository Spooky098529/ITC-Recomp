// The game modes' adapters' guest-calling work (hooks/game_modes.cpp), for
// the harness too.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// StateMachine::Add, Find and Handle (sub_82106158, sub_821061F0,
// sub_82106280) with r3 as the originals leave it.
void GameModesAdd(lifted::Regs& ctx);
void GameModesFind(lifted::Regs& ctx);
void GameModesHandle(lifted::Regs& ctx);
// RaceOverlaysMachine::Init (sub_8215E6C8), in the original's stack frame.
void GameModesRaceOverlaysMachineInit(lifted::Regs& ctx);

// sub_8215DE10 (the race overlays' state machine's reset handler): r3 = the
// state machine; each overlay's slot 3 a guest call.
void GameModesResetOverlays(lifted::Regs& ctx);

}  // namespace hooks
